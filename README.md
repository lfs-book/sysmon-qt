# Qkrellm
A Qt version of gkrellm.

There are several ways to monitor the system performace on a linux system.
This includes console based applications like top, htop, free, df, and ps. 

For graphical applications there are desktop based programs including 
gnome-system-monitor, qjs (from LXQt), plasma-systemmonitor.

The problem with the above programs is that they take up a lot of screen 
real estate.  A grapical program that can run continuously but can be 
configured to use a small amount of screen space is GKrellM. 
See http://gkrellm.srcbox.net/

GKrellM is a powerful application, but it no longer seems to be maintained.
It is is gtk2 based and that has reached its end-of-life.  There have been
no substantial changes to the program since 2016.

Qkrellm is a program similar to GKrellM but based on Qt5.  It does not
have all the capabilities of GKrellM, but provides the capability of 
monitoring CPU usage, memory use, and system temperatures.  It is also 
highly configurable with the ability choose which items to monitor and
what colors and fonts to use.

