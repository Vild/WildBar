#ifndef PROCESS_H_
#define PROCESS_H_

struct process
{
  char * exec;
  char * buf;
};

struct process * process_new(char * exec);
void process_free(struct process * process);

const char * process_run(struct process * process);
const char * process_getbuf(struct process * process);

/* Possible to change in the future */
#define process_getbufsize(p) (1*1024*1024)

#endif // PROCESS_H_

