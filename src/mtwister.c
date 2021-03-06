/**
 * @file mtwister.c
 * @author Rilder Pires
 * @version 1.0
 *
 * @section LICENSE
 *
 * Adaptation of Mersenne Twister Pseudo Random Number Generator
 *
 * Copyright (C) 2013, Rilder Pires.
 * All rights reserved.
 *
 * Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
 * All rights reserved.
 *                          
 * Copyright (C) 2005, Mutsuo Saito,
 * All rights reserved.                          
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *  1. Redistributions of source code must retain the above copyright
 * 	  notice, this list of conditions and the following disclaimer.
 * 
 *  2. Redistributions in binary form must reproduce the above copyright
 * 	  notice, this list of conditions and the following disclaimer in the
 * 	  documentation and/or other materials provided with the distribution.
 * 
 *  3. The names of its contributors may not be used to endorse or promote 
 * 	  products derived from this software without specific prior written 
 * 	  permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * @section DESCRIPTION
 * 
 * Adaptation of Mersenne Twister Pseudo Random Number Generator. The original
 * C-program for MT19937, with initialization improved 2002/1/26 was coded by
 * Takuji Nishimura and Makoto Matsumoto. This adaptation was coded by Rilder
 * Pires in 2013/08/22.
 * 
 * Any feedback is very welcome.
 * http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
 * email: m-mat@math.sci.hiroshima-u.ac.jp
 *    
 * Any feedback about the adaptation is also welcome.
 * email: rilder@fisica.ufc.br
 */

#include <stdio.h>
#include <math.h>
#include "mtwister.h"

static unsigned long MTwister_mt[MTwister_N]; /* the array for the state vector  */
static int MTwister_mti=MTwister_N+1; /* mti==N+1 means mt[N] is not initialized */


/* Initializes MTwister_mt[N] with a seed */ 

void MTwister_Init(unsigned long s)
{
    MTwister_mt[0]= s & 0xffffffffUL;
    for (MTwister_mti=1; MTwister_mti<MTwister_N; MTwister_mti++) {
        MTwister_mt[MTwister_mti] = 
	    (1812433253UL * (MTwister_mt[MTwister_mti-1] ^ (MTwister_mt[MTwister_mti-1] >> 30)) + MTwister_mti); 
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        MTwister_mt[MTwister_mti] &= 0xffffffffUL;
        /* for >32 bit machines */
    }
}

/* Initialize by an array with array-length
   init_key is the array for initializing keys
   key_length is its length */
   
void MTwister_InitByArray(unsigned long init_key[], int key_length)
{
    int i, j, k;
    MTwister_Init(19650218UL);
    i=1; j=0;
    k = (MTwister_N>key_length ? MTwister_N : key_length);
    for (; k; k--) {
        MTwister_mt[i] = (MTwister_mt[i] ^ ((MTwister_mt[i-1] ^ (MTwister_mt[i-1] >> 30)) * 1664525UL))
          + init_key[j] + j; /* non linear */
        MTwister_mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++; j++;
        if (i>=MTwister_N) { MTwister_mt[0] = MTwister_mt[MTwister_N-1]; i=1; }
        if (j>=key_length) j=0;
    }
    for (k=MTwister_N-1; k; k--) {
        MTwister_mt[i] = (MTwister_mt[i] ^ ((MTwister_mt[i-1] ^ (MTwister_mt[i-1] >> 30)) * 1566083941UL))
          - i; /* non linear */
        MTwister_mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++;
        if (i>=MTwister_N) { MTwister_mt[0] = MTwister_mt[MTwister_N-1]; i=1; }
    }

    MTwister_mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
}

/* Generates a random number on [0,0xffffffff]-interval */
unsigned long MTwister_Int32(void)
{
    unsigned long y;
    static unsigned long mag01[2]={0x0UL, MTwister_MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (MTwister_mti >= MTwister_N) { /* generate N words at one time */
        int kk;

        if (MTwister_mti == MTwister_N+1)   /* if init_genrand() has not been called, */
            MTwister_Init(5489UL); /* a default initial seed is used */

        for (kk=0;kk<MTwister_N-MTwister_M;kk++) {
            y = (MTwister_mt[kk]&MTwister_UPPER_MASK)|(MTwister_mt[kk+1]&MTwister_LOWER_MASK);
            MTwister_mt[kk] = MTwister_mt[kk+MTwister_M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<MTwister_N-1;kk++) {
            y = (MTwister_mt[kk]&MTwister_UPPER_MASK)|(MTwister_mt[kk+1]&MTwister_LOWER_MASK);
            MTwister_mt[kk] = MTwister_mt[kk+(MTwister_M-MTwister_N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (MTwister_mt[MTwister_N-1]&MTwister_UPPER_MASK)|(MTwister_mt[0]&MTwister_LOWER_MASK);
        MTwister_mt[MTwister_N-1] = MTwister_mt[MTwister_M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        MTwister_mti = 0;
    }
  
    y = MTwister_mt[MTwister_mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

/* Generates a random number on [0,0x7fffffff]-interval */
long MTwister_Int31(void)
{
    return (long)(MTwister_Int32()>>1);
}

/* Generates a random number on [0,1]-real-interval */
double MTwister_Real1(void)
{
    return MTwister_Int32()*(1.0/4294967295.0); 
    /* divided by 2^32-1 */ 
}

/* Generates a random number on [0,1)-real-interval */
double MTwister_Real2(void)
{
    return MTwister_Int32()*(1.0/4294967296.0); 
    /* divided by 2^32 */
}

/* Generates a random number on (0,1)-real-interval */
double MTwister_Real3(void)
{
    return (((double)MTwister_Int32()) + 0.5)*(1.0/4294967296.0); 
    /* divided by 2^32 */
}

/* Generates a random number on [0,1) with 53-bit resolution */
double MTwister_Res53(void) 
{ 
    unsigned long a=MTwister_Int32()>>5, b=MTwister_Int32()>>6; 
    return(a*67108864.0+b)*(1.0/9007199254740992.0); 
} 
/* These real versions are due to Isaku Wada, 2002/01/09 added */

