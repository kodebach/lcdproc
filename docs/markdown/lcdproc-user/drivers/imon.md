# The imon Driver

## General

The `imon` driver controls [Soundgraph iMON
VFD](http://www.soundgraph.com/vfd-feature-en/) devices, that are either
preinstalled or available as optional accessories for a variety of Home
Theater PC (HTPC) cases from Ahanix, Silverstone, Cooler Master and
others. They can also be bought separately and then fit into a 5,25"
disk drive bay of any regular PC.

The iMON VFD sports a vacuum fluorescent display with 16x2 characters
that connects to the computer using USB. Although the device is shipped
with an IR remote control and some versions even have a volume knob,
LCDproc's driver currently only supports the display part of the device.

In order to be able to use it, you have to get and install one of the
following Linux kernel modules:

  - standalone iMON VFD driver from [](http://venky.ws/projects/imon/)

  - the iMON module included with LIRC ver. 0.7.1 or newer from the
    [LIRC project](http://www.lirc.org)

For further details, please consult the page and the forum at
[](http://venky.ws/projects/imon/).

## Configuration in LCDd.conf

### \[imon\]

  - Size = PARAMETERS.SIZE  
    Set the display size. The default `16x2` should be safe for most if
    not all users, since the device seems to be made only with this one
    size. But who knows HELLIP

  - Device = `DEVICE`  
    Select the output device to use. Change this from the default
    `/dev/lcd` to the device file that gets created when the kernel
    module ([see above](#imon-kernelmodules)) is loaded.

  - CharMap = { *`hd44780_euro`* | `hd44780_koi8_r` | `hd44780_cp1251` |
    `hd44780_8859_5` | `upd16314` | `none` }  
    Set the character mapping depending on the display you have:
    
      - The default, `hd44780_euro` is for displays with a ROM mask
        supporting the european charset (ROM code A02).
    
      - The `none` charmap does not translate any characters and is only
        useful for debugging.
    
    You only need to set this parameter if you have a non-standard
    charmap.
    
    If LCDproc was configured with '--enable-extra-charmaps' option the
    following character mappings are available, too:
    
      - `hd44780_koi8_r` maps input from a client in Russian KOI8-R to
        displays with a ROM mask supporting the european charset (ROM
        code A02).
    
      - `hd44780_cp1251` maps input from a client in Russian CP1251
        (Windows-1251) to displays with a ROM mask supporting the
        european charset (ROM code A02).
    
      - `hd44780_8859_5` maps input from a client in Russian ISO 8859-5
        to displays with a ROM mask supporting the european charset (ROM
        code A02).
    
      - `upd16314` is for displays with a Nec uPD16314 vacuum
        fluorescent display (VFD) controller with ROM code 002 character
        set. If your display has ROM code 001 character set you may use
        the `hd44780_euro` charmap instead.
    
    > **Tip**
    > 
    > See `server/drivers/hd44780-charset.h` in LCDproc's source
    > directory for the actual mappings.
