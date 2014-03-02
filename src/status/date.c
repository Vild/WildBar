#include <time.h>
#include <stdio.h>

#include "date.h"
#include "icons.h"

char * getDate(char * buf)
{
  time_t rawtime;
  struct tm * timeinfo;
  char tmp[1024];

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(tmp, 1024, "%a %b %d %H:%M:%S", timeinfo); // %Y

  sprintf(buf, "%s %s", getIcon("time"), tmp);

  return buf;
}
