/*
 * Copyright 2009 Cornell Database Group
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _DBUTL_ALIGNED_MEM_H_
#define _DBUTL_ALIGNED_MEM_H_

/*
 *  AlignedMem.h
 *  DBUtl
 *
 *  Created by Alan Demers on 10/10/09.
 *  Copyright 2009 Cornell. All rights reserved.
 *
 */

#include <cstdlib> /* for size_t */

namespace DBUtl {

class AlignedMem {

public:
	static size_t const DEFAULT_ALIGN = sizeof(double);
	static void * allocate(size_t nBytes, size_t alignSize = DEFAULT_ALIGN);
	static void
			* allocateClear(size_t nBytes, size_t alignSize = DEFAULT_ALIGN);
	static void deallocate(void * p);

#ifdef USE_HUGEPAGE
	static int const MAX_SEGMENTS = 20;
	static int const MAX_PAGES = 600;
	static int shm_id[MAX_SEGMENTS];
	static int shm_size[MAX_SEGMENTS]; // # of hugepages allocated & attached
	static void * shm_addr[MAX_SEGMENTS]; // starting address of allocated memory
	static int num_segments;
	static int total_pages;
#endif //USE_HUGEPAGE
};

} // namespace DBUtl

#endif /* _DBUTL_ALIGNED_MEM_H_ */
