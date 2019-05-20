# The Joystick Input Driver

This section covers the joystick input driver for LCDd.

## Configuration in LCDd.conf

### \[joy\]

  - Device = `DEVICE`  
    Select the input device to use \[default: `/dev/js0`\]

  - Map\_AxisNUMneg = `KEY`; Map\_AxisNUMpos = `KEY`  
    Set the axis map.
    
    NUM is an integer starting with `1` that represents each axis with
    the affixes `neg` and `pos` determining the direction. The exact
    numbering of the axes depends on the hardware used.
    
    KEY can be one of the keys that LCDd recognizes (`Left`, `Right`,
    `Up`, `Down`, `Enter` or `Escape`) or any other string that a client
    can parse.

  - Map\_ButtonNUM = `KEY`  
    Set the button map.
    
    NUM is an integer starting with `1` that represents each button. The
    exact numbering of the buttons depends on the hardware used.
    
    KEY can be one of the keys that LCDd recognizes (`Left`, `Right`,
    `Up`, `Down`, `Enter` or `Escape`) or any other string that a client
    can parse.
