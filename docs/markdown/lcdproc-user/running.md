# Running LCDproc

# Running LCDd

If you have installed the init-scripts you can simply start, stop and
restart LCDd with the init-script.

## Running LCDd from the command line

There are several reasons for running LCDd from the command line

  - You don't want to install LCDd but run it from the source directory.

  - You want to do some debugging.

  - You want to get the output directly on stderr.

  - ...

> **Note**
> 
> If you run LCDd as a "normal" user, it will not change to the user
> specified in the config file. For some devices, mostly parallel port
> devices but also some USB devices, you will need root privileges
> anyway ;)

The simplest command that will run LCDd is the following. It is useful
for running LCDd from the source directory, e.g. after building.

    $ server/LCDd -c LCDd.conf

## The Command Line Options of LCDd

Running `LCDd -h` gives you an overview of the currently available
command line options.

``` 
 

LCDd - LCDproc Server Daemon, 0.5dev

Copyright (c) 1999-2006 Selene Scriven, William Ferrell, and misc. contributors.
This program is released under the terms of the GNU General Public License.

Usage: LCDd [<options>]
  where <options> are:
    -h                  Display this help screen
    -c <config>         Use a configuration file other than /etc/lcdproc/LCDd.conf
    -d <driver>         Add a driver to use (overrides drivers in config file) [curses]
    -f                  Run in the foreground
    -a <addr>           Network (IP) address to bind to [127.0.0.1]
    -p <port>           Network port to listen for connections on [13666]
    -u <user>           User to run as [nobody]
    -w <waittime>       Time to pause at each screen (in seconds) [4]
    -s <bool>           If set, reporting will be done using syslog
    -r <level>          Report level [2]
    -i <bool>           Whether to rotate the server info screen

```

# Running lcdproc

You will probably more often run `lcdproc` from the command line than
you will run LCDd.

## The Command Line Options of lcdproc

Running `lcdproc -h` gives you an overview of the currently available
command line options.

``` 
 

lcdproc - LCDproc system status information viewer

Copyright (c) 1999-2006 Selene Scriven, William Ferrell, and misc. contributors.
This program is released under the terms of the GNU General Public License.

Usage: lcdproc [<options>] [<screens> ...]
  where <options> are
    -s <host>           connect to LCDd daemon on <host>
    -p <port>           connect to LCDd daemon using <port>
    -f                  run in foreground
    -e <delay>          slow down initial announcement of screens (in 1/100s)
    -c <config>         use a configuration file other than /etc/lcdproc/lcdproc.conf
    -h                  show this help screen
    -v                  display program version
  and <screens> are
    C CPU               detailed CPU usage
    P SMP-CPU           CPU usage overview (one line per CPU)
    G CPUGraph          CPU usage histogram
    L Load              load histogram
    M Memory            memory & swap usage
    S ProcSize          biggest processes size
    D Disk              filling level of mounted file systems
    I Iface             network interface usage
    B Battery           battery status
    T TimeDate          time & date information
    O OldTime           old time screen
    U Uptime            uptime screen
    K BigClock          big clock
    N MiniClock         minimal clock
    A About             credits page

Example:
    lcdproc -s my.lcdproc.server.com -p 13666 C M X

```

> **Note**
> 
> You will not be able to connect to a remote server, unless it listens
> to the correct interface and port\! See [`LCDd.conf`: The \[server\]
> Section](#server-section) for details on the server setup.
