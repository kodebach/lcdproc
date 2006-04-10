/*
 * MatrixOrbital GLK Graphic Display Driver
 *
 * http://www.matrixorbital.com
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <sys/errno.h>

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#include "lcd.h"
#include "shared/str.h"
#include "glk.h"
#include "glkproto.h"
#include "report.h"

#define GLK_DEFAULT_DEVICE	"/dev/lcd"
#define GLK_DEFAULT_SPEED	19200
#define GLK_DEFAULT_CONTRAST	560


//////////////////////////////////////////////////////////////////////////
//////////////////// Matrix Orbital Graphical Driver /////////////////////
//////////////////////////////////////////////////////////////////////////

typedef struct driver_private_data {
  char device[256];
  GLKDisplay *PortFD;
  speed_t speed;

  unsigned char *screen_contents;
  int fontselected;
  int gpo_count;

  unsigned char *framebuf;
  int width;
  int height;
  int cellwidth;
  int cellheight;
  int contrast;

  int clearcount;
  unsigned char CGRAM[8];
} PrivateData;


// Vars for the server core
MODULE_EXPORT char *api_version = API_VERSION;
MODULE_EXPORT int stay_in_foreground = 1;
MODULE_EXPORT int supports_multiple = 0;
MODULE_EXPORT char *symbol_prefix = "glk_";



////////////////////////////////////////////////////////////
// init() should set up any device-specific stuff, and
// point all the function pointers.
MODULE_EXPORT int
glk_init(Driver *drvthis)
{
  PrivateData *p;
  int i;

  /* Allocate and store private data */
  p = (PrivateData *) calloc(1, sizeof(PrivateData));
  if (p == NULL)
    return -1;
  if (drvthis->store_private_ptr(drvthis, p))
    return -1;

  /* initialize private data */
  p->PortFD = NULL;
  p->speed = GLK_DEFAULT_SPEED;
  p->screen_contents = NULL;
  p->fontselected = -1;		// No font selected
  p->gpo_count = 0;
  p->framebuf = NULL;
  p->cellwidth = LCD_DEFAULT_CELLWIDTH;
  p->cellheight = LCD_DEFAULT_CELLHEIGHT;
  p->contrast = GLK_DEFAULT_SPEED;
  p->clearcount = 0;

  /* Read config file */

  /* What device should be used */
  strncpy(p->device, drvthis->config_get_string(drvthis->name, "Device", 0,
					     GLK_DEFAULT_DEVICE), sizeof(p->device));
  p->device[sizeof(p->device)-1] = '\0';
  report(RPT_INFO, "%s: using Device %s", drvthis->name, p->device);

  /* What speed to use */
  p->speed = drvthis->config_get_int(drvthis->name, "Speed", 0, 19200);
  
  if (p->speed == 9600)       p->speed = B9600;
  else if (p->speed == 19200) p->speed = B19200;
  else if (p->speed == 38400) p->speed = B38400;
  else {
    report(RPT_WARNING, "%s: illegal Speed: %d; must be one of 9600, 19200 or 38400; using default %d",
		    drvthis->name, p->speed, 19200);
    p->speed = B19200;
  }

  /* Which p->contrast */
  p->contrast = drvthis->config_get_int(drvthis->name, "Contrast" , 0 , GLK_DEFAULT_CONTRAST);
  if ((p->contrast < 0) || (p->contrast > 1000)) {
    report(RPT_WARNING, "%s: Contrast must be between 0 and 1000. Using default %d",
		    drvthis->name, GLK_DEFAULT_CONTRAST);
    p->contrast = GLK_DEFAULT_CONTRAST;
  }

  /* End of config file parsing */

  /* open device */
  p->PortFD = glkopen(p->device, p->speed);
  if (p->PortFD == NULL) {
    report(RPT_ERR, "%s: unable to open device %s", drvthis->name, p->device);
    return -1;
  }

  // Query the module for a device type
  glkputl(p->PortFD, GLKCommand, 0x37, EOF);
  i = glkget(p->PortFD);
  if (i < 0) {
    report(RPT_ERR, "%s: GLK did not respond to READ MODULE TYPE", drvthis->name);
    return -1;
  }
  else {
    switch (i) {
      case 0x10 :  // GLC12232
	p->width = 20; p->height = 4;
	break;
      case 0x11 :  // GLC12864
	p->width = 20; p->height = 8;
	break;
      case 0x12 :  // GLC128128
	p->width = 20; p->height = 16;
	break;
      case 0x13 :  // GLC24064
	p->width = 40; p->height = 8; p->gpo_count = 1;
	break;
      case 0x14 :  // GLK12864-25
	p->width = 20; p->height = 8;
	break;
      case 0x15 :  // GLK24064-25
	p->width = 40; p->height = 8; p->gpo_count = 1;
	break;
      case 0x21 :  // GLK128128-25
	p->width = 20; p->height = 16;
	break;
      case 0x22 :  // GLK12232-25
	p->width = 20; p->height = 4; p->gpo_count = 2;
	break;
      case 0x23 :  // GLK12232-25SM
	p->width = 20; p->height = 4; p->gpo_count = 2;
	break;
      case 0x24 :  // GLK12232-25SM-Penguin
	p->width = 20; p->height = 4; p->gpo_count = 2;
	break;
      default :
	report(RPT_ERR, "%s: unrecognized module type: 0x%02X", drvthis->name, i);
	return -1;
    }
  }

  p->framebuf = malloc(p->width * p->height);
  p->screen_contents = malloc(p->width * p->height);

  if (p->framebuf == NULL || p->screen_contents == NULL) {
    report(RPT_ERR, "%s: Unable to allocate memory for screen buffers", drvthis->name);
    glk_close(drvthis);
    return -1;
  }

  memset(p->framebuf, ' ', p->width * p->height);

//  glk_clear();
//  glkputl(p->PortFD, GLKCommand, 0x58, EOF);


  // Enable flow control
  glkflow(p->PortFD, 40, 2);

  // Set read character timeout to 0
  glktimeout(p->PortFD, 0);

  // Enable auto-transmit of up/down key events
  // This allows us to generate REPEAT keys distinct from
  //   normal keys using timeouts.  (see glk_get_key)
  glkputl(p->PortFD, GLKCommand, 0x7e, 1, GLKCommand, 0x41, EOF);

  // Set p->contrast
  glk_set_contrast(drvthis, p->contrast);

  report(RPT_DEBUG, "%s: init() done", drvthis->name);

  return 1;
}


/////////////////////////////////////////////////////////////////
// Close the driver
//
MODULE_EXPORT void
glk_close(Driver *drvthis)
{
  PrivateData *p = drvthis->private_data;

  if (p != NULL) {
    if (p->PortFD != NULL)
      glkclose(p->PortFD);

    if (p->framebuf != NULL)
      free(p->framebuf);

    if (p->screen_contents != NULL)
      free(p->screen_contents);

    free(p);
  }
  drvthis->store_private_ptr(drvthis, NULL);
}


/////////////////////////////////////////////////////////////////
// Returns the display width
//
MODULE_EXPORT int
glk_width (Driver *drvthis)
{
  PrivateData *p = drvthis->private_data;

  return p->width;
}


/////////////////////////////////////////////////////////////////
// Returns the display height
//
MODULE_EXPORT int
glk_height (Driver *drvthis)
{
  PrivateData *p = drvthis->private_data;

  return p->height;
}


/////////////////////////////////////////////////////////////////
// Returns the display's cell width
//
MODULE_EXPORT int
glk_cellwidth (Driver *drvthis)
{
  PrivateData *p = drvthis->private_data;

  return p->cellwidth;
}


/////////////////////////////////////////////////////////////////
// Returns the display's cell height
//
MODULE_EXPORT int
glk_cellheight (Driver *drvthis)
{
  PrivateData *p = drvthis->private_data;

  return p->cellheight;
}


/////////////////////////////////////////////////////////////////
// Clears the LCD screen
//
#define CLEARCOUNT  (1000000)

void glk_clear_forced(Driver *drvthis)
{
  PrivateData *p = drvthis->private_data;

  p->clearcount = CLEARCOUNT;
  glkputl(p->PortFD, GLKCommand, 0x58, EOF);
  memset(p->screen_contents, ' ', p->width * p->height);
}


MODULE_EXPORT void
glk_clear(Driver *drvthis)
{
  PrivateData *p = drvthis->private_data;

  memset(p->framebuf, ' ', p->width * p->height);
  if (--p->clearcount < 0)
    glk_clear_forced(drvthis);
}


//////////////////////////////////////////////////////////////////
// Flushes all output to the lcd...
//
MODULE_EXPORT void
glk_flush(Driver *drvthis)
{
  PrivateData *p = drvthis->private_data;

//   puts("glk_flush()");
  unsigned char *pf = p->framebuf;
  unsigned char *qf = p->screen_contents;
  int x, y;
  unsigned char *ps= NULL;

  debug(RPT_DEBUG, "flush()");

  for (y = 0; y < p->height; ++y) {
    int xs = -1;  /* XStart not set */
    
    for (x = 0; x < p->width; ++x) {
      if ((*qf == *pf) && (xs >= 0)) {
        /* Write accumulated string */
        glkputl(p->PortFD, GLKCommand, 0x79, xs * 6 + 1, y * 8, EOF);
        glkputa(p->PortFD, x - xs, ps);
        debug(RPT_DEBUG, "draw_frame: Writing at (%d,%d) for %d", xs, y, x - xs);
        xs = -1;
      } else if ((*qf != *pf) && (xs < 0)) {
        /* Start new string of changes */
        ps = pf;
        xs = x;
      }
      *qf++ = *pf++;  /* Update screen_contents from framebuf */
    }
    if (xs >= 0) {
      /* Write accumulated line */
      glkputl(p->PortFD, GLKCommand, 0x79, xs * 6 + 1, y * 8, EOF);
      glkputa(p->PortFD, p->width - xs, ps);
      debug(RPT_DEBUG, "draw_frame: Writing at (%d,%d) for %d", xs, y, p->width - xs);
    }
  }  /* For y */
}


/////////////////////////////////////////////////////////////////
// Prints a string on the lcd display, at position (x,y).  The
// upper-left is (1,1), and the lower right should be (20,4).
//
MODULE_EXPORT void
glk_string(Driver *drvthis, int x, int y, char string[])
{
  PrivateData *p = drvthis->private_data;
  char *s;

  debug(RPT_DEBUG, "glk_string(%d, %d, \"%s\")", x, y, string);

  if ((y <= 0) || (y > p->height))
     return;

  for (s = string; (*s != '\0') && (x <= p->width); s++, x++) {
    glk_chr(drvthis, x, y, *s);
  }
}


/////////////////////////////////////////////////////////////////
// Prints a character on the lcd display, at position (x,y).  The
// upper-left is (1,1), and the lower right should be (20,4).
//
MODULE_EXPORT void
glk_chr(Driver *drvthis, int x, int y, char c)
{
  PrivateData *p = drvthis->private_data;
  int  myc = (unsigned char) c;

  x--;  // Convert 1-based coords to 0-based...
  y--;

  if (p->fontselected != 2) {
    debug(RPT_DEBUG, "Switching to font 2");
    /* Select font 2 */
    glkputl(p->PortFD, GLKCommand, 0x31, 2, EOF);
    p->fontselected = 2;
    /* Set font metrics */
    glkputl(p->PortFD, GLKCommand, 0x32, 1, 0, 1, 1, 32, EOF);
    /* Clear the screen */
    glk_clear_forced(drvthis);
  }

  if ((myc >= 0) && (myc <= 15)) {
    /* CGRAM */
    debug(RPT_DEBUG, "CGRAM changing %d => %d", myc, p->CGRAM[myc & 7]);
    myc = p->CGRAM[myc & 7];
  } else if ((myc == 255) || (myc == -1)) {
    /* Solid block */
    myc = 133;
  } else if (((myc > 15) && (myc < 32)) || (myc > 143)) {
    debug(RPT_DEBUG, "Attempt to write %d to (%d,%d)", myc, x, y);
    myc = 133;
  }

  if ((x >= 0) && (y >= 0) && (x < p->width) && (y < p->height))
    p->framebuf[(y * p->width) + x] = myc;
}


/////////////////////////////////////////////////////////////////
// Returns current p->contrast
// This is only the locally stored contrast, the contrast value
// cannot be retrieved from the LCD.
// Value 0 to 1000.
//
MODULE_EXPORT int
glk_get_contrast(Driver *drvthis)
{
  PrivateData *p = drvthis->private_data;

  return p->contrast;
}


//////////////////////////////////////////////////////////////////////
// Sets the p->contrast of the display.  Value is 0-255, where 140 is
// what I consider "just right".
//
MODULE_EXPORT void
glk_set_contrast(Driver *drvthis, int promille)
{
  PrivateData *p = drvthis->private_data;

  // Check it
  if ((promille < 0) || (promille > 1000))
    return;

  // Store it
  p->contrast = promille;

  // Do it: map logical [0, 1000] -> physical [0, 255] for the hardware
  debug(RPT_DEBUG, "Contrast: %d", p->contrast);
  glkputl(p->PortFD, GLKCommand, 0x50, (int) ((long) promille * 255 / 1000), EOF);
}

//////////////////////////////////////////////////////////////////////
// Turns the lcd backlight on or off...
//
MODULE_EXPORT void
glk_backlight(Driver *drvthis, int on)
{
  PrivateData *p = drvthis->private_data;

  if (on) {
    debug(RPT_DEBUG, "Backlight ON");
    glkputl(p->PortFD, GLKCommand, 0x42, 0, EOF);
  }
  else {
    debug(RPT_DEBUG, "Backlight OFF");
    glkputl(p->PortFD, GLKCommand, 0x46, EOF);
  }
}

//////////////////////////////////////////////////////////////////////
// Sets general purpose outputs on or off
MODULE_EXPORT void
glk_output(Driver *drvthis, int on)
{
  PrivateData *p = drvthis->private_data;

  if (p->gpo_count < 2) {
    glkputl(p->PortFD, GLKCommand, ((on) ? 'W' : 'V'), EOF);
  }
  else {
    int  i;

    for (i = 1; i <= p->gpo_count; ++i, on >>= 1) {
      glkputl(p->PortFD, GLKCommand, ((on & 1) ? 'W' : 'V'), i, EOF);
    }
  }
}

//////////////////////////////////////////////////////////////////////
// Tells the driver to get ready for vertical bargraphs.
//
MODULE_EXPORT void
glk_init_vbar(Driver *drvthis)
{
  //PrivateData *p = drvthis->private_data;

  debug(RPT_DEBUG, "glk_init_vbar()");
}

//////////////////////////////////////////////////////////////////////
// Tells the driver to get ready for horizontal bargraphs.
//
MODULE_EXPORT void
glk_init_hbar(Driver *drvthis)
{
  //PrivateData *p = drvthis->private_data;

  debug(RPT_DEBUG, "glk_init_hbar()");
}

//////////////////////////////////////////////////////////////////////
// Tells the driver to get ready for big numbers, if possible.
//
MODULE_EXPORT void
glk_init_num(Driver *drvthis)
{
  PrivateData *p = drvthis->private_data;

  debug(RPT_DEBUG, "glk_init_num()");
  if (p->fontselected != 3) {
    /* Select Big Numbers font */
    glkputl(p->PortFD, GLKCommand, 0x31, 3, EOF);
    p->fontselected = 3;
    /* Set font metrics */
    glkputl(p->PortFD, GLKCommand, 0x32, 1, 0, 1, 1, 32, EOF);
    /* Clear the screen */
    glk_clear_forced(drvthis);
  }
}

//////////////////////////////////////////////////////////////////////
// Draws a big (4-row) number.
//
MODULE_EXPORT void
glk_num(Driver *drvthis, int x, int num)
{
  PrivateData *p = drvthis->private_data;

  debug(RPT_DEBUG, "glk_num(%d, %d)", x, num);

  if ((x > 0) && (x <= p->width))
    p->framebuf[x-1] = num + '0';
}

//////////////////////////////////////////////////////////////////////
// Changes the font data of character n.
//
MODULE_EXPORT void
glk_set_char(Driver *drvthis, int n, char *dat)
{
  //PrivateData *p = drvthis->private_data;

  debug(RPT_DEBUG, "glk_set_char(%d)", n);
}

/////////////////////////////////////////////////////////////////
// Draws a vertical bar, from the bottom of the screen up.
//
MODULE_EXPORT void
glk_old_vbar(Driver *drvthis, int x, int len)
{
  PrivateData *p = drvthis->private_data;
  int y = p->height;

  debug(RPT_DEBUG, "glk_old_vbar(%d, %d)", x, len);

  while (len > p->cellheight) {
    glk_chr(drvthis, x, y, 255);
    --y;
    len -= p->cellheight;
  }

  if (y >= 0) {
    int lastc;
    
    switch (len) {
      case 0 :  return; break;  /* Don't output a char */
      case 1 :  lastc = 138; break;  /* One bar */
      case 2 :  lastc = 139; break;
      case 3 :  lastc = 140; break;
      case 4 :  lastc = 141; break;
      case 5 :  lastc = 142; break;
      case 6 :  lastc = 143; break;
      default:  lastc = 133; break;
    }
    glk_chr(drvthis, x, y, lastc);
  }
}

/////////////////////////////////////////////////////////////////
// Draws a horizontal bar to the right.
//
MODULE_EXPORT void
glk_old_hbar(Driver *drvthis, int x, int y, int len)
{
  PrivateData *p = drvthis->private_data;

  debug(RPT_DEBUG, "glk_old_hbar(%d, %d, %d)", x, y, len);
  
  while (len > p->cellwidth) {
    glk_chr(drvthis, x, y, 255);
    ++x;
    len -= p->cellwidth;
  }

  if (x <= p->width) {
    int lastc;

    switch (len) {
      case 0 :  lastc = ' '; break;
      case 1 :  lastc = 134; break;  /* One bar */
      case 2 :  lastc = 135; break;
      case 3 :  lastc = 136; break;
      case 4 :  lastc = 137; break;
      default:  lastc = 133; break;
    }
    glk_chr(drvthis, x, y, lastc);
  }
}


/////////////////////////////////////////////////////////////////
// Sets character 0 to an icon...
//
MODULE_EXPORT void
glk_old_icon(Driver *drvthis, int which, int dest)
{
  /* TODO IMPLEMENTATION OF NEW API */
  /* any volonteers ? */

  PrivateData *p = drvthis->private_data;
  unsigned char old, new;
  unsigned char *pf = p->framebuf;
  unsigned char *qf = p->screen_contents;
  int count;

  debug(RPT_DEBUG, "glk_old_icon(%i, %i)", which, dest);

  if ((dest < 0) || (dest > 7)) {
    /* Illegal custom character */
    return;
  }

  /* which == 0  => empty heart   => 131
   * which == 1  => filled heart  => 132
   * which == 2  => ellipsis      => 128
   */
  switch (which) {
    case 0:  new = 131; break;
    case 1:  new = 132; break;
    case 2:  new = 128; break;
    default:  return;  /* ERROR */
  }

  old = p->CGRAM[(int) dest];
  p->CGRAM[(int) dest] = new;

  /* Replace all old icons with new icon in new frame */
  for (count = p->width * p->height; count > 0; count--) {
    if (*qf == old) {
      debug(RPT_DEBUG, "icon %d to %d at %d", old, new, qf - p->screen_contents);
      *pf = new;
    }
    ++qf; ++pf;
  }
}


//////////////////////////////////////////////////////////////////////
// Tries to read a character from an input device...
//
// Return NULL for "nothing available".
//
MODULE_EXPORT const char *
glk_get_key(Driver *drvthis)
{
  PrivateData *p = drvthis->private_data;
  int c;
  static int keycode = -1;
  static struct timeval lastkey;
  struct timeval now;
  const char *key = NULL;

  debug(RPT_DEBUG, "glk_get_key()");

  c = glkgetc(p->PortFD);

  if ((c >= 'A') && (c <= 'Z')) {
    /* Key down event */
    keycode = c;
    gettimeofday(&lastkey, NULL);
    debug(RPT_DEBUG, "KEY %c at %ld.%06ld", c, lastkey.tv_sec, lastkey.tv_usec);
  } else if ((c >= 'a') && (c <= 'z')) {
    /* Key up event */
    debug(RPT_DEBUG, "KEY %c UP", c);
    keycode = -1;
    c = 0;
  } else {
    /* Assume timeout */
    c = 0;
    if (keycode > 0) {
      int msec_diff;

      /* A key is down */
      gettimeofday(&now, NULL);
      msec_diff  = (now.tv_sec - lastkey.tv_sec) * 1000;
      msec_diff += (now.tv_usec - lastkey.tv_usec) / 1000;
      debug(RPT_DEBUG, "KEY %c down for %d msec", keycode, msec_diff);
      if (msec_diff > 1000) {
        /* Generate repeat event */
        c = keycode | 0x20;  /* Upper case to lower case */
        ++lastkey.tv_sec;  /* HACK HACK. repeat at 1 sec intervals */
        debug(RPT_DEBUG, "KEY %c REPEAT", c);
      }
    }
  }

  /* Remap keys according to what LCDproc expects */
  switch (c) {
    case 'V' : key = "Enter";
	       break;
    case 'P' : key = "Left";
	       break;
    case 'Q' : key = "Right";
	       break;
    case 'L' : key = "Escape";
	       break;
    case 'U' : key = "Up";
	       break;
    case 'K' : key = "Down";
	       break;
    default :  break;

    // What to do with repeated keys? We currently ignore them.
    //case 'v' : c = 'N'; break;
    //case 'p' : c = 'O'; break;
    //case 'q' : c = 'P'; break;
    //case 'l' : c = 'Q'; break;
    //case 'u' : c = 'R'; break;
    //case 'k' : c = 'S'; break;
  }

  debug(RPT_DEBUG, "%s_ get_key() returns %s", drvthis->name, (key != NULL) ? key : "<null>");

  return key;
}
