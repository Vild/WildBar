#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "wildbar.h"

int main(int argc, char ** argv)
{
  int ret = 0;
  char * line = NULL;
  char ch;
  int nobar = 0;

  while((ch = getopt(argc, argv, "hnbf")) != -1)
  {
    switch(ch)
    {
    case 'h':
      printf("usage: %s [-h | -n] [-f]\n"
             "\t-h Show this help\n"
             "\t-n Prints out the data to the console instead of starting bar\n"
             "\t-f Force docking(use this if your WM isn't EWMH compliant)\n", argv[0]);
      exit(0);

    case 'n':
      nobar = 1;
      break;
    case 'f':
      force_docking = 1;
      break;
    }
  }

  wildbar_init();

  if (nobar)
    while (1)
    {
      line = wildbar_getLine();

      if (line != NULL)
        fprintf(stdout, "%s\n", line);
    }
  else
    ret = bar_main();

  wildbar_cleanup();
  return ret;
}
