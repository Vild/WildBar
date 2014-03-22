Wildbar - based on b(ar) a(in't) r(ecursive) by The Lemon man
=============================================================

License
=======
src/bar/* - 2012-2013 (C) The Lemon Man       - MIT License   - See LICENSE.bar

src/*     - 2014 (C) Dan "WildN00b" Printzell - GPLv3 License - See LICENSE.wildbar


Description
-----------

Bar
===
A lightweight bar based on XCB (yay). Provides foreground/background color
switching along with text alignment (screw you dzen!), full utf8 support
and reduced memory footprint. It also supports transparency when using a
compositor such as compton. Nothing less and nothing more.

WildBar
=======
Added multiple of different data providers and a json based config which you when you save get instantly reloaded.

Data providers:
---------------
```
text       - Prints out the text in "value".
  value      - The text.
icon       - Prints out the icon with the name "value".
  value      - The icon name.
split      - Prints out a vertical line.


workspaces - Prints out the all the workspaces (Currently only bspwm).
	value              - The screen to take the workspaces from.
  occupied_focused   - What color the the workspace should be when it is occupied and focused (format: FBU, Foreground Background Underline, example "rr3").
  occupied_unfocused - Same as above but for occupied_unfocused.
  free_focused       - Same as above but for free_focused.
  free_unfocused     - Same as above but for free_unfocused.
  urgent_focused     - Same as above but for urgent_focused.
  urgent_unfocused   - Same as above but for urgent_unfocused.
date       - Prints out the current time and date.
hostname   - Prints out the hostname.
music      - Prints out the current playing song from mpd (Needs mpc).
  play               - The text color when a song is playing (Optional).
  stop               - The text color when a song is stopped (Optional).
volume     - Prints out the current volume from mpd (Needs mpc).
title      - Prints out the selected windows title (Needs xtitle).
```

Every setting supports the following settings:
```
onclick   - What command to run when you click on the text.
fcolor    - The text color.
bcolor    - The background color.
ucolor    - The underline color.
```

Wildbar.conf
------------
```
structure - The structure for the bar.
  screens   - Array of all the screens. The first element is for the first screen, the second for the second screen etcetera, etcetera..
    left      - Array of all the data providers, align to the left.
      ...       - All the data providers.
    center    - Array of all the data providers, align to the left.
      ...       - All the data providers.
    right     - Array of all the data providers, align to the left.
      ...       - All the data providers.
```

Example of using a data provider:
```
{
  "type" : "text",
  "value" : "Text",
  "onclick" : "echo Text",
  "fcolor" : "4",
  "bcolor" : "0",
  "ucolor" : "2"
}
```

A simple config to show the current workspaces, title of selected program and the time:
```
{
	"structure" : {
		"screens" : [
			{
				"left" : [
					{
						"type" : "workspaces",
						"value" : "1"
					}
				],
				"center" : [
					{
						"type" : "title"
					}
				],
				"right" : [
					{
						"type" : "date"
					}
				]
			}
		]
	}
}
```


See wildbar.conf for a in use example.

Options
-------
WildBar accepts a couple of command line switches.

```
Usage: wildbar [-h] [-n] [-f] [-c CONFIG]
  -h      Show the help.
  -n      Prints out the data to the console instead of starting bar.
  -b      Show the bar at the bottom of the screen.
  -b      Show the bar at the bottom of the screen.
  -f      Force docking (use this if your WM isn't EWMH compliant).
  -c      Loaded the config CONFIG instead of the default.
```

Configuration
-------------
Change the config.h file to change the basics settings like colors.
The settings for what to write on the bar is in the wildbar.conf file, typically located in /etc/wildbar/wildbar.conf.
