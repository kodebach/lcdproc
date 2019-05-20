# The irtrans Driver

## General

The `irtrans` driver controls [IRTrans
VFD](http://www.irtrans.de/en/main.php) devices, that are preinstalled
in cases such as the Ahanix MCE303.

The IRTrans VFD sports a vacuum fluorescent display with 16x2 characters
that connects to the computer using USB. Although the device is shipped
with an IR remote control, LCDproc's driver currently only supports the
display part of the device.

In order to be able to use it, you have to get and install the IRTrans
irserver package from [](http://www.irtrans.de/en/download/linux.php).

## Configuration in LCDd.conf

### \[irtrans\]

  - Backlight = PARAMETERS.YESNODEF  
    Tell whether the device has a backlight, or whether the backlight
    shall be used. If not given, it defaults to `no`.

  - Hostname = `HOSTNAME_OR_IP-ADDRESS`  
    Set the hostname or IP address of the IRTrans device to connect to.
    If not set or set, the default is `localhost`.

  - Size = PARAMETERS.SIZE  
    Select the display size \[default: `16x2`\]
