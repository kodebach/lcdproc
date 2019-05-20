# The \*Really\* Simple Serial interface (rawserial)

This section talks about using LCDproc with custom hardware.

This driver is intended for using LCDproc with custom hardware. In the
original implementation, this was for a PFsense router with a homemade
arduino-style circuit that controlled analog panel meters to display
network traffic. It is intended for this sort of application, where the
LCD control characters emitted normally, as well as the partial screen
drawing, make parsing the serial stream a considerably complex affair,
particularly on small (generally 8 bit) microprocessors.

## Features

The driver emulates a 40x4 display, and simply dumps the entire
framebuffer out the serial port at a rate configured in the config file
(default of 1 Hz), post-pended with a single "\\n" character. At 1 Hz,
it therefore generates 161 serial characters per second.

The maximum update-rate is limited by the internal update-rate of
LCDproc, which is currently 8 Hz, so speeds greater then 8 Hz will be
simply limited to 8 Hz. The update-rate is also granular at 1 second/8
time-intervals (125 ms), so interesting fractional update-rates will
behave oddly. The event-loop is rigidly deterministic, so fractional
rates will \*average\* out to the desired rate, but that will be
achieved by dithering between nearby available intervals.

The baud rate is configurable, with a default of 9600 baud. Baud rate
will have to be chosen with the required data-per-second in mind, since
at 1 Hz, there are 161 characters (40x4+1) per second, which requires
that's a theoretical minimum baud of 161\*10 = 1610 baud. Higher
frame-rates will require higher baud rates.

## Requirements

None. The only requirement is a serial port of some sort.

## Configuration in LCDd.conf

### \[rawserial\]

  - Device = `DEVICE`  
    Serial port to use. Default: `/dev/cuaU0`

  - Speed = `NUMBER`  
    Desired baud-rate. Possible values: `1200`, `2400`, `9600`
    (default), `19200`, and `115200`.

  - Size = PARAMETERS.SIZE  
    Specifies the size of the LCD. If this driver is loaded as a
    secondary driver it always adopts to the size of the primary driver.
    If loaded as the only (or primary) driver, the size can be set.
    Default: `40x4`.

  - UpdateRate = `NUMBER`  
    Desired update-rate in Hertz (e.g. updates per second). Fractional
    values and values less than one are valid (e.g. 0.5). Legal values
    are `0.0005` (equals 2000 seconds) - `10`, with ` 1.0  ` being the
    default.
