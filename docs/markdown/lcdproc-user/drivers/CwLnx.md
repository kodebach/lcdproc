# The CwLnx Driver

This section talks about using LCDproc with the serial / USB LCDs
CW12232, CW12832 and CW1602 by CwLinux.

## Features

The CwLinux CW12232 LCDs are graphical LCDs with 122 x 32 dots that also
have a text mode with 20 x 4 characters, the CW12832 are graphical
displays with 128 x 32 dots and a 21 x 4 character text mode, the CW1602
LCDs are character LCDs that are 16 characters wide and 2 lines high.

The modules can be ordered bare or as part of a kit mounted on brackets
that fit in half-height 5.25" (CW12232 and CW1608) or 3,5" (CW12832)
drive bays. The mounting brackets optionally feature a 6 button keypad
that makes use of the keypad connector on the display modules.

The kits allow to programmatically switch on/off their backlight. Newer
revisions of the kits also have programmable brighness as well as 4
general purpose IO ports.

The displays come in 2 variants that differ how they communicate with
the host: The serial modules are connected to the PC using a serial
RS232 connection getting operating power using the standard floppy drive
power connector, while the USB modules only require an USB connection.

For more information see the [CwLinux web site](http://www.cwlinux.com)

## Configuration in LCDd.conf

### \[CwLnx\]

  - Model = { *`12232`* | `12832` | `1602` }  
    Select the LCD model \[default: `12232`; legal: `12232`, `12832`,
    `1602`\]

  - Device = `DEVICE`  
    Select the output device to use \[default: `/dev/lcd`\] May be
    serial device or USB device in serial emulation mode.

  - Size = PARAMETERS.SIZE  
    Select the LCD size \[default: depending on model: 12232: `20x4`,
    12832: `21x4`\] 1602: `16x2`\]

  - Speed = { `9600` | *`19200`* }  
    Set the the baud rate for communication with the LCD. If not given,
    the default is `19200`.

  - Reboot = PARAMETERS.YESNODEF  
    Reinitialize the LCD's BIOS \[default: `no`; legal: `yes`, `no`\]
    normally you shouldn't need this

  - Keypad = PARAMETERS.YESNODEF  
    Tells if you have a keypad connected. Keypad layout is currently not
    configurable from the config file.

  - KeyMap\_A = `KEY`; KeyMap\_B = `KEY`; KeyMap\_C = `KEY`; KeyMap\_D =
    `KEY`; KeyMap\_E = `KEY`; KeyMap\_F = `KEY`  
    If you have a non standard keypad you can associate any keystrings
    to keys. There are 6 input key in the CwLnx hardware that generate
    characters from '`A`' to '`F`'. Legal values for KEY are `Up`,
    `Down`, `Left`, `Right`, `Enter` and `Escape`.
    
    The following is the built-in default mapping hardcoded in the
    driver.
    
    |           |          |
    | --------- | -------- |
    | KeyMap\_A | `Up`     |
    | KeyMap\_B | `Down`   |
    | KeyMap\_C | `Left`   |
    | KeyMap\_D | `Right`  |
    | KeyMap\_E | `Enter`  |
    | KeyMap\_F | `Escape` |
    

    You may leave it unchanged if you have a standard keypad. You can
    change it if you want to report other keystrings or have a non
    standard keypad.

  - keypad\_test\_mode = PARAMETERS.YESNODEF  
    keypad\_test\_mode permit to test keypad assignment Default value is
    no
