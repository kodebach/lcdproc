# The NoritakeVFD Driver

This section talks about using LCDproc with text mode VFD displays from
[Noritake Itron](http://www.noritake-itron.com/).

## Configuration in LCDd.conf

### \[NoritakeVFD\]

  - Device = `DEVICE`  
    Port where the VFD is. Usual values are `/dev/ttyS0` and
    `/dev/ttyS1` Default: `/dev/lcd`

  - Size = PARAMETERS.SIZE  
    Specifies the size of the LCD. Default: `20x4`

  - Brightness = `BRIGHTNESS`  
    Set the initial brightness \[default: `1000`; legal: `0` - `1000`\]

  - OffBrightness = `BRIGHTNESS`  
    Set the initial off-brightness \[default: `0`; legal: `0` - `1000`\]
    This value is used when the display is normally switched off in case
    LCDd is inactive

  - Speed = { `1200` | `2400` | *`9600`* | `19200` | `115200` }  
    Set the the baud rate to use when communicating with the VFD. If not
    specified, it defaults to `9600`.

  - Parity = { *`0`* | `1` | `2` }  
    Set the parity for communication with the device to even parity
    (`2`), odd parity (`1`) or no parity (`0`). If not given, it
    defaults to `0`.

  - Reboot = PARAMETERS.YESNODEF  
    Reinitialize the VFD \[default: `no`; legal: `yes`, `no`\]
