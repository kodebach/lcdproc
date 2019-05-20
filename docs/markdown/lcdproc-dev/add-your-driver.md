# Adding your driver to LCDproc

# Introduction

LCDproc is meant to be modular, it is relatively easy to add new input
and output drivers to LCDproc.

This chapter will explain you the major steps and few gotchas of adding
your own driver to LCDproc. Enjoy\!

Be sure to read [???](#programming) and [???](#driver-api) as well.

As a starting point you may take a look at the debug driver. It is
available as `server/drivers/debug.c`.

# Rules for accepting new drivers

LCDproc is open source software. Anyone is free to take LCDproc's code,
write his own driver and publish the modified sources somewhere again.
If you want your driver to be included in LCDproc's code some conditions
have to be met:

1.  The hardware (display or enclosing product) is publicly sold *OR*
    the schematics and firmware (if required) are publicly available.
    \[1\]

2.  The driver is released under (L)GPL and has an appropriate copyright
    notice.

3.  The submitter is or is acting on behalf of the original driver
    developer. \[2\]

4.  The driver description contains a valid email address for contacting
    the submitter or developer.

5.  The code is commented *AND* includes appropriate Doxygen comments,
    especially for internal / non-API functions.

6.  End user documentation (updates to man pages *AND* User's Guide in
    Docbook format) is available.

7.  Driver options are described in the end user documentation *AND*
    `LCDd.conf`.

8.  The driver adheres to the style guide as described in
    [???](#code-style).

# Autoconf, automake, and Everything\!

How I Learned to Stop Worrying and Love the Configure Script

It was decided pretty early in LCDproc's life to use GNU autoconf and
GNU automake. This allows LCDproc to be ported to several platforms with
much less effort. It can be quite daunting to understand how autoconf &
automake interact with each others and with your code, but don't be
discouraged. We have taken great care in making this as simple as
possible for programmers to add their own driver to LCDproc. Hopefully,
you'll only have to modify two files, one for autoconf and one for
automake.

The first thing you need to do is to find a name for your driver, it
should be as descriptive as possible; most drivers are named after their
respective chipset, for example hd44780, mtc\_s16209x, sed1330 and
stv5730, others are named after the company that makes that particular
LCD display, for example CFontz and MtxOrb. Remember that these names
are case sensitive. In this chapter, we'll use myDriver (which is an
absolute non-descriptive name).

## Autoconf and its friend, acinclude.m4

You need to add your driver to function LCD\_DRIVERS\_SELECT of file
acinclude.m4. This can be done in three steps.

### Step 1

First you need to add your driver name to the list of possible choices
in the help screen.

This:

    AC_ARG_ENABLE(drivers,
        [  --enable-drivers=<list> compile driver for LCDs in <list>.]
        [                  drivers may be separated with commas.]
        [                  Possible choices are:]
        [                    bayrad,CFontz,CFontz633,CFontzPacket,curses,CwLnx,]
        [                    glcdlib,glk,hd44780,icp_a106,imon,IOWarrior,irman,]
        [                    joy,lb216,lcdm001,lcterm,lirc,ms6931,mtc_s16209x,]
        [                    MtxOrb,NoritakeVFD,pyramid,sed1330,sed1520,serialVFD,]
        [                    sli,stv5730,svga,t6963,text,tyan,ula200,xosd]
        [                  'all' compiles all drivers;]
        [                  'all,!xxx,!yyy' de-selects previously selected drivers],
        drivers="$enableval",

becomes:

    AC_ARG_ENABLE(drivers,
        [  --enable-drivers=<list> compile driver for LCDs in <list>.]
        [                  drivers may be separated with commas.]
        [                  Possible choices are:]
        [                    bayrad,CFontz,CFontz633,CFontzPacket,curses,CwLnx,]
        [                    glcdlib,glk,hd44780,icp_a106,imon,IOWarrior,irman,]
        [                    joy,lb216,lcdm001,lcterm,lirc,ms6931,mtc_s16209x,]
        [                    MtxOrb,NoritakeVFD,pyramid,sed1330,sed1520,serialVFD,]
        [                    sli,stv5730,svga,t6963,text,tyan,ula200,xosd,myDriver]
        [                  'all' compiles all drivers;]
        [                  'all,!xxx,!yyy' de-selects previously selected drivers],
        drivers="$enableval",

### Step 2

Second, you need to add your driver to the list of all
    drivers.

This:

    allDrivers=[bayrad,CFontz,CFontz633,...(big list)...,tyan,ula200,xosd]

becomes:

    allDrivers=[bayrad,CFontz,CFontz633,...(big list)...,tyan,ula200,xosd,myDriver]

### Step 3

Then last, you need to add your driver to be big switch-case in this
function, see below.

``` 
        myDriver)
            DRIVERS="$DRIVERS myDriver${SO}"
            actdrivers=["$actdrivers myDriver"]
            ;;
```

If your driver only works in some platform or requires a particular
library or header, you can add your autoconf test here. You can see how
other drivers do it, but if you're not sure on how to do this, just send
an email to the mailing list and we'll make it for you.

## Automake and its friend, Makefile.am

Already half of the job is done\! Not to bad, wasn't it? The rest should
be just as easy. In this section, you'll be adding your driver to the
file server/drivers/Makefile.am. As you can guess, it's the Makefile for
the drivers. This can be done in three (or two) simple steps.

### Step 1

First, you need to add your driver to the list of drivers in this file,
this list is called EXTRA\_PROGRAMS.

This

    EXTRA_PROGRAMS = bayrad CFontz ...(big list)... ula200 xosd

becomes

    EXTRA_PROGRAMS = bayrad CFontz ...(big list)... ula200 xosd myDriver

### Step 2

This second step is only needed if your driver needs a particular
library. If it doesn't, you can skip to step 3.

You basically need to put you driver name followed by \_LDADD and equal
this to the name of the library that you need. Usually, these library
are substituted by a autoconf variable, if you're not comfortable with
this, you send an email to the mailing list and we'll set this up for
you.

For example, we would put this for our fictional driver

    myDriver_LDADD = @SOMESTRANGELIB@

### Step 3

Last but not least, you need to specify which source files should be
associated with your driver. You put your driver name followed by
`_SOURCES` and equal this to a space separated list of the source and
header files. See below for an example.

    myDriver_SOURCES =  lcd.h myDriver.c myDriver.h

## Test your setup

You're almost done\! You only need to check out if you didn't made any
mistake. Just run sh autogen.sh to regenerate the configure script and
Makefiles, then run ./configure --enable-drivers=myDriver and type make.
If your driver compiles without error, then congratulations, you've just
added your driver to LCDproc\! Remember to submit a patch to the mailing
list so that we can add it to the standard distribution, but do not
forget the documentation.

If you had an error, just send us an email describing it to the mailing
list and we'll try to help you.

# It's all about documentation

Please do not forget to also add the required documentation, so that
your driver can be used from others as well.

## Within the source code

We use Doxygen to document functions and data types. The doxygen
documentation can be created anytime by changing to the `docs/`
directory and running `doxygen`.

When documenting your driver's API functions you may use a short hand
version and add 'API:' to the beginning of your comment and leave out
the parameter and return value description (as we know what the API is
doing). If you use some clever algorithm inside a function please add a
few words about it.

> **Note**
> 
> Always document functions internal to the driver\! We do know what the
> API does (or is expected to do) but we don't know about what your
> driver does internally.
> 
> Read [???](#code-style-comments) on how for format comments.

## The configuration file, LCDd.conf

Extend the LCDproc server's configuration file with a section that holds
a standard configuration for your driver together with short
descriptions of the options used.

    HELLIP
    
    ## MyDriver for MyDevice ##
    [MyDriver]
    
    # Select the output device to use [default: /dev/lcd]
    Device=/dev/ttyS0
    
    # Set the display size [default: 20x4]
    Size=20x4
    
    HELLIP

## The daemon's manual page, LCDd.8

Append your driver to the list of drivers in `docs/LCDd.8.in`, the
manual page of LCD, so that users can find your driver when doing `man
LCDd`.

    HELLIP
    .TP
    .B ms6931
    MSI-6931 displays in 1U rack servers by MSI
    .TP
    .B mtc_s16209x
    MTC_S16209x LCD displays by Microtips Technology Inc
    .TP
    .B MtxOrb
    Matrix Orbital displays (except Matrix Orbital GLK displays)
    .TP
    .B MyDriver
    displays connected using MyDevice
    .TP
    .B NoritakeVFD
    Noritake VFD Device CU20045SCPB-T28A
    .TP
    .B pyramid
    LCD displays from Pyramid (http://www.pyramid.de)
    .TP
    .B sed1330
    SED1330/SED1335 (aka S1D13300/S1D13305) based graphical displays
    HELLIP

## The user guide

### Step 1

Please add a file `myDriver.docbook`, that describes the configuration
of your driver and the hard/software needed, to the directory
`docs/lcdproc-user/drivers/`.

### Step 2

Define a Docbook entity for your driver file in `lcdproc-user.docbook`.

    HELLIP
    <!ENTITY ms6931 SYSTEM "drivers/ms6931.docbook">
    <!ENTITY mtc_s16209x SYSTEM "drivers/mtc_s16209x.docbook">
    <!ENTITY MtxOrb SYSTEM "drivers/mtxorb.docbook">
    <!ENTITY MyDriver SYSTEM "drivers/MyDriver.docbook">
    <!ENTITY NoritakeVFD SYSTEM "drivers/NoritakeVFD.docbook">
    <!ENTITY pylcd SYSTEM "drivers/pylcd.docbook">
    <!ENTITY sed1330 SYSTEM "drivers/sed1330.docbook">
    HELLIP

### Step 3

Add the freshly defined entity to `drivers.docbook` to include the
documentation of your driver into the *LCDproc User's Guide*.

    HELLIP
    &ms6931;
    &mtc_s16209x;
    &MtxOrb;
    &MyDriver;
    &NoritakeVFD;
    &pylcd;
    &sed1330;
    HELLIP

### Step 4

Add the newly defined file `myDriver.docbook` to the Makefile in the
directory `docs/lcdproc-user/drivers/`.

    ## Process this file with automake to produce Makefile.in
    
    EXTRA_DIST =    bayrad.docbook \
            CFontz.docbook \
            ...
            MyDriver.docbook \
            ...
    ## EOF

1.  Therefore I will not commit drivers for displays ripped out from an
    old telephone for your private hardware project and are not
    available otherwise.

2.  I will not submit drivers found somewhere on the internet and
    submitted without the original developer's written acknowledgement.
