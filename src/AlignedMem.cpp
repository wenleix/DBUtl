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


#include "DBUtl/AlignedMem.h"

#include <cstdlib>
#include <cstring>
#include <stdexcept>

#ifdef USE_HUGEPAGE
#include <sys/ipc.h>
#include <sys/shm.h>
#define MB_1 (1024*1024)
#define PAGESIZE (2*MB_1) // OS&hardware dependent, 2MB for x86
int AlignedMem::shm_id[AlignedMem::MAX_SEGMENTS];
int AlignedMem::shm_size[AlignedMem::MAX_SEGMENTS]; // # of hugepages allocated & attached
void * AlignedMem::shm_addr[AlignedMem::MAX_SEGMENTS]; // starting address of allocated memory
int AlignedMem::num_segments = 0;
int AlignedMem::total_pages = 0;
#endif //USE_HUGEPAGE


using namespace DBUtl;

//inline void * AlignedMem::allocate(size_t nBytes, size_t alignSize) {
void * AlignedMem::allocate(size_t nBytes, size_t alignSize) {
#	ifndef DBUTL_ALIGNED_MEM_NDEBUG
		if( (alignSize & (alignSize-1)) != 0 ) {
			throw std::logic_error(
				"AllignedMem::allocate alignment not a power of 2" );
		}
#	endif
#ifdef USE_HUGEPAGE
	if (alignSize > PAGESIZE)	{
		printf("alignment cannot be satisfied.");
		exit(1);
	}
	int pages = 1 + (nBytes - 1) / PAGESIZE;
	if (AlignedMem::num_segments >= AlignedMem::MAX_SEGMENTS) 	{
		printf("Too many segments allocated.");
		exit(1);
	}
	if (pages + AlignedMem::total_pages > AlignedMem::MAX_PAGES)	{
		perror("Too many pages allocated.");
	}
	int shmid = shmget(AlignedMem::num_segments, pages * PAGESIZE, SHM_HUGETLB | IPC_CREAT | SHM_R | SHM_W);
  	if ( shmid < 0 ) {
 		perror("shmget");
    		exit(1);
 	}
  	void * p = shmat(shmid, 0, 0);
  	if (p == (void *)-1) {
    		perror("Shared memory attach failure");
    		shmctl(shmid, IPC_RMID, NULL);
    		exit(2);
  	}
	AlignedMem::shm_id[AlignedMem::num_segments] = shmid;
	AlignedMem::shm_size[AlignedMem::num_segments] = pages;
	AlignedMem::shm_addr[AlignedMem::num_segments] = p;
	AlignedMem::total_pages += pages;
	AlignedMem::num_segments++;
#else
	void * base = malloc(nBytes + alignSize);
	char * p = (char *)base;
	size_t off = ((size_t)(p)) & (alignSize-1);
	p = p - off + alignSize;
	((void **)(p))[-1] = base;
#endif //USE_HUGEPAGE

	return (void *)(p);
}

void * AlignedMem::allocateClear(size_t nBytes, size_t alignSize) {
	void * p = allocate(nBytes, alignSize);
	memset( p, 0, nBytes );
	return (void *)(p);
}

void AlignedMem::deallocate(void * p) {
#ifdef USE_HUGEPAGE
	for (int i = 0; i < AlignedMem::num_segments; ++i)	{
		if (p = AlignedMem::shm_addr[i])	{
			shmctl(AlignedMem::shm_id[i], IPC_RMID, NULL);
			AlignedMem::shm_addr[i] = 0;
		}
	}
#else
	void * base = ((void **)(p))[-1];
	free( base );
#endif //USE_HUGEPAGE
}

/*
 * TODO: when "make check" is available ...
 */
#ifdef DBUTL_ALIGNED_MEM_TEST
#include <cstdio>
#include <cstdlib>
#include "AlignedMem.h"

#define COUNT 20
size_t ALIGN = 16;

int main( int argc, char const ** argv ) {

	if( argc > 1 ) ALIGN = (size_t)(atoi(argv[1]));

	fprintf(stdout, "Hello, World!\n");
	fprintf(stdout, "Alignment %zu:\n", ALIGN);
	for( size_t s = 8; s <= 4096; s <<= 1 ) {
		void * ptrs[COUNT];
		for( int i = 1; i < COUNT; i++ ) {
			ptrs[i] = AlignedMem::allocate(i*s, ALIGN);
			fprintf(stdout, "\tSize %zd  Addr %zx\n", i*s, (size_t)(ptrs[i]));
		}
		for( int i = COUNT-1; i > 0; i-- ) {
			AlignedMem::deallocate(ptrs[i]);
		}
	}
	fprintf(stdout, "Good-Bye, World!\n");
	return 0;
}
#endif /* DBUTL_ALIGNED_MEM_TEST */
