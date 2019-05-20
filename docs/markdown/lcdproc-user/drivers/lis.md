# The lis Driver

This section talks about using LCDproc with the VLSystem L.I.S MCE 2005
Vacuum Fluorescent Display (VFD) based on the FTDI USB-to-serial
converter, the Microchip PIC-16F716 microcontroller, and the NEC
UPD16314 display driver manufactured by
[VLSystem](http://www.vlsys.co.kr).

## Features

This device uses a vacuum fluorescent display of 20 characters by 2
lines. Each each character is 5 pixels wide by 8 pixels high. The device
is connected by USB. The FTDI chip translates the USB protocol to serial
expected by the VFD driver chip, an NEC UPD16314. A programmable
interrupt controller (PIC), the PIC16F716 by Microchip, provides the
glue between the FTDI and the NEC chips.

## Requirements

The driver depends on `libftdi`, version 0.8 or higher, from
<http://www.intra2net.com/en/developer/libftdi/>.

`libftdi` itself depends on the [`libusb`](http://libusb.info/) USB
library.

> **Note**
> 
> When using a `libusb` based driver like `lis`, LCDd needs to be
> started as root.

## Configuration in LCDd.conf

### \[lis\]

  - Size = PARAMETERS.SIZE  
    Set the display size. The default `20x2` should be safe for most if
    not all users, since the device seems to be made only with this one
    size. But who knows HELLIP

  - VendorID = `VENDORID`  
    The USB Vendor ID of the device to use. If not given, it defaults to
    `0x0403` for a VLSystems L.I.S. MCE 2005 VFD based on a FT232BL
    USB-to-RS232 converter by FTDI, which was produced before March
    2007.
    
    It is usually not necessary to specify a VENDORID. Please do so only
    if you want to test a compatible device.

  - ProductID = `PRODUCTID`  
    The USB Product ID of the device to use. If not given, it defaults
    to `0x6001` for a VLSystems L.I.S. MCE 2005 VFD based on a FT232BL
    USB-to-RS232 converter by FTDI, which was produced before March
    2007.
    
    It is usually not necessary to specify a PRODUCTID. Please do so
    only if you want to test a compatible device.

  - Brightness = `BRIGHTNESS`  
    Set the initial brightness \[default: `1000`; legal: `0` - `1000`.
    Values between 0 and 250 give 25% brightness, 251 to 500 give 50%
    brightness, 501 and 750, give 75% brightness, and values higher than
    751 give 100% brightness.
