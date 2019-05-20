# The CFontz Driver

This section talks about using LCDproc with the serial LCD displays of
the CFA632 and CFA634 series by CrystalFontz, Inc.

## Configuration in LCDd.conf

### \[CFontz\]

  - Device = `DEVICE`  
    Select the serial output device to use. If not given, default is
    `/dev/lcd`.

  - Size = PARAMETERS.SIZE  
    Set the LCD's dimensions in terms of characters per line and lines.
    If not given, it defaults to `20x4`.

  - Contrast = `CONTRAST`  
    Set the initial contrast. Legal values for CONTRAST are in the range
    between `0` and `1000`. If not given, it defaults to `560`.

  - Brightness = `BRIGHTNESS`  
    Set the initial brightness. Legal values for BRIGHTNESS range from
    `0` to `1000`. If not given, it defaults to `1000`.

  - OffBrightness = `BRIGHTNESS`  
    Set the initial off-brightness. This value is used when the display
    is normally switched off in case LCDd is inactive. Legal values
    BRIGHTNESS are in the range from `0` to `1000`. The default is `0`.

  - Speed = { `1200` | `2400` | *`9600`* | `19200` | `115200` }  
    Set the the baud rate to use when communicating with the LCD. It
    defaults to `9600` if not specified.

  - NewFirmware = PARAMETERS.YESNODEF  
    Set the firmware version (*New* means \>= 2.0) \[default: `no`;
    legal: `yes`, `no`\].

  - Reboot = PARAMETERS.YESNODEF  
    Reinitialize the LCD's BIOS \[default: `no`; legal: `yes`, `no`\]
    normally you shouldn't need this.
