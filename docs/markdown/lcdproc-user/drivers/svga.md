# The svga Driver

This section talks about using LCDproc with LCD displays that use the
svga library.

## Configuration in LCDd.conf

### \[svga\]

  - Mode = `SCGALIB-MODE`  
    svgalib mode to use \[default: `G320x240x256`\] SCGALIB-MODE can be
    any legal mode string for svgalib. See the svgalib 7 manual page for
    legal mode strings.

  - Size = PARAMETERS.SIZE  
    set display size \[default: `20x4`\]

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
