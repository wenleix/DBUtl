/*
 *  HashRand.cpp
 *  HashRand
 *
 *  Created by Alan Demers on 7/29/10.
 *  Copyright 2010 Cornell University. All rights reserved.
 *    (except for psdes, below, which I don't own)
 *
 */

#include "RNG.h"

#include <pstdint.h>

#include <climits>
#include <ctime>

#define _DBUTL_HASH_RAND_TEST_
#define _DBUTL_PSDES_TEST_
#undef _DBUTL_HASH_RAND_TEST_
#undef _DBUTL_PSDES_TEST_


#if INT_MAX != 0x7fffffff
#	error int is not 32-bits
#endif
#if UINT_MAX != 0xffffffff
#	error unsigned is not 32 bits
#endif
#if USHRT_MAX != 0xffff
#	error unsigned short is not 16 bits
#endif


RNG RNG::_global;
bool RNG::_globalInitialized = false;
void RNG::_initGlobal() {
	RNG rng; // call constructor to initialize _coeff; probably not necessary
	_global.setSeed(0); RNG::_globalInitialized = true;
}

double RGenI::_coeff; // TODO: XXX = 1.0 / (((double)(UINT_MAX)) + 1.0);

uint32_t RGenI::_initialClientSeed = 0xeeeeeeee;

uint32_t RGenO::_seed = 0x5a5a5a;

void RGenI::_setRandomSeed() {
	uint32_t lw = getSeed();
	uint32_t newSeed = ((uint32_t)(time(0)));
	psdes( lw, newSeed );
	if( (newSeed <<= 8) == 0 ) newSeed = (INIT_SEED);
	setSeed(newSeed);
}

uint32_t RGenO::hash64to32( uint32_t lword, uint32_t rword ) {
	return RGenI::hash64to32( lword, rword );
}

uint32_t RGenO::getSeed() {
	return RGenO::_seed;
}

void RGenO::setSeed(uint32_t s) {
	RGenO::_seed = s;
}

void RGenC::_fill() {
	/* assert _ix == 0 */
	for( int i = _CACHE_SZ; i > 0; ) {
		_cache[--i] = gen.rand();
	}
	_ix = _CACHE_SZ;
}

double RGenO::rand() {
	return ((RGenI *)(this))->rand();
}

/*
 * Tests for psdes implementation (see table in Section 7.5)
 * Frequency moments of generated random numbers
 * Timing tests
 */
#if (defined(_DBUTL_HASH_RAND_TEST_) || defined(_DBUTL_PSDES_TEST_))

#include <cstdio>
#include <cstring>
#include <cstdlib>

namespace /* anon */ {
	#define NM 10
	double a[NM];
	
#	ifdef _DBUTL_PSDES_TEST_
		void printpsdes(unsigned lw, unsigned lv, unsigned rw, unsigned rv) {
			uint32_t nlw = lw;
			uint32_t nrw = rw;
			RGenI::psdes(nlw, nrw);
			fprintf( stdout, "\tleft: 0x%x -> 0x%x (0x%x)   right: 0x%x -> 0x%x (0x%x)%s\n",
					lw, nlw, lv, rw, nrw, rv,
					(((nlw != lv) || (nrw != rv)) ? " <----------- ERROR!" : "") );
					
		}
#	endif
}


int main (int argc, char * const argv[]) {
	int N = 1000000;
	int nm = 0;
	int rngID = 0;
	if( (argc > 1) && (strcmp(argv[1], "--help") == 0) ) {
		fprintf(stdout, "Usage %s [n [nMoments [type]]]\n\ttype 0=cache 1=small\n", argv[0]);
		return -1;
	}
	if( argc > 1 ) {
		int NTmp = atoi(argv[1]);  N = NTmp;
	}
	if( argc > 2 ) {
		int nmTmp = atoi(argv[2]);  if( (nmTmp >= 0) && (nmTmp <= NM) ) nm = nmTmp;
	}
	if( argc > 3 ) {
		int rngIDTmp = atoi(argv[3]);  if( (rngIDTmp >= 0) || (rngIDTmp <= 2) ) rngID = rngIDTmp;
	}
#	ifdef _DBUTL_PSDES_TEST_
	{
		printf("Testing psdes ...\n");
		/* Table from Sec 7.5 ... */
		printpsdes(1, 0x604d1dce, 1, 0x509c0c23);
		printpsdes(1, 0xd97f8571, 99, 0xa66cb41a);
		printpsdes(99, 0x7822309d, 1, 0x64300984);
		printpsdes(99, 0xd7f376f0, 99, 0x59ba89eb);
	}
#	endif
	RNG rng;
	RNGSmall rngSmall;
	RNGMin rngMin;
    fprintf(stdout, "Generating %d random numbers using generator %d ... ", N, rngID); fflush(stdout);
	for( int i = 0; i < N; i++ ) {
		double di = i;
		double di1 = (i+1.0);
		double x;
		switch( rngID ) {
		case 0:
			x = rng.rand();  break;
		case 1:
			x = rngSmall.rand();  break;
		case 2:
			x = rngMin.rand();  break;
		}
		double xj = 1.0;
		for( int j = 0; j < nm; j++ ) {
			a[j] = (di * a[j] + xj) / di1;
			xj *= x;
		}
	}
	fprintf(stdout, "done\n");
	if( nm > 0 ) {
		fprintf(stdout, "\nMoments:\n");
		for( int j = 0; j < nm; j++ ) {
			fprintf(stdout, "\t%3d: %e\n", j, a[j]);
		}
	}
	printf("\nTests done.\n");
    return 0;
}


#endif
