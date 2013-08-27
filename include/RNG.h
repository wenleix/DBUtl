/*
 *  RNG.h
 *
 *  Created by Alan Demers on 7/29/10.
 *
 *	A RNG package based on Section 7.5 of "Numerical Recipes in C++" Second Edition.
 *
 *  The basic idea is to create random numbers by hashing (seed # pos) to create the
 *    pos'th random number in the pseudorandom sequence identified by seed.  This
 *    enables you to reset the position either backward or forward at unit cost.
 *
 *  Really this is just a 2**64 element pseudorandom sequence; we treat the high
 *    order 24 bits as the sequence identifier, and the remaining 40 bits as position.
 *
 *  The hash is the authors simplified version of DES ("psdes()").
 *
 *  They claim it produces very high quality random numbers:
 *
 *	  ... we have been unable to find *any* statistical deviation from
 *    randomness in sequences of up to 10**9 floating numbers ...
 *  
 *
 *  There are three useful versions of the generator:
 *
 *	  RNG has a big RNG state but small code size: it does the easy cases inline,
 *    and keeps a cache of the next few random numbers in the sequence, doing
 *    the heavy lifting of filling the cache in an out-of-line call.
 *
 *    RNGSmall has a small (64-bit) state but generates lots of code, doing
 *    absolutely everything inline.  This is the fastest version.
 *
 *    RNGMin has a small (64-bit) state and small code size, as it does the
 *    hashing out-of-line on every call.  This is the slowest version.
 *
 *	All versions generate identical pseudorandom sequences.
 *  All will generate > 50 million random numbers per second on a 2.66 GHz i7.
 */

#ifndef DBUTL_HASH_RAND_H_
#define DBUTL_HASH_RAND_H_

#include "DBUtl/pstdint.h"
#include <limits>

/*
 * This template defines the operatons available on any generator ...
 *
 *	G will be one of the RGenx classes below -- it must have getSeed/setSeed,
 *    getPos/setPos and rand implemented.
 */
template < class G >
class TRNG {

	G g;
	
public:
	
	/*
	 * return a random real ...
	 */
	inline double rand() {
		return g.rand();
	}
	inline float randf() {
		return (float)(g.rand());
	}
	
	/*
	 * return true with probability p
	 */
	inline bool flip(double p) {
		return (g.rand() <= p);
	}
	
	/*
	 * choose an unsigned int distributed uniformly in [0, n)
	 */
	inline uint32_t select(unsigned n) {
		return ((unsigned)(n * g.rand()));
	}
	
	/*
	 * choose an int distributed uniformly in [minValue, limitValue)
	 */
	inline int32_t uniformInt(int32_t minValue, int32_t limitValue) {
		return minValue + select(limitValue - minValue);
	}
	
	/*
	 * gaussian
	 *
	 * Jim Gray, Prakash Sundaresan, Susanne Englert, Ken Baclawski, Peter J. Weinberger. 
     * Quickly generating billion-record synthetic databases, SIGMOD 1994
     * (http://portal.acm.org/citation.cfm?id=191886)
	 */
	double gauss(double mu, double sigma) {
		double ans = 0.0;
		for (int i = 0; i < 12; i++){
			ans = ans + (rand()) - 0.5;
		}
		return (mu + sigma*ans/6.0) ;
	}

	/*
	 * Set/Query seed and position in pseudo-random number sequence ...
	 */
	inline uint32_t getSeed() { return g.getSeed(); }
	inline void setSeed(uint32_t seed) { g.setSeed(seed); }
		// seed == 0 ==> construct a random seed using time of day
	
	inline uint64_t getPos() { return g.getPos(); }
	inline void setpos(uint64_t pos) { g.setPos(pos); }
	
	/*
	 * construct a new object
	 */
	
	TRNG() : g() {}
	TRNG(uint32_t seed) : g() { g.setSeed(seed); }
	~TRNG() {}
	
};


/*
 * Inline generator: state is only 64 bits, code is big (all inlined).
 */
class RGenI {
	
	uint64_t data;
	static const uint32_t INIT_SEED = 0x5a5a5a;
	static const uint64_t INIT_DATA = (((uint64_t)(INIT_SEED)) << 40);
	
	static double _coeff; // = 1.0/2**32
	
	static uint32_t _initialClientSeed;
	
	void _setRandomSeed();

public:
	
	/*
	 * This is psdes() from Ch.7 of Numerical Recipes in C++ ...
	 *
	 * I have made it public as it could be generally useful?
	 */
	inline static void psdes(uint32_t & lword, uint32_t & irword ) {
		const int NITER = 4;
		static uint32_t c1[NITER]={
			0xbaa96887, 0x1e17d32c, 0x03bcdc3c, 0x0f33d1b2
		};
		static uint32_t c2[NITER]={
			0x4b0f3b58, 0xe874f0c3, 0x6955c5a6, 0x55a7ca46
		};
		
		for (unsigned i = 0; i < NITER; i++) {
			uint32_t iswap = irword;
			uint32_t ia = iswap ^ c1[i];
			uint32_t itmpl = ia & 0xffff;
			uint32_t itmph = ia >> 16;
			uint32_t ib = itmpl*itmpl + ~(itmph*itmph);
			irword = (lword) ^ (((/*ia = */(ib >> 16) |
								  ((ib & 0xffff) << 16)) ^ c2[i])+itmpl*itmph);
			lword = iswap;
		}
	}
	
public:
	
	static inline void setInitialSeed(uint32_t as) { _initialClientSeed = as; }
	static inline uint32_t getInitialSeed() { return _initialClientSeed; }

	static inline double toUnitReal(uint32_t n) { return ((double)(n)) * _coeff; }

	static inline uint32_t hash64to32( uint32_t lword, uint32_t rword ) {
		psdes( lword, rword );
		return rword;
	}
	
	inline uint32_t getSeed() {
		return ((uint32_t)(data >> 40));
	}
	inline void setSeed(uint32_t s) {
		if( s ) {
			data = (data & ((uint64_t)(0xffffffffffLL))) + (((uint64_t)(s)) << 40);
		} else {
			_setRandomSeed();
		}
	}
	
	inline uint64_t getPos() {
		return data & ((uint64_t)(0xffffffffffLL));
	}
	inline void setPos(uint64_t pos) {
		data = (data & ((uint64_t)(0xffffff0000000000LL)))
			| (pos & ((uint64_t)(0xffffffffffLL)));
	}
	
	inline double rand() {
		uint32_t lw = ((uint32_t)(data >> 32));
		uint32_t rw = ((uint32_t)(data));
		psdes( lw, rw );
		data += 1;
		// return ((double)(rw)) * _coeff;
		return toUnitReal(rw);
	}
	
	RGenI() : data(INIT_DATA) {
		_coeff = (((double)(1.0)) - std::numeric_limits<float>::epsilon()) / ((double)(UINT_MAX));
	}
	~RGenI() {}
};


/*
 * Out-of-line generator: state is only 64 bits, code is small (not inlined),
 *   so this is slightly slower than RGenI.
 */

class RGenO : public RGenI {
	
public:
	static uint32_t _seed;

	static uint32_t hash64to32( uint32_t lword, uint32_t rword );

	static uint32_t getSeed();

	static void setSeed( uint32_t s);

	double rand();
	
	RGenO() : RGenI()  {}
	~RGenO() {}
};


/*
 * Caching generator: state is big, code is small (not inlined) but maintains
 *   a cache of the next few random values to avoid function calls.  This is
 *    faster than RGenO but not as fast as RGenI.
 */
class RGenC {
	
	RGenI gen;
	static const unsigned _CACHE_SZ = 4;

	double _cache[_CACHE_SZ];
	uint32_t _ix;

	void _fill();

public:
	
	uint32_t getSeed() {
		return gen.getSeed();
	}
	void setSeed(uint32_t s) {
		gen.setSeed(s);  gen.setPos(0);  _ix = 0;
	}
	uint64_t getPos() {
		return gen.getPos() - _ix;
	}
	void setPos(uint64_t p) {
		_ix = 0;  gen.setPos(p);
	}

	double rand() {
		if( _ix == 0 ) _fill();
		return _cache[--(_ix)];
	}
	
	RGenC() : _ix(0), gen() {}
	~RGenC() {}
};

/*
 *
 * Classes that should be used in client programs ...
 *
 */

/*
 * Big state, small code ...
 */
class RNG : public TRNG < RGenC > {
	
	static RNG _global;
	static bool _globalInitialized;
	static void _initGlobal();
	
public:
	
	/*
	 * The shared "global" generator.  Usage is
	 *   ... RNG::get().rand() ... RNG::get().rand() ...
	 * or
	 *   RNG & rng = RNG::get(); ... rng.rand() ... rng.rand() ...
	 * but NOT
	 *   RNG rng = RNG::get(); ... rng.rand() ... rng.rand() ...
	 * as the last of these would just give an unshared copy of the state
	 */
	inline static RNG & get() {
		if( !_globalInitialized ) _initGlobal();
		return _global;
	}
	
	RNG() {}
	RNG(int s1, int s2 = 0) { setSeed( s1 ^ s2 ); }
	~RNG() {}
};


/*
 * Small state, big code (fastest) ...
 */
class RNGSmall : public TRNG < RGenI > {
	
public:
	
	RNGSmall() {}
	RNGSmall(int s1, int s2 = 0) { setSeed( s1 ^ s2 ); }
		// s2 is deprecated
	~RNGSmall() {}
};

/*
 * Small state, small code (slowest) ...
 */
class RNGMin : public TRNG < RGenO > {public:
	
	RNGMin() {}
	RNGMin(int s1, int s2 = 0) { setSeed( s1 ^ s2 ); }
		// s2 is deprecated
	~RNGMin() {}
};


#endif /* DBUTL_HASH_RAND_H_ */
