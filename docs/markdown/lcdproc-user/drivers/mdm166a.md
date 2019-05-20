# The mdm166a driver

This section talks about using LCDproc with the Futaba / Targa USB
Graphic Vacuum Fluorescent Display (MDM166A; USB VID=0x19c2,
PID=0x6a11).

The MDM166A is a graphical VFD with a 96x16 pixel dot matrix area which
is used for 16x2 characters with a 6x8 pixel font. It features several
icons, volume level and WLAN strength indicator which are all software
controllable using the `output` function.

The mdm166a driver builds on top of `libhid` which in turn uses
`libusb`.

The driver was developed by Christoph Rasim ([](http://www.rasim.net/))
without any available documentation from the vendor, but with a good
protocol description from Thomas Koos ([](http://www.muetze1.de))
created by reverse engineering the protocol.

## Configuration in LCDd.conf

### \[mdm166a\]

  - Clock = `TYPE`  
    Show self-running clock after LCDd shutdown. Possible values for
    TYPE are `no`, `small` and `big`. If not given no clock is shown.

  - Dimming = PARAMETERS.YESNODEF  
    Dim display, no dimming gives full brightness.

  - OffDimming = PARAMETERS.YESNODEF  
    Dim display in case LCDd is inactive.

## Using the icons

The icons, the volume bar and the WLAN indicator supported by the VFD
can be controlled by use of the `output` function.

The `on` parameter is used as a bitmask to control which elements to
display. Setting an icon bit to `1` enables the icon, setting the bit to
`0` disables an icon. Volume and WLAN strength indicator accept an
numeric value in the given bit mask.

| bit(s)     | Icon                       |
| ---------- | -------------------------- |
| 0          | Play                       |
| 1          | Pause                      |
| 2          | Record                     |
| 3          | Message                    |
| 4          | Mail (at-symbol)           |
| 5          | Mute                       |
| 6          | WLAN tower                 |
| 7          | Volume (the word)          |
| 8HELLIP12  | Volume (decimal 0HELLIP28) |
| 13HELLIP14 | WLAN strength (0HELLIP3)   |

mdm166a\_output bitmask

## Special device hints

This display may appear as a HID device in your system which may prevent
`libhid` from being able to open the USB device. In this case you have
to create some OS-specific configuration to prevent the HID driver to
take control of this display.

### Configuring FreeBSD 7.x to exclude this device from uhid

To make the uhid driver ignore this device you have to apply the
following patch to your kernel source and recompile and install your
kernel:

``` 

--- sys/dev/usb/usbdevs.orig    2010-11-24 02:06:30.000000000 +0100
+++ sys/dev/usb/usbdevs 2010-11-24 07:36:38.000000000 +0100
@@ -623,6 +623,7 @@
 vendor AMIT            0x18c5  AMIT
 vendor QCOM            0x18e8  Qcom
 vendor LINKSYS3                0x1915  Linksys
+vendor TARGA           0x19c2  Targa Corporation
 vendor QUALCOMMINC     0x19d2  Qualcomm, Incorporated
 vendor STELERA         0x1a8d  Stelera Wireless
 vendor DRESDENELEKTRONIK 0x1cf1 dresden elektronik
@@ -2343,6 +2344,9 @@
 /* System TALKS, Inc. */
 product SYSTEMTALKS SGCX2UL    0x1920  SGC-X2UL

+/* Targa Corporation */
+product TARGA VFD              0x6a11  Targa USB Graphic VFD
+
 /* Tapwave products */
 product TAPWAVE ZODIAC         0x0100  Zodiac

--- sys/dev/usb/usb_quirks.c.orig       2010-11-24 02:07:03.000000000 +0100
+++ sys/dev/usb/usb_quirks.c    2010-11-24 02:16:30.000000000 +0100
@@ -112,6 +112,8 @@
        ANY, { UQ_HID_IGNORE }},
  { USB_VENDOR_APPLE, USB_PRODUCT_APPLE_IPHONE_3G,
        ANY, { UQ_HID_IGNORE }},
+ { USB_VENDOR_TARGA, USB_PRODUCT_TARGA_VFD,
+       ANY, { UQ_HID_IGNORE }},

  /* Devices which should be ignored by both ukbd and uhid */
  { USB_VENDOR_CYPRESS, USB_PRODUCT_CYPRESS_WISPY1A,
```
