#ifndef WILDBAR_H_
#define WILDBAR_H_

#define BUF_SIZE 8*1024
#define TMPBUF_SIZE 4*1024

/* Bar */
int bar_main();
extern int force_docking;

/* WildBar */
void wildbar_init();
void wildbar_cleanup();
char * wildbar_getLine();
extern char * configFile;

#endif // WILDBAR_H_
