# The tyan Driver

This section talks about using LCDproc with LCD modules used in Tyan
GS10 and GS12 barebones.

## Features

The LCD modules used on the front side of the Tyan GS10 and GS12 series
barebones consist of an LCD display by Winstar Display Co. LTD that is
16 characters wide and 2 lines high. To the right of the display there
is a 6 button keypad: 4 array buttons and two buttons labeled `C` and
`S`.

For more information see the [LCD
pack](ftp://ftp.tyan.com/barebone_support/GS10-GS12%20LCD-Pack.zip) from
the Tyan support page. Besides a useless old version of LCDproc it
contains some documentation about the panel itself including a PDF
specification of the LCD display by its manufacturer.

## Configuration in LCDd.conf

### \[tyan\]

  - Device = `DEVICE`  
    Select the output device to use \[default: `/dev/lcd`\]

  - Speed = { `4800` | *`9600`* }  
    Set the the baud rate communication with the LCD. If not given, the
    default is `9600`.

  - Size = PARAMETERS.SIZE  
    set display size \[default: `16x2`\]
