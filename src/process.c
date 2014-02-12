#define _BSD_SOURCE
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "process.h"
#include "memory.h"

struct process * process_new(char * exec)
{
  struct process * process = mem_salloc(struct process);

  process->exec = mem_strdup(exec);
  process->buf = mem_alloc(process_getbufsize());
  return process;
}

void process_free(struct process * process)
{
  mem_free(process->buf);
  mem_free(process->exec);
  mem_free(process);
}

const char * process_run(struct process * process)
{
  FILE * p = NULL;
  int length = 0;
  char * buf = process->buf;
  memset(buf, 0, process_getbufsize());
  p = popen(process->exec, "r");

  fread(buf, process_getbufsize(), 1, p);

  /* Makes the buffer valid for strlen and removed any characters that can make the parser fail */

  buf[process_getbufsize()] = '\0';

  length = strlen(buf);

  if (buf[length - 1] == '\n')
    buf[length - 1] = '\0';

  pclose(p);
  return buf;
}

const char * process_getbuf(struct process * process)
{
  return process->buf;
}
