/*
 * mm.c - Good Utilization, Poor Throughput
 *
 * In this Explicit Segregated List approach, finding blocks for mm_malloc is
 * done by finding fit blocks in segregated free block list when requested.
 * mm_realloc is implemeted by mm_free and mm_malloc. Most of the code is come
 * from the textbook and problems.
 *
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "memlib.h"
#include "mm.h"

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* MACROs for Implicit malloc package */
#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1 << 12)

#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define PACK(size, alloc) ((size) | (alloc))
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

#define HDRP(bp) ((char *)(bp)-WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp)-WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp)-GET_SIZE(((char *)(bp)-DSIZE)))

/* MACROs for Explicit malloc package */
#define NEXTPTR(bp) ((char *)(bp))
#define PREVPTR(bp) ((char *)(bp) + WSIZE)

#define GETNEXT(bp) (*(char **)(bp))
#define GETPREV(bp) (*(char **)(PREVPTR(bp)))

#define GETNEXTITEM(bp) ((char *)GET(NEXTPTR(bp)))
#define GETPREVITEM(bp) ((char *)GET(PREVPTR(bp)))

#define SETNEXT(bp, val) (PUT(NEXTPTR(bp), (int)val))
#define SETPREV(bp, val) (PUT(PREVPTR(bp), (int)val))

/* Global variables */
static char *heap_listp;
static char **listp;

/* Functions for Implicit malloc package */
static void *coalesce(void *);
static void *extend_heap(size_t);
static void *find_fit(size_t);
static void *place(void *, size_t);

/* Functions for Explicit malloc package */
static void insert(void *);
static void delete (void *);

/* Function for Segregated list */
static size_t gainoffset(size_t);

/*
 * gainoffset - parse size in to list offset
 */
static size_t gainoffset(size_t size) {
  size_t i = 0;
  while ((i < 19) && (size > 0)) {
    size >>= 1;
    i++;
  }
  return i;
}

/*
 * insert - insert free block in to the list
 */
static void insert(void *ptr) {
  if (ptr == NULL)
    return;
  size_t size = GET_SIZE(HDRP(ptr));
  size_t i = gainoffset(GET_SIZE(HDRP(ptr)));
  void *walker, *tracer;
  int indicator = 0;
  walker = *(listp + i);
  while ((walker != NULL) && (size > GET_SIZE(FTRP(walker)))) {
    tracer = walker;
    walker = GETNEXT(walker);
    indicator = 1;
  }
  if (walker == NULL)
    SETNEXT(ptr, NULL);
  else {
    SETNEXT(ptr, walker);
    SETPREV(walker, ptr);
  }
  if (indicator == 0) {
    SETPREV(ptr, NULL);
    *(listp + i) = ptr;
  } else {
    SETPREV(ptr, tracer);
    SETNEXT(tracer, ptr);
  }
}

/*
 * delete - delete free blocks from the list
 */
static void delete (void *ptr) {
  if (ptr == NULL)
    return;
  size_t i = gainoffset(GET_SIZE(HDRP(ptr)));
  if ((GETNEXT(ptr) != NULL) && (GETPREV(ptr) != NULL)) {
    SETNEXT(GETPREV(ptr), GETNEXT(ptr));
    SETPREV(GETNEXT(ptr), GETPREV(ptr));
  }
  if ((GETNEXT(ptr) == NULL) && (GETPREV(ptr) != NULL)) {
    SETNEXT(GETPREV(ptr), NULL);
  }
  if ((GETNEXT(ptr) != NULL) && (GETPREV(ptr) == NULL)) {
    *(listp + i) = GETNEXT(ptr);
    SETPREV(GETNEXT(ptr), NULL);
  }
  if ((GETNEXT(ptr) == NULL) && (GETPREV(ptr) == NULL)) {
    *(listp + i) = NULL;
  }
}

/*
 * coalesce - coalesce free blocks
 */
static void *coalesce(void *ptr) {
  if (ptr == NULL)
    return ptr;
  size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(ptr)));
  size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(ptr)));
  size_t size = GET_SIZE(HDRP(ptr));

  if (prev_alloc && next_alloc) {
    insert(ptr);
    return ptr;
  } else if (prev_alloc && !next_alloc) {
    delete (NEXT_BLKP(ptr));
    size += GET_SIZE(HDRP(NEXT_BLKP(ptr)));
    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
  } else if (!prev_alloc && next_alloc) {
    delete (PREV_BLKP(ptr));
    size += GET_SIZE(HDRP(PREV_BLKP(ptr)));
    PUT(FTRP(ptr), PACK(size, 0));
    PUT(HDRP(PREV_BLKP(ptr)), PACK(size, 0));
    ptr = PREV_BLKP(ptr);
  } else {
    delete (NEXT_BLKP(ptr));
    delete (PREV_BLKP(ptr));
    size += GET_SIZE(HDRP(PREV_BLKP(ptr))) + GET_SIZE(FTRP(NEXT_BLKP(ptr)));
    PUT(HDRP(PREV_BLKP(ptr)), PACK(size, 0));
    PUT(FTRP(NEXT_BLKP(ptr)), PACK(size, 0));
    ptr = PREV_BLKP(ptr);
  }
  insert(ptr);
  return ptr;
}

/*
 * extend_heap - extend heap size by size, at least chunksize
 */
static void *extend_heap(size_t words) {
  char *bp;
  size_t size = ALIGN(words) * WSIZE;
  if ((long)(bp = mem_sbrk(size)) == -1)
    return NULL;
  PUT(HDRP(bp), PACK(size, 0));
  PUT(FTRP(bp), PACK(size, 0));
  PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));

  return coalesce(bp);
}

/*
 * find_fit - find fit block from segregated free block list
 */
static void *find_fit(size_t asize) {
  size_t i = gainoffset(asize);
  void *ptr;
  while (i < 20) {
    if ((ptr = *(listp + i)) != NULL) {
      while ((ptr != NULL) && (asize > GET_SIZE(HDRP(ptr)))) {
        ptr = GETNEXT(ptr);
      }
      if (ptr != NULL)
        return ptr;
    }
    i++;
  }
  return NULL;
}

/*
 * place - place the block pointed by input pointer with appropriate size.
 */
static void *place(void *bp, size_t asize) {
  size_t csize = GET_SIZE(HDRP(bp));
  delete (bp);
  if ((csize - asize) >= (2 * DSIZE)) {
    if (asize <= 72) {
      PUT(HDRP(bp), PACK(asize, 1));
      PUT(FTRP(bp), PACK(asize, 1));
      PUT(HDRP(NEXT_BLKP(bp)), PACK(csize - asize, 0));
      PUT(FTRP(NEXT_BLKP(bp)), PACK(csize - asize, 0));
      insert(NEXT_BLKP(bp));
    } else { // to obtain good utility, place small free block to the front.
      PUT(HDRP(bp), PACK(csize - asize, 0));
      PUT(FTRP(bp), PACK(csize - asize, 0));
      insert(bp);
      bp = NEXT_BLKP(bp);
      PUT(HDRP(bp), PACK(asize, 1));
      PUT(FTRP(bp), PACK(asize, 1));
    }
  } else {
    PUT(HDRP(bp), PACK(csize, 1));
    PUT(FTRP(bp), PACK(csize, 1));
  }
  return bp;
}

/*
 * mm_init - initialize the list and malloc package.
 */
int mm_init(void) {
  if ((listp = mem_sbrk(20 * WSIZE)) == (void *)-1)
    return -1;
  for (int i = 0; i < 20; i++)
    *(listp + i) = NULL;

  if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)-1)
    return -1;

  PUT(heap_listp, 0);
  PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));
  PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));
  PUT(heap_listp + (3 * WSIZE), PACK(0, 1));

  heap_listp += (2 * WSIZE);

  if (extend_heap(CHUNKSIZE / WSIZE) == NULL)
    return -1;
  return 0;
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size) {
  size_t asize;
  size_t extendsize;
  char *bp;
  if (size == 0)
    return NULL;

  asize = MAX(ALIGN(size + DSIZE), 16);

  if ((bp = find_fit(asize)) != NULL) {
    bp = place(bp, asize);
    return bp;
  }
  extendsize = MAX(asize, CHUNKSIZE);
  if ((bp = extend_heap(extendsize / WSIZE)) == NULL)
    return NULL;
  bp = place(bp, asize);
  return bp;
}

/*
 * mm_free - Freeing a block.
 */
void mm_free(void *ptr) {
  size_t size = GET_SIZE(HDRP(ptr));
  PUT(HDRP(ptr), PACK(size, 0));
  PUT(FTRP(ptr), PACK(size, 0));
  coalesce(ptr);
}

/*
 * mm_realloc - If the next block is free, unite them.
 */
void *mm_realloc(void *ptr, size_t size) {
  void *oldptr = ptr;
  void *newptr;
  size_t copySize, asize;
  copySize = GET_SIZE(HDRP(oldptr));
  size_t nsize = copySize + GET_SIZE(HDRP(NEXT_BLKP(ptr)));
  asize = MAX(ALIGN(size + DSIZE), 16);
  if (copySize == asize)
    return ptr;

  if ((nsize >= asize) && (!GET_ALLOC(HDRP(NEXT_BLKP(oldptr))))) {
    delete (NEXT_BLKP(oldptr));
    PUT(HDRP(oldptr), PACK(nsize, 1));
    PUT(FTRP(oldptr), PACK(nsize, 1));
    return ptr;
  }

  newptr = mm_malloc(size);
  if (newptr == NULL)
    return NULL;
  memcpy(newptr, oldptr, copySize);
  mm_free(oldptr);
  return newptr;
}