#define __USE_XOPEN_EXTENDED
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>
#include <sys/time.h>
#include <ctype.h>

#include "wildbar.h"

#include "memory.h"
#include "process.h"
#include "config.h"

#include "status/bspwm.h"
#include "status/date.h"
#include "status/hostname.h"
#include "status/icons.h"
#include "status/mpd.h"
#include "status/xtitle.h"

static char * buf;
static char * tmpbuf;

char * configFile = NULL;
static struct config * config;

/*static struct process * cpu_pro;*/

static struct timeval last;

extern int bar_bottom;
extern int force_docking;
extern int bar_main();

/*static char * getram(char * buf)
{
  char * pos;
  long total, free;
  FILE * fp = fopen("/proc/meminfo", "r");
  fread(buf, TMPBUF_SIZE, 1, fp);

  fclose(fp);

  pos = buf;

  while(*pos != ':')
    pos++;

  pos++;
  total = atol(pos);

  while(*pos != ':')
    pos++;

  pos++;
  free = atol(pos);

  sprintf(buf, "\\f2"U_RAM" %3d%%\\f7", (int)round(((free*1.0)/(total*1.0))*100));
  return buf;
}

static char * getcpu(char * buf)
{
  sprintf(buf, "\\f2"U_CPU" %3s%%\\f7", process_run(cpu_pro));
  return buf;
}*/


static int diff_ms(struct timeval t1, struct timeval t2)
{
  return (((t1.tv_sec - t2.tv_sec) * 1000000) +
          (t1.tv_usec - t2.tv_usec)) / 1000;
}

#define _(...) length += snprintf(buf+length, BUF_SIZE-length, __VA_ARGS__)
#define l() length += snprintf(buf+length, BUF_SIZE-length, " \\f4|\\f7 ")
#define s() length += snprintf(buf+length, BUF_SIZE-length, " ")

char * wildbar_getLine()
{
  const char * POS[3] = {"left", "center", "right"};
  const int POS_LENGTH = 3;

  json_t * structure, * screens, * position, * value, * pos_value;

  size_t index, pos_index;
  int length = 0;
  int i = 0;
  struct timeval now;
  gettimeofday(&now, 0);

  int diff = diff_ms(now, last);
  if (diff < 50)
    usleep(diff * 1000);

  gettimeofday(&last, 0);

  /*_("\\s1\\l%s", "");
  _("\\ab%s\\ac/home/wildn00b/.config/bspwm/scripts/spawn_dmenu\\ae", gethost(tmpbuf));
  _(" %s", getworkspaces(tmpbuf, 1));

  _("\\c %s", gettitle(tmpbuf));

  _("\\r%s", "");

  _("\\ab%s\\ac/home/wildn00b/.config/bspwm/scripts/toggle_conky mpd\\ae", getmpd(tmpbuf));
  //TODO: Implement working memory and cpu code
  /s();
  _("\\ab%s\\ac/home/wildn00b/.config/bspwm/scripts/toggle_conky mem\\ae", getram(tmpbuf));
  s();
  _("\\ab%s\\ac/home/wildn00b/.config/bspwm/scripts/toggle_conky cpu\\ae", getcpu(tmpbuf));/
  l();
  _("\\ab%s\\ac/home/wildn00b/.config/bspwm/scripts/toggle_conky cal\\ae", gettime(tmpbuf));

  _("\\s0\\l%s", "");
  _(" %s", getworkspaces(tmpbuf, 2));

  _("\\r%s", "");
  _("\\ab%s\\ac/home/wildn00b/.config/bspwm/scripts/toggle_conky cal\\ae", gettime(tmpbuf));*/

  structure = config_getStructure(config);

  screens = json_object_get(structure, "screens");
  json_array_foreach(screens, index, value)
  {
    _("\\s%d", (int)index);

    for (i = 0; i < POS_LENGTH; i++)
    {
      position = json_object_get(value, POS[i]);

      if (position)
      {
        _("\\%c", POS[i][0]);
        _(" ");
        json_array_foreach(position, pos_index, pos_value)
        {
          json_t * tmp;
          const char * p_type;
          tmp = json_object_get(pos_value, "type");

          if (tmp)
            p_type = json_string_value(tmp);
          else
            continue;

#define GetValue(var, name, default) tmp = json_object_get(pos_value, name);\
                                                  const char * var;\
                                                  if (tmp)\
                                                    var = json_string_value(tmp);\
                                                  else\
                                                    var = default


          GetValue(p_value, "value", "MISSING VALUE");
          GetValue(p_onclick, "onclick", NULL);

          GetValue(p_fcolor, "fcolor", NULL);
          GetValue(p_bcolor, "bcolor", NULL);
          GetValue(p_ucolor, "ucolor", NULL);

          GetValue(p_mplay, "play", "3");
          GetValue(p_mstop, "stop", "9");

          GetValue(p_wof, "occupied_focused", "rr3");
          GetValue(p_wou, "occupied_unfocused", "rrr");
          GetValue(p_wff, "free_focused", "7r4");
          GetValue(p_wfu, "free_unfocused", "7rr");
          GetValue(p_wuf, "urgent_focused", "rr9");
          GetValue(p_wuu, "urgent_unfocused", "03r");

          if (p_onclick)
            _("\\ab");

          if (p_fcolor)
            _("\\f%c", p_fcolor[0]);

          if (p_bcolor)
            _("\\b%c", p_bcolor[0]);

          if (p_ucolor)
            _("\\u%c", p_ucolor[0]);

          if (strcmp(p_type, "text") == 0)
            _("%s", p_value);
          else if (strcmp(p_type, "icon") == 0)
            _("%s", getIcon(p_value));
          else if (strcmp(p_type, "split") == 0)
            _("\\f4|\\f7");
          else if (strcmp(p_type, "workspaces") == 0)
          {
            int screen = 0;
            screen = atoi(p_value);
            _("%s", getWorkspaces(tmpbuf, screen, p_wof, p_wou, p_wff, p_wfu, p_wuf, p_wuu));
          }
          else if (strcmp(p_type, "date") == 0)
            _("%s", getDate(tmpbuf));
          else if (strcmp(p_type, "hostname") == 0)
            _("%s", getHostName(tmpbuf));
          else if (strcmp(p_type, "music") == 0)
            _("%s", getMusic(tmpbuf, p_mplay, p_mstop));
          else if (strcmp(p_type, "volume") == 0)
            _("%s", getVolume(tmpbuf));
          else if (strcmp(p_type, "title") == 0)
            _("%s", getTitle(tmpbuf));
          else
          {
            _("UNKNOWN TYPE: '%s' VALUE: '%s' ONCLICK: '%s'", p_type, p_value, p_onclick);
          }

          if (p_ucolor)
            _("\\ur");

          if (p_bcolor)
            _("\\br");

          if (p_fcolor)
            _("\\fr");

          if (p_onclick)
            _("\\ac%s\\ae", p_onclick);

          _(" ");
        }

      }
    }
  }
  return buf;
}
#undef l
#undef _

void wildbar_init()
{
  buf = mem_alloc(BUF_SIZE);
  tmpbuf = mem_alloc(TMPBUF_SIZE);

  if (configFile)
  {
    config = config_new(configFile);

    if (!config)
      config = config_new("");
  }
  else
  {
    config = config_new("wildbar.conf");

    if (!config)
      config = config_new("/etc/wildbar/wildbar.conf");

    if (!config)
      config = config_new("/etc/wildbar.conf");

    if (!config)
      config = config_new("");
  }

  gettimeofday(&last, 0);
}

void wildbar_cleanup()
{
  /*process_free(mpd_pro);
  process_free(title_pro);
  process_free(workspaces_pro);*/

  config_free(config);

  mem_free(tmpbuf);
  mem_free(buf);

  mem_freeleaks(0);
}
