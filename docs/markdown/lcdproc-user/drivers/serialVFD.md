# The serialVFD Driver

This section talks about using LCDproc with VFD character-displays build
by NEC, FUTABA and NORITAKE.

The serialVFD-driver is working with NEC FIPC8367 based VFDs and the "KD
Rev 2.1" (an ATMEL AT90S.... based FM20X2KB-AB replacement). It is also
known to work on FUTABA VFDs.

The driver should operate most of NEC, Futaba and Noritake 7x5 dot VFDs
with serial(RS-232) and/or parallel interface. See the following table
for testing-status. Feedback is
welcome.

| Display                                 | Controller                | Serial    | Parallel | Display tested | `Type`    | Remark                                                                                                                 |
| --------------------------------------- | ------------------------- | --------- | -------- | -------------- | --------- | ---------------------------------------------------------------------------------------------------------------------- |
| NEC FM20X2KB-AB                         | NEC FIPC8367              | Ok        | Ok       | Yes            | 0         |                                                                                                                        |
| NEC FC20X2JA                            | NEC FIPC8367              | (Ok)\[1\] | (Ok)     | No             | 0         | Same Controller as on FM20X2KB-AB                                                                                      |
| NEC FC20X1SA-AB/AA                      | NEC FIPC8367              | (Ok)      | (Ok)     | No             | 0         | Same Controller as on FM20X2KB-AB                                                                                      |
| KD Rev 2.1                              | AT90S.... microcontroller | Ok        | \-       | Yes            | 1, 0\[2\] |                                                                                                                        |
| FUTABA M402SD06GJ                       | ?                         | (?)       | Ok       | Yes            | 3         | The display has no user-characters. Serial interface with PC compatible baudrate is optional only, feature not tested. |
| FUTABA M204SD01AA                       | FUTABA 5P00A016           | (Ok)      | (Ok)     | No             | 3         |                                                                                                                        |
| Futaba NA202SD08FA                      | ?                         | Ok        | Ok       | Yes            | 6         | almost IEE compatible, no Custom-Characters                                                                            |
| Samsung 20S204DA2 and 20S207DA1         | ?                         | Ok        | Ok       | Yes            | 3, 7      | The display is FUTABA compatible (hard- and software). Custom-Characters not supported(?).                             |
| Samsung 20S207DA4 and 20S207DA6         | ?                         | Ok        | Ok       | Yes            | 7, 3      | almost Futaba compatible                                                                                               |
| Noritake CU20026SCPB-T                  | microcontroller           | (Ok)      | (Ok)     | No             | 2         |                                                                                                                        |
| Noritake CU20045SCPB-T28A               | ?                         | (Ok)      | (Ok)     | No             | 2         |                                                                                                                        |
| IEE 36657-01 (= 02S-93290-VFD 36657-01) | ?                         | Ok        | Ok       | Yes            | 4         |                                                                                                                        |
| IEE S03601-95B                          | ?                         | (Ok)      | (Ok)     | No             | 4         |                                                                                                                        |
| IEE S03601-96-080                       | ?                         | (Ok)      | (Ok)     | No             | 5         |                                                                                                                        |
| Siemens/Wincor Nixdorf BA63/66          | ?                         | Ok        | \-       | Yes            | 8         | Display needs different connection, see below\! no Custom-Characters, no brightness-control                            |

serialVFD: Display Status

If your display isn't working 100% satisfactorily you can add a new
device with modified hardware commands to the driver if you want. To do
that you have to add a new section to the display `Type`-switch-case in
`./server/drivers/serialVFD-displays.c` and to write a new "load"
function with the correct commands for the display. (Try which display
`Type` works best with your display, then copy, rename and modify this
function to your needs - that is the easiest way I guess.)

On Malte Poeggel's page you may find pictures and datasheets of the
VFDs: [](http://www.maltepoeggel.de/index.php?site=vfd)

It is possible to switch the display off and back on while the server is
running. It may take a few minutes until the next full refresh makes the
display show everything correctly.

## Connecting The Display

The Connections shown have been tested successfully.

> **Warning**
> 
> Always cross-check with your datasheet, before connecting your
> display\! Different displays of even the same manufacturer may have
> different pin assignments\!

With this example connections it will be easy to connect displays with
different connector pin-layouts, the pins are commonly named equal in
the datasheet.

### Serial Connections

It is *not* possible to connect most of the displays directly to the
serial port. The signal has to be inverted. I use the following circuit
to do that job.

``` 

Computer                                          Display
                          (signal)
HDD Powerconnector
color(Voltage)

red(+5V)    ----------------------------------o----- +5V
                                              |
black(GND)  ----------------------o          .-.
                                  |          | | 10k
Serial(SUB-D  9Pin female)        |          | |
pin(signal)                       V*         '-'
           ___                                |
3(TxD)  --|___|--o---o      o-----------------o----- RxD
           10k   |   |      |
                 |   |     C|
                .-.  |   -----  BC547c (or similar NPN)
                | |  | B|  /  |
            10k | |  o--|-|   |
                '-'     |  \  |
                 |       --->-    A*
                 |          E|    |
                 |           |    |
5(GND)  ---------o-----------o----o--------------o-- GND
                                                 |*
Shield  -----------------------------------------o
optional

*connect near display
```

The pins on the different displays vary.

#### NEC Displays (FM20X2KB-AB):

CN1:

    Pin 33 ------- RxD
    (Testmode: connect pins 25 and 26, then power up)

CN2:

    Pin 1  ------- +5V
    Pin 2  ------- GND

#### KD Rev 2.1:

The blue connector (6pin in a row) (the important one\!):

``` 

--------------------------------
| +5V  +5V  RxD  GND  GND  GND |
--------------------------------
```

> **Tip**
> 
> Hold the display in that position where you can read the `KD Rev 2.1`
> marking normally\!

The gray connector (10pin 2 rows):

    Do not use. (the ATMEL ISP connector I guess)

The two jumpers next to the gray connector:

    Normally not used.
    You can activate two different testmodes with them.

#### FUTABA Displays (M402SD06GJ):

    Pin 2   ------ +5V
    Pin 4   ------ +5V
    Pin 6   ------ +5V
    
    Pin 10  ------ GND
    Pin 12  ------ GND
    Pin 14  ------ GND
    
    Pin 19  ------ RxD

#### Siemens/Wincor Nixdorf BA63/66:

This display doesn't need the inverter\! It must be connected directly
with the serial port.

Check the serial port setup of the display, it has to be "9600 8N1". In
most cases JP3 needs to be modified (closed) by the user\!

    Suggested Jumper Setting:
    JP1 open, JP2 open, JP3 closed, JP4 open, JP5 open

More detailed information can be found in the users manual of the
display.

### Parallel Connections

#### NEC Displays (FM20X2KB-AB):

CN1:

    parallel Port:          Display (NEC):
    Pin:    Name:           Name:   Pin:
    
    1   /STROBE --------------  /WR 17
    2   D0      --------------  D0  15
    3   D1      --------------  D1  13
    4   D2      --------------  D2  11
    5   D3      --------------  D3  9
    6   D4      --------------  D4  7
    7   D5      --------------  D5  5
    8   D6      --------------  D6  3
    9   D7      --------------  D7  1
    10  /ACK    --o
              |
    11  BUSY    --o-----------  BUSY    27
    
                   o--  /CS 23
                   |
    18 - 25 GND    ------------o--  GND 2,4,6,8,10,12,14,16,18,20,22,24,26,28,20,32,34
                   |*
    Shield  -------------------o
    optional
    
    *connect near display

CN2:

    Pin 1  ------ +5V
    Pin 2  ------ GND

#### FUTABA Displays (M402SD06GJ):

    parallel Port:          Display (FUTABA):
    Pin:    Name:           Name:   Pin:
    
    1   /STROBE --------------  /WR 17
    2   D0      --------------  D0  15
    3   D1      --------------  D1  13
    4   D2      --------------  D2  11
    5   D3      --------------  D3  9
    6   D4      --------------  D4  7
    7   D5      --------------  D5  5
    8   D6      --------------  D6  3
    9   D7      --------------  D7  1
    10  /ACK    --o
              |
    11  BUSY    --o-----------  BUSY    20
    
                   o--  /SEL    18
                   |
    18 - 25 GND    ------------o--  GND 10,12,14
                   |*
    Shield  -------------------o
    optional
                   0--  TEST    16
                   |
            +5V -------o--  +5V 2,4,6
    
    *connect near display

## Configuration in LCDd.conf

### \[serialVFD\]

  - use\_parallel = PARAMETERS.YESDEFNO  
    "`no`" if display connected serial, "`yes`" if connected parallel
    \[default: `no`(serial)\].

  - Port = `PORT`  
    Portaddress where the LPT is. Used in parallel mode only. Usual
    values are 0x278, 0x378 and 0x3BC \[default: `0x278`\].

  - PortWait = `DELAY`  
    Set parallel port timing delay (us). Used in parallel mode only.
    \[default: `2`; legal: 0 - 255\].

  - Device = `DEVICE`  
    Device to use in serial mode. Usual values are `/dev/ttyS0` and
    `/dev/ttyS1` \[default: `/dev/lcd`\].

  - Custom-Characters = `CUSTOM-CHARACTERS`  
    Number of Custom-Characters \[default: Display-Type dependent\].

  - Size = PARAMETERS.SIZE  
    Specifies the size of the VFD. \[default: `20x2`\]

  - Type = `CODE`  
    Specifies the display type.\[default: 0\] The following type codes
    are available:
    
    | CODE  | VFD model                                   |
    | ----- | ------------------------------------------- |
    | *`0`* | NEC (FIPC8367 based) VFDs                   |
    | `1`   | KD Rev 2.1                                  |
    | `2`   | Noritake VFDs                               |
    | `3`   | Futaba VFDs                                 |
    | `4`   | IEE S03601-95B                              |
    | `5`   | IEE S03601-96-080                           |
    | `6`   | Futaba NA202SD08FA (allmost IEE compatible) |
    | `7`   | Samsung 20S207DA?                           |
    | `8`   | Siemens/Wincor Nixdorf BA63/66              |
    

    > **Note**
    > 
    > Noritake VFDs have not been tested yet. Feedback is welcome.

  - Brightness = `BRIGHTNESS`  
    Set the initial brightness \[default: 1000; legal: 0 - 1000\] (4
    steps 0-250, 251-500, 501-750, 751-1000)

  - OffBrightness = `OFFBRIGHTNESS`  
    Set the initial off-brightness \[default: 0; legal: 0 - 1000\]. This
    value is used when the display is normally switched off in case LCDd
    is inactive. (4 steps 0-250, 251-500, 501-750, 751-1000)

  - Speed = { `1200` | `2400` | *`9600`* | `19200` | `115200` }  
    Set the the baud rate communication with the VFD. If not given
    \[default `9600`\].

  - ISO\_8859\_1 = PARAMETERS.YESDEFNO  
    Enable ISO-8859-1 compatibility \[default is `yes`\].

<!-- end list -->

1.  Should work, but feature not tested yet.

2.  Custom-Characters are not supported with this `Type`, set
    `Custom-Characters=0` in `LCDd.conf`.
