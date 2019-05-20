# The pyramid Driver

This section talks about using LCDproc with programmable LC displays
sold by [Pyramid Computer](http://www.pyramid.de/).

Pyramid Computer builds these LC displays into its thriving server
products to show system data, and to allow the user to change important
parameters or shut down the appliance in a controlled manner. The LCD
module, accessible via USB, can be integrated by Pyramid's BTO server
and appliance manufacturing division at the customer's request or it can
be made available separately for self-integration, e.g. as a 5.25"
module.

## Features

The displays are 16 characters wide and 2 lines high. They have 4
programmable buttons as well as 3 (or 9) LEDs which can also be software
controlled.

## Connector

The display is connected to the host system using USB, with the usual
USB pinout as shown below. The +5V V<sub>CC</sub> pin is marked with red
colour.

| GND | D+             | D-             | V<sub>CC</sub> |
| --- | -------------- | -------------- | -------------- |
| GND | P<sub>0</sub>+ | P<sub>0</sub>- | \+5V           |

USB Pinout

## Requirements

For Linux the driver depends on features of the `ftdi_sio.ko` that have
only been added to version 2.6.15 version of the 2.6 kernel series. For
older Linux kernels of the 2.6 series these patches to `ftdi_sio.c` may
help:

  - [Linux 2.6.9-11](http://www.pyramid.de/upload/files/divers/patch-4-ftdi-2.6.9.txt).

  - [Linux 2.6.12.x](http://www.pyramid.de/upload/files/divers/patch-4-ftdi-2.6.12.6.txt).

  - [Linux 2.6.13-14](http://www.pyramid.de/upload/files/divers/patch-4-linux-2.6.13-14.txt).

## LED output

I've added support for the LEDs on the Pyramid LC-Display to the
"pyramid" driver of lcdproc.

Since it seems that LEDs on an LCD are not directly supported by the
lcdproc API I've used the "output" command of the server to trigger the
LEDs, similar to what the IOWarrior driver does.

The Pyramid LC displays come in two different versions, with 3 and with
9 LEDs. Two of these LEDs can not be controlled by software but are
usually hard wired to power and HDD. The other 1 or 7 LEDs can now be
controlled by sending an `output` command to the server.

The argument to the `output` command is a bitmask that controls the LEDs
according to the table below:

| Bit | LED |
| --- | --- |
| 0   | 3   |
| 1   | 4   |
| 2   | 5   |
| 3   | 6   |
| 4   | 7   |
| 5   | 8   |
| 6   | 9   |

Mapping of `output` bits to LEDs

For the LCDd server there is no way to find out whether 3 or 9 LEDs are
available, so it is up to the client software to do the right thing.

    telnet localhost 13666
    hello
    output 67

will light up LEDs 3, 4 and 9.

    output 0

will clear all LEDs.

More information on the Pyramid LC-Display can be found here:
[](http://www.pyramid.de/en/products/specialities.php)

## Configuration in LCDd.conf

### \[pyramid\]

  - Device = `DEVICE`  
    Device to connect to. Default: `/dev/lcd`
