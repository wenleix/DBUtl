/*
 *  CmdLine.h
 *  DBUtl
 *
 *  Created by Alan Demers on 11/19/09.
 *  Copyright 2009 Cornell. All rights reserved.
 *
 */

#ifndef _DBUTL_CMD_LINE_H_
#define _DBUTL_CMD_LINE_H_

#include <cstddef>
#include <vector>


namespace DBUtl {
	
	namespace CmdLine {
	
		// strdup() is not portable ...
		char const * copyCString(char const *s);	

		void compressArgs( int & argc, const char ** argv );	

		char const * getSuffixAfterPrefixMatch(const char *s, const char *flagArg);
		
		int getToken(char *b, size_t blen, char const **ps, char const *delims = ",:");
		
		int getArrayOfStrings(const char **a, int sz, char const * delims);
		
		int getArrayOfDoubles(double * a, int sz, char const * delims);
		
		int getArrayOfVectorsOfDoubles(std::vector<double> * a, int sz, char const * delims);
		
		char const * nameByWhichWeWereInvoked = "?";
		
		void throwUsage(char const * usage, char const * m = 0);

	}

}

#endif /* _DBUTL_CMD_LINE_H_ */
