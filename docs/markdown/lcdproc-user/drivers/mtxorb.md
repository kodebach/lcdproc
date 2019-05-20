# The MtxOrb Driver

This section covers the installation process for the Matrix Orbital LCD
module intended for use with LCDproc.

We will examine the installation process of the hardware in small steps,
as it is vitally important to pay close attention to detail during
hardware installation to avoid damaging equipment.

## Matrix Orbital LCD Modules

LCDproc was born out of original tinkering by William Ferrell with one
of these LCD modules. Their ease of installation and use (as well as the
amazing amount of patience demonstrated by the folks at Matrix Orbital
whilst William figured things out) meant one less thing to worry about
during the early stages of LCDproc's life.

These 20x4 alphanumeric modules are connected via standard DB-9 cabling
and connectors. They draw either 5V or 12V, depending on the module
purchased, and are attached with a standard floppy cable connector (with
a slightly modified wire configuration).

Once connected, using them is a breeze. They can operate at any number
of different baud rates and serial configurations, but normally they run
at 19,200 baud, 8-N-1, making them quite quick. Sending ASCII to the
module will make it simply display that text at its current cursor
position. The module has a built-in BIOS that recognizes commands (sent
by transmitting a single-byte "marker" signifying that a command is on
the way, followed by the single-byte command character itself along with
any parameters, if needed) allowing the programmer to clear the screen,
position the cursor anywhere, define custom characters (up to 8 at a
time), draw bar graphs and large numbers, change the LCD's contrast, and
so on.

The BIOS included also implements line-wrapping (i.e. writing past the
twentieth character on the first row will automatically move the cursor
to the first character on the second row), and screen scrolling (i.e.
writing past the twentieth character on the fourth row causes the whole
screen to scroll up one row, clearing the fourth line and positioning
the cursor at the first character on that line).

These modules are fast. Using the auto-line-wrap feature and disabling
the auto-scrolling feature, the screen can be updated thirty times per
second if \*every\* character on the screen is changed. If updating less
than the whole screen, the LCD can update faster than can be seen by the
human eye. This, of course, more than meets LCDproc's needs.

## Matrix Orbital Hardware Installation

Regardless of what specific type of hardware you intend to use with
LCDproc, installation is usually straightforward, and requires only a
few steps. Regardless, you must use caution while working inside your
computer system or with any hardware attachments.

> **Warning**
> 
> Installing new hardware inside a computer system can be dangerous to
> both system components and the installer. Use caution whenever adding
> a component to the inside of your system, altering a power cable, or
> physically mounting a device inside a computer system.
> 
> When installing hardware inside a computer, make sure it's turned off
> and that its power is disconnected. This is especially important when
> making changes to power cables (as some LCD modules require).

### Matrix Orbital LCD/VFD Module Installation

The LCD and VFD modules from Matrix Orbital are relatively
straightforward to install. With a small, regular (flat-head)
screwdriver, a spare floppy drive power cable, and a bit of luck,
installation will take less than an hour.

These installation instructions assume that you are installing the
module into a PC or PC-style system (one with AT- or ATX-compliant power
cabling) and that you have some idea of where you intend to permanently
mount the module.

> **Tip**
> 
> Your Matrix Orbital LCD or VFD module should be clearly marked with an
> indication of the module's power requirements. It should be either a 5
> volt or 12 volt unit. You should have this information available
> before proceeding.

#### Power Cable Modification

The first step in installing the module is making the necessary
modifications to a floppy drive power cable in order to provide power to
the module. The modifications must be made based on the module's power
requirements -- either 5V or 12V -- depending on which module you
purchased.

A standard floppy drive power cable has a smaller connection than a
"normal" PC power connector. However, like a "normal" power connector,
it has four wires: one yellow, one red, and two black. The red wire
provides +5V power, and is "hot" or live when the system is powered up.
The yellow wire provides +12V power, and is also hot when the system is
powered up. Both black wires are ground. \[TODO: INCLUDE A FIGURE HERE
SHOWING A "STANDARD" FLOPPY CONNECTOR\]

One of the hot wires and one of the black wires will not be needed for
your module's power connection; they will be completely removed when the
power cable modification is complete.

> **Warning**
> 
> Do NOT make this modification to a power cable attached to a running
> system\! Electrocution resulting in personal injury and/or damage to
> the system can result.

Using a regular screwdriver, press down the small metal locking flap of
one of the two black wires on the small end of the cable, and pull the
black wire from the connector. Using a pair of needle-nose pliers,
squeeze the other end of the same black wire, and pull it out of the
large end of the cable. This black wire can be set aside; it will not be
used for the module's power connection. Either wire can be safely
removed; you may safely remove either wire. \[TODO: INCLUDE A FIGURE
HERE SHOWING THIS PROCESS\]

Next, using the same procedure, remove the unneeded hot wire. If your
module is 5V, you do not need the yellow (+12V) wire. Conversely, if
your module is 12V, you do not need the red (+5V) wire. The removed wire
can be set aside; it will not be used for the module's power connection.
\[TODO: INCLUDE A FIGURE HERE\]

The floppy power connector should now have only two wires attached to
it. Leave the larger end alone from now on; these connections are
correct (the larger end connects to your system's power mains). Move the
two remaining wires to the outside connectors on the small end of the
cable. Orientation does not particularly matter here; the connector will
fit on the module's receptacle in either orientation. \[TODO: A FIGURE
HERE\]

You should now have a properly modified power connector. When physically
attaching this connector to the module, the black (ground) lead should
be connected to the pin labelled GND, while the colored (+5V/+12V) lead
should be connected to the pin labelled +5V/+12V.

Test the power connection before connecting the data line or mounting
the module. Connect the module to the power connector, and the connector
to your system's power mains. Turn the system on.

> **Caution**
> 
> If the module does not immediately display its initial BIOS screen and
> light up its backlight (or light up the screen if a VFD module is
> being used), *immediately* power down the system, disconnect the
> module and connector, and double-check the modification before trying
> again. Do NOT leave the system on if the module does not immediately
> respond; module or system damage could result.

When the LCD powers up and displays its initial BIOS screen, you've
gotten the power connection wired properly and can now properly mount
the module and make its final connections. Matrix Orbital Corporation
sells a PC bay insert mount for the 20x4 and 20x2 modules (LCDproc,
however, only supports the 20x4 at present). The inserts provide an easy
means of mounting the LCD modules inside a PC using one (for the 20x2)
or two (for the 20x4) 5 1/4" bays.

> **Note**
> 
> Describing how to physically mount the module in a PC case is beyond
> the scope of this document; LCDproc's website contains more detailed
> mounting information and examples.

#### Serial Connection

The LCD module uses a standard DB9 serial connector. You can attach the
module to your system using a direct cable to the motherboard, or by
removing one of your system's serial ports from the back of the case,
then connecting it to a standard serial cable to the module.

While connecting the serial cable to the module, be sure to configure
the module's serial interface settings. Typically, setting the module to
its fastest setting (19,200 baud, 8-N-1) is recommended. The speed
settings can be configured from the config file `/etc/LCDd.conf`. If not
specified in the config file, the Matrix Orbital module driver in
LCDproc default to use these settings.

## Configuration in LCDd.conf

### \[MtxOrb\]

  - Device = `DEVICE`  
    Select the output device to use \[default: `/dev/lcd`\]

  - Size = PARAMETERS.SIZE  
    Set the display size \[default: `20x4`\]

  - Type = { *`lcd`* | `lkd` | `vfd` | `vkd` }  
    Set the display type \[default: `lcd`; legal: `lcd`, `lkd`, `vfd`,
    `vkd`\]

  - Contrast = `CONTRAST`  
    Set the initial contrast. Legal values for CONTRAST are `0` -
    `1000`. If not given, the default value is `480`.
    
    > **Note**
    > 
    > The driver will ignore this setting if the display is a VFD or VKD
    > as they do not support this feature.

  - hasAdjustableBacklight = PARAMETERS.YESDEFNO  
    Some old firmware versions of Matrix Orbital modules do not support
    an adjustable backlight but only can switch the backlight on/off. If
    you own such a module and experience randomly appearing block
    characters and backlight cannot be switched on or off, use this to
    `no` \[default: `yes`\].

  - Brightness = `BRIGHTNESS`  
    Set the initial brightness \[default: `1000`; legal: `0` - `1000`\]

  - OffBrightness = `BRIGHTNESS`  
    Set the initial off-brightness \[default: `0`; legal: `0` - `1000`\]
    This value is used when the display is normally switched off in case
    LCDd is inactive

  - Speed = { `1200` | `2400` | `9600` | *`19200`* }  
    Set the the baud rate to use when communicating with the LCD. If not
    specified, it defaults to `19200`.

  - KeyMap\_LETTER = `KEY`  
    Matrix Orbital displays support keypads with up to 25 keys, which
    return one of the letters `A` - `Y` for each pressed key.
    
    These settings allow to map the letter LETTER, that is generated by
    the display when a key is pressed, to be mapped to a key name KEY
    that LCDd can understand (see [\#menu-section](#menu-section) for
    more information).
    
    There is no default key mapping; if no keys are mapped in the
    `LCDd.conf` config file, the display is treated as if it had no keys
    attached.
    
    ``` 
          KeyMap_A=Left
          KeyMap_B=Right
          KeyMap_C=Up
          KeyMap_D=Down
          KeyMap_E=Enter
          KeyMap_F=Escape
          
    ```

  - keypad\_test\_mode = PARAMETERS.YESNODEF  
    You can find out which key of your display sends which character by
    setting keypad\_test\_mode to yes and running LCDd. LCDd will output
    all characters it receives. Afterwards you can modify the settings
    above and set keypad\_set\_mode to no again.
