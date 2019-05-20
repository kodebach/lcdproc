# The LCDproc driver API

This chapter describes the driver API of v0.5 of LCDproc.

# Overview of Operation

The API defines functions which drivers may implement to provide certain
services. If a driver implements a function, the function will be
detected by the server. For some optional functions the server core
provides default implementations.

The API consists of functions to:

  - print data to the display,

  - set display properties (backlight, brightness, contrast, extra
    LEDs),

  - read input from an input device (may be the display), and

  - inform the server of the driver/display capabilities.

The API is best described by starting with the struct
lcd\_logical\_driver which is defined in server/drivers/lcd.h.

Below is a commented version of lcd\_logical\_driver. Each function is
described more detailed in [Functions in Detail](#function-details).

``` 

typedef struct lcd_logical_driver {

    //////// Variables to be provided by the driver module
    // The driver loader will look for symbols with these names !

    // pointer to a string describing the API version
    char *api_version;

    // Does this driver require to be in foreground ?
    int *stay_in_foreground;

    / Does this driver support multiple instances ?
    int *supports_multiple;

    // What should alternatively be prepended to the function names ?
    char **symbol_prefix;

    //////// Functions to be provided by the driver module

    //// Mandatory functions (necessary for all drivers)

    // initialize driver: returns >= 0 on success
    int (*init)     (Driver *drvthis);

    // close driver
    void (*close)       (Driver *drvthis);


    //// Essential output functions (necessary for output drivers)

    // get display width / height (in characters; 1-based)
    int (*width)        (Driver *drvthis);
    int (*height)       (Driver *drvthis);

    // clear screen
    void (*clear)       (Driver *drvthis);

    // flush screen contents to LCD
    void (*flush)       (Driver *drvthis);

    // write string s at position (x,y)
    void (*string)      (Driver *drvthis, int x, int y, const char *str);

    // write char c at position (x,y)
    void (*chr)     (Driver *drvthis, int x, int y, char c);


    //// essential input functions (necessary for input drivers)

    // get key from driver: returns a string denoting the key pressed
    const char *(*get_key)  (Driver *drvthis);


    //// Extended output functions (optional; core provides alternatives)

    // draw a bar from pos (x,y) upward / to the right filling promille of len chars
    void (*vbar)        (Driver *drvthis, int x, int y, int len, int promille, int options);
    void (*hbar)        (Driver *drvthis, int x, int y, int len, int promille, int options);

    // draw a progressbar covering lenght chars at pos (x,y) showing promille% progress
    void (*pbar)        (Driver *drvthis, int x, int y, int len, int promille);

    // display (big) number num at horizontal position x
    void (*num)     (Driver *drvthis, int x, int num);

    // set heartbeat state; animate heartbeat
    void (*heartbeat)   (Driver *drvthis, int state);

    // draw named icon at position (x,y)
    int (*icon)     (Driver *drvthis, int x, int y, int icon);

    // set cursor type and move it to position (x,y)
    void (*cursor)      (Driver *drvthis, int x, int y, int type);


    //// User-defined character functions

    // set special character / get free characters
    // - It is currently unclear how this system should work exactly
    // - The set_char function expects a simple block of data with 1 byte for each pixel-line.
    //   (So that is 8 bytes for a 5x8 char)
    void (*set_char)    (Driver *drvthis, int n, unsigned char *dat);
    int (*get_free_chars)   (Driver *drvthis);

    // get width / height of a character cell (in pixels)
    // - necessary to provide info about cell size to clients
    // - if not defined, the core will provide alternatives returning default values
    int (*cellwidth)    (Driver *drvthis);
    int (*cellheight)   (Driver *drvthis);


    //// Hardware functions

    // get / set the display's contrast
    int (*get_contrast) (Driver *drvthis);
    void (*set_contrast)    (Driver *drvthis, int promille);

    // get / set brightness for given backlight state
    int (*get_brightness)   (Driver *drvthis, int state);
    void (*set_brightness)  (Driver *drvthis, int state, int promille);

    // set backlight state
    void (*backlight)   (Driver *drvthis, int state);

    // set output
    void (*output)      (Driver *drvthis, int state);


    //// Informational functions
    // get a string describing the driver and it's features
    const char * (*get_info) (Driver *drvthis);



    //////// Variables in server core, available for drivers

    // name of the driver instance (name of the config file section)
    // - do not change from the driver; consider it read-only
    // - to be used to access the driver's own section in the config file
    char * name;

    // pointer to the driver instance's private data
    // - filled by the server by calling store_private_ptr()
    // - the driver should cast this to it's own private structure pointer
    void * private_data;


    //////// Functions in server core, available for drivers

    // store a pointer to the driver instance's private data
    int (*store_private_ptr) (struct lcd_logical_driver * driver, void * private_data);

    // Config file functions, cwprovided by the server
    // - see configfile.h on how to use these functions
    // - as sectionname, always use the driver name: drvthis->name
    short (*config_get_bool) (char * sectionname, char * keyname,
                int skip, short default_value);
    long int (*config_get_int) (char * sectionname, char * keyname,
                int skip, long int default_value);
    double (*config_get_float) (char * sectionname, char * keyname,
                int skip, double default_value);
    const char *(*config_get_string) (char * sectionname, char * keyname,
                int skip, const char * default_value);
                // Returns a string in server memory space.
                // Copy this string.
    int config_has_section  (const char *sectionname);
    int config_has_key  (const char *sectionname, const char *keyname);

    // Display properties functions (for drivers that adapt to other loaded drivers)
    // - the return the size of another already loaded driver
    // - if no driver is loaded yet, the return values will be 0
    int (*get_display_width) ();
    int (*get_display_height) ();
} Driver;
```

# Private Data

To support multiple instances it is necessary to stop using global
variables to store a driver's data in. Instead, you should store it in a
structure, that you allocate and store on driver's init.

In the driver's private structure will probably at least be something
like:

    typedef struct MyDriver_private_data {
        int fd;             // file descriptor for the LCD device
            int width, height;      // dimension of the LCD (in characters, 1-based
            int cellwidth, cellheight;  // Size of each LCD cell, in pixels
            unsigned char *framebuf;    // Frame buffer...
    } PrivateData;

You allocate and store this structure like this:

``` 
    PrivateData *p;

    // Allocate and store private data
    p = (PrivateData *) malloc(sizeof(PrivateData));
    if (p == NULL)
        return -1;
    if (drvthis->store_private_ptr( drvthis, p ) < 0)
        return -1;

    // initialize private data
    p->fd = -1;
    p->cellheight = 8;
    p->cellwidth = 6;

    (... continue with the rest of your init routine)
```

You retrieve this private data pointer by adding the following code to
the beginning of your functions:

``` 
    PrivateData *p = (PrivateData *) drvthis->private_data;
```

Then you can access your data like:

``` 
    p->framebuf
```

# Functions in Detail

int

(\*init)

Driver \*

drvthis

The init() function. It starts up the LCD, initializes all variables,
allocates private data space and stores the pointer by calling
store\_private\_ptr(); Returns \<0 on error.

void

(\*close)

Driver \*

drvthis

Shut down the connection with the LCD. Called just before unloading the
driver.

int

(\*width)

Driver \*

drvthis

Get the screen width in characters. The result is 1-based.

int

(\*height)

Driver \*

drvthis

Get the screen height in character lines. The result is 1-based.

void

(\*clear)

Driver \*

drvthis

Clear the framebuffer.

void

(\*flush)

Driver \*

drvthis

Flush the framebuffer to the LCD.

void

(\*string)

Driver \*

drvthis

int

x

int

y

const char \*

str

Place string str into position (x,y) in the framebuffer. All coordinates
are 1-based, i.e. (1,1) is top left. The driver should check for
overflows, i.e. that the positional parameters are within the screen's
boundaries and cut off the part of the string that is out of bounds.

void

(\*chr)

Driver \*

drvthis

int

x

int

y

char

c

Place a single character c into position (x,y) in the framebuffer. The
driver should check for overflows, i.e. that the positional parameters
are within the screen's boundaries and ignore the request if the
character is out of bounds.

void

(\*vbar)

Driver \*

drvthis

int

x

int

y

int

len

int

promille

int

options

Draw a vertical bar at position (x,y) that has maximal length len, where
a fraction of (promille / 1000) is filled.

void

(\*hbar)

Driver \*

drvthis

int

x

int

y

int

len

int

promille

int

options

Draw a solid horizontal bar at position (x,y) that has maximal length
len, where a fraction of (promille / 1000) is filled. The non filled
part of the maximum length should not be drawn / touched by this
function.

void

(\*pbar)

Driver \*

drvthis

int

x

int

y

int

len

int

promille

Draw a progress bar covering len characters at position (x,y), where a
fraction of (promille / 1000) is filled. Unlike a hbar, this function
must draw a background for the the non-filled part of the bar. For
example this function may draw an outline in the form of a (non-filled)
rectangle covering the entire length and then fill part of that
rectangle to indicate the current progress.

void

(\*num)

Driver \*

drvthis

int

x

int

num

Display big number num at horizontal position x.

void

(\*heartbeat)

Driver \*

drvthis

int

state

Sets the heartbeat to the indicated state: 0=off, 1=on. Use
HEARTBEAT\_ON to say that we want to display/refresh the heartbeat. The
driver choose how to do it.

int

(\*icon)

Driver \*

drvthis

int

x

int

y

int

icon

Draw named icon icon at position (x,y). If the driver returns -1 the
server core will draw an appropriate replacement character.

void

(\*cursor)

Driver \*

drvthis

int

x

int

y

int

type

Move cursor to position (x,y), setting its type to type.

void

(\*set\_char)

Driver \*

drvthis

char

ch

unsigned char \*

dat

The set\_char function expects a simple block of data with 1 byte for
each pixel-line. (So that is 8 bytes for a 5x8 char)

int

(\*get\_free\_chars)

Driver \*

drvthis

Get total number of custom characters available.

int

(\*cellwidth)

Driver \*

drvthis

Return the width of a character cell in pixels. The result is 1-based.

int

(\*cellheight)

Driver \*

drvthis

Return the height of a character cell in pixels. The result is 1-based.

int

(\*get\_contrast)

Driver \*

drvthis

Get the contrast value from the driver. The return value is an integer
in the range from 0 to 1000. Many displays do not support getting or
setting contrast using software.

int

(\*set\_contrast)

Driver \*

drvthis

int

promille

Sets the contrast to the given value, which is an integer in the range
from 0 to 1000. It is up to the driver to map the logical interval \[0,
1000\] into the interval that the hardware supports. Many displays do
not support software setting of contrast.

int

(\*get\_brightness)

Driver \*

drvthis

int

state

Get the brightness value from the driver for the given backlight state.
The parameter `state` determines which one is returned. The return value
is an integer in the range from 0 to 1000. Many displays do not support
getting or setting brightness using software.

int

(\*set\_brightness)

Driver \*

drvthis

int

state

int

promille

Set the brightness for the given backlight state to the value given.
Value must be an integer in the range from 0 to 1000. It is up to the
driver to map the logical interval \[0, 1000\] into the interval that
the hardware supports. Many displays do not support software setting of
brightness.

void

(\*backlight)

Driver \*

drvthis

int

state

Sets the backlight to the given brightness state. Often hardware can
only support two values for the backlight: on and off. In that case any
value of state \> 0 will switch the backlight on.

void

(\*output)

Driver \*

drvthis

int

state

Sets the output value. Some displays/wirings have a general purpose
output, which can be controlled by calling this function. See the
'output' command in the 'widget language'.

const char \*

(\*get\_key)

Driver \*

drvthis

Checks if a key has been pressed on the device. Returns NULL for "no key
pressed", or a string describing the pressed key. These characters
should match the keypad-layout.

const char \*

(\*get\_info)

Driver \*

drvthis

Returns a string describing the driver and its features.

short

(\*config\_get\_bool)

char \*

sectionname

char \*

keyname

int

skip

short

default\_value

Call to server. Retrieve a bool from the config file. Sectionname should
be the name of the driver (as in the struct). If the key cannot be
found, the default value will be returned. skip should be 0 usually, but
if you want to retrieve multiple identical keys, then increase skip to
get every next value.

long int

(\*config\_get\_int)

char \*

sectionname

char \*

keyname

int

skip

long int

default\_value

Call to server. Retrieve an integer from the config file.

double

(\*config\_get\_float)

char \*

sectionname

char \*

keyname

int

skip

double

default\_value

Call to server. Retrieve a float from the config file.

const char \*

(\*config\_get\_string)

char \*

sectionname

char \*

keyname

int

skip

const char \*

default

Call to server. Retrieve a string from the config file. Fill result with
a pointer to some available space. You can fill it with a default value.
If the key is found, it will be overwritten with the value from the key.
Note that you should always first copy the the returned string. It is in
the address space of the server, and will be freed at the next call.

int

config\_has\_section

const char \*

sectionname

Returns whether a section exists. Does not need to be called prior to a
call to a config\_get\_\* function.

int

config\_has\_key

const char \*

sectionname

const char \*

keyname

Returns the number of times a key exists. Does not need to be called
prior to a call to a config\_get\_\* function.

    First version, Joris Robijn, 20011016
    Corrected and expanded, Peter Marschall 20060411
    Sync'd with lcd.h, Markus Dolze, 20090322
