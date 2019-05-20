# The irman Driver

The `irman` driver allows you to use the IrMan IR remote control to
control the LCDproc server LCDd and/or clients that can handle input.

The keys are mapped according to the following table:

| LCDproc key | IrMan command  |
| ----------- | -------------- |
| `Up`        | lcdproc-Up     |
| `Down`      | lcdproc-Down   |
| `Left`      | lcdproc-Left   |
| `Right`     | lcdproc-Right  |
| `Enter`     | lcdproc-Enter  |
| `Escape`    | lcdproc-Escape |

Mapping between LCDproc keys and IrMan commands

> **Tip**
> 
> If you have trouble using the `irman` driver, you might try the `lirc`
> driver. lirc supports IrMan as well.

## Configuration in LCDd.conf

### \[IrMan\]

  - Device = `DEVICE`  
    Select the input device to use, e.g. `/dev/irman`.

  - Config = `FILENAME`  
    Select the IrMan configuration file to use, e.g. `/etc/irman.cfg`.
