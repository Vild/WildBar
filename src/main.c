#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <unistd.h>

#include "wildbar.h"

int main(int argc, char ** argv)
{
  int ret = 0;
  char * line = NULL;
  char ch;
  int nobar = 0;

  while((ch = getopt(argc, argv, "hnbfc:")) != -1)
  {
    switch(ch)
    {
    case 'h':
      printf("usage: %s [-h] [-n] [-f] [-c CONFIG]\n"
             "\t-h Show this help.\n"
             "\t-n Prints out the data to the console instead of starting bar.\n"
             "\t-b Show the bar at the bottom of the screen.\n"
             "\t-f Force docking(use this if your WM isn't EWMH compliant).\n"
             "\t-c Loaded the config CONFIG instead of the default.", argv[0]);
      exit(0);

    case 'n':
      nobar = 1;
      break;

    case 'f':
      force_docking = 1;
      break;

    case 'b':
      bar_bottom = 1;
      break;

    case 'c':
      configFile = optarg;
      break;

    case '?':
      if (optopt == 'c')
        fprintf (stderr, "Option -%c requires an argument.\n", optopt);
      else if (isprint (optopt))
        fprintf (stderr, "Unknown option `-%c'.\n", optopt);
      else
        fprintf (stderr,
                 "Unknown option character `\\x%x'.\n",
                 optopt);

      return 1;

    default:
      return 1;
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
