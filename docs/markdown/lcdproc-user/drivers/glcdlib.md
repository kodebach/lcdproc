# The glcdlib Driver

This section talks about using LCDproc with LCD displays supported by
graphlcd-base.

## Connections

The so-called "meta-driver" glcdlib extends LCDproc's supported drivers
by all the drivers supported by graphlcd-base, which you can get from
[](http://projects.vdr-developer.org/projects/graphlcd/).

In order to be able to use it, you have to get and install the
glcdprocdriver from [](http://lucianm.github.com/GLCDprocDriver/) before
configuring the LCDproc build process --enable-drivers=glcdlib.

## Configuration in LCDd.conf

### \[glcdlib\]

  - Driver = `GRAPHLCD-DRIVER`  
    Specify which graphical display supported by graphlcd-base to use.
    Legal values for GRAPHLCD-DRIVER are specified in graphlcd's
    configuration file `/etc/graphlcd.conf`. For graphlcd 0.13 they
    comprise `avrctl`, `framebuffer`, `gu140x32f`, `gu256x64-372`,
    `gu256x64C-3xx0`, `hd61830`, `image`, `ks0108`, `noritake800`,
    `sed1330`, `sed1520`, `serdisp`, `simlcd`, and `t6963c`. If not
    specified it defaults to `image`.

  - UseFT2 = PARAMETERS.YESDEFNO  
    Tell whether to use FreeType2 or not. If set to `no` use graphlcd's
    bitmap fonts, which is only one size/font file. If set to to the
    default value `yes` use the fonts that FreeType2 provides.
    
    > **Note**
    > 
    > Setting it to `yes` requires Freetype2 support in
    > libglcdprocdriver and its dependants.

  - TextResolution = PARAMETERS.SIZE  
    Give text resolution in fixed width characters. If it won't fit
    according to the available physical pixel resolution and the minimum
    available font face size in pixels, 'DebugBorder' will automatically
    be turned on. If not specified, it defaults to `16x4`.

  - FontFile = `FILENAME`  
    Set path to font file to use, e.g.
    `/usr/share/fonts/corefonts/courbd.ttf`.

<!-- end list -->

  - CharEncoding = `CHARSET`  
    Specify character encoding to use, e.g. `iso8859-2`. If not given,
    use the default `ISO8859-1`.

  - MinFontFaceSize = ` 
                            COLUMNS
                            x
                            ROWS
                     `  
    minimum size in pixels in which fonts should be rendered

<!-- end list -->

  - Brightness = `BRIGHTNESS`  
    Brightness (in %) if applicable Legal values are `0` - `100`. If not
    specified, the default is `50`.

  - Contrast = `CONTRAST`  
    Set the contrast (in %) if applicable. Legal values are `0` - `100`,
    with `50` being the default when not specified.

  - Backlight = PARAMETERS.YESNODEF  
    Backlight if applicable

  - UpsideDown = PARAMETERS.YESNODEF  
    flip image upside down

  - Invert = PARAMETERS.YESNODEF  
    invert light/dark pixels

  - ShowDebugFrame = PARAMETERS.YESNODEF  
    turns on/off 1 pixel thick debugging border within the usable text
    area, for setting up TextResolution and MinFontFaceSize (if using
    FT2);

  - ShowBigBorder = PARAMETERS.YESNODEF  
    border around the unused area

  - ShowThinBorder = PARAMETERS.YESNODEF  
    border around the unused area

  - PixelShiftX = `SHIFTX`; PixelShiftY = `SHIFTY`  
    Shifts the content of the display by SHIFTX (default: `0`) and
    SHIFTY (default: `0`) pixels.
