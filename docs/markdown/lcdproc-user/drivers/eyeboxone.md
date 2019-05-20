# The Eyebox One Driver (EyeboxOne)

This section describes the Eyebox One.

## Eyebox One LCD Module

Eyebox One is a small rackmounted server marketed by Rightvision
([](http://www.alcateleyebox.rightvision.com/)). This server has an LCD
module, a keypad, two graphbars and some leds.

The LCD is a 20x4 alphanumeric module connected via standard DB-9
cabling and connector.

I couldn't find any documentation about it. All I know has been obtained
with some reverse engineering. It seems that it can run only at 19.200
baud. Sending ASCII to the module will make it simply display that text
at its current cursor position. The module has a built-in BIOS that
recognizes commands (sent by transmitting a single-byte "marker"
signifying that a command is on the way, followed by the single-byte
command character itself along with any parameters, if needed) allowing
the programmer to clear the screen, position the cursor anywhere,
hide/show the cursor, on/off the backlight, and so on.

This module is fast. If updating less than the whole screen, the LCD can
update faster than can be seen by the human eye. This, of course, more
than meets LCDproc's needs.

## Eyebox One Driver and lcdproc client

You can use the two Eyebox One graphbars, one as a free CPU meter, and
one as a free RAM meter with lcdproc client (see eyebox.c in lcdproc
client sources).

In order to use it, you must execute ./configure with a special
parameter: `CPPFLAGS=-DLCDPROC_EYEBOXONE ./configure
--enable-drivers=EyeboxOne`

This is only a BETA version modification, take it as a demo...

## Copyright

This section was originally part of the mtxorb.docbook file by Rene
Wagner <reenoo@gmx.de>

This section has been modified by Cédric TESSIER
(http://www.nezetic.info)

## Configuration in LCDd.conf

### \[EyeboxOne\]

  - Device = `DEVICE`  
    Select the output device to use \[default: `/dev/ttyS1`\]

  - Size = PARAMETERS.SIZE  
    Set the display size \[default: `20x4`\]

  - Backlight = PARAMETERS.YESDEFNO  
    Switch on the backlight \[default: `yes`; legal: `yes`, `no`\]
    
    > **Note**
    > 
    > If you choose yes, you can switch on/off the backlight in real
    > time using the LCDproc server menu with the keypad.

  - Cursor = PARAMETERS.YESNODEF  
    Switch on the cursor? \[default: `no`; legal: `yes`, `no`\]

  - Speed = { `1200` | `2400` | `9600` | *`19200`* }  
    Set the the baud rate to use when communicating with the LCD. If not
    specified, it defaults to `19200`.
    
    > **Note**
    > 
    > As I said, I think only `19200` is a good choice.

  - LeftKey = `D`; RightKey = `C`; UpKey = `A`; DownKey = `B`; EscapeKey
    = `P`; EnterKey = `  `  
    The following table translate from EyeboxOne Key to Logical Key.
    EyeboxOne Enter Key is a \\r character, so it's hardcoded in the
    driver.

  - keypad\_test\_mode = PARAMETERS.YESNODEF  
    You can find out which key of your display sends which character by
    setting keypad\_test\_mode to yes and running LCDd. LCDd will output
    all characters it receives. Afterwards you can modify the settings
    above and set keypad\_set\_mode to no again.
