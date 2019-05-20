# The SDEC LCD Driver

The SDEC LCD modules are fitted to the Watchguard Firebox firewall
appliances. Watchguard identifies these units by the X-Core, X-e and
X-Peak product lines. The model number indicates a software license
level, and has no bearing on the hardware configuration.

This driver implements the same protocol as the [HD44780 with lcm162
connection type](#hd44780-8bit-lcm162).

## Features

The SDEC LCDs are interfaced to the appliance using a parallel port. All
lines are hard-wired by Watchguard. The LCD displays are 20 characters
wide and 2 lines high, and have 4 buttons: `Up`, `Down`, `Left`, and
`Right`. The SDEC model number is `LMC-S2D20-01`. There is a back light
that can be controlled by software. Due to its low half-life, the driver
tries to keep the light off a short while after buttons have been
pressed.

For more detailed information about the SDEC LCD, locate the technical
and programming guide LMC-S2D20-01.pdf. The manufacturer web site is:
[SDEC home page](http://www.sdec.com.tw)
