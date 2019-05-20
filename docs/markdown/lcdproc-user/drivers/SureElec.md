# The SureElec Driver

Driver for the LCD modules (actually the controller board) available
from the 'SURE electronics' shop ([](http://www.sureelectronics.net/)).

These devices are PIC based controlled, using a serial communication
protocol with the host. The actual connection to host is done through
USB through a serial-to-USB converter (CP2102 USB to UART Bridge)
integrated on the board.

## Configuration in LCDd.conf

### \[SureElec\]

  - Device = `DEVICE`  
    Name of the device the display appears as. By default first USB
    serial device `/dev/ttyUSB0` is used.

  - Edition = `EDITION`  
    Edition level of the device (can be 1, 2 or 3). The default is `2`.

  - Size = PARAMETERS.SIZE  
    Set the display size in characters. This is required for edition 1
    devices. For edition 2 & 3 devices this value, if defined, overrides
    the size read directly from the device.

  - Contrast = `CONTRAST`  
    Select the display's contrast, `480` is the default. Permissible
    values are in the range of `0`-`1000`.

  - Brightness = `BRIGHTNESS`  
    Select the display's brightness, `480` is the default. Permissible
    values are in the range of `0`-`1000`.

  - OffBrightness = `OFFBRIGHTNESS`  
    Select the display's when the display is normally switched off in
    case LCDd is inactive, `100` is the default. Permissible values are
    in the range of `0`-`1000`.
