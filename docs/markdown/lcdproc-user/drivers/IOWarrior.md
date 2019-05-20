# The IOWarrior Driver

## General

IOWarrior is the name of a series of multi-purpose USB controller chips
produced and sold by [Code Mercenaries](http://www.codemercs.com/). This
series currently consists of three main types, that - among other
features - support controlling LCD displays:

  - IOWarrior24
    
      - USB 1.1 Low Speed
    
      - 16 generic I/O Pins, typ. 125Hz read rate
    
      - I<sup>2</sup>C master function, 100kHz, throughput typ. 750
        bytes/sec
    
      - SPI master interface, up to 2MHz, throughput typ. 750 bytes/sec
    
      - control an HD44780 compatible LCD
    
      - drive a matrix of up to 8x32 LEDs
    
      - decode RC5 compatible infrared remote controls

  - IOWarrior40
    
      - USB 1.1 Low Speed
    
      - 32 generic I/O Pins, typ. 125Hz read rate
    
      - I<sup>2</sup>C master function, 100kHz, throughput typ. 750
        bytes/sec
    
      - control an HD44780 compatible LCD
    
      - drive a matrix of up to 8x32 LEDs
    
      - drive a 8x8 switch or button matrix

  - IOWarrior56
    
      - USB 1.1 Full Speed
    
      - 50 generic I/O Pins, typ. 1000Hz read rate
    
      - I<sup>2</sup>C master function, 50, 100, or 400kHz
    
      - SPI master interface, up to 12MHz, throughput up to 62Kbytes/sec
    
      - control various display modules, including most graphic modules
    
      - drive a matrix of up to 8x64 LEDs
    
      - drive a 8x8 switch or button matrix

The `IOWarrior` driver currently only supports writing to a
single-controller HD44780-type display. LED output using the `output()`
function is also implemented, although not tested very well. The
hardware's support for input using keys or IR and dual-controller
displays is not implemented yet. Please note that the latter requires
extra circuitry with IOWarrior24 and IOWarrior40.

The driver was developed and tested with IOWarrior24 and the
IOWarrior40. Although there are good chances for it to work with an
IOWarrior56, the current state regarding support of this chip is unknown
due to the lack of the required hardware.

## Requirements

The driver is based on the [`libusb`](http://libusb.info/) USB library,
which should make it work with Linux, the different BSD variants as well
as Darwin/MacOS X.

> **Note**
> 
> When using a `libusb` based driver like `IOWarrior`, LCDd needs to be
> started as root.

> **Note**
> 
> Newer Linux kernels (2.6.20 and higher) provide a kernel module
> `iowarrior.ko` that allows controlling IOWarrior chips using device
> files. LCDd tries to unload this kernel module for `libusb` to be able
> to control IOWarrior devices. If this fails, this may hinder LCDd from
> starting using the `IOWarrior` driver. In this case, simply unload the
> kernel module by hand.

## Configuration in LCDd.conf

### \[IOWarrior\]

  - Size = PARAMETERS.SIZE  
    Set the display dimensions. If not given, it defaults to `20x4`.

  - SerialNumber = `SERIALNO`  
    Use the IOWarrior module with the serial number SERIALNO. If this
    parameter is missing, the default is to use the first IOWarrior
    module found.

  - ExtendedMode = PARAMETERS.YESNODEF  
    If you have a KS0073 or an other 'almost HD44780-compatible' display
    connected to the IOWarrior, set this flag to get into extended,
    4-line linear addressing mode

  - Lastline = PARAMETERS.YESDEFNO  
    Specifies if the last line is pixel addressable or it controls an
    underline effect. The default `yes` means, it is pixel addressable.
