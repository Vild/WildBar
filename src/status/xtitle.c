#include <stdlib.h>
#include <stdio.h>

#include "xtitle.h"

static struct process * xtitle_pro = NULL;

char * getTitle(char * buf)
{
  if (!xtitle_pro)
    xtitle_pro = process_new("xtitle");

  sprintf(buf, "%s", process_run(xtitle_pro));
  return buf;
}
