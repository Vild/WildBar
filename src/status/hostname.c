#define _BSD_SOURCE
#include <unistd.h>
#include <stdio.h>

#include "hostname.h"

char * getHostName(char * buf)
{
  char tmp[1024] = "";
  gethostname(tmp, 1024);
  sprintf(buf, "%s", tmp);
  return buf;
}
