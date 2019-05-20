# The lcdm001 Driver

This section talks about using LCDproc with serial LCD displays from
[kernel concepts](http://www.kernelconcepts.de/).

## Configuration in LCDd.conf

### \[lcdm001\]

  - Device = `DEVICE`  
    Default: `/dev/lcd`

  - PauseKey = `KEY`; BackKey = `KEY`; ForwardKey = `KEY`; MainMenuKey =
    `KEY`  
    keypad
    settings
    
    | key name    | function (normal context)   | function (menu context) |
    | ----------- | --------------------------- | ----------------------- |
    | PauseKey    | Pause/Continue              | Enter/select            |
    | BackKey     | Back(Go to previous screen) | Up/Left                 |
    | ForwardKey  | Forward(Go to next screen)  | Down/Right              |
    | MainMenuKey | Open main menu              | Exit/Cancel             |
    

    You can rearrange the settings here.

If your device is broken, have a look at server/drivers/lcdm001.h
