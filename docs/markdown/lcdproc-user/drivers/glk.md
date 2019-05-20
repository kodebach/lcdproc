# The glk Driver

This section talks about using LCDproc with LCD displays that use the
Matrix Orbital GLK and GLC chipset.

## Supported devices

Currently the drivers supports the following devices:

  - GLC12232 (20x4)

  - GLC12864 (20x8)

  - GLC128128 (20x16)

  - GLC24064 (40x8)

  - GLK12232-25 (20x4)

  - GLK12232-25-SM (20x4)

  - GLK12864-25 (20x8)

  - GLK128128-25 (20x16)

  - GLK24064-25 (40x8)

  - GLK19264-7T-1U-USB (32x8)

> **Note**
> 
> Modules not in the list above are not recognized and the driver will
> not load if it encounteres an unrecognized display.

## Configuration in LCDd.conf

### \[glk\]

  - Device = `DEVICE`  
    select the serial device to use \[default: `/dev/lcd`\]

  - Contrast = `CONTRAST`  
    Set the initial contrast. Legal values for CONTRAST are `0` -
    `1000`. If not given, it defaults to `500`.

  - Speed = { `9600` | *`19200`* | `38400` | `57600` | `115200` }  
    Set the the baud rate for communication with the LCD. The default is
    `19200`.
