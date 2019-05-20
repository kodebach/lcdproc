# The curses Driver

This section talks about using LCDproc with the (n)curses library. This
diver displays an emulated LCD display of configurable size at a
configurable position of the terminal screen using (n)curses.

## Configuration in LCDd.conf

### \[curses\]

  - Foreground = `COLOUR`  
    Set the foreground color. If not given, it defaults to `blue`.
    
    Legal values for COLOUR are `red`, `black`, `green`, `yellow`,
    `blue`, `magenta`, `cyan` and `white`.

  - Background = `COLOUR`  
    Set the background color. The default is `cyan`.
    
    The legal values for COLOUR are the same as for the Foreground
    setting.

  - Backlight = `COLOUR`  
    Set the background color that is to be used when backlight is set
    on. backlight color. If not given, the default is `red`.
    
    The legal values for COLOUR are the same as for the Foreground
    setting.

  - Size = PARAMETERS.SIZE  
    display size \[default: `20x4`\]

  - TopLeftx = `X-OFFSET`; TopLefty = `Y-OFFSET`  
    What position (X,Y) to start the left top corner at. Default: (7,7)

  - UseACS = PARAMETERS.YESNODEF  
    Tell whether to use ACS (alternative character set) symbols for
    icons and bars instead of simple ASCII characters.

  - DrawBorder = PARAMETERS.YESDEFNO  
    Tell whether to draw a border around the screen.
