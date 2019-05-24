/* xmalloc: general-purpose storage allocator */
void *xmalloc (size_t nbytes)
{
	// *p is *currp
	Header *prevp, *p; 
	Header *best = NULL;
	Header *prevBest = NULL;
	int dif = 1000000;
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
	
	//BEST FIT ALGORITHM

	if (( prevp = freep) == NULL ) { 
		base.s.ptr = freep = prevp = & base; 
		base.s.size = 0;
	}

	prevp = freep; //solo para visualizar
	for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
		
		if(p == freep && best != NULL){			
			//if(best->s.size == nunits || best->s.size - nunits < 1 + eqHeader)
            if(best->s.size == nunits || best->s.size - nunits < 2)
				prevBest->s.ptr = best->s.ptr;
			else{
				best->s.size -= nunits; 
				best += best->s.size;
				best->s.size = nunits;
				/*para align probar con esto
				best->s.size -= nunits;
				Align* p2 = (Align*)best; 
				p2 += best->s.size;
				best = (Header*)p2; 
				best->s.size = nunits;
				*/
			}
			freep = prevBest; 				
			return (void *)(best + 1);
		}
		
		if (p->s.size >= nunits) {  /* big enough */		
			if(p->s.size - nunits < dif){
				prevBest = prevp;
				best = p;
				dif = p->s.size - nunits;
			}
		}

		if (p == freep && best == NULL) 
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

	//para BEST FIT el freep->s.ptr sera siempre el punto de inicio de la busqueda, 
	//siendo freep  el best fit ultimo encontrado
	//por ello no se debe actualizar cada vez que se libera 

	/* reset the start of the free list */
	//freep = p; /* estrategia next-fit */
}