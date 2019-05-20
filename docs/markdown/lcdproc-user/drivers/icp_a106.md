# The icp\_a106 Driver

This section talks about using LCDproc with the ICP Peripheral
Communication Protocol used by A125 and A106 LCD displays

Both LCD and alarm functions are accessed via one serial port, using
separate commands. Unfortunately, the device runs at slow 1200bps and
the LCD does not allow user-defined characters, so the bargraphs do not
look very nice. The A125 does include two buttons marked ENTER and
SELECT. Short press of ENTER: ENTER Long press of ENTER: ESC Short press
of SELECT: DOWN Long press of SELECT: UP

## Configuration in LCDd.conf

### \[icp\_a106\]

  - Device = `DEVICE`  
    Sets the device to use. Defaults to `/dev/lcd`.

  - Size = `SIZE`  
    Sets the display resolution to use. Defaults to 20x2
