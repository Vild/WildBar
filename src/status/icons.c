#include <stdio.h>
#include <string.h>

#include "icons.h"

char * getIcon(const char * icon)
{
  if (strcmp(icon, "arch") == 0)
    return "";
  else if (strcmp(icon, "ip") == 0)
    return "";
  else if (strcmp(icon, "ram") == 0)
    return "";
  else if (strcmp(icon, "cpu") == 0)
    return "";
  else if (strcmp(icon, "time") == 0)
    return "";
  else if (strcmp(icon, "note") == 0)
    return "♪";
  else if (strcmp(icon, "vol_mute") == 0)
    return "";
  else if (strcmp(icon, "vol_full") == 0)
    return "";
  else if (strcmp(icon, "vol_rest") == 0)
    return "";

  else
    return "X";
}
