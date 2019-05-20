# The bayrad Driver

This section talks about using LCDproc with the BayRAD LCD modules by
EMAC, Inc.

## Features

The BayRAD LCD modules are designed to fit into 5,25" drive bays. They
contain an LCD display that is 20 characters wide and 2 lines high
surrounded by 4 buttons labeled `Menu`, `Select`, `+/Yes`, and `-/No`.

BayRAD modules are connected to the PC using a serial RS232 connection
getting operating power using the standard floppy drive power connector.

For more information see the [BayRAD home
page](http://www.emacinc.com/bay_rad.htm)

## Configuration in LCDd.conf

### \[bayrad\]

  - Device = `DEVICE`  
    Select the serial output device to use. If not given, default is
    `/dev/lcd`.

  - Speed = { `1200` | `2400` | *`9600`* | `19200` }  
    Set the the baud rate to use when communicating with the LCD. `9600`
    is the default, if not specified.
