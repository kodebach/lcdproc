# Shared files

# Introduction

Here we provide functions that should be used by all parts of the
program.

# report.h : Debugging and reporting

To enable the debug() function on all of the software, just type:

    ./configure --enable-debug and recompile with 'make'.

Configure without enabling debug (that is without --enable-debug).

Edit the source file that you want to debug and put the following line
at the top, before the \#include "report.h" line: `#define DEBUG`.

Then recompile with 'make'.

This way, the global DEBUG macro is off but is locally enabled in
certain parts of the software.

The reporting levels have the following meaning.

  - 0 RPT\_CRIT  
    Critical conditions: the program stops right after this. Only use
    this if the program is actually exited from the current function.

  - 1 RPT\_ERR  
    Error conditions: serious problem, program continues. Use this just
    before you return -1 from a function.

  - 2 RPT\_WARNING  
    Warning conditions: Something that the user should fix, but the
    program can continue without a real problem. Ex: Protocol errors
    from a client.

  - 3 RPT\_NOTICE  
    Major event in the program: (un)loading of driver, client
    (dis)connect.

  - 4 RPT\_INFO  
    Minor event in the program: the activation of a setting, details of
    a loaded driver, a key reservation, a keypress, a screen switch.

  - 5 RPT\_DEBUG  
    Insignificant event: What function has been called, what subpart of
    a function is being executed, what was received and sent over the
    socket, etc.

Levels 4 (maybe) and 5 (certainly) should be reported using the debug
function. The code that this function generates will not be in the
executable when compiled without debugging. This way memory and CPU
cycles are saved.

report.h file defines 3 functions for debugging and reporting:

## Sets reporting level and message destination

int

set\_reporting

char \*

application\_name

int

new\_level

int

new\_dest

Returns -1 on error or 0 on success.

## Report the message to the selected destination if important enough

void

report

const int

level

const char \*

format

...

Returns nothing (void).

The format parameter is the same as the one used by printf.

## Send debugging information if important enough

Consider the debug function to be exactly the same as the report
function. The only difference is that it is only compiled in if DEBUG is
defined.

# LL.h : Linked Lists (Doubly-Linked Lists)

## Creating a list

To create a list, do the following:

    LinkedList *list;
    list = LL_new();
    if(!list) handle_an_error();

The list can hold any type of data. You will need to typecast your
datatype to a "void \*", though. So, to add something to the list, the
following would be a good way to start:

    typedef struct my_data {
      char string[16];
      int number;
    } my_data;
    
    my_data *thingie;
    
    for(something to something else) {
      thingie = malloc(sizeof(my_data));
      LL_AddNode(list, (void *)thingie);  // typecast it to a "void *"
    }

For errors, the general convention is that "0" means success, and a
negative number means failure. Check LL.c to be sure, though.

## Changing data

To change the data, try this:

    thingie = (my_data *)LL_Get(list);  // typecast it back to "my_data"
    thingie->number = another_number;

You don't need to "Put" the data back, but it doesn't hurt anything.

    LL_Put(list, (void *)thingie);

However, if you want to point the node's data somewhere else, you'll
need to get the current data first, keep track of it, then set the data
to a new location:

    my_data * old_thingie, new_thingie;
    
    old_thingie = (my_data *)LL_Get(list);
    LL_Put(list, (void *)new_thingie);
    
    // Now, do something with old_thingie.  (maybe, free it?)

Or, you could just delete the node entirely and then add a new one:

    my_data * thingie;
    
    thingie = (my_data *)LL_DeleteNode(list, NEXT);
    free(thingie);
    
    thingie->number = 666;
    
    LL_InsertNode(list, (void *)thingie);

## Iterations throught the list

To iterate on each list item, try this:

    LL_Rewind(list);
    do {
      my_data = (my_data *)LL_Get(list);
      /* ... do something to it ... */
    } while(LL_Next(list) == 0);

## Using the list as a stack or a queue

You can also treat the list like a stack, or a queue. Just use the
following functions:

    LL_Push()      // Regular stack stuff: add, remove, peek, rotate
    LL_Pop()
    LL_Top()
    LL_Roll()
    
    LL_Shift()     // Other end of the stack (like in perl)
    LL_Unshift()
    LL_Look()
    LL_UnRoll()
    
    LL_Enqueue()   // Standard queue operations
    LL_Dequeue()

There are also other goodies, like sorting and searching.

## Future

Array-like operations will come later, to allow numerical indexing:

    LL_nGet(list, 3);
    LL_nSwap(list, 6, 13);
    LL_nPut(list, -4, data);   // Puts item at 4th place from the end..

More ideas for
    later:

    LL_MoveNode(list, amount);  // Slides a node to another spot in the list
    -- LL_MoveNode(list, -1); // moves a node back one toward the head

That's about it, for now... Be sure to free the list when you're done\!

See LL.c for more detailed descriptions of these functions.

MAKE-DRIVER
