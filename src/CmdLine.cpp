/*
 *  CmdLineParsing.cpp
 *  XPSimFW
 *
 *  Created by Alan Demers on 11/19/09.
 *  Copyright 2009 Cornell. All rights reserved.
 *
 */

#include "DBUtl/CmdLine.h"
#include "DBUtl/Log.h"

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include <vector>


namespace DBUtl {
	
	namespace CmdLine {
		
		// strdup() is not portable ...
		char const * copyCString(char const *s)	{
			if( s == 0 ) return 0;
			size_t sz = 1 + strlen(s);
			void * p = malloc(sz);
			memcpy(p, ((void *)(s)), sz);
			return ((char const *)(p));
		}
		
		void compressArgs( int & argc, const char ** argv ) {	
			int newArgc = 0;
			for( int i = 0; i < argc; i++ ) {
				if( argv[i] != 0 ) argv[newArgc++] = argv[i];
			}
			argc = newArgc;
		}
		
		char const * getSuffixAfterPrefixMatch(const char *s, const char *flagArg) {
			size_t n = strlen(flagArg);
			char const * ans = ((strncmp(s, flagArg, n) == 0) ? (s+n) : 0);
			if( ans != 0 ) DBUTL_LOG_FPRINTF( Log::DEBUG, ("  match %s value %s\n", flagArg, ans) );
			return ans;
		}
		
		int getToken(char *b, size_t blen, char const **ps, char const *delims) { // TODO: is char signed or unsigned?
			// returns delimiter char, or (-1) if no token found
			char *p = b;
			char const *s = *ps;
			char *plim = p + blen - 1;
			for(;;) {
				char c = *s;
				if( !isspace(c) ) {
					*p = c;
					if( strchr(delims, c) != 0 /* succeeds if c == 0 */ ) {
						*p = 0;
						*ps = ((c == 0) ? s : s+1);
						if( (c == 0) && (p == b) ) return (-1);
						return ((int)((unsigned char)(c)));
					}
					if( p < plim ) p++;
				}
				s++;
			}
		}
		
		int getArrayOfStrings(const char **a, int sz, char const * delims) {
			char tokBuf[100];
			if( delims == 0 ) return (-1);
			for( int i = 0; ; i++ ) {
				int delim = getToken(tokBuf, (sizeof tokBuf), &delims);
				if( delim < 0 ) return i;
				if( i >= sz ) throw std::logic_error("Too many values in composite arg");
				DBUTL_LOG_FPRINTF( Log::DEBUG, ("  composite( %d ): %s\n", i, tokBuf) );
				a[i] = copyCString(tokBuf);
				if( delim == 0 ) return (i+1);
			}
			return 0;
		}
		
		int getArrayOfDoubles(double * a, int sz, char const * delims) {
			char tokBuf[100];
			if( delims == 0 ) return (-1);
			for( int i = 0; ; i++ ) {
				int delim = getToken(tokBuf, (sizeof tokBuf), &delims);
				if( delim < 0 ) return i;
				if( i >= sz ) throw std::logic_error("Too many values in composite arg");
				DBUTL_LOG_FPRINTF( Log::DEBUG, ("  composite( %d ): %s\n", i, tokBuf) );
				a[i] = strtod(tokBuf, 0);
				if( delim == 0 ) return (i+1);
			}
			return 0;
		}
		
		int getArrayOfVectorsOfDoubles(std::vector<double> * a, int sz, char const * delims) {
			char tokBuf[100];
			if( delims == 0 ) return (-1);
			for( int i = 0; ; i++ ) {
				int delim = getToken(tokBuf, (sizeof tokBuf), &delims);
				if( delim < 0 ) return i;
				if( i >= sz ) throw std::logic_error("Too many values in composite arg");
				std::vector<double> & v = a[i];  v.clear();
				DBUTL_LOG_FPRINTF( Log::DEBUG, ("  composite( %d, %d ): %s\n", i, v.size(), tokBuf) );
				v.push_back( strtod(tokBuf, 0) );
				if( delim == 0 ) return (i+1);
				for(;;) {
					if( delim == ':' ) break;
					delim = getToken(tokBuf, (sizeof tokBuf), &delims);
					if( delim < 0 ) return (i+1);
					DBUTL_LOG_FPRINTF( Log::DEBUG, ("  composite( %d, %d ): %s\n", i, v.size(), tokBuf) );
					v.push_back( strtod(tokBuf, 0) );
					if( delim == 0 ) return (i+1);;
				}
			}
			/*NOTREACHED*/
		}
		
		void throwUsage(char const * usage, char const * m){
			if( usage == 0 ) usage = "";
			const int BSIZE = 256 + strlen(usage);
			char * buf = (char *)malloc(BSIZE);
			int n = 0;
			if( m ) n = snprintf(buf, BSIZE, "%s\n", m);
			if( n < BSIZE ) n += snprintf( (buf+n), (BSIZE-n), "Usage: %s [args]\n", nameByWhichWeWereInvoked );
			if( n < BSIZE ) n += snprintf((buf+n), (BSIZE-n), "%s", usage);
			throw std::logic_error(buf);
		}
		
		
	}

}