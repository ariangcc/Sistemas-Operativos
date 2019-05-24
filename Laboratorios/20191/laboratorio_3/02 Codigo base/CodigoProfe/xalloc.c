
/*
 * ssoo/xalloc.98/xalloc.c
 *
 * CONTENIDO INICIAL:
 *	Codigo correspondiente a la Seccion 8.7 del libro:
 *	"The C Programing Language", de B. Kernigham y D. Ritchie.
 *
 * En este fichero se incluiran las rutinas pedidas 
 *
 */

#include <unistd.h>
#include "xalloc.h"
#include <stdio.h>

/*
 * Definicion de la cabecera para huecos y bloques. 
 * La union con un campo de tipo Align fuerza que el tama~no
 * de la cabecera sea multiplo del tama~no de este tipo.
 * El campo Align nunca es utilizado, solo hace que cada 
 * header esté alineado al limite del peor caso
 */
typedef long Align;    /* para alineamiento al limite mayor */

union header {   /* block header: */
	struct {
		union header *ptr;  /* *next block if on free list */
		size_t size;     /* size of this block */
	} s;
	Align x;             /* force alignment of blocks */
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
static Header  *freep = NULL;  /* start of the free list */


#define NALLOC 1024

//Actividad 1
int eqHeader = (sizeof(Header) + sizeof(Align) - 1)/sizeof(Align); //cuantos Aligs equivale un header

////////////////////////////////////////////////
/*
 * morecore: ask system for more memory 
 *
 * Esta funcion se llama desde xmalloc cuando no hay espacio.
 * Aumenta el tama~no de la zona de datos como minimo en NALLOC*sizeof(Header)
 * y a~nade esta nueva zona a la lista de huecos usando xfree.
 *
 * recibe el numero de unidades necesarias para almacenar nbytes
 */

static Header *morecore(size_t nu)
{
	// *cp es como *freemem y *up es *insertp
	char *cp;
	Header *up;
    /*
        Debido a que pe­dir memoria al sistema es una operación 
        comparativamente costosa, no deseamos hacerlo en cada llamada
        a malloc, así que morecore solicita al menos NALLOC
        unidades; este bloque grande será seccionado de acuerdo con 
        las necesidades
    */
	if (nu < NALLOC)	
		nu = NALLOC;

	cp = sbrk(nu * sizeof(Header));
	if (cp == (char *) -1) /* no space at all */
		return NULL;
	
	printf("**llamada a morecore** El nuevo bloque tendrá %ld units \n",nu);
	
	//se contruye un nuevo bloque
	up = (Header *) cp;
	up ->s.size = nu;
    /* 
    * Insert block into the free list so that it is available for malloc.  Note
    * that we add 1 to the address, effectively moving to the first position
    * after the header data, since of course we want the block header to be
    * transparent for the user's interactions with malloc and free.
    */	
	xfree((void *)(up+1));
    /* Returns the start of the free list; recall that freep has been set to the
    * block immediately preceeding the newly allocated memory (by free).  Thus by
    * returning this value the calling function can immediately find the new
    * memory by following the pointer to the next block.
    */
	return freep;
}


void *xmalloc (size_t nbytes)
{
	// *p is *currp
	Header  *p, *prevp;
	size_t nunits;

	/* 
	   Calcula cuanto ocupara la peticion medido en tama~nos de
	   cabecera (incluyendo la propia cabecera). 
	   El termino "sizeof(Header)-1" provoca un redondeo por exceso.
	   El termino "+ 1" es para incluir la propia cabecera.
	*/
	nunits = (nbytes+sizeof(Header)-1)/sizeof(Header) + 1;
    //nunits = (nbytes + sizeof(Align) - 1)/sizeof(Align) + eqHeader; // <----ACTIVIDAD 1

	printf("-> necesito %ld bytes que equivalen a %ld units : 1 [header] + %ld [nbytes]-\n",nbytes,nunits,nunits-1);
	//printf("-necesito %ld bytes que equivalen a %ld Aligns: 2 [header] + %ld [nbytes]-\n",nbytes,nunits,nunits-2);
	
	/* En la primera llamada se construye una lista de huecos con un
	   unico elemento de tama~no cero (base) que se apunta a si mismo */
	if (( prevp = freep) == NULL ) { /* no free list yet */
		base.s.ptr = freep = prevp = & base; 
		base.s.size = 0;
	}

	/*
	   Recorre la lista circular de huecos, empezando por el siguiente al
	   que apunta freep, hasta que encuentra uno que satisface la peticion
	   o da toda una vuelta a la lista (no hay espacio suficiente)
		- recordar p es currp 
		- ptr es next
	*/
	prevp = freep; //solo para visualizar
	for (p= prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
		if (p->s.size >= nunits) {  /* big enough */
			//if (p->s.size == nunits || p->s.size - nunits < 1 + eqHeader) <--- ACTIVIDAD 1 
            if (p->s.size == nunits || p->s.size - nunits < 2) // <--- ACTIVIDAD 2 
				prevp->s.ptr = p->s.ptr;
			
			else {  /* allocate tail end */
				p->s.size -= nunits; /* adjust the size */
				p += p->s.size;/* find the address to return */  
				p->s.size = nunits;
				/*
				p->s.size -= nunits;
				pA = (Align *)p;
				pA += p->s.size;
				p = (Header *)pA;
				p->s.size = nunits;
				*/
			}
			freep = prevp; 
							/* se comienza cada vez la búsqueda 
                              desde donde termino la última
                              estrategia next-fit */

			return (void *)(p+1); /* devuelve un puntero a la
						 zona de datos del bloque */
		}
		/* Si ha dado toda la vuelta pide mas memoria y vuelve
		   a empezar */
		if (p == freep) 
			if ((p = morecore(nunits)) == NULL)
				return NULL;  /* none left */
	}
}


/* xfree: put block ap in the free list */
void xfree(void *ap)
{
	// *bp is *insertp y *p is *currp
	Header *bp, *p;
	bp = ((Header *)ap) - 1;  /* apunta al header de un bloque */

	/*
	   Bucle que recorre la lista de huecos para buscar el hueco
	   anterior al bloque que se va a liberar.
	   Del bucle se sale cuando se encuentran los dos huecos
	   de la lista (el apuntado por p y el apuntado por p->s.ptr)
	   entre los que se incluira el nuevo hueco (el apuntado por bp).
	   Hay dos casos:
		- La direccion del nuevo hueco es mayor (bp > p) o
		menor (bp < p->s.ptr) que la de ningun otro hueco de la
		lista (corresponde al break)
		- La direccion del nuevo hueco esta comprendida entre
		dos huecos de la lista (corresponde a la salida normal
		del for)
	*/

	/* 
	* currp >= currp->s.ptr implies that the current block is the rightmost
	* block in the free list.  Then if the insertion block is to the right of
	* that block, then it is the new rightmost block; conversely if it is to
	* the left of the block that currp points to (which is the current leftmost
	* block), then the insertion block is the new leftmost block.  Note that
	* this conditional handles the case where we only have 1 block in the free
	* list (this case is the reason that we need >= in the first test rather
	* than just >).
	*/	

	for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
		if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
			break;  /* freed block at start or end of arena */ 


	// p is currp and bd is insertp
	/* Comprueba compactacion con hueco posterior */
	//if ((Header*)((Align*)bp + bp->s.size) == p->s.ptr) 
	if (bp + bp->s.size == p->s.ptr) {  /* join to upper nbr */
		bp->s.size += p->s.ptr->s.size;
		bp->s.ptr = p->s.ptr->s.ptr;
	} else
		 bp->s.ptr = p->s.ptr;

	/* Comprueba compactacion con hueco anterior */
	//if ((Header*)((Align*)p + p->s.size) == bp)
	if (p + p->s.size == bp) {         /* join to lower nbr */
		p->s.size += bp->s.size;
		p->s.ptr = bp->s.ptr;
	} else
		p->s.ptr = bp;

	/* reset the start of the free list */
	freep = p; /* estrategia next-fit */
}
/////////////////////////////////////////////////////////////////////////////////

void copiarDatos(void* ptr1, void* ptr2, int nbytes){
	char* dest = (char*)ptr1; 
	char* src = (char*)ptr2;
	int i;

	for(i = 0;i< nbytes;i++){
		dest[i] = src[i];
	}
}

Header* aumentar_memoria(Header* bp,int size){
	int nunits = (size + sizeof(Header)-1)/sizeof(Header) + 1;
	int required_units = nunits - bp->s.size;// numero de units requeridos
	int nunits_disp = 0; //es el numero de units disponibles de los bloques adyacentes

	Header *p;
	//se busca el lugar correcto para el nuevo hueco
	for (p= freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr){
		if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
			break;
	}

	Header* p_next = p->s.ptr;

	//((Header*)((Align*)bp + bp->s.size) == p->s.ptr)
	int hay_ady_der = ((bp + bp->s.size) == p->s.ptr);
	//((Header*)((Align*)p + p->s.size) == bp)
	int hay_ady_izq = ((p + p->s.size) == bp);

	if(hay_ady_izq) nunits_disp += p->s.size;
	if(hay_ady_der) nunits_disp += p_next->s.size;


	if(hay_ady_der && (p_next->s.size >= required_units)){  
		if ((p_next->s.size == required_units) || (p_next->s.size - required_units) < 2){
			p->s.ptr = p_next->s.ptr;
		}
		else {
			p_next->s.size -= required_units;			
			/*
			Align* p2 = (Align*)p_next;
			p2 += required_units;
			p_next = (Header*)p2; 
			//p->s.size = required_units;			
			*/
			p_next += required_units; 
		}
		bp->s.size += required_units;
		freep = p; 
	}

	else if (hay_ady_izq && (nunits_disp >= required_units)){ 
		Header *new_nextp, *prev, *new_bp;
		int bp_size = bp->s.size;

		if(hay_ady_der){ 
			required_units -= p_next->s.size; 
			new_nextp = p_next->s.ptr;
			p->s.ptr = p_next->s.ptr;
			bp->s.size += p_next->s.size;
		}
		else 
			new_nextp = p_next;

		for(prev = p->s.ptr; prev != p ; prev = prev->s.ptr);

		// Si es exacto o queda espacio de uno no genera hueco
		if (p->s.size == required_units || (p->s.size - required_units) < 2){ 
			prev->s.ptr = new_nextp;
			new_bp = p;
		}
		else {
			p->s.size -= required_units;
			//new_bp = (Header*)((Align*)p + p->s.size);
			new_bp = (Header*)(p + p->s.size);
			new_bp->s.size = bp->s.size + required_units;
		}
		//copiarDatos(new_bp + 1, bp + 1, sizeof(Align)*(bp_size - (sizeof(Header) - 1)/sizeof(Align) - 1));
		copiarDatos(new_bp + 1, bp + 1, sizeof(Align)*(bp_size - (sizeof(Header) - 1)/sizeof(Align) - 1));
		bp = new_bp;			
		freep = prev;
	}
	//Si no hay memoria que pueda satisfacer las condiciones
	else{ 
		Header *new_bp;
		new_bp =  (Header*)((Header*)xmalloc(size) - 1);
		if(new_bp == NULL ) return NULL;
		//copiarDatos(new_bp + 1, bp + 1, sizeof(Align)*(bp->s.size - (sizeof(Header)-1)/sizeof(Align) - 1));
		copiarDatos(new_bp + 1, bp + 1, sizeof(Align)*(bp->s.size - (sizeof(Header)-1)/sizeof(Align) - 1));		
		xfree(bp + 1);	
		bp = new_bp;
	}
	//finalmente se retorna la memoria realloc-ada
	return bp;
}

Header* reducir_memoria(Header* bp, int nunits){
	Header *p;
	Header *newbp = NULL;
	//se busca el lugar correcto para el nuevo hueco
	for (p= freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr){
		if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
			break;
	}
	
	//if(bp->s.size - nunits >= 2 || (Header*)((Align*)bp + bp->s.size) == p->s.ptr)
	if(bp->s.size - nunits >= 2 || (bp + bp->s.size) == p->s.ptr){
		//Header *newbp = (Header*)( (Align*)bp + nunits);
		newbp = bp + nunits;
		newbp->s.size = bp->s.size - nunits; 
		//if((Header*)((Align*)bp + bp->s.size) == p->s.ptr)
		if((bp + bp->s.size) == p->s.ptr){ 
			newbp->s.size += p->s.ptr->s.size;
			newbp->s.ptr = p->s.ptr->s.ptr;
		}
		else{
			newbp->s.ptr = p->s.ptr;	
		}
		p->s.ptr = newbp;
		bp->s.size = nunits;
		freep = p;
	}
	return bp;
}

void *xrealloc(void * ptr, size_t size) {
	Header *bp;
	Header *new_p = NULL;
	int nunits;

	bp = (Header*)ptr-1;
	nunits = (size + sizeof(Header)-1)/sizeof(Header) + 1;
	printf("-> necesito pasar de %ld units a %d units\n",bp->s.size,nunits);

	if (bp->s.size < nunits) {
		printf("*se va a aumentar la memoria*\n");
		new_p = aumentar_memoria(bp,size);
	} 
	else if (bp->s.size > nunits) {
		printf("*se va a disminuir la memoria*\n");		    
		new_p = reducir_memoria(bp,nunits); 
	}
	else {
		printf("*el tamanyo solicitado es el mismo\n"); 
		new_p = (Header*)ptr-1;
	}
	return new_p != NULL ? (void*)(new_p+1) : NULL;  
}

/////////////////////////////////////////////////////////////////////////////////

void visualize(const char* msg)
{
    Header *tmp;

    printf("Free list despues de \"%s\":\n", msg);

    if (freep == NULL) {                  
        printf("\tLa lista aun no existe\n\n");
        return;
    }
    if  (freep == freep->s.ptr) {         
        printf("\tLa lista esta vacia\n\n");
        return;
    }
    //enmarco el freep

	tmp = &base;
	do{	
		if(tmp == freep)
    		printf("  [ next: %10p size: %-3lu] -->  ", (void *) freep, freep->s.size);
		else
			printf("next: %10p size: %-3lu -->  ", (void *) tmp, tmp->s.size);
		tmp = tmp->s.ptr;
	}while (tmp != &base);
	
   printf("\n\n");
}