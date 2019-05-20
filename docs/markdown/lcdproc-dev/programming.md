# Programming for LCDproc

# Get the source

If you want to start programming for LCDproc you will need the have the
most current source code available. You can get it several ways:

1.  Download GIT version of as a tarball (preferred).

2.  Download the latest version from GIT.

3.  Download the last stable release from Github. (This is not
    recommended as stable release may be months behind the current
    version.)

## Download GIT Version of LCDproc as a Tarball

The source is available as tarball. You can download them from
[](https://github.com/lcdproc/lcdproc/archive/master.tar.gz). For
development we recommended to use the 'master' branch.

To extract the files run

    $ tar xvfz lcdproc-master.tar.gz

## Download The Latest Version of LCDproc from GitHub

Of course you can download the latest stuff from GitHub. For more
information on how to use GitHub see [](https://guides.github.com/).

    $ git clone https://github.com/lcdproc/lcdproc.git

Once you've done that and want to update the downloaded files to the
latest stuff you can use the "pull" command of git (make sure to be in
the lcdproc directory\!):

    $ git pull

Now that once you have downloaded the files you can prepare them for
compiling, but first you should (you don't have to) copy them to another
place on your machine.

# Code style guideline

LCDproc has been developed by many contributors over many years. You may
find different programming styles (naming, indention, etc) in the source
code.

When modifying an existing file, please take a careful look at its style
and program continuing that style instead of mixing it up with another
one even if it does not comply with the guidelines written below.

For newly added files the following guideline describes how source code
should look like.

> **Note**
> 
> All new submitted files will be passed through GNU `indent` to enforce
> the style described below.

## File format and indention

  - *Language:*The programming language used for LCDd (server core),
    drivers and the lcdproc client is Standard `C`, currently `C99`. No
    other programming language will be accepted.

  - *File encoding:*Files shall either encoded as UTF-8 or ISO-8859-1
    and line endings shall be Unix type.

  - *Line length:*Lines of source code should be wrapped at column 100.
    Comment lines should wrap at column 79.

  - *Indention:*Tab indention shall be used (with tab width set to 8
    characters).

  - *License:*LCDproc is released under GNU General Public License
    version 2 (GPL v2) and every file shall have a standard copyright
    notice.

## Naming conventions

  - *Function names:* Function names shall be lowercase. We do not use
    CamelCase (some historical exceptions may exist). Multiple words are
    separated by underscore.

  - *Variable names:* We do not use Hungarian Notation. CamelCase may be
    used, but names shall begin with a lowercase letter.

  - *Constants:* Constants shall be written in uppercase using
    underscore to separate multiple words.

<!-- end list -->

``` 
/* Constants */
#define KEYPAD_AUTOREPEAT_DELAY 500
#define KEYPAD_AUTOREPEAT_FREQ 15

/* Variable names */
MODULE_EXPORT char * api_version = API_VERSION;
MODULE_EXPORT int stay_in_foreground = 0;
MODULE_EXPORT int supports_multiple = 1;

/* Function names */
void HD44780_position(Driver *drvthis, int x, int y);
static void uPause(PrivateData *p, int usecs);
unsigned char HD44780_scankeypad(PrivateData *p);
  
```

## Comments

  - All code comments shall be `C`-style comments (/\* \*/). Comments
    spanning multiple lines shall have a star at the beginning of each
    line.

  - `C++`-style comments (//) may be used to comment out single lines of
    code to disable these lines. Larger blocks of code which shall be
    disabled should be wrapped within `C`-style comments or using
    pre-processor directives (\#if ... \#endif).
    
    `C++`-style comments should not be used in general.

  - We use Doxygen to document our source code. Functions shall be
    documented using Doxygen-style comments (/\*\* \*). See [Doxygen
    Manual](http://www.stack.nl/~dimitri/doxygen/manual.html) for more
    information and how to use it.

  - If you carefully formatted a comment, you may use the special
    comment /\*- \*/ (comment start is “star minus”) to prevent
    automatic reformatting. This usually applies to the standard
    copyright notice.
    
    ``` 
    /*-
     * Copyright (C) 2010 Your Name <your_email_address>
     *
     * This file is released under the GNU General Public License.
     * Refer to the COPYING file distributed with this package.
     */
          
    ```

## Statement style

  - *Function declarations:*
    
    Function declarations have their declaration and opening brace split
    across two lines.
    
    Function names start in column one. The return type is placed on the
    previous line.
    
    There is no space between the function name and '('.
    
    ``` 
    /**
     * This is a Doxygen function description.
     *
     * \param y     The number of years
     * \param str   Pointer to a string containing X
     * \return      0 on success; -1 on error
     */
    int
    this_is_a_function(int y, char *str)
    {
        code
    }
          
    ```

  - *Operators:*
    
    There shall be a space characters before/after an operator or
    assignment, except for increment (“++”) or decrement (“--”)
    operators.
    
    ``` 
    if (p->dispSizes[dispID - 1] == 1 && p->width == 16) {
        if (x >= 8) {
            x -= 8;
            relY = 1;
        }
    }
    
    x--;            /* Convert 1-based coords to 0-based */
    y--;
          
    ```

  - *Function calls:*
    
    There shall be no space between the function call and the opening
    brace '(' of the parameter list. Within the parameter list a space
    shall be after each
    parameter.
    
    ``` 
    lib_vbar_static(drvthis, x, y, len, promille, options, p->cellheight, 0);
          
    ```

  - *Compound statements:*
    
    Opening braces occur on the same line as the statement.
    
    Else statements: Else statements are placed on a line of their own,
    even is there is a previous closing brace.
    
    Opening and closing braces may be omitted on single line compound
    statements. However, if one part of an if-else-statement requires
    braces the other part shall have braces as well.
    
    ``` 
    if (...) {
        code
    }
    else {
        code
    }
          
    ```
    
    ``` 
    if (...)
        print();
    else
        err = 1;
          
    ```
    
    ``` 
    while (...) {
        code
    }
    
    for (a = 0; a < max; a++) {
        code
    }
    
    /* case labels are not indented */
    switch (icon) {
    case ICON_BLOCK_FILLED:
        HD44780_set_char(drvthis, 6, block_filled);
        break;
    case ICON_HEART_FILLED:
        HD44780_set_char(drvthis, 0, heart_filled);
        break;
    case ICON_HEART_OPEN:
        HD44780_set_char(drvthis, 0, heart_open);
        break;
    default:
        return -1;      /* Let the core do other icons */
    }
          
    ```

## Example indent profile

The following example shows an indent profile that checks and corrects
the coding style guideline described above. Copy the following text into
file `.indent.pro` in your home directory.

``` 
-TPrivateData
-TDriver

-l100
-lc79
-i8
-ip8
-di1
-cd33
-ci8
-cli0
-ts8

-ut
-nsob
-sc
-psl
-npcs
-lp
-fc1
-nbc
-nce
-brs
-br
-sbi0
-saw
-sai
-saf
-nprs
-hnl
-fca
-cdw
-nbfda
-nbbo
-ncdb
-nbad
-cs
    
```

# Submitting code

When you have finished modifying the code you may decide to submit it to
the LCDproc project as a pull request. [Read the
documentation](https://help.github.com/articles/about-pull-requests/) to
learn how to make one.
