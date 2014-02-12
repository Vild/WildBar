#ifndef MEMORY_H_
#define MEMORY_H_

#include <stdlib.h>

struct allocation
{
  void * memory;
  int size;

  const char * file;
  const char * function;
  int line;

  struct allocation * next;
};

#define mem_alloc(s) mem_alloc_((s), __FILE__, __FUNCTION__, __LINE__)
#define mem_free(p) mem_free_((void *)(p))

/* s as in struct */
#define mem_salloc(struct) (struct *)mem_alloc(sizeof(struct))

void * mem_alloc_(int size, const char * file, const char * function, int line);
void mem_free_(void * memory);

char * mem_strdup(char * str);

void mem_freeleaks();

#endif // MEMORY_H_

