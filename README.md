pr-as-planning-demo
===================

Demo for Plan Recognition as Planning over Classical Action Theories

Some quick and dirty instructions to build this project:

1) Clone the repository

https://github.com/miquelramirez/LAPKT-public

2) go into the folder lapkt-lib and edit the script SConstruct, changing the paths in lines 18-19 so they point to the location where you cloned LAPKT-public,
3) issue the command 'scons', this will produce the binaries for the planning algorithms needed by the demo,
4) go back into the root folder of the demo sources and edit line 10 of the  sim-home.pro file, changing the references to LAPKT-public as appropiate,
5) issue the command 'qmake', this will produce a working Makefile script,
6) build the app with make/
