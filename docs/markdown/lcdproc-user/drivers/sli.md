# The sli Driver

This section talks about using LCDproc with Serial LCD Interface
(SLI-OEM) boards from Wirz Electronics.

> **Note**
> 
> This driver is intended for use with the original SLI-OEM boards from
> Wirz Electronics. As of 2010 [Parallax,
> Inc.](http://www.parallax.com/) sells serial displays designed by
> Element Products (formerly Wirz Electronics). Due to the lack of
> documentation it is unclear if these devices do work with this driver.

## Configuration in LCDd.conf

### \[sli\]

  - Device = `DEVICE`  
    Select the output device to use \[default: `/dev/lcd`\]

  - Speed = { `1200` | `2400` | `9600` | *`19200`* | `38400` | `57600` |
    `115200` }  
    Set the the baud rate communication with the LCD. If not specified,
    the default is `19200`.
