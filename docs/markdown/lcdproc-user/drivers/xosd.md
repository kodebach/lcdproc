# The xosd Driver

This section talks about using LCDproc with libxosd.

[libxosd](http://sourceforge.net/projects/libxosd/) is a system that
displays text on top of your X-Windows screen, much like the on-screen
display (OSD) used by most modern televisions and video-players.

## Configuration in LCDd.conf

### \[xosd\]

  - Size = PARAMETERS.SIZE  
    set display size \[default: `20x4`\]

  - Offset = ` 
                            X-OFFSET
                            x
                            Y-OFFSET
                     `  
    Offset (in pixels) of the top-left corner of LCDproc's xosd window
    from the top-left corner of the monitor. If not given, it defaults
    to `0x0`.

  - Font = `FONT`  
    X font to use, in XLFD (X Logical Font Description) format, as given
    by the xfontsel property. E.g.
    `-*-terminus-*-r-*-*-*-320-*-*-*-*-*`.
    
    For best results it is recommended to use a mono-spaced font to
    mimic the aspect of a physical LCD display that most clients expect
    with regard to their screen layouts.

  - Contrast = `CONTRAST`  
    Set the initial contrast. Legal values for CONTRAST are `0` -
    `1000`. If not given, the default value is `500`.
    
    > **Note**
    > 
    > This parameter can be set but it does not change anything in the
    > driver.

  - Brightness = `BRIGHTNESS`  
    Set the initial brightness \[default: `1000`; legal: `1` - `1000`\]

  - OffBrightness = `BRIGHTNESS`  
    Set the initial off-brightness \[default: `500`; legal: `1` -
    `1000`\] This value is used when the display is normally switched
    off in case LCDd is inactive
