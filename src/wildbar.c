#define __USE_XOPEN_EXTENDED
#define _BSD_SOURCE
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
#include "icons.h"

#define BUF_SIZE 8*1024
#define TMPBUF_SIZE 4*1024

static char * buf;
static char * tmpbuf;

static struct process * workspaces_pro;
static struct process * title_pro;
static struct process * mpd_pro;

static struct timeval last;

extern int bar_bottom;
extern int force_docking;
extern int bar_main();

static char * gethost(char * buf)
{
  char tmp[1024] = {0};
  gethostname(tmp, sizeof(1024));
  sprintf(buf, " \\f7"U_ARCHLOGO" \\f9 %s\\f7", tmp);
  return buf;
}

static char * getworkspaces(char * buf)
{
  const char * pos = process_run(workspaces_pro);
  char tmp[128] = "";
  int length = 0;

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

  if (*pos == '\0') pos--;

  while (*pos != '\0')
  {
    memset(tmp, 0, 128);

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
      //length += snprintf(buf+length, TMPBUF_SIZE-length,  " \\u3\\br\\fr%s\\ur", tmp);
      break;


    case 'O':
      pos++;
      AddStr
      length += snprintf(buf+length, TMPBUF_SIZE-length, " \\u3\\br\\fr%s\\ur", tmp);
      break;

    case 'o':
      pos++;
      AddStr
      length += snprintf(buf+length, TMPBUF_SIZE-length, " \\br\\fr%s", tmp);
      break;

    case 'F':
      pos++;
      AddStr
      length += snprintf(buf+length, TMPBUF_SIZE-length, " \\u4\\br\\f7%s\\ur", tmp);
      break;

    case 'f':
      pos++;
      AddStr
      length += snprintf(buf+length, TMPBUF_SIZE-length, " \\br\\f7%s", tmp);
      break;

    case 'U':
      pos++;
      AddStr
      length += snprintf(buf+length, TMPBUF_SIZE-length, " \\u9\\br\\fr%s\\ur", tmp);
      break;

    case 'u':
      pos++;
      AddStr
      length += snprintf(buf+length, TMPBUF_SIZE-length, " \\b1\\fr%s", tmp);
      break;

    case 'L':
      pos++;
      AddStr
      length += snprintf(buf+length, TMPBUF_SIZE-length, " \\f3M:%c\\fr", toupper(tmp[0]));
      break;
    }

    pos++;
  }

#undef AddStr

  return buf;
}

static char * gettitle(char * buf)
{
  sprintf(buf, "%s", process_run(title_pro));
  return buf;
}

static char * getmpd(char * buf)
{
  int tonewline = 0;
  char * icon;
  char * note = "♪";
  const char * pos;
  const char * song = NULL;
  int vol;
  pos = process_run(mpd_pro);

  while(pos[tonewline] != '\n')
    tonewline++;

  /* First line index '0' to 'tonewline' */
  if (strncmp(pos, "volume: ", 8) != 0)
  {
    song = pos;
    pos += tonewline + 1;

    while(*pos != '\n')
      pos++;

    pos++;
  }

  pos += strlen("volume:");
  vol = atoi(pos);

  if (vol == 0)
    icon = U_VOL_MUTE;
  else if (vol >= 75)
    icon = U_VOL_FULL;
  else
    icon = U_VOL_REST;

  sprintf(buf, "%s%s %.40s \\f4|\\f7 \\f2%s %3d%%\\f7", (song == NULL ? "\\f9" : "\\f3"), note, (song == NULL ? "Stopped!" : song), icon, vol);
  return buf;
}

static char * getram(char * buf)
{
  char * pos;
  long total, free;
  FILE * fp = fopen("/proc/meminfo", "r");
  fread(buf, 4*1024, 1, fp);

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

  sprintf(buf, "\\f2"U_RAM" %3d%%\\f7", 100-((int)floor(((free*1.0)/(total*1.0))*100)));
  return buf;
}

static char * getcpu(char * buf)
{
  //double avg[4] = {0., 0., 0., 0.};
  //getloadavg(avg, 1);
  //process_run(cpu_pro)
  //if (cpu == 1)
  //printf("\\f1");
  sprintf(buf, "\\f2"U_CPU" %3s%%\\f7", "UNK");
  return buf;
}

static char * gettime(char * buf)
{
  time_t rawtime;
  struct tm * timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  sprintf(buf, "\\f6"U_TIME" %s\\f7 ", asctime(timeinfo));

  if (buf[strlen(buf) - 1] == '\n')
    buf[strlen(buf) - 1] = '\0';

  return buf;
}

static int diff_ms(struct timeval t1, struct timeval t2)
{
  return (((t1.tv_sec - t2.tv_sec) * 1000000) +
          (t1.tv_usec - t2.tv_usec)) / 1000;
}

#define _n(s, ...) length += snprintf(buf+length, BUF_SIZE-length, (" "s), __VA_ARGS__)
#define _(s, ...) length += snprintf(buf+length, BUF_SIZE-length, (" \\f4|\\f7 "s), __VA_ARGS__)

char * wildbar_getLine()
{
  int length = 0;
  struct timeval now;
  gettimeofday(&now, 0);


  if (diff_ms(now, last) < 50)
    return NULL;

  gettimeofday(&last, 0);

  _n("\\s1\\l%s", "");
  _n("\\ab%s\\ac/home/wildn00b/.config/bspwm/scripts/spawn_dmenu\\ae", gethost(tmpbuf));
  _n(" %s", getworkspaces(tmpbuf));

  _n("\\c %s", gettitle(tmpbuf));

  _n("\\r%s", "");

  _n("\\ab%s\\ac/home/wildn00b/.config/bspwm/scripts/toggle_conky mpd\\ae", getmpd(tmpbuf));
  _("\\ab%s\\ac/home/wildn00b/.config/bspwm/scripts/toggle_conky mem\\ae", getram(tmpbuf));

  _("\\ab%s\\ac/home/wildn00b/.config/bspwm/scripts/toggle_conky cpu\\ae", getcpu(tmpbuf));
  _("\\ab%s\\ac/home/wildn00b/.config/bspwm/scripts/toggle_conky cal\\ae", gettime(tmpbuf));

  return buf;
}
#undef _

void wildbar_init()
{
  buf = mem_alloc(BUF_SIZE);
  tmpbuf = mem_alloc(TMPBUF_SIZE);
  //cpu_pro = process_new("top -bn2 | grep 'Cpu(s)' | sed 's/.*, *\\([0-9.]*\\)%* id.*/\\1/' | awk '{print 100 - $1}' | awk '{getline; print $1}' | xargs");
  workspaces_pro = process_new("bspc control --get-status");
  title_pro = process_new("xtitle");
  mpd_pro = process_new("mpc");
}

void wildbar_cleanup()
{
  process_free(mpd_pro);
  process_free(title_pro);
  process_free(workspaces_pro);
  //process_free(cpu_pro);

  mem_free(tmpbuf);
  mem_free(buf);

  mem_freeleaks();
}
