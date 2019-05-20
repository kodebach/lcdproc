# The imonlcd Driver

## General

This section talks about using LCDproc with LCD devices manufactured by
SoundGraph. For example, [the iMON OEM
LCD](http://www.soundgraph.com/oem-lcd-feature-en/).

This driver currently supports versions 15c2:ffdc and 15c2:0038 of the
device. (You can find the version of your LCD via the lsusb command).

In many systems, the LCD backlight will remain on after the system is
shutdown. This behavior remains a mystery - somehow the LCD receives a
reset command (or similar) AFTER LCDd is stopped.

This driver requires the iMON module included with LIRC v0.8.4a or
newer, available from the [LIRC project](http://www.lirc.org). The
15c2:0038 device may require LIRC v0.8.5 or newer.

## Configuration in LCDd.conf

### \[imonlcd\]

  - Protocol = `PROTOCOL`  
    Specify which version of iMON LCD is installed. The default, `0`
    specifies the :ffdc device. `1` should be used for the :0038 device.

  - OnExit = `ONEXIT`  
    Specify the exit behavior. The default is `1`, which turns on the
    big ugly clock upon shutdown. `0` leaves the shutdown message on the
    screen. `2` turns the LCD off.

  - Device = `DEVICE`  
    Select the output device to use. Change this from the default
    `/dev/lcd0` to the device file that gets created when the kernel
    module ([see above](#imonlcd-kernelmodules)) is loaded.

  - Contrast = `CONTRAST`  
    Select the display's contrast `200` is the default. Permissible
    values are in the range of `0`-`1000`.

  - Size = PARAMETERS.SIZE  
    Set the display size in pixels. The default `96x16` should be safe
    for most if not all users, since the device seeems to be made only
    with this one size.

  - Backlight = `BACKLIGHT`  
    Set the backlight state. The default is `1`, which turns the
    backlight on. `0` turns the backlight off.

  - DiscMode = `DISCMODE`  
    Sets the disc mode. The default is `0` which spins the "slim" disc.
    `1` spins their complement.
