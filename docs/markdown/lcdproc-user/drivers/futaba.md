# The Futaba Driver

This section covers the use of the
[[Futaba](Futaba)](http://www.futaba.co.jp/en/display/vfd/lineup.html)
TOSD-5711BB USB VFD display. This as commonly used on Elonex Artisan,
Fujitsu Scaleo E and FIC Spectra Media Centre PCs.

## Displays

The Futaba TOSD-5711BB is a VFD based USB 2.0 display, very much in the
style of VHS Recorders/Players from the 1990s. It contains a single
line, 7 Character 14-segment display and multiple icons to show the
media type, codec, etc; current state of the PC; Volume knob and
indicator. Each segment will display one character (Letters can only be
in Capitals), and has a '`:`' (colon) and '.' (dot) between each
character. There is no contrast control or back-light. The volume knob
is lit using a blue LED and spins eternally. An image of the VFD circuit
board and Volume knob can be seen
[[here](here)](http://images.esellerpro.com/2131/I/772/87/DCP_1700.JPG).

![](http://images.esellerpro.com/2131/I/772/87/DCP_1700.JPG)

The IR attached is a standard MCE eHome remote which does not need any
new drivers to work with lirc. It also works with scripts.xbmc.lcdproc
on Kodi.

It comes pre-installed on Elonex Artisan, Fujitsu Scaleo E and FIC
Spectra Media Centre PCs [FIC Spectra
Manual](http://www.fic.com.tw/spectraviiv/drivers/SPECTRA%20ID1%20Viiv%20User%20Manual.pdf).

> **Note**
> 
>   - If the display crashes on start-up, this diff will solve the
>     problem: [Linux
>     Diff](https://github.com/ajw107/LibreELEC.tv/blob/master/packages/linux/patches/default/linux-061-add-elonex-artisan-vfd-support.patch).
>     I have submitted this patch for inclusion into the standard Linux
>     Kernel.
> 
>   - The patched version of scripts.xbmc.lcproc can be found
>     [here](https://github.com/ajw107/script.xbmc.lcdproc), this should
>     be added upstream soon too.

## Requirements

The driver is based on the [`libusb`](http://libusb.info/) USB library
(Versions 1.0 and 0.1), which should make it work with Linux, the
different BSD variants as well as Darwin/MacOS X.

> **Note**
> 
> When using a `libusb` based driver like `futaba`, LCDd needs to be
> started as root. However LCDd may then drop down to a less privlidged
> user, such as nobody, after it has started.

On Linux, the only kernel module required is the USB host controller
driver to fire up the USB bus to which the LCD is attached. For other
operating systems, analogous requirements apply.

## Configuration in LCDd.conf

### \[futaba\]

> **Note**
> 
> There are no Brightness, Contrast or Back-light controls as the
> display does not support these functions

## futaba driver status

All icons/indicators function well. Currently the ':' (colon) or '.'
(dot) in between characters do NOT operate. In a future update I hope to
get these to work, with three settings: one to enable/disable flashing;
one to set the flash rate; and one to choose between ':' (colon) or '.'
(dot).

## Copyright

The lcdproc futaba driver originally was written by Blackeagle email:
gm(dot)blackeagle(at)gmail(dot)com Additions by Alex Wood (`2015``/6`)
email: thetewood(at)gmail(dot)com.
