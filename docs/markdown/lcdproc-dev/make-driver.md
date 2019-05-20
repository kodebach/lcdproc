# Shared files specific for drivers

Driving an LCD display is not easy; you need to address ports, to send
bytes in a certain order, to respect timing, and unfortunately no two
operating system let you do this in the same way. But don't despair\!
There's hope\! Someone in a galaxy far far away, has Already done the
dirty job for you\! This dirty job has been put in shared files. These
shared files are full cross platform and are auto-magically configured
by the configure script. You only need to include them and use their
functions to benefit from them.

These files are provided only for drivers, others are provided for all
of LCDproc. These files are located in the shared directory, they have a
dedicated chapter in this book.

## port.h : Parallel port I/O

The file `port.h`, located in the `server/drivers/` directory provide
Input/Output and port permissions for the PC compatible parallel port,
also known as the LPT port.

Of course, these functions will only work if the computer where LCDproc
runs has parallel port\! In these situations, the configure script will
see this and disable drivers that need a parallel port.

The functions in `port.h` are defined and as 'static inline'. Therefore
each driver including this header file gets its own copy of the
functions and they are inlined into the driver's code. As a result calls
to port\_in() and port\_out() are directly translated to inb() or outb()
or assembly code by the compiler. There is less to no overhead in using
them.

`port.h` file defines 6 static inline functions for port I/O:

### Read a byte from port

static inline int

port\_in

unsigned short int

port

Returns the content of the byte.

### Write a char(byte) 'val' to port

static inline void

port\_out

unsigned short int

port

unsigned char

val

Returns nothing (void).

### Get access to a specific port

static inline int

port\_access

unsigned short int

port

Returns `0` if successful, `-1` if failed.

### Close access to a specific port

static inline int

port\_deny

unsigned short int

port

Returns `0` if successful, `-1` if failed.

### Get access to multiple sequential ports

static inline int

port\_access\_full

unsigned short int

port

unsigned short int

count

Returns `0` if successful, `-1` if failed.

### Close access to multiple sequential ports

static inline int

port\_deny\_full

unsigned short int

port

unsigned short int

count

Returns `0` if successful, `-1` if failed.

### Example use

    #include "port.h"
    
    /*
     * Get access to these 3 ports:
     *  0x378 (CONTROL),
     *  0x379 (STATUS) and
     *  0x37A (DATA)
     */
    if (port_access_multiple(0x378, 3) == -1) {
        /* Access denied, do something */
    }
    
    /* Write a 'A' to the control port */
    port_out(0x378, 'A');
    
    /* Read from the status port */
    char status = port_in(0x379);
    
    /* Close the 3 ports */
    port_deny_multiple(0x378, 3);

## adv\_bignum.h : Write Big-Numbers

`adv_bignum.h` is the headerfile for `libbignum.a` (made from
`adv_bignum.c`) which contains everything needed to show big-numbers,
including the fonts for the different displays. (All files are located
in the `server/drivers/` directory.)

There are only a few requirements to the calling driver:

  - The following functions have to be implemented by the driver:
    
      - `height()`  
        to determine the display's height and thus the maximal height of
        the big numbers to be displayed.
    
      - `get_free_chars()`  
        to determine the number of user-definable characters that can be
        used in the generation of big numbers.
    
      - `set_char()`  
        to define a character necessary to write a big number. Of course
        this is only necessary if there really are user-definable
        characters, i.e. only if `get_free_chars()` returns a value
        greater `0`.
    
      - `chr()`  
        to actually write the characters the big numbers consist of.

  - The display's cellwidth has to be `5` (`6` works also in some cases)
    and the cellheight `7` or `8`.

  - The custom-characters (if any) have to be at character positions
    offset`+0`, offset`+1`, offset`+2`, ... offset`+`
    `get_free_chars()``-1`,

  - offset`+` `get_free_chars()``-1` must be less than `32`,

The library determines the correct font, depending on the display size
and the number of user-defined characters itself. So it is easy to
integrate into the driver.

### Provided Functions

void

lib\_adv\_bignum

Driver \*

drvthis

int

x

int

num

int

offset

int

do\_init

The main thing the driver has to do is to call this function from its
`num()` function with the parameters described below.

  - `drvthis`  
    the pointer pointing to the Driver structure passed to the driver's
    `num()` function.

  - `x`  
    the horizontal position of the top-left corner of the big-number
    (the big-numbers don't have a y position). The placing of the
    characters is done by the client, so the driver only has to forward
    the position to the lib. The bignumlib has no influence on the
    placing of the characters.

  - `num`  
    the number (legal: `0` - `9`, and `:`) to be written.

  - `offset`  
    the character position where the user-definable characters start
    (usually `0`). The user-definable characters (if any) are then
    expected to be at the character positions offset`+0`, offset`+1`,
    offset`+2`, ... offset`+` `get_free_chars()``-1` and offset`+`
    `get_free_chars()``-1` is required to be less than `32`.

  - `do_init`  
    if not `0`, lib\_adv\_bignum will set the custom characters of the
    display for the big-numbers.
    
    The driver has to check if the custom-characters have to be set or
    if it is already done and tell it to the lib (using the `do_init`
    parameter). The common way is to use variable called p-\>ccmode or
    similar. In the different drivers there are some differences in the
    naming and handling of this variable. So the responsibility of
    checking and setting is left to the driver.

<!-- end list -->

    #include "adv_bignum.h"
    
    MODULE_EXPORT void
    myDriver_num( Driver * drvthis, int x, int num )
    {
      PrivateData *p = drvthis->private_data;
      int do_init = 0;
    
      if (p->ccmode != CCMODE_BIGNUM){     // Are the custom-characters set up correctly? If not:
        do_init = 1;                       // Lib_adv_bignum has to set the custom-characters.
        p->ccmode = CCMODE_BIGNUM;         // Switch custom-charactermode to bignum.
      }
    
      // Lib_adv_bignum does everything needed to show the big-numbers.
      lib_adv_bignum(drvthis, x, num, 0, do_init);
    }

All that's left to do is to add `libbignum.a` to the libs and
`adv_bignum.h` sources of your driver in the `Makefile` (or the file
that generates the `Makefile`).

    myDriver_LDADD: libLCD.a libbignum.a
    
    myDriver_SOURCES: lcd.h lcd_lib.h myDriver.c myDriver.h adv_bignum.h

### Internal Structure and Functions

The only purpose of `lib_adv_bignum()` is to determine the best
display-dependent big-number function, based upon the values of the
driver's `height()` and `get_free_chars()` functions, and call it.

The display-dependent functions are named `adv_bignum_num_N_M()`, where
N is the display's height in lines and M the number of used
user-definable characters. The bits of the user-characters are stored in
static char bignum (take a look at the source and you will see what I
mean). (On a display with a cellheight of 7 the lowest line stored is
not shown.) While static char num\_map defines the placing in the big
number. (A big number is always 3 characters wide and 4 characters high.
On a big number for 2 line displays the 2 lower lines are not in use.)

If user-definable characters have to be set, the driver's `set_char()`
function will be called once for every user-definable character.

Now `adv_bignum_write_num()` is called. This function places the 6 or 12
characters the big-number consists of in the framebuffer using the
drivers `chr()` function.
