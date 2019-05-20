# The serialPOS Driver

This section talks about using LCDproc with a point of sale ("POS")
character-display.

The `serialPOS` driver is currently working with the AEDEX emulation
protocol, and may support the CD5220, Epson ESC/POS, Logic Controls,
EMAX, and Ultimate (UTC/S) protocols. It can be extended to work with
various other protocol displays.

Brightness adjustment, custom characters, and cursor control are
available for protocols that support them. At this moment, custom
character support is limited to only devices with protocols that are
able to load custom characters into the "extended"-ASCII range,
characters from `0x80` onwards. This is to avoid clashing with the usual
ASCII characters.

Custom characters are only used for the rendering of horizontal bars and
vertical bars. For displays whose cell character cell widths are lower
than the number of custom characters supported, then custom characters
will be used to render the horizontal bars.

For displays with sufficient custom character support to include another
set of custom characters, equal in size to the character cell height
minus one, on top of the characters already used to support rendering
custom horizontal bars, then custom characters will be used to render
the vertical bars as well.

Note that some displays use EEPROM to contain their custom characters,
and, upon receiving custom character commands, directly write the
received characters to EEPROM. Excessive writes to EEPROM on
initialization may then cause fast EEPROM wear. Users who do not want
this may like to set the number of custom characters supported by their
display to zero. Displays that store their custom characters in RAM
should work fine, as the driver does not attempt to send commands to
permanently store the custom characters.

The driver should operate most character POS displays with a serial
(RS-232) input. Because these displays use a standardized protocol, if
the protocol is supported by your display, it should work as expected.
Feedback is
welcome.

| Protocol       | Display tested | Currently Supported | Remark                                                      |
| -------------- | -------------- | ------------------- | ----------------------------------------------------------- |
| AEDEX          | Emax, KPD220V7 | Yes                 | Max size: 40x2                                              |
| IEE            |                | No                  |                                                             |
| CD5220         | KPD220V7       | Yes                 | Max size: 20x2, Cell size must be 5x7 for custom characters |
| Epson          | KPD220V7       | Yes                 | Max size: 20x4, Cell size must be 5x7 for custom characters |
| Emax           |                | Yes                 |                                                             |
| IBM            |                | No                  |                                                             |
| Logic Controls | KPD220V7       | Yes                 | Max size: 20x2                                              |
| Ultimate       | KPD220V7       | Yes                 | Max size: 20x2                                              |

serialPOS: Emulation Protocol Status

`(`HELLIP`)`: Feature not tested.

## Connecting The Display

Connecting the display should consist of simply plugging it into your
computer's RS-232 serial port. Because these displays typically support
full RS-232, no additional wiring is needed. If your computer does not
have such a port (many newer computers don't), you can use a USB to
serial adapter with a driver provided by the adapter manufacturer.

If your display supports a *pass-through* function, you can connect an
RS-232 keyboard or terminal to the pass-through port. This will allow
you to input keystrokes to LCDproc and control features and menus. Use
the pass-through keyboard's arrow, delete, and return keys by default.

> **Note**
> 
> If your display supports a *pass-through* function, but you do not
> have another RS-232 device connected to the pass-through port, you may
> experience hangs if an improperly formatted command sneaks through.
> This is because the display is waiting for the pass-through device to
> accept the data and a blocking state is created within the display.
> You can either connect another RS-232 device or use a wire to jumper
> the CTS and RTS pins together within the display.

## Configuration in LCDd.conf

### \[serialPOS\]

  - Device = `DEVICE`  
    Device to use in serial mode. Usual values are `/dev/ttyS0` or
    `/dev/cu.usbserial`. Default is `/dev/ttyS0`.

  - Size = PARAMETERS.SIZE  
    Specifies the size of the VFD in characters. If not given, it
    defaults to `16x2`.

  - Cellsize =  
    Specifies the cell size of each character cell on the display, in
    pixels. If not given, it defaults to `5x8`.

  - Custom\_chars =  
    Specifies the number of custom characters supported by the display.
    Maximum number of custom characters supported by the driver is `32`.
    If the protocol supports custom characters but your display is not
    of the right cell size, set this to zero so the protocol driver
    skips initializing custom character support and succeeds
    initialization. If not given, it defaults to `0`.

  - Type = { *`AEDEX`* | `CD5220` | `Epson` | `Emax` | `LogicControls` |
    `Ultimate` }  
    Set the communication protocol to use with the POS display. If not
    specified it defaults to `AEDEX`.

  - Speed = { `1200` | `2400` | `4800` | *`9600`* | `19200` | `115200`
    }  
    Set the the baud rate communication with the POS display. If not
    given the default of `9600` is used.
