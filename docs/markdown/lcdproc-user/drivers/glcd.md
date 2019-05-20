# The glcd Driver

The glcd driver (graphic lcd) driver is a "meta driver" that renders
text for display on graphic displays. It uses either a built-in 5x8 font
(the same as used in elsewhere in LCDproc) or Freetype 2 to draw the
characters and icons into an internal frame buffer. That frame buffer is
then copied to the display by a small sub-driver called connection type
driver (CT-driver).

> **Note**
> 
> LCDproc is compiled with FreeType support by default if it is
> installed on your system. It is enabled by default, too. However a
> font is not configured by default. Thus the driver will not start
> unless a font is configured\!

## Connections

### Connection type t6963

Support for displays using a Toshiba T6863 controller connected to the
parallel port. Wiring is the same as for the t6963 driver. Refer to
[???](#t6963-connections) for details.

If used without FreeType this connection type uses the same font as the
t6963 driver but draws the characters into the graphic memory of the
T6963.

> **Important**
> 
> You must configure the display for 8x8 font to make this connection
> type work\!

### Connection type png

This connection type writes out the frame buffer into files in `/tmp`.
The files are named `lcdproc######.png` where \#\#\#\#\#\# is a number
starting at 0.

> **Tip**
> 
> As a new file is written on any change to the screen it is best to
> turn off the heartbeat.

### Connection type serdisplib

Use the serdisplib library ([](http://serdisplib.sourceforge.net/)) for
output. This enables use of a number of graphical displays connected to
a parallel, serial, or USB port. See the serdisplib web page for
details, especially for available options for your display.

### Connection type glcd2usb

Support for the glcd2usb device
([](http://www.harbaum.org/till/glcd2usb/index.shtml), a graphic LCD to
USB converter based on Atmel ATmega16 and the V-USB stack. This device
features an adjustable backlight and 4 keys. Right now only displays
with the KS0108 controller are supported.

The device has 4 keys which may be configured using the `KeyMap_A`
(leftmost key) to `KeyMap_D` (rightmost key) settings.

Note that any size setting configured in `LCDd.conf` is ignored as the
size reported by the device is used.

### Connection type x11

This connection type draws the frame buffer to a X window. It features
adjustable LCD pixel size, pixel color, backlight color and simulates
contrast and brightness. PC keyboard is used to simulate buttons.

### Connection type picolcdgfx

Support for the picoLCD Graphics 256x64 display from mini-box.com.

## Configuration in LCDd.conf

### \[glcd\]

  - ConnectionType = { *`t6963`* | `glcd2usb` | `png` | `picolcdgfx` |
    `serdisplib` | `x11` }  
    Specify which connection type to use. See [above](#glcd-connections)
    for details.

  - Size = PARAMETERS.SIZE  
    Specifies the size of the LCD in pixels. Default: `128x64`. Maximum
    value supported: `640x480`.
    
    The size in characters is automatically calculated from this value
    and the CellSize value (see below) and cannot be configured.

  - Contrast = `CONTRAST`  
    Set the initial contrast (if supported by the connection type
    driver). Legal values for CONTRAST are `0` - `1000`. If not given,
    it defaults to `600`.

  - Brightness = `BRIGHTNESS`  
    Set the initial brightness when the backlight is "on" (if supported
    by the connection type driver). Legal values are `0` - `1000`. If
    not given, it defaults to `800`.

  - OffBrightness = `BRIGHTNESS`  
    Set the initial brightness when the backlight is set "off" (if
    supported by the connection type driver). Legal values are `0` -
    `1000`. If not given, it defaults to `100`.

<!-- end list -->

  - useFT2 = PARAMETERS.YESDEFNO  
    Tell whether to use FreeType2 or not. It is set to `yes` by default.
    If turned off (set to `no`), the fixed internal 5x8 font is used to
    draw characters and icons.

  - normal\_font = `FONTFILE`  
    Set path to the font file to use, e.g.
    `/usr/local/lib/X11/fonts/TTF/andalemo.ttf`. This option is required
    if FreeType 2 support is enabled, but it is not set to any default
    value. A font with a fixed character width (monotype) is strongly
    recommended.

  - fontHasIcons = PARAMETERS.YESDEFNO  
    Many fonts to not include the Unicode glyphs used for drawing icons.
    If this option is set to `no` then the internal 5x8 font is used
    even if FreeType is enabled. This option is on (`yes`) by default.

  - CellSize = PARAMETERS.SIZE  
    Specifies the size of the character cell in pixels. Characters will
    be drawn within this cell. Default: `6x8`; minimum value `4x6`;
    maximum value: `24x32`.

<!-- end list -->

  - KeyRepeatDelay = `DELAY`  
    DELAYis the time in milliseconds from first key report to first
    repeat. Set to `0` to disable repeated key reports. Allowed values
    are between `0` and `3000`, the default is `500`.

  - KeyRepeatInterval = `DELAY`  
    DELAYis the time in milliseconds between repeated key strokes.
    Allowed values are between `0` and `3000`, the default is `300`.
    This setting is ignored, if KeyRepeatDelay is disabled (set to
    zero).

  - KeyMap\_A = `KEY`; KeyMap\_B = `KEY`; KeyMap\_C = `KEY`; KeyMap\_D =
    `KEY`; KeyMap\_E = `KEY`; KeyMap\_F = `KEY`  
    These settings allow to assign arbitrary key strings to key strokes.
    Up to 26 keys may be configured ('`A`' to '`Z`').
    
    The following is the built-in default mapping hard-coded in the
    driver. Only keys '`A`' to '`F`' are used by default.
    
    |           |          |
    | --------- | -------- |
    | KeyMap\_A | `Up`     |
    | KeyMap\_B | `Down`   |
    | KeyMap\_C | `Left`   |
    | KeyMap\_D | `Right`  |
    | KeyMap\_E | `Enter`  |
    | KeyMap\_F | `Escape` |
    

<!-- end list -->

  - Port = `PORT`  
    Specify the address of the parallel port the LCD is connected to.
    Common values for PORT are `0x278`, `0x378` and `0x3BC`. If not
    given, the default is `0x378`.

  - bidirectional = PARAMETERS.YESDEFNO  
    Use parallel port in bi-directional mode. \[default: `yes`; legal:
    `yes`, `no`\]
    
    Most LPT ports can be used in bi-directional mode. It is required
    for proper timing of the display.

  - delayBus = PARAMETERS.YESNODEF  
    Use additional delay in read / write operations. \[default: `no`;
    legal: `yes`, `no`\]. As the driver implements busy checking usually
    no additional delays are required.

<!-- end list -->

  - serdisp\_name = `NAME`  
    This is the name or alias to select a display driver in the
    serdisplib library. As there is not default value setting this
    option is mandatory.

  - serdisp\_device = `DEVICE`  
    Set the device to use. Unlike elsewhere in `LCDd.conf` this is a
    device description understood by serdisplib. As there is not default
    value setting this option is mandatory.

  - serdisp\_options = `OPTIONS`  
    OPTIONS is a serdisplib option string, which is a list of semicolon
    separated key / value pairs. Use this to pass any additional options
    to serdisplib, e.g. wiring or rotation settings.
    
    > **Note**
    > 
    > As the value will contain equal signs the whole OPTIONS value must
    > be enclosed in double quotes.
    > 
    > The display width and height are always set using the values from
    > the glcd driver (see above). Any width / height values set in the
    > option string will be ignored.

<!-- end list -->

  - x11\_PixelSize = ` 
                            PIXELS
                            +
                            GAP
                     `  
    Each LCD dot is drawn in the X window as a filled rectangle of this
    size plus a gap between each filled rectangle. A PixelSize of `3+1`
    would draw a 3x3 filled rectangle with a gap of 1 pixel to the right
    and bottom, effectively using a 4x4 area of the window. Default is
    `3+1`.

  - x11\_PixelColor = `0xRRGGBB`  
    The color used to draw each LCD dot at full contrast and full
    brightness. Default is `0x000000`.

  - x11\_BacklightColor = `0xRRGGBB`  
    The color used for the backlight at full brightness. Default is
    `0x80FF80`.

  - x11\_Border = `PIXELS`  
    Extra space in pixels around the LCD portion of the X window.
    Default is `20`.

  - x11\_Inverted = PARAMETERS.YESNODEF  
    Swap the colors for the pixel and backlight within the LCD portion
    of the X window. This results in an inverted display.

<!-- end list -->

  - picolcdgfx\_KeyTimeout = `TIMEOUT`  
    The time in milliseconds to wait for a key press before assuming no
    key was pressed. This settings is the usb\_read timeout. Default
    125.

  - picolcdgfx\_Inverted = PARAMETERS.YESNODEF  
    When set to yes, the pixel will be inverted. Default no.
