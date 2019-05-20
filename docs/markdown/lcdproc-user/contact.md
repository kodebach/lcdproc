# Contact Us

# Errata

Of course LCDproc is not perfect (yet). We do our very best to improve
it, but in some cases we are very much restricted in our efforts.

The main reason for that is the fact that we do *not* have all the
hardware people have written drivers for. Unfortunately some developers
have kind of vanished and don't react to mails from the mailing list any
more.

So, as far as drivers are concerned we rely on *you* as testers. We have
developed elaborate "coding in the dark" skills over the time. E.g. the
CFontz driver has been updated and ported to 0.4.3 without the
developers having the hardware.

# The LCDproc Mailing List

We are a bit lazy about the bug-tracking and whatever stuff on
SourceForge. So, please contact us directly through the mailing list.

For details on how to subscribe to the list see
<http://lcdproc.omnipotent.net/mail.php3>.

We like people to subscribe to the list with their real names. Of course
we cannot and do not want to force you to do so. Anyway, we need to know
your name, if you want to contribute code to LCDproc (legal issues of
copyrights).

# Reporting Bugs

Possibly you will find a bug in the LCDproc distribution. Before
reporting this to the mailing list, please respect the following:

  - Your system should be more or less up to date. This does not mean
    that you have to update from GNU/Linux kernel 2.2.x to 2.4.x or from
    2.4.x to 2.6.x. But we would like to make sure that your problem is
    not related to a known bug in the kernel or maybe your compiler.

  - Especially LCDd might need certain privileges to access a device.
    Make sure LCDd *has* the required rights to do so.

  - When experiencing problems with LCDd, make sure that your hardware
    is OK. E.g. you should make sure that the wiring for your (in this
    case most likely parallel) device is correct.

  - Make sure that you use the correct speed settings for your device.
    Incorrect speed settings (baud rate) are most likely to produce
    garbage scrolling on your display. Refer to the specifications of
    your device. If your device needs a speed setting that is not
    supported by LCDd send us a mail.

  - Make sure that you have modified the configuration file according to
    your needs and that LCDd actually uses the configuration file. I.e.
    you might have to run LCDd with the `-c`
    [option](#lcdd-commandline-options)

Anyway, no question is too stupid to ask ;) Feel free to ask whatever
you want. Unfortunately replying to mails takes time as well (a damn lot
of time). So, if you want LCDproc to develop faster, please try to solve
a problem yourself first.

BUT if you have actually FOUND A BUG we will be quite happy if you let
us know. We NEED YOU as testers and appreciate any feedback.
