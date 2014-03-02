#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"

struct allocation * first = NULL;

static void put(struct allocation * alloc)
{
  struct allocation * next = first;

  if (first == NULL)
    first = alloc;
  else
  {
    while (next->next != NULL)
      next = next->next;

    next->next = alloc;
  }
}

void * mem_alloc_(int size, const char * file, const char * function, int line)
{
  struct allocation * alloc = (struct allocation *)calloc(sizeof(struct allocation), 1);
  assert(alloc != NULL);

  alloc->memory = malloc(size);
  assert(alloc->memory != NULL);
  memset(alloc->memory, 0, size);

  alloc->size = size;

  alloc->file = file;
  alloc->function = function;
  alloc->line = line;
  alloc->next = NULL;

  put(alloc);
  return alloc->memory;
}

void mem_free_(void * memory, const char * file, const char * function, int line)
{
  struct allocation * prev = NULL;
  struct allocation * cur = first;

  assert(memory != NULL);

  while (cur != NULL)
  {
    if (cur->memory == memory)
    {
      if (prev == NULL)
        first = cur->next;
      else
        prev->next = cur->next;

      free(memory);
      free(cur);
      return;
    }

    prev = cur;
    cur = cur->next;
  }

  fprintf(stderr, "Trying to unallocate memory we didn't allocate: %p!  Freeing it for anyway, expect unknown behavior! @ %s:%s:%i\n", memory, file, function, line);

  /* We did not allocate this memory! Freeing it for anyway, expect unknown behavior! */
  free(memory);
}

char * mem_strdup(const char * str)
{
  char * mem;
  int len = strlen(str) + 1;
  mem = mem_alloc(len);
  snprintf(mem, len, "%s", str);
  return mem;
}

void mem_freeleaks(int quiet)
{
  struct allocation * cur = first;

  while (cur != NULL)
  {
    if (!quiet)
      fprintf(stderr, "MEMORY LEAK %p:%i @ %s:%s:%i\n", cur->memory, cur->size, cur->file, cur->function, cur->line);

    /* Moves the first element to the next one */
    first = cur->next;

    free(cur->memory);
    free(cur);

    /* Because we set the next allocation to 'first' before freeing */
    cur = first;
  }
}

