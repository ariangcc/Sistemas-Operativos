#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

#include <unistd.h>

/*
 * El espacio libre de almacenamiento es mantenido como una 
 * lista de bloques libres, Cada bloque contiene un tamaño ,
 * un apuntador al siguiente bloque, y el espacio en sí, Los
 * bloques son mantenidos en orden ascendente de dirección de 
 * almacenamiento (ordenados por sus diercciones) el ultimo bloque
 * apunta al primero. 
*/

/* 
 * Definicion de la cabecera para huecos y bloques. 
 * La union con un campo de tipo Align fuerza que el tama~no
 * de la cabecera sea multiplo del tama~no de este tipo.
 * 
 * El campo Align nunca es utilizado, solo hace que cada 
 * header esté alineado al limite del peor caso
 */
typedef long Align;    /* para alineamiento al limite mayor */

union header {   /* block header: */
	struct {
		union header *next;  /* siguiente bloque si está en la lista libre */
		size_t size;     /* tamaño de este bloque
                            medido en numero de 
                            bloques, no menor a NALLOC
                            */
	} s;
	Align x;             /* obliga a la alineacion de bloques */
};

typedef union header Header;
/*
 * La lista de huecos esta ordenada por direcciones y es circular.
 * base es el "falso" hueco de tama~no cero que asegura que la lista
 * nunca esta vacia 
 */
static Header base;   /* empty list to get started */
/*
 * freep apuntara al hueco situado en la lista antes del hueco
 * por el que comenzara la busqueda.
 * Necesario para implementar la estrategia next-fit
 */

static Header *freep = NULL;  /* start of the free list */

#define NALLOC 10 /* minima memoria que se puede solicitar al so*/


static Header* morecore(size_t nu);
void* xmalloc (size_t nbytes);
void xfree(void *ap);
size_t getfreem(void); 
void visualize(const char* msg);

size_t totalUnits = 0;

int main(void){ 
    unsigned int *ptlong1;
    char *ptstring1;
    char *ptstring2;
    char *ptstring3;

    visualize("start");
    unsigned int long_size = sizeof(long);
    unsigned int char_size = sizeof(char);

    ptlong1 = (unsigned int *)xmalloc(long_size);
    visualize("solicitar long");
    ptstring1 = (char*) xmalloc(20*char_size);
    visualize("solicitar 20 char");
    ptstring2 = (char*) xmalloc(25*char_size);
    visualize("solicitar 25 char");
    ptstring3 = (char*) xmalloc(16*char_size);
    visualize("solicitar 16 char");
    
    xfree(ptstring2);
    visualize("xfree(pstring2)");
    
    //xfree(ptstring1);
    //visualize("xfree(pstring1)");
    
    xfree(ptlong1);
    visualize("xfree(ptlong1)"); 
    exit(0);     
}


void *xmalloc (size_t nbytes)
{
	Header *currp;
    Header *prevp;
    size_t nunits;

	/* 
	   Calcula cuanto ocupara la peticion medido en tama~nos de
	   cabecera (incluyendo la propia cabecera). 
       nunits: el numero exacto de unides necesarias para brindar 
       nbytes de almacenamiento 
	   El termino "+ 1" es para incluir la propia cabecera.
	*/
	nunits = (nbytes+sizeof(Header)-1)/sizeof(Header) + 1;
    
    printf("-necesito %ld bytes que equivalen a %ld units : 1 [header] + %ld [nbytes]-\n",nbytes,nunits,nunits-1);
	/* En la primera llamada se construye una lista de huecos con un
	   unico elemento de tama~no cero (base) que se apunta a si mismo */
	if (( prevp = freep) == NULL ) { /* no free list yet */
        // Create degenerate free list; base points to itself and has size 0
        base.s.next = &base;
        base.s.size = 0;
        
        // Set free list starting point to base address
        freep = &base;
        prevp = &base;
	}

	/*
	   Recorre la lista circular de huecos, empezando por el siguiente al
	   que apunta freep, hasta que encuentra uno que satisface la peticion
	   o da toda una vuelta a la lista (no hay espacio suficiente)
	*/
    prevp = freep; //solo para visualizar en realidad siempre se hace en el if
    currp = prevp->s.next;

	for (; ; prevp = currp, currp = currp->s.next) {
		if (currp->s.size >= nunits) {  /* big enough */
			if (currp->s.size == nunits)  /* exactly */
				prevp->s.next = currp->s.next;

			else {  
                /* adjust the size */
				currp->s.size -= nunits;
                /* find the address to return */                                 
				currp += currp->s.size;
				currp->s.size = nunits;
			}
			freep = prevp; /* se comienza cada vez la búsqueda 
                              desde donde termino la última
                              estrategia next-fit */
			return (void *)(currp + 1); /* devuelve un puntero a la
						 zona de datos del bloque */
		}
		/* Si ha dado toda la vuelta pide mas memoria y vuelve
		   a empezar */
		if (currp == freep) 
			if ((currp = morecore(nunits)) == NULL)
				return NULL;  /* none left */
	}
}


/*
 * morecore: ask system for more memory 
 *
 * Esta funcion se llama desde xmalloc cuando no hay espacio.
 * Aumenta el tama~no de la zona de datos como minimo en NALLOC*sizeof(Header)
 * y a~nade esta nueva zona a la lista de huecos usando xfree.
 * 
 * recibe el numero de unidades necesarias para almacenar nbytes
 */

static Header *morecore(size_t nunits)
{
	char *freemem; /*The address of the newly created memory*/
	Header *insertp;
    /*
        Debido a que pe­dir memoria al sistema es una operación 
        comparativamente costosa, no deseamos hacerlo en cada llamada
        a malloc, así que morecore solicita al menos NALLOC
        unidades; este bloque grande será seccionado de acuerdo con 
        las necesidades
    */
	if (nunits < NALLOC)	
		nunits = NALLOC;

    /* Request that the OS increment the program's data space.  sbrk changes the
        * location of the program break, which defines the end of the process's data
        * segment (i.e., the program break is the first location after the end of the
        * uninitialized data segment).  Increasing the program break has the effect
        * of allocating memory to the process.  On success, brk returns the previous
        * break - so if the break was increased, then this value is a pointer to the
        * start of the newly allocated memory.
    */
    freemem = sbrk(nunits * sizeof(Header)); 
	if (freemem == (char *) -1) /* no space at all */
		return NULL;
    
    printf("**llamada a morecore** El nuevo bloque tendrá %ld units \n",nunits);
    totalUnits += nunits;

    //se contruye un nuevo bloque
	insertp = (Header *) freemem;
	insertp->s.size = nunits;

    /* Insert block into the free list so that it is available for malloc.  Note
    * that we add 1 to the address, effectively moving to the first position
    * after the header data, since of course we want the block header to be
    * transparent for the user's interactions with malloc and free.
    */
	xfree((void *)(insertp+1));
    /* Returns the start of the free list; recall that freep has been set to the
    * block immediately preceeding the newly allocated memory (by free).  Thus by
    * returning this value the calling function can immediately find the new
    * memory by following the pointer to the next block.
    */
	return freep;
}

/* xfree: put block apuntado por ptr in the free list */
void xfree(void *ptr)
{
	Header *insertp, *currp;

	insertp = ((Header *)ptr) - 1;  /* apunta al header de un bloque */

	/*
	   Bucle que recorre la lista de huecos para buscar el hueco
	   anterior al bloque que se va a liberar.
	   Del bucle se sale cuando se encuentran los dos huecos
	   de la lista (el apuntado por p y el apuntado por p->s.next)
	   entre los que se incluira el nuevo hueco (el apuntado por insertp).
	   Hay dos casos:
		- La direccion del nuevo hueco es mayor (insertp > p) o
		menor (insertp < p->s.next) que la de ningun otro hueco de la
		lista (corresponde al break)
		- La direccion del nuevo hueco esta comprendida entre
		dos huecos de la lista (corresponde a la salida normal
		del for)
	*/

	for (currp = freep; 
        !(insertp > currp && insertp < currp->s.next);
         currp = currp->s.next)
        /* currp >= currp->s.ptr implies that the current block is the rightmost
        * block in the free list.  Then if the insertion block is to the right of
        * that block, then it is the new rightmost block; conversely if it is to
        * the left of the block that currp points to (which is the current leftmost
        * block), then the insertion block is the new leftmost block.  Note that
        * this conditional handles the case where we only have 1 block in the free
        * list (this case is the reason that we need >= in the first test rather
        * than just >).
        */		
        if (currp >= currp->s.next && (insertp > currp || insertp < currp->s.next))
			break;  /*libera el bloque al inicio o al final*/ 

    /* Having found the correct location in the free list to place the insertion
    * block, now we have to (i) link it to the next block, and (ii) link the
    * previous block to it.  These are the tasks of the next two if/else pairs.
    */

	/* Comprueba compactacion con hueco posterior */
	if (insertp + insertp->s.size == currp->s.next) {  /* une al nbr superior */
		insertp->s.size += currp->s.next->s.size;
		insertp->s.next = currp->s.next->s.next;
	} else
		 insertp->s.next = currp->s.next;

	/* Comprueba compactacion con hueco anterior */
	if (currp + currp->s.size == insertp) {         /* une al nbr inferior*/
		currp->s.size += insertp->s.size;
		currp->s.next = insertp->s.next;
	} else
		currp->s.next = insertp;
    
    /* reset the start of the free list */
	freep = currp; /* estrategia next-fit */
}

void *xrealloc(void * next, size_t size)
{
	return NULL;
}


void visualize(const char* msg)
{
    Header *tmp;

    printf("--- Free list after \"%s\":\n", msg);

    if (freep == NULL) {                   /* does not exist */
        printf("\tList does not exist\n\n");
        return;
    }

    if  (freep == freep->s.next) {          /* self-pointing list = empty */
        printf("\tList is empty\n\n");
        return;
    }

    //enmarco el freep
    printf("  [  next: %10p size: %-3lu] -->  ", (void *) freep, freep->s.size);

    tmp = freep;                           /* find the start of the list */
    while (tmp->s.next != freep) {           /* traverse the list */
        tmp = tmp->s.next;
        printf("next: %10p size: %-3lu -->  ", (void *) tmp, tmp->s.size);
    }
    printf("\n\n");
}

/* calculate the total amount of available free memory */
size_t getfreem(void)
{
    if (freep == NULL)
        return 0;

    Header *tmp;
    tmp = freep;
    size_t res = tmp->s.size;

    while (tmp->s.next > tmp) {
        tmp = tmp->s.next;
        res += tmp->s.size;
    }

    return res;
}
