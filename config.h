#ifndef CONFIG_H_
#define CONFIG_H_
enum
{
  OVERLINE = 0,
  UNDERLINE = 1,

  TOP = 0,
  BOTTOM = 1,

  LEFT = 1,
  SCROLL = 2,
  RIGHT = 3
};


#define i int
#define ci const int
#define cu const unsigned
#define cs const char *
#define cf const float

/* Configuring starts here */

i bar_width = -1; /* The width of the bar. Set to -1 to fit screen */
i bar_height = 14; /* The height of the bar (in pixels) */

ci bar_offset = 0; /* Offset from the left. Set to 0 to have no effect */

ci bar_underline = UNDERLINE; /* Choose between an UNDERLINE or an OVERLINE */
ci bar_underline_height = 2; /* The thickness of the underline (in pixels). Set to 0 to disable. */
i bar_bottom = TOP; /* Bar position TOP or BOTTOM */

cs bar_font[2] =   /* The fonts used for the bar, comma separated. Only the first 2 will be used. */
{
  "-misc-stlarch-medium-r-normal--10-100-75-75-c-80-iso10646-1",
  "-lucy-tewi-medium-r-normal--11-90-*-*-*-*-iso10646-1"
};

ci bar_font_fallback_width = 6; /* Some fonts don't set the right width for some chars, pheex it */

/* Define the opacity of the bar (requires a compositor such as compton) */
cf bar_opacity = 0.95; /* 0 is invisible, 1 is opaque */

/* Color palette */
cu palette[] =
{
  0x2D3C46, /* color0 */
  0xAC4142, /* color1 */
  0x90A959, /* color2 */
  0xDE935F, /* color3 */
  0x6A9FB5, /* color4 */
  0xAA759F, /* color5 */
  0x75B5AA, /* color6 */
  0x6C7A80, /* color7 */
  0x425059, /* color8 */
  0xCC6666, /* color9 */
  0x232C31, /* background */
  0xC5C8C6 /* foreground */
};

ci mouse_button = LEFT; /* Mouse button to react to LEFT, SCROLL or RIGHT */

#undef cf
#undef cs
#undef cu
#undef ci
#undef i

#endif // CONFIG_H_
