#ifndef WILDBAR_H_
#define WILDBAR_H_

/* Bar */
int bar_main();
extern int force_docking;

/* WildBar */
void wildbar_init();
void wildbar_cleanup();
char * wildbar_getLine();

#endif // WILDBAR_H_
