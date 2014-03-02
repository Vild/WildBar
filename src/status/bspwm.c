#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bspwm.h"

static struct process * bspc_pro = NULL;

char * getWorkspaces(char * buf, int monitor, const char * p_wof, const char * p_wou, const char * p_wff, const char * p_wfu, const char * p_wuf, const char * p_wuu)
{
  const char * pos;
  char tmp[128] = "";
  int length = 0;
  int done = 0;

  int i = 0;

  if (!bspc_pro)
    bspc_pro = process_new("bspc control --get-status");

  pos =  process_run(bspc_pro);

  if (pos == NULL || strlen(pos) == 0)
  {
    snprintf(buf, TMPBUF_SIZE, "\\u3Need a newer bspwm version\\ur");
    return buf;
  }

#define AddStr while (*pos != ':' && *pos != '\0' && *pos != '\n') {\
                  tmp[strlen(tmp)] = *pos; \
                  pos++; \
                } \
                if (*pos == '\0') pos--;
#define _(...) length += snprintf(buf+length, TMPBUF_SIZE-length, __VA_ARGS__)
#define color(colors) _("\\f%c\\b%c\\u%c", (colors)[0], (colors)[1], (colors)[2])
#define colorreset() _("\\fr\\br\\ur")

  if (*pos == '\0') pos--;

  while (*pos != '\0')
  {
    memset(tmp, 0, 128);

    length += snprintf(buf+length, TMPBUF_SIZE-length, "\\fr\\br\\ur");
    switch (*pos)
    {
    default:
      fprintf(stderr, "getworkspaces: Get unimplemented char: %c - %X\n", *pos, (int)*pos);
      break;

    case 'W': /* bspwm status_prefix */
      break;

    case 'M': /* Monitor*/
    case 'm':
      pos++;
      AddStr

      if (done == monitor)
        return buf;

      done++;
      length = 0;
      i = 0;

      break;

    case 'O':
      pos++;
      AddStr
      _(" ");
      color(p_wof);
      _("%s", tmp);
      colorreset();
      break;

    case 'o':
      pos++;
      AddStr
      _(" ");
      color(p_wou);
      _("%s", tmp);
      colorreset();
      break;

    case 'F':
      pos++;
      AddStr
      _(" ");
      color(p_wff);
      _("%s", tmp);
      colorreset();
      break;

    case 'f':
      pos++;
      AddStr
      _(" ");
      color(p_wfu);
      _("%s", tmp);
      colorreset();
      break;

    case 'U':
      pos++;
      AddStr
      _(" ");
      color(p_wuf);
      _("%s", tmp);
      colorreset();
      break;

    case 'u':
      pos++;
      AddStr
      _(" ");
      color(p_wuu);
      _("%s", tmp);
      colorreset();
      break;

    case 'L':
      pos++;
      AddStr
      //_(" \\f3M:%c", toupper(tmp[0]));
      break;
    }

    pos++;
    i++;
  }

#undef colorreset
#undef color
#undef _
#undef AddStr

  length += snprintf(buf+length, TMPBUF_SIZE-length, "\\fr\\br\\ur");
  return buf;
}
