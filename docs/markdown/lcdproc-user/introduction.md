# Introduction

# About this Document

This document was meant as a tutorial for LCDproc users. It tries to
introduce you into the world of LCDproc giving you an overview of the
project. After reading this document you will be able to set up your own
LCDproc'ed system.

> **Note**
> 
> Please note that this document is still under construction". We hope
> to finish it until the final release of LCDproc SMILE. If you run into
> any trouble feel free to write to the LCDproc mailing list. See
> [](http://lcdproc.omnipotent.net/mail.php3) for details on how to
> subscribe to the list.
> 
> Therefore you might want to have a look at
> [](http://lcdproc.sourceforge.net/docs/), to get the latest version of
> this document (unless you want to generate it yourself from the
> docbook files in the Git).

This document was originally written for LCDproc 0.4.3, but has been
dramatically updated since. At the time of writing there had already
been the "LCDproc User's Guide" written by William W. Ferrel in 1999.
His version covered an early version of LCDproc and therefore
concentrated on Matrix Orbital displays.

William's document was "recycled" for the description of the Matrix
Orbital display driver and for other parts of this document.

In several other places e-mails and other documents have been included
in this document. The authors of those are listed below every such
document.

# What is LCDproc?

LCDproc is a client/server suite including drivers for all kinds of
nifty LCD displays.

The server LCDd makes it possible to display text and other data on an
LCD display. As well LCDd can handle certain input devices.

Support for devices is added by drivers. We distinguish between output
and input drivers. LCDd currently supports only one single output
driver, which may at the same time handle input. Nevertheless several
input (only) drivers are supported.

Currently there are drivers for several serial devices: Matrix Orbital,
Crystal Fontz, Bayrad, LB216, LCDM001 (kernelconcepts.de), Wirz-SLI and
PIC-an-LCD; and some devices connected to the LPT port: HD44780,
STV5730, T6963, SED1520 and SED1330. There are input (only) drivers for
LIRC and joysticks.

Clients can connect to LCDd through common TCP sockets.

Various clients are available. The *main* client lcdproc, which is
shipped with the LCDproc distribution, can display things like CPU load,
system load, memory usage, uptime, and a lot more.

# The LCDproc Server - LCDd

LCDd is one of those well known \*NIX daemons. BUT it's not just \*one\*
daemon. It's the one that is supposed to drive your LCD ;)

LCDd can either be run from the command line or automatically by the
init scripts shipped with the distribution.

As other daemons, LCDd has to be configured. In this respect a lot has
changed since LCDproc 0.4.1. While LCDd retrieved all its configuration
settings from the command line in 0.4.1, it now has a configuration
file, which is normally `/etc/LCDd.conf`.

# The LCDproc "Main" Client - lcdproc

While LCDd only offer the functionality of displaying text on a display,
lcdproc actually retrieves data worth displaying.

lcdproc gets its information from the `/proc` filesystem.

lcdproc can connect to an LCDproc server either on the local system or
on a remote system as long as it is reachable. It extracts the same
statistics regardless of where it sends this information. The statistics
it gathers include CPU utilization, memory utilization, disk
utilization, network utilization, system uptime, time, and date, and so
on. It displays this information in assorted ways, and can be tailored
to taste.

# Other LCDproc Clients

As it is rather simple to write an LCDproc client, you can find various
clients on the Internet.

Unfortunately we cannot provide a list of LCDproc clients here. So, take
a look at the [Clients page on LCDproc's web
site](http://www.lcdproc.org/clients.php3) or have fun searching
[Google](http://www.google.com) or [freshmeat](http://freshmeat.net).
Simply use `lcdproc` as the search pattern.
