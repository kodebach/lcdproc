# The Linux Event Device Input Driver

This section covers the linux event device input driver for LCDd.

## Configuration in LCDd.conf

### \[linux\_input\]

  - Device = `DEVICE`  
    Select the input device to use \[default: `/dev/input/event0`\]

  - key = `KEYCODE,KEY`  
    Set an alternate key map, e.g. to use custom buttons instead of the
    standard codes for Escape, Enter, Left, Right, Up and Down. This
    entry typically repeaded for any non-standard key code.
    
    KEYCODE is an integer like the ones defined in
    `/usr/include/linux/input.h`. You can also find the key code in the
    log output of LCDd when the [`ReportLevel`](#server-section) is at
    least 4.
    
    KEY can be one of the keys that LCDd recognizes (`Left`, `Right`,
    `Up`, `Down`, `Enter` or `Escape`) or any other string that a client
    can parse.
