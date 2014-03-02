#ifndef MEMORY_H_
#define MEMORY_H_

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
#define mem_free(p) mem_free_((void *)(p), __FILE__, __FUNCTION__, __LINE__)

/* s as in struct */
#define mem_salloc(struct) (struct *)mem_alloc(sizeof(struct))

void * mem_alloc_(int size, const char * file, const char * function, int line);
void mem_free_(void * memory, const char * file, const char * function, int line);

char * mem_strdup(const char * str);

void mem_freeleaks(int quiet);

#endif // MEMORY_H_

