# ChangeLog for version 1.4 #

## Version 1.04.11 ##
_Wed, 19 Mar 2014 01:59:00 +0400_

  * Fix unix signal handling - use socketpair for every listening signal...

## Version 1.04.10 ##
_Sun, 16 Mar 2014 03:37:00 +0400_

  * Merge changes from 1.05.14:
    * Update project build files
    * Rewrite unix signal handling
    * Add confguration options for actions on USR1 and USR2 signals

## Version 1.04.09 ##
_Sun, 19 Jan 2014 18:07:00 +0400_

  * Merge changes from 1.05.13:
    * Fix turning off text selection after next page load
    * Add page load indication with progress bar
    * Add option to scale pages

## Version 1.04.08 ##
_Sat, 18 Jan 2014 22:11:00 +0400_

  * Merge changes from 1.05.12:
    * Fix styles and scripts inserting into page after it's loaded
    * Add option to disable text selection

## Version 1.04.07 ##
_Sat, 18 Jan 2014 18:30:00 +0400_

  * Fix scroll to the bottom of page
  * Merge changes from 1.05.10:
    * startup delay for window resize and page load
    * add minimum window size options
    * add hotkeys for page scroll
    * add new options:
      * window on top
      * hide scrollbars (and disable scroll by mouse)
      * disable hotkeys

## Version 1.04.06 ##
_Thu, 23 Jul 2013 10:09:00 +0400_

  * Qt project files clean up and fix

## Version 1.04.05 ##
_Mon, 22 Jul 2013 23:35:00 +0400_

  * Merge changes from 1.05.06-1.05.07:
> > + Fix handle link click and sound play
> > + Fix handle window.close()
> > + New sounds
> > + Handle window.close()

## Version 1.04.04 ##
_Sat, 06 Jul 2013 03:18:33 +0400_

  * Fix segfault on exit
  * Disable printing by default

## Version 1.04.03 ##
_Sat, 06 Jul 2013 00:37:36 +0400_

  * Merge changes from 1.05.04:
  * Handle SIGTERM signals
  * Cache cleanup on start or exit
  * New commandline option -C
  * Fix cache use - tune request objects

## Version 1.04.02 ##
_Fri, 05 Jul 2013 21:59:17 +0400_

  * Merge changes from 1.05:
  * Handle new window links
  * Separate sounds player class
  * Handle SSL errors

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
  * User config stored in user home directory: ~/.config/QtWebkitKiosk/config.ini

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