# ChangeLog for version 1.5 #

## Version 1.05.15 ##
_Web, 19 Mar 2014 02:20:00 +0400_

  * Merge changes from 1.4.11
    * Fixed signal handling - use socketpair for every listening signal...

## Version 1.05.14 ##
_Sun, 16 Mar 2014 01:34:00 +0400_

  * Update project build files
  * Rewrite unix signal handling
  * Add confguration options for actions on USR1 and USR2 signals

## Version 1.05.13 ##
_Sun, 19 Jan 2014 19:15:00 +0400_

  * Fix turning off text selection after next page load
  * Add page load indication with progress bar
  * Add option to scale pages

## Version 1.05.12 ##
_Sat, 18 Jan 2014 21:44:00 +0400_

  * Fix styles and scripts inserting into page after it's loaded
  * Add option to disable text selection

## Version 1.05.11 ##
_Sat, 18 Jan 2014 19:02:00 +0400_

  * Fix scroll to the bottom of page

## Version 1.05.10 ##
_Sat, 18 Jan 2014 17:45:00 +0400_

  * Add new hotkeys for page scrolling
  * Add new options:
    * hide scroll bars
    * window on top
    * disable hotkeys

## Version 1.05.09 ##
_Sun, 12 Jan 2014 21:16:00 +0400_

  * Add startup delay for window resize and page load
  * Add minimum window size options

## Version 1.05.08 ##
_Thu, 23 Jul 2013 10:46:00 +0400_

  * Qt project files clean up and fix

## Version 1.05.07 ##
_Mon, 22 Jul 2013 22:57:00 +0400_

  * Fix handle link click and sound play
  * New sounds

## Version 1.05.06 ##
_Sun, 21 Jul 2013 23:05:00 +0400_

  * Fix handle link click and sound play
  * Fix handle window.close()

## Version 1.05.05 ##
_Sat, 06 Jul 2013 03:31:42 +0400_

  * Merged changes from 1.04.04
  * Fix segfault on exit
  * Disable printing by default

## Version 1.05.04 ##
_Sat, 06 Jul 2013 00:09:43 +0400_

  * Handle SIGTERM signals
  * Cache cleanup on start or exit
  * New commandline option -C
  * Fix cache use - tune request objects

## Version 1.05.03 ##
_Fri, 05 Jul 2013 12:12:17 +0400_

  * Use fake webview to catch new window URL
  * Disable console output

## Version 1.05.02 ##
_Fri, 05 Jul 2013 06:18:22 +0400_

  * Add support of opening links from new windows into main
  * Add support of ignoring SSL errors

## Version 1.05.01 ##
_Fri, 05 Jul 2013 00:53:17 +0400_

## Version 1.04.01 ##
_Fri, 05 Jul 2013 00:09:17 +0400_

  * New hot keys - page reload, developer tools
  * Add sound file for windows (wav)
  * Add sound event for link click
  * Put user scripts into bottom of page

## Version 1.03.01 ##
_Thu, 19 Apr 2012 23:03:00 +0400_

  * Fixed build version number
  * Skip empty scripts and styles attach lists
  * User config stored in user home directory:  ~/.config/QtWebkitKiosk/config.ini

## Version 1.03.00 ##
_Thu, 19 Apr 2012 00:36:00 +0400_

  * Remove debug output in release build
  * Attach user scripts and styles when page loaded

## Version 1.02.01 ##
_Thu, 18 Apr 2012 22:10:00 +0400_

  * Fixed startup issue with only one command prompt parameter defined.

## Version 1.02.00 ##
_Thu, 17 Apr 2012 23:28:00 +0400_

  * Fixed startup issue with only one command prompt parameter defined.
  * Fixed issue with window size and position when screen resolution changed.
  * Fixed build issues on older than Ununtu 11.10 systems.

## Version 1.01.00 ##
_Mon, 12 Dec 2011 02:36:28 +0400_

  * Initial Release.