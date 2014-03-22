#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mpd.h"
#include "icons.h"

static struct process * mpd_pro = NULL;

char * getMusic(char * buf, const char * music_play, const char * music_stop)
{
  int tonewline = 0;
  const char * pos;
  char song[100] = "";
  int i;
#define SKIP(str) if (strncmp(pos, str, strlen(str)) == 0) pos += strlen(str)
#define SKIPEND(str) if (strncmp(pos+tonewline-strlen(str), (str), strlen(str)) == 0) tonewline -= strlen(str);
#define MIN(a,b) (((a)<(b))?(a):(b))

  if (!mpd_pro)
    mpd_pro = process_new("mpc");

  pos = process_run(mpd_pro);

  while(pos[tonewline] != '\n')
    tonewline++;

  /* First line index '0' to 'tonewline' */
  if (strncmp(pos, "volume: ", 8) != 0)
  {
    SKIP("NERadio Sweden - Best of Techno & Trance | www.neradio.se: ");
    SKIP("NERadio Hardstyle - www.HardStyle.nu: ");
    SKIP("DMusic/");
    SKIP("Hardcore - DIGITALLY IMPORTED - DJ mixes, hard dance and NuNRG!: ");
    SKIP("Classic EuroDance - DIGITALLY IMPORTED - Finest imported cheese on the net!: ");
    SKIP("Drum and Bass - DIGITALLY IMPORTED - tasty assortment to satisfy your drum and bass fix!: ");
    SKIP("Electro House - DIGITALLY IMPORTED - an eclectic mix of electro and dirty house: ");

    SKIPEND(".mp3");
    SKIPEND(".m4a");
    SKIPEND(".ogg");

    strncpy(song, pos, MIN(sizeof(song), tonewline));

    for (i = 0; i < strlen(song); i++)
      if (song[i] == '\n')
      {
        song[i] = '\0';
        break;
      }
  }

  sprintf(buf, "\\f%c%s %.100s", (song == NULL ? music_stop[0] : music_play[0]), getIcon("note"), (song[0] == '\0' ? "Stopped!" : song));
  return buf;
#undef SKIP
#undef MIN
}

char * getVolume(char * buf)
{
  int tonewline = 0;
  char * icon;
  const char * pos;
  int vol;

  if (!mpd_pro)
    mpd_pro = process_new("mpc");

  pos = process_run(mpd_pro);

  while(pos[tonewline] != '\n')
    tonewline++;

  /* First line index '0' to 'tonewline' */
  if (strncmp(pos, "volume: ", 8) != 0)
  {
    pos += tonewline + 1;

    /*Skips progress line*/
    while(*pos != '\n')
      pos++;

    pos++;
  }

  pos += strlen("volume:");
  vol = atoi(pos);

  if (vol == 0)
    icon = getIcon("vol_mute");
  else if (vol >= 75)
    icon = getIcon("vol_full");
  else
    icon = getIcon("vol_rest");

  sprintf(buf, "%s %3d%%", icon, vol);
  return buf;
}
