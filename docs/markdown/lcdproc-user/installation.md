# Installation

# Build LCDproc

Now that you have downloaded the LCDproc distribution you can start
building it.

> **Note**
> 
> If you have installed the Debian package with apt-get (or another
> Debian package management tool), you can skip this this chapter.

If you're building this version from Git, you'll need autoconf,
automake, aclocal and autoheader installed.

If you have autoconf and friends, run:

    $ sh autogen.sh

This produces the configure script and supporting files. It has already
been run if you are using the tarball distribution.

Once the above command has run, the rest is pretty standard:

    $ ./configure --help

Read about the options, figure out what to use.

    $ ./configure --prefix=/usr/local --enable-drivers=curses,CFontz

Be sure to replace `/usr/local` with the prefixdir you want (e.g. `/usr`
for RedHat) and `curses,CFontz` with the comma-separated list of drivers
you want to have compiled.

    $ make

Congratulations: You have just compiled your version of LCDproc ;)

# Install LCDproc Directly From The Sources

If you want to install LCDproc more or less permanently you can run:

    $ su
    Password: top secret
    # make install

> **Note**
> 
> `make install` is absolutely OPTIONAL You can also run LCDproc
> directly from the source directory. See [below](#lcdd-commandline) for
> details.

# Generate And Install Packages of LCDproc

As an alternative (which is actually better ;) to installing directly
from the sources you can generate packages using the packaging tool EPM.

First of all you may need to download EPM from <http://www.epmhome.org/>
and install it according to the instructions that are included in its
source distribution.

Debian users (who do not want to download the official lcdproc debs via
apt-get) can of course use Debian's epm package:

    # apt-get install epm

> **Note**
> 
> There are of course other and maybe better ways to generate packages
> for your system. The reason for us to choose EPM was that it provide
> the developers with a tool that makes it possible to write one list
> file for all platforms defining what the resulting package is meant to
> look like. This way we do not have to learn all the package managing
> tools of the different platforms that are supported by LCDproc.

To generate an LCDproc package follow these instructions:

> **Tip**
> 
> It is of certain importance that you have run ./configure with the
> correct pathname settings for your system. Otherwise the resulting
> package will install the files in the wrong directories.

    $ epm -v -f native LCDproc

> **Note**
> 
> Generating an RPM package as a non-root user will fail, RPM wants to
> generate the files from the tree under `/usr/src/RPM`, which you do
> not have write access to as a non-root user. If you want to generate
> the package as a non-root user anyway, you may want to follow these
> instructions.

A workaround for the described problem is creating a file named
`~/.rpmmacros` which contains:

``` 

%_topdir ~/rpm
```

> **Important**
> 
> `~/rpm` must contain the same tree usually found under `/usr/src/RPM`

Unfortunately epm does not read `~/.rpmmacros` and of course returns
warnings. Don't worry\! That's OK ;)

In order to actually install the generated package follow the
instructions in your system's manual.
