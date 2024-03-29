#ifndef UTIL_H
#define UTIL_H

#include <sys/types.h>
#include <vmsim.h>

#ifdef WORDS_BIGENDIAN
/* The tracefiles store in little-endian; convert if needed for
 * this host. Note that no conversion is ever needed on single bytes.
 * From http://remus.rutgers.edu/~rhoads/Code/swapbyte.c */
static inline uint file_to_host_uint(uint tni4) {
   return (((tni4>>24)&0xff) | ((tni4&0xff)<<24) |
	      ((tni4>>8)&0xff00) | ((tni4&0xff00)<<8));
}
#else
/* No conversion needed. */
#define file_to_host_uint(i) (i)
#endif

/* From K&R, 2nd Ed., pg. 49: get n bits from position p 

  Regresa el campo de n bits de x (ajustado a la derecha) que
  principia en la posicion p 

  el bit 0 se encuentra en el borde derecho para este caso 

  vfn : 1111 0000 1111 0000
 addr_space_bits - 1  = 15
 vfn_bits : bits para rep el numero de pagina

*/
static inline uint getbits(uint x, int p, int n) {
  return (x >> (p+1-n)) & ~(~0 << n);
}

/*ayuda a sacar el numero de pagina de una direccion virtual mediante el uso de getbits*/
static inline uint vaddr_to_vfn(vaddr_t vfn) {
  return getbits(vfn, addr_space_bits-1, vfn_bits);
}


uint log_2(uint x);
uint pow_2(uint pow);

void util_test();

#endif /* UTIL_H */
