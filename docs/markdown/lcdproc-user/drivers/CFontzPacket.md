# The CFontzPacket Driver

[CrystalFontz](http://www.crystalfontz.com/) offers a wide range of
character and graphical LCD modules. The `CFontzPacket` driver supports
the modules that communicate with the host computer using a packet-based
communications protocol with 16-bit CRC (hence the driver name).

Currently this line of modules comprises the models:

  - CFA-631
    
      - 20x2 character LCD with backlight
    
      - keypad with 4 keys: Up, Down, enter & Escape
    
      - USB connection for data and power
    
      - mounting bracket to fit into 3,5" drive bays
    
      - Optional (via add-on board) temperature sensor, fan and ATX
        power control connectors (all unsupported by LCDproc)

  - CFA-633
    
      - 16x2 character LCD with backlight
    
      - keypad with 6 keys: 4 directions, Enter & Escape
    
      - serial or USB connection
    
      - mounting bracket to fit into 5,25" drive bays
    
      - Temperature sensor and fan control connectors (both unsupported
        by LCDproc)

  - CFA-533
    
      - This is the same as CFA-633 except it misses the fan control
        capabilities. The temperature monitoring is not supported in
        LCDproc though.

  - CFA-635
    
      - 20x4 characters LCD with backlight
    
      - keypad with 6 keys: 4 directions, Enter & Escape
    
      - USB connection for data and power
    
      - four bi-color LEDs to show status information
    
      - optional mounting bracket to fit into 5,25" drive bays

## Configuration in LCDd.conf

### \[CFontzPacket\]

  - Model = { `533` | `631` | *`633`* | `635` }  
    Select the LCD model that is connected. Legal values for this option
    are `631`, `533`, `633`, or `635`, with the default being `633`.

  - Device = `DEVICE`  
    Select the output device to use. It may be a serial device or a USB
    device in serial emulation mode. If not given, it defaults to
    `/dev/lcd`.
    
    > **Note**
    > 
    > Mac OS X users may need to use one of the `/dev/cu` devices
    > instead of the `/dev/tty` ones.

  - USB = PARAMETERS.YESNODEF  
    Enable this flag if the device is connected to an USB port. For
    serial ports leave it disabled. \[default: `no`; legal: `yes`,
    `no`\]

  - Size = PARAMETERS.SIZE  
    Select the LCD size. This overrides the size the driver uses for the
    selected model (631: `20x2`, 533/633: `16x2`, 635: `20x4`).
    
    > **Note**
    > 
    > You should usually not need to set this value\!

  - Contrast = `CONTRAST`  
    Set the initial contrast. Legal values for CONTRAST are `0` -
    `1000`. If not specified, it defaults to `560`.

  - Brightness = `BRIGHTNESS`  
    Set the initial brightness \[default: `1000`; legal: `0` - `1000`\]

  - OffBrightness = `BRIGHTNESS`  
    Set the initial off-brightness \[default: `0`; legal: `0` - `1000`\]
    This value is used when the display is normally switched off in case
    LCDd is inactive

  - Speed = { `19200` | `115200` }  
    Override the default baud rate the driver uses for communication
    with the selected LCD model. Allowed values are `19200` (default for
    CFA-533 and CFA-633) and `115200` (default for the CFA631 and
    CFA635).
    
    > **Note**
    > 
    > You should usually not need to set this value\!

  - OldFirmware = PARAMETERS.YESNODEF  
    Very old 633 firmware versions do not support partial screen updates
    using 'Send Data to LCD' command (31). For those devices it may be
    necessary to enable this flag. \[default: `no`; legal: `yes`, `no`\]

  - Reboot = PARAMETERS.YESNODEF  
    Reinitialize the LCD's BIOS \[default: `no`; legal: `yes`, `no`\].
