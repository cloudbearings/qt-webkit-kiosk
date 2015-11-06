# Introduction #

  1. Download sources archive
  1. You'll need to install the developer tools and libraries for Qt, QtWebkit and Phonon.

## Ubuntu 14.04 ##

  1. `sudo apt-get install build-essential devscripts fakeroot`
  1. `sudo apt-get install qtbase5-dev libqt5webkit5-dev qtmultimedia5-dev qtdeclarative5-dev libqt5v8-5-dev qtsystems5-dev qtscript5-dev libpulse-dev libsqlite3-dev`

## Ubuntu 12.04+ ##

  1. `sudo apt-get install build-essential devscripts fakeroot`
  1. `sudo apt-get install libqt4-dev libqtwebkit-dev libphonon-dev`

## Ubuntu 10.04 ##

  1. Add Kubuntu PPA: `sudo add-apt-repository ppa:kubuntu-ppa/backports`
  1. `sudo apt-get update`
  1. `sudo apt-get install build-essential devscripts fakeroot`
  1. `sudo apt-get install libqt4-dev libqtwebkit-dev libphonon-dev`

## Create package ##

  1. Unpack sources: `tar -xf qt-webkit-kiosk-<version>.tar.gz`
  1. `cd qt-webkit-kiosk-<version>`
  1. `debuild -us -uc`
  1. **Done**