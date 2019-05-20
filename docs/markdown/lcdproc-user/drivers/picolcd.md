# The Mini-Box.com USB LCD picoLCD Driver

This section covers the use of the Mini-Box USB LCD displays.

## Displays

[Mini-Box.com](http://www.mini-box.com/) offers two types of USB LCD
displays:

  - PicoLCD
    4x20-Sideshow  
    [PicoLCD 4x20-Sideshow](http://www.mini-box.com/PicoLCD-4X20-Sideshow)
    is the desktop variant targeted at end users. It is an external USB
    2.0 full speed device that comes in a stylish case and sports a 4
    line by 20 character display with white letters on a blue
    background, a built-in InfraRed receiver as well as a keypad with 8
    keys labeled `Escape`, `F1`, `F2`, `F3`, `Home`, `Up`, `Down` and
    `Enter`.

  - picoLCD 20x2 (OEM)  
    [picoLCD-20x2-OEM](http://www.mini-box.com/picoLCD-20x2-OEM) is the
    OEM version. It is a 2 line by 20 character display with black
    letters on a yellow-green background, that can be connected to the
    system via USB, I<sup>2</sup>C or USART (the latter two are not
    supported by this driver). It has connectors for an InfraRed
    receiver, keypad and LEDs.
    
    When pre-installed in enclosures like the [Mini-Box M300
    LCD](http://www.mini-box.com/Mini-Box-M300-LCD) it comes equipped
    with an InfraRed receiver as well as key pad with 12 keys labeled
    `Plus`, `Minus`, `F1`, `F2`, `F3`, `F4`, `F5`, `Up`, `Down`, `Left`,
    `Right`, and `Enter`.
    
    Finally, the picoLCD 20x2 (OEM) supports 8 general purpose outputs
    and 10 custom splash screens. When the keypad is connected the
    outputs control the key LEDs. The output command and KeyLight
    settings below can be used to control the outputs. Although splash
    screens are not supported by this driver, the splash screens can be
    changed using the `usblcd` tool, that can be built from the Linux
    SDK available on the picoLCD web page.

## Requirements

The driver is based on the [`libusb`](http://libusb.info/) USB library,
which should make it work with Linux, the different BSB variants as well
as Darwin/MacOS X.

> **Note**
> 
> When using a `libusb` based driver like `picolcd`, LCDd needs to be
> started as root.

On Linux, the only kernel module required is the USB host controller
driver (`uhci_hcd` on the M300) to fire up the USB bus to which the LCD
is attached. For other operating systems, analogous requirements apply.

Lastly, for `libusb` to work correctly, the `usbfs` file system must be
mounted on `/proc/bus/usb`, e.g. using the command `mount -t usbfs usbfs
/proc/bus/usb` or by your system's default configuration.

## Configuration in LCDd.conf

### \[picolcd\]

  - Backlight = PARAMETERS.YESDEFNO  
    Turns the backlight on or off on start-up, default `yes`.

  - Brightness = `BRIGHTNESS`  
    Set the initial brightness if the backlight is on. Legal values are:
    `0` - `1000`. If not given, it defaults to `1000`.

  - OffBrightness = `OFFBRIGHTNESS`  
    Set the initial value for the backlight if it is off. Legal values
    are: `0` - `1000`. If not given, it defaults to `0`.

  - Contrast = `CONTRAST`  
    Contrast: `0`-`1000`, default to `1000` (full contrast).

  - LinkLights = PARAMETERS.YESDEFNO  
    Allow key LEDs to be turned on or off with the backlight. Default is
    `yes`.

  - KeyLights = PARAMETERS.YESDEFNO  
    Allow key LEDs to be turned on or off. Default is `yes`. This
    setting affects all keys. If set to `on` each key can be disabled
    independently by setting `KeyXLight` below.

  - Key0Light = PARAMETERS.YESDEFNO  
    If Keylights is set, you can disable the directional pad LED by
    setting this value to `no`. Default is `yes`.

  - Key1Light = PARAMETERS.YESDEFNO  
    If Keylights is set, you can disable the F1 LED by setting this
    value to `no`. Default is `yes`.

  - Key2Light = PARAMETERS.YESDEFNO  
    If Keylights is set, you can disable the F2 LED by setting this
    value to `no`. Default is `yes`.

  - Key3Light = PARAMETERS.YESDEFNO  
    If Keylights is set, you can disable the F3 LED by setting this
    value to `no`. Default is `yes`.

  - Key4Light = PARAMETERS.YESDEFNO  
    If Keylights is set, you can disable the F4 LED by setting this
    value to `no`. Default is `yes`.

  - Key5Light = PARAMETERS.YESDEFNO  
    If Keylights is set, you can disable the F5 LED by setting this
    value to `no`. Default is `yes`.

  - KeyTimeout = `DURATION`  
    KeyTimeout is only used if the picoLCD driver is built with
    libusb-0.1, when built with libusb-1.0 key and IR data is input
    asynchronously so there is no need to wait for the USB data thus
    allowing LCDd to process other inputs at the correct rate.
    
    This value controls how long LCDd waits for a key press when
    get\_key() is called. The value represents milliseconds and the
    default is `500` or .5 seconds. Lowering this value will make LCDd
    more responsive but also causes LCDd to use more CPU time and, as
    the timeout grows shorter, key presses become harder to detect.
    Large values make key presses more reliable but may slow down LCDd.
    Values in the range `0`-`1000` (1s) are allowed.

  - KeyRepeatDelay = `DURATION`  
    KeyRepeatDelay is only used if the picoLCD driver is built with
    libusb-1.0, when built with libusb-0.1 key input blocks all other
    processing until the key is released.
    
    This value controls how long LCDd waits from when a key is pressed
    and reported before generating the first repeat. The value
    represents milliseconds and the default is `300` (0.3 second). Use
    zero to disable auto repeat. Values in the range `0`-`3000` (3s) are
    allowed.

  - KeyRepeatInterval = `DURATION`  
    KeyRepeatInterval is only used if the picoLCD driver is built with
    libusb-1.0, when built with libusb-0.1 key input blocks all other
    processing until the key is released.
    
    This value controls how long LCDd waits between key reports after
    generating the first repeat. The value represents milliseconds and
    the default is `200` (0.2 second). Use zero to disable auto repeat.
    Values in the range `0`-`3000` (3s) are allowed.

  - LircHost = `HOSTNAME_OR_IP-ADDRESS`  
    Set the hostname or IP address to which the driver will send IR data
    from the sensor. If not set or set to an empty value, IR support for
    LIRC will be disabled.
    
    LIRC should be configured to use the driver "udp", which will cause
    it to listen on some UDP port for packets containing a series of
    integers, representing mark and space intervals from the sensor. It
    doesn't matter whether LCDd or LIRC is started first; if LIRC isn't
    listening, the packets from LCDd will be discarded. When LIRC comes
    back, it will start picking up the packets. Similarly, LCDd can be
    stopped and restarted without affecting anything, because UDP is a
    connectionless protocol.

  - LircPort = `PORTNUM`  
    This value determines the UDP port to which the driver will send IR
    data from the sensor. It defaults to `8765`, which is also the
    default port on which LIRC will listen.

  - LircTime\_us = PARAMETERS.YESNODEF  
    If LircTime\_us is set to on mark and space times are sent to LIRC
    in microseconds (requires a LIRC UDP driver that accepts this (LIRC
    0.9.3 onwards)).
    
    If LircTime\_us is set to off mark and space times are sent to LIRC
    in 'jiffies' (1/16384s) (supported by the standard LIRC UDP driver).
    
    Default is `off`.
    
    > **Note**
    > 
    > One 'jiffy' is approximately 61 microseconds about a tenth of
    > typical IR mark and space times. LIRC configuration program
    > `irrecord` cannot reliably detect the IR data timing when measured
    > in 'jiffies' it works better with microseconds.
    > 
    > I have submitted a patch that modifies the LIRC udp driver to
    > support timing data in microseconds. This was included in LIRC
    > 0.9.3. The default (61µs) UDP driver settings will work but I
    > recommend using 1 microsecond. The LIRC upd plugin timing
    > resolution can be set using the `--driver-option=clocktick:value`
    > or `-A clocktick:value` command line switch.

  - LircFlushThreshold = `DURATION`  
    This value is the length in microseconds of the gap that will
    trigger sending the queued IR data to LIRC. Values greater than 1000
    (1ms) are permitted, lower values will set the default value 8000
    (8ms). The maximum depends on the setting of LircTime\_us; if
    LircTime\_us is on values greater than 32.767ms will disable the
    flush, if LircTime\_us is off values greater than 1.999938s will
    disable the flush. The value should be less than the gap times
    specified in `lircd.conf` and greater than any space time specified
    in any header, one, zero, etc. field.

## picolcd driver status

The hardware also reports key-up events. Normally this would be of no
issue (they are usually a 0 or 'no key') except that when keys are used
in combination, the key-up event may actually come back as multiple
events depending on how the user released the keys. If the key-up event
for a multiple key press comes back as two events, the first up event
will actually look like a new key press. The algorithm in get\_key tries
to deal with this in a sane way and toss out all key-up events for now.
The hardware is touchy and both combo key-down and key-up actions may be
reported as multiple events if the user is more than a tenth of a second
(maybe less?) off in motions. The hardware is *not* "touchy" it reports
what it receives. Two key presses or releases may appear simultaneous to
a human but they are always some time apart. The hardware probably
samples the keys for every USB transfer cycle; that is every 10ms,
significantly faster than the typical human response time of a few
hundred milliseconds\!

### Infrared sensor status

LIRC expects sensor data that starts with a longish 'sync' space,
denoting the start of a command; followed by the code data, a sequence
of mark/space pairs; sometimes followed by a 'gap', which should be a
space long enough to make the entire command up to a preset duration in
milliseconds. The 'sync' and the 'gap' are absent from the data emitted
by the picolcd hardware. I found that LIRC configuration files for
remotes similar to the ones I tested all used such a fixed-duration
encoding, and as that was the only way I could get it working, this
driver by default adds the gap as well as the sync. However I have
*still* had trouble getting `irrecord` to work; you need at least to
feed it a template configuration containing sync and gap data. The LIRC
configuration program `irrecord` cannot reliably detect the IR data
timing when measured in 'jiffies' it works better with microseconds, see
`LircTime_us` above.

> **Note**
> 
> The current libusb-1.0 implementation polls at 32Hz to see if any USB
> processing is required, this is needed when a key has been pressed or
> some IR data has been received. 32Hz has a period of 31.25ms this is
> not really fast enough the picoLCD USB transfers can occur every 10ms.
> This may cause buffer overrun problems for long bursts of IR data. The
> best solution would be to include USB processing in the main loop
> `select` statement this has not been done to avoid major changes to
> the core code, to work-round this extra USB transfer buffers are
> allocated. It may also be worth building with `PROCESS_FREQ` set to
> 100Hz (in `server/main.h`).

## Copyright

The lcdproc picolcd driver originally was written by Gatewood Green
(woody@nitrosecurity.com) or (woody@linif.org) and paid for by
NitroSecurity, Inc (nitrosecurity.com), but has been extended with code
from various contributors since then.
