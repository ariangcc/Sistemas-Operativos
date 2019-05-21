
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

/*
 * El espacio libre de almacenamiento es mantenido como una 
 * lista de bloques libres, Cada bloque contiene un tamaño ,
 * un apuntador al siguiente bloque, y el espacio en sí, Los
 * bloques son mantenidos en orden ascendente de dirección de 
 * almacenamiento (ordenados por sus diercciones) el ultimo bloque
 * apunta al primero. 
*/

/* (esto aun no lo entiendo bien)

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
		union header *ptr;  /* siguiente bloque si está en la lista libre */
		size_t size;     /* tamaño de este bloque */
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


#define NALLOC 1024

/*
 * morecore: ask system for more memory 
 *
 * Esta funcion se llama desde xmalloc cuando no hay espacio.
 * Aumenta el tama~no de la zona de datos como minimo en NALLOC*sizeof(Header)
 * y a~nade esta nueva zona a la lista de huecos usando xfree.
 *
 */

static Header *morecore(size_t nu)
{
	char *cp;
	Header *up;

	if (nu < NALLOC)	
		nu = NALLOC;

	/*sbrk(n) regresa un apuntador a n bytes mas de almacenamiento, regresa -1 si no hubo espacio*/
	cp = sbrk(nu * sizeof(Header)); 
	if (cp == (char *) -1) /* no space at all */
		return NULL;
	
	up = (Header *) cp;
	up ->s.size = nu;

	xfree((void *)(up+1)); /*inserta la memoria adicional en up + 1*/
	return freep;
}

/* xmalloc: asignador de almacenamiento de proposito general */
void *xmalloc (size_t nbytes)
{
	Header  *p, *prevp;
	size_t nunits;

	/* 
	   Calcula cuanto ocupara la peticion medido en tama~nos de
	   cabecera (incluyendo la propia cabecera). 
	   El termino "sizeof(Header)-1" provoca un redondeo por exceso.
	   El termino "+ 1" es para incluir la propia cabecera.
	*/
	nunits = (nbytes+sizeof(Header)-1)/sizeof(Header) + 1;


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
	*/
	for (p= prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
		if (p->s.size >= nunits) {  /* big enough */
			if (p->s.size == nunits)  /* exactly */
				prevp->s.ptr = p->s.ptr;

			else {  /* asigna la parte final */
				p->s.size -= nunits;
				p+= p->s.size;
				p->s.size = nunits;
			}
			freep = prevp; /* estrategia next-fit */
			return (void *)(p+1); /* devuelve un puntero a la
						 zona de datos del bloque */
		}
		/* Si ha dado toda la vuelta pide mas memoria y vuelve
		   a empezar */
		if (p == freep) /* wrapped around free list */
			if ((p = morecore(nunits)) == NULL)
				return NULL;  /* none left */
	}
}


/* xfree: put block ap in the free list */
void xfree(void *ap)
{
	Header *bp, *p;

	bp = (Header *)ap - 1;  /* apunta al header de un bloque */

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

	for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
		if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
			break;  /*libera el bloque al inicio o al final*/ 


	/* Comprueba compactacion con hueco posterior */
	if (bp + bp->s.size == p->s.ptr) {  /* une al nbr superior */
		bp->s.size += p->s.ptr->s.size;
		bp->s.ptr = p->s.ptr->s.ptr;
	} else
		 bp->s.ptr = p->s.ptr;

	/* Comprueba compactacion con hueco anterior */
	if (p + p->s.size == bp) {         /* une al nbr inferior*/
		p->s.size += bp->s.size;
		p->s.ptr = bp->s.ptr;
	} else
		p->s.ptr = bp;

	freep = p; /* estrategia next-fit */
}

void *xrealloc(void * ptr, size_t size)
{
	return NULL;
}
