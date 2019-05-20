# The vlsys\_m428 driver

## General

The vlsys\_m428 driver handles the VFD/IR combination from VLSystem
built into the case MonCaso 320 from the manufacturer Moneual. The
driver functionality is rudimentary as it is derived from a reverse
engineering from the Windows driver behaviour.

This driver writes only to the serial port, and it triggers no
transmission activity of the combination. Thus, it cooperates with
drivers reading only from the port. Other drivers are allowed to
initialize the combination for IR operation.

The vlsys\_m428 driver assumes the character encoding ISO 8859-1. Note:
The mapping from the character encoding to the display encoding is not
complete; furthermore some characters are approximated by similar ones.

## Configuration

The only configuration parameter is Device, the path name of the serial
device (a mapper between USB and serial line emulation). By default it
is `/dev/ttyUSB0`.
