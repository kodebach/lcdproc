# The lirc Driver

The `lirc` driver enables you to use any IR remote control that works
with LIRC to control the LCDproc server LCDd and/or clients that can
handle input.

Of course you need a working LIRC setup. Refer to the [LIRC
project](http://www.lirc.org) for more information on LIRC itself.

## Checking Your LIRC Setup

Basically all you need is a running lircd. And of course you have to
start lircd as root.

Also, make sure that the permission of `/dev/lircd` are correct.

## Build LCDd with the lirc Driver

You need to add lirc to the --enable-drivers=... list.

Then simply run make.

## Configure LCDd to Use the lirc Driver

First of all you need to activate the driver by adding a `Driver=lirc`
line to your `LCDd.conf`

``` 
 Driver=mtxorb
 Driver=lirc
```

This activates the `mtxorb` driver as the output driver and the `lirc`
driver as the input driver.

Then you have to modify the `[lirc]` section of your `LCDd.conf`.

## Configuration in LCDd.conf

### \[lirc\]

  - lircrc = `FILENAME`  
    Normally all LIRC clients scan the file `~/.lircrc`. However, you
    might want to have a separate file to configure the LCDproc lirc
    driver only.
    
    This option enables you to specify the file you want the lirc driver
    to scan. If not given it defaults to `~/.lircrc`.

  - Prog = `PROGRAM`  
    All LIRC keys are assigned to a program using the `prog=...`
    directive in the lirc configuration file.
    
    PROGRAM must be the same as in your lirc configuration file.

## Modify Your `~/.lircrc`

As mentioned above you can either modify the `~/.lircrc` or use a
separate file for the lirc LCDproc driver (See [above](#which-lircrc)
for details).

No matter which file you use, you have to add at least the following
lines to the file:

    begin
            prog = lcdd
            button = 2
            config = Up
    end
    
    begin
            prog = lcdd
            button = 4
            config = Left
    end
    
    begin
            prog = lcdd
            button = 6
            config = Right
    end
    
    begin
            prog = lcdd
            button = 8
            config = Down
    end
    
    begin
            prog = lcdd
            button = 1
            config = Escape
    end
    
    begin
            prog = lcdd
            button = 0
            config = Enter
    end

Which buttons you specify here depends on your remote control and your
LIRC configuration.

The `config` values need to be one of `Up`, `Down`, `Left`, `Right`,
`Escape` or `Enter`. For LCDd's server menu at least the keys `Up`,
`Escape` and `Enter` are necessary.

Of course you can define other keys. Those keys will not be handled by
the server but sent to a client. Refer to the documentation of the
client you want to use, to find out which keys are necessary for that
client.
