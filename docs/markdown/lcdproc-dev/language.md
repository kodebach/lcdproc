# The LCDproc client language

# Introduction

The LCDproc clients, for example lcdproc, connect over the network to
LCDd. In their communication they use a protocol, often referred to as
the "widget language". In this chapter the widget language will be
discussed.

# Opening a session

The essence of talking to LCDd is quite simple. First you will need to
connect to the LCDproc port (usually 13666) on the correct IP address
(by default localhost). Once you have established the connection you
should say "hello", to let LCDd know you are a good guy. It will respond
by telling some LCDproc data, like version and screen width and height.
Now your session is open and you can start sending 'real' commands.

LCDd can send a number of strings itself. As a response to your
commands, it will usually send a "success" string, or a string starting
with "huh" in case of any error. See further below for other strings
sent by LCDd.

You can test all these commands by opening a TCP/IP connection manually,
like with:

    telnet localhost 13666

This way, you can check how the various commands work. It's in this case
best to have no other clients. If you do have other clients, you will
receive "listen" and "ignore" messages that will disturb your typing.

# Command reference

In this section all commands and their parameters are listed, along with
the responses you can expect. If you need a space or a special char in a
string, you should quote the string with double quotes. If you need to
use a double quote, escape it with a backslash. Escaping also works for
"\\n\\r\\t", however not all widget types or display drivers will handle
these characters well. The listing is divided into subsections for

1.  [Basic stuff](#language-basic)

2.  [Screens and widgets](#language-screens)

3.  [Menu stuff](#language-menus)

4.  [Miscellaneous](#language-misc)

## Basic stuff

  - `hello`  
    Opens the session with the LCDd server program. This command is
    required before other commands can be issued.
    
    The response will be a string in the format:
    
    `connect parameter...`
    
    The client should read all parameters it needs and store their
    values. The following parameters are in use:
    
      - `LCDproc version`  
        Indicates the version number of LCDd.
    
      - `protocol version`  
        Indicates the widget language version number. This number is
        only changed when the language of a newer version has become
        incompatible with the previous version.
        
        > **Note**
        > 
        > Each part of the version number shall be treated as an
        > independent numeric value. This means that 0.9 is followed by
        > 0.10.
    
      - `lcd`  
        This word introduces the next key / value pairs that describe
        the display's properties.
    
      - `wid int`  
        Tells the client the width of the attached display device in
        characters.
    
      - `hgt int`  
        Tells the client the height of the attached display device in
        characters.
    
      - `cellwid int`  
        How many pixels is a character wide (space between character
        cells not included)
    
      - `cellhgt int`  
        How many pixels is a character high (space between character
        cells not included)

  - `client_set -name name`  
    Sets attributes for the current client. The current client is the
    one from the connection that you send this command on, in other
    words: yourself.
    
    name is the client's name as visible to a user.

## Screens and widgets

  - `screen_add new_screen_id`  
    Adds a screen to be displayed. The screen will be identified by the
    string new\_screen\_id, which is used later when manipulating on the
    screen.

  - `screen_del screen_id`  
    Removes the screen identified by screen\_id from the client's
    screens.

  - ` screen_set
                                            screen_id
                                            attributes...
                                     `  
    Sets attributes for the given screen. The following attributes
    exist:
    
      - `-name name`  
        Sets the screen's name as visible to a user.
    
      - `-wid int` `-hgt int`  
        Sets the size of the screen in characters. If unset, the full
        display size is assumed.
    
      - `-priority pri_class`  
        Sets the screen's priority. The following priority classes
        exist:
        
          - `hidden`  
            The screen will never be visible
        
          - `background`  
            The screen is only visible when no normal info screens
            exists
        
          - `info`  
            normal info screen, default priority
        
          - `foreground`  
            an active client
        
          - `alert`  
            The screen has an important message for the user.
        
          - `input`  
            The client is doing interactive input.
        
          - int  
            a positive integer that maps to priority classes above
            according to the mapping given in the table below.
            
            | range    | priority   |
            | -------- | ---------- |
            | 1 - 64   | foreground |
            | 65 - 192 | info       |
            | 193 - ∞  | background |
            

        LCDd will only show screens with the highest priority at that
        moment. So when there are three `info` screens and one
        `foreground` screen, only the `foreground` screen will be
        visible. Only `background`, `info` and `foreground` screens will
        rotate; higher classes do not rotate because their purpose is
        not suitable for rotation.
    
      - `-heartbeat { on | off | open }`  
        Changes the heartbeat setting for this screen. If set to `open`,
        the default, the client's heartbeat setting will be used.
    
      - `-backlight { on | off | toggle | open | blink | flash }`  
        Changes the screen's backlight setting. If set to the default
        value `open`, the state will be determined by the client's
        setting. `blink` is a moderately striking backlight variation,
        `flash` is *very* striking.
    
      - `-duration value`  
        A screen will be visible for this amount of time every rotation.
        The value is in eights of a second.
    
      - `-timeout value`  
        After the screen has been visible for a total of this amount of
        time, it will be deleted. The value is in eights of a second.
        Currently the client will not be informed of the deletion
        (TODO?).
    
      - `-cursor { on | off | under | block}`  
        Determines the visibility of a cursor. If `on`, a cursor will be
        visible. Depending on your hardware, this will be a hardware or
        software cursor. The specified cursor shape (`block` or `under`)
        might not be available in which case an other cursor shape will
        be used instead. Default is `off`.
    
      - `-cursor_x int` `-cursor_y int`  
        Set the cursor's x and y coordinates respectively. If not given,
        the cursor will be set to the leftmost (`-cursor_x`) resp.
        topmost (`-cursor_y`) position. Coordinates are always 1-based.
        So the default top-left corner is denoted by (1,1).

  - ` widget_add
                                            screen_id
                                            new_widget_id
                                            widgettype
                                            -in frame_id
                                     `  
    Adds a widget to the given screen. The new\_widget\_id sets the
    identifier for this widget. The optional `-in frame_id` places the
    widget into the given frame. The following widget types exist:
    
      - `string`  
        A simple text.
    
      - `title`  
        A title bar on top of the screen.
    
      - `hbar`  
        A horizontal bar.
    
      - `vbar`  
        A vertical bar.
    
      - `pbar`  
        A percentage / progress bar.
    
      - `icon`  
        A predefined icon. For a list of valid names consult
        `server/widget.c.`
    
      - `scroller`  
        A variation of the string type that scrolls the text
        horizontally or vertically.
    
      - `frame`  
        A frame with that can contain widgets itself. In fact a frame
        displays an other screen in it.
    
      - `num`  
        A big number. They have a size of 3x4 characters. The special
        number 10 is a colon, that you can use for a clock. This
        character is 1x4.

  - ` widget_del
                                            screen_id
                                            widget_id
                                     `  
    Deletes the given widget from the screen.

  - ` widget_set
                                            screen_id
                                            widget_id
                                            widgettype_specific_parameters
                                     `  
    Sets parameters for a widget. Because not all widgets are created
    equal, the various widget types require different parameters.
    
      - `string`  
        x
        y
        text
        Displays text at position (x,y).
      - `title`  
        text
        Uses text as the title to display.
      - `hbar`; `vbar`  
        x
        y
        length
        Displays a horizontal (`hbar`) resp. vertical (`vbar`) starting
        at position (x,y) that is length pixels wide resp. high.
      - `pbar`  
        x
        y
        width
        promille
        begin-label
        end-label
        Displays a horizontal (`pbar`) and its optional (`begin-label`)
        and (`end-label`) labels covering width characters starting at
        position (x,y) where a fraction of (promille / 1000) is filled.
      - `icon`  
        x
        y
        iconname
        Displays the icon iconname at position (x,y).
      - `scroller`  
        left
        top
        right
        bottom
        direction
        speed
        text
        Displays a scroller spanning from position (left,top) to
        (right,bottom) scrolling text in horizontal (`h`), vertical
        (`v`) or marquee (`m`) direction at a speed of speed, which is
        the number of movements per rendering stroke (8 times/second).
      - `frame`  
        left
        top
        right
        bottom
        width
        height
        direction
        speed
        Sets up a frame spanning from (left,top) to (right,bottom) that
        is width columns wide and height rows high. It scrolls in either
        horizontal (`h`) or vertical (`v`) direction at a speed of
        speed, which is the number of movements per rendering stroke (8
        times/second).
        > **Note**
        > 
        > In the current implementation frames can only scroll
        > vertically and only string and hbar widgets work inside
        > frames.
      - `num`  
        x
        int
        Displays decimal digit int at the horizontal position x, which
        is a normal character x coordinate on the display. The special
        value 10 for int displays a colon.

## Menu stuff

In this section all commands for creation, modification of menus and for
interaction with them are described. Although keys may be used for other
tasks they are listed here too.

TODO: example for normal (static) menu structure.

Menus may be even be used for wizards (the user is automatically guided
through a number of configuration options) by virtue of the options
-next and -prev. Here a complete example:

``` 
      client_set name Parenttest
      # to be entered on escape from test_menu (but overwritten
      # for test_{checkbox,ring})
      menu_add_item "" ask menu "Leave menus?" -is_hidden true
        menu_add_item "ask" ask_yes action "Yes" -next _quit_
        menu_add_item "ask" ask_no action "No" -next _close_

      menu_add_item "" test menu "Test"
        menu_add_item "test" test_action action "Action"
        menu_add_item "test" test_checkbox checkbox "Checkbox"
        menu_add_item "test" test_ring ring "Ring" -strings "one\ttwo\tthree"
        menu_add_item "test" test_slider slider "Slider" -mintext "" -maxtext "" -value "50"
        menu_add_item "test" test_numeric numeric "Numeric" -value "42"
        menu_add_item "test" test_alpha alpha "Alpha" -value "abc"
        menu_add_item "test" test_ip ip "IP" -v6 false -value "192.168.1.1"
        menu_add_item "test" test_menu menu "Menu"
        menu_add_item "test_menu" test_menu_action action "Submenu's action"

      # no successor for menus. Since test_checkbox and test_ring have their
      # own predecessors defined the "ask" rule will not work for them
      menu_set_item "" test -prev "ask"

      menu_set_item "test" test_action -next "test_checkbox"
      menu_set_item "test" test_checkbox -next "test_ring" -prev "test_action"
      menu_set_item "test" test_ring -next "test_slider" -prev "test_checkbox"
      menu_set_item "test" test_slider -next "test_numeric" -prev "test_ring"
      menu_set_item "test" test_numeric -next "test_alpha" -prev "test_slider"
      menu_set_item "test" test_alpha -next "test_ip" -prev "test_numeric"
      menu_set_item "test" test_ip -next "test_menu" -prev "test_alpha"
      menu_set_item "test" test_menu_action -next "_close_"

      # replace the main menu with the client's menu as created above
      menu_set_main ""
    
```

  - ` client_add_key
                                            
                                                    -exclusively | -shared
                                            
                                            key...
                                     `  
    Tells the server that the current client wants to make use of the
    given key(s). If you reserve the key(s) in shared mode, other
    clients can still reserve these keys too. If you reserve the key(s)
    in exclusive mode no other client can reserve them again. Key(s)
    reserved in shared mode will only be returned when a screen of the
    current client is active. These keys can be used for interaction
    with a visible screen (default). Key(s) reserved in exclusive mode
    will be returned regardless of which screen is active. They can be
    used to trigger a special feature or to make a screen come to
    foreground. Note that you cannot reserve a key in exclusive mode
    when an other client has reserved it in shared mode.

  - ` client_del_key
                                            key...
                                     `  
    Ends the reservation of the given key(s).

  - ` menu_add_item
                                            menu_id
                                            new_item_id
                                            type
                                            text
                                            item_specific_options
                                     `  
    Adds a new menu item to a menu. The main menu of a client, will be
    created automatically as soon as the client adds an item. This main
    menu has an empty id ("") and the name is identical to the name of
    the client. The item specific options are described under
    menu\_set\_item below. Use of text is optional and is a shortcut for
    "-text text" option.
    
    > **Note**
    > 
    > Some menu commands (`menu_goto`) and options (`-prev`, `-next`)
    > assume that menu\_ids are *unique* (at least within a clients menu
    > hierarchy).
    > 
    > If you want to use a text label that starts with a '-' (minus)
    > character, you have to use the "-text text" option.
    
      - `action`  
        This item should trigger an action. It consists of simple text.
    
      - `checkbox`  
        Consists of a text and a status indicator. The status can be on
        (Y), off (N) or gray (o).
    
      - `ring`  
        Consists of a text and a status indicator. The status can be one
        of the strings specified for the item.
    
      - `slider`  
        Is visible as a text. When selected, a screen comes up that
        shows a slider. You can set the slider using the cursor keys.
        When Enter is pressed, the menu returns.
    
      - `numeric`  
        Allows the user to input an integer value. Is visible as a text.
        When selected, a screen comes up that shows the current numeric
        value, that you can edit with the cursor keys and Enter. The
        number is ended by selecting a 'null' input digit. After that
        the menu returns.
    
      - `alpha`  
        Is visible as a text. When selected, a screen comes up that
        shows the current string value, that you can edit with the
        cursor keys and Enter. The string is ended by selecting a 'null'
        input character. After that the menu returns.
    
      - `ip`  
        Allows the user to input an IP number (v4 or v6). When selected,
        a screen comes up that shows an IP number that can be edited -
        digit by digit - via left/right (switch digit) and up/down keys
        (increase/decrease).
    
      - `menu`  
        This is a submenu. It is visible as a text, with an appended
        `>`. When selected, the submenu becomes the active menu.

  - ` menu_del_item
                                            menu_id
                                            item_id
                                     `  
    Removes a menu item item\_id from menu menu\_id. The menu with the
    special id "" (i.e. the empty string) is the client's main menu.

  - ` menu_set_item
                                            menu_id
                                            item_id
                                            item_specific_options
                                     `  
    Sets parameters for the menu item(s). Each item type knows different
    parameters.
    
      - for all item types
        
          - `-text string`  
            The visible text of the item.
        
          - `-is_hidden { true | false}` (false)  
            If the item currently should not appear in a menu.
        
          - `-next successor_id`  
            Sets the menu item to show after hitting the ENTER key when
            this item is active. This works for *all* menu item types
            *except menus* i.e. also for menu item types without an own
            screen e.g., checkbox, ring and action.
            
              - `_close_`  
                Equivalent to `-menu_result close`: Close the menu.
            
              - `_quit_`  
                Equivalent to `-menu_result quit`: Quit the menu system.
            
              - `_none_`  
                Equivalent to `-menu_result none`: Keep the item open.
        
          - `-prev predecessor_id`  
            Sets the menu item to show after hitting the ESCAPE key when
            this Item is active. This works for *all* menu item types
            i.e. also for menu item types without an own screen e.g.,
            checkbox, ring and action.
            
            > **Note**
            > 
            > If you define a predecessor for e.g., a checkbox and its
            > parent menu too, the menu's predecessor is ignored in
            > favor of the checkboxes one.
            
            This option accepts the same special values as the `-next`
            option.
    
      - `action`
        
          - `-menu_result { none | close | quit}` (none)  
            Sets what to do with the menu when this action is selected:
            none: the menu stays as it is; close: the menu closes and
            returns to a higher level; quit: quits the menu completely
            so you can foreground your app.
    
      - `checkbox`
        
          - `-value { off | on | gray }`  
            Set the value of the item.
        
          - `-allow_gray { false | true}` (false)  
            Sets if a grayed checkbox is allowed.
    
      - `ring`
        
          - `-value int` (0)  
            Sets the index in the stringlist that is currently selected.
        
          - `-strings string` (empty)  
            This single string should contain the strings that can be
            selected. They should be tab-separated (\\t).
    
      - `slider`
        
          - `-value int` (0)  
            Sets its current value.
        
          - `-mintext string` (""); `-maxtext string` ("")  
            The texts at the left and right side of the slider.
        
          - `-minvalue int` (0); `-maxvalue int` (100)  
            The minimum and maximum values of the slider.
        
          - `-stepsize int` (1)  
            The stepsize of the slider. If you use 0, you can control
            the movement completely from your client.
    
      - `numeric`
        
          - `-value int` (0)  
            Sets its current value.
        
          - `-minvalue int` (0); `-maxvalue int` (100)  
            The minimum and maximum values that are allowed. If one of
            them is negative, the user will be able to enter negative
            numbers too.
            
            TODO: floats\!
    
      - `alpha`
        
          - `-value string` ("")  
            Sets its current value.
        
          - `-password_char string` ("")  
            If used, instead of the typed characters, this character
            will be visible.
        
          - `-minlength int` (0); `-maxlength int` (10)  
            Sets the minimum and maximum allowed lengths.
        
          - `-allow_caps { false | true }` (true); `-allow_noncaps {
            false | true }` (false); `-allow_numbers { false | true }`
            (false)  
            (Dis)allow these groups of characters.
        
          - `-allowed_extra string` ("")  
            The chars in this string are also allowed.
    
      - `ip`
        
          - `-value string` ("192.168.1.245")  
            Set the value of the item, e.g. "192.168.1.245" (v4) or
            ":::ffff:ffff:ffff:ffff:ffff" (v6).
        
          - `-v6 { false | true }` (false)  
            Changes IP version from default v4.
    
      - `menu`  
        This is a submenu. It is visible as a text, with an appended
        '`>`'. When selected, the submenu becomes the active menu.
        
          - `-parent parentid`  
            (Re)sets the parent of this menu. Parentid has to be of type
            menu. This function does not change any menu (neither the
            old nor the new parent) since this option is normally used
            with hidden menus. Otherwise use menu\_add/del\_item.
            Applying this option is equivalent to second argument of the
            menu\_goto command.

  - ` menu_goto
                                            menu_id
                                            parent_id
                                     `  
    Changes current menu to menu\_id. Depending on the configure option
    `--enable-permissive-menu-goto` the client may switch to any (if
    enabled) or his menus only (if not enabled).
    
      - menu\_id  
        The menu item to go to (any menu type e.g. an action or a menu).
    
      - parent\_id  
        Resets the parent of menu\_id. This optional parameter can be
        used to reuse a menu from different places (for wizards etc.).
        Use it with caution: This may lead to a messy menu structure in
        particular due to the fact that the menus are not changed \!

  - ` menu_set_main
                                            menu_id
                                     `  
    Sets the entry point into the menu system. Use this to make the
    server menu invisible. Note that you may only set the menu to your
    own clients menus unless the configure option
    `--enable-permissive-menu-goto` is used. (See `menuscreens.c` for
    the menu ids of the server menus.)
    
      - menu\_id  
        The new main menu, restricted to the client's own menus. Special
        values:
        
          - "" (i.e. the empty string)  
            The client's main menu.
        
          - `_main_`  
            Resets main to the "real" main menu.

## Miscellaneous

  - ` backlight {
                                            on |
                                            off |
                                            toggle |
                                            blink |
                                            flash }
                                     `  
    Sets the client's backlight state.

  - ` output {
                                            on |
                                            off |
                                            int }
                                     `  
    Sets the general purpose output on some display modules to this
    value. Use `on` to set all outputs to high state, and `off` to set
    all to low state. The meaning of the integer value depends on your
    specific device, usually it is a bit pattern describing the state of
    each output line.

  - `info`  
    This command provides information about the driver.

  - `noop`  
    This command does nothing and is always successful. Can be useful to
    be sent at regular intervals to make sure your connection is still
    alive.

  - ` sleep
                                            int
                                     `  
    Sleep for the given number of seconds. int must be a positive
    integer in the range from 1 to 60.
    
    > **Note**
    > 
    > This command is currently ignored on the server side.

# LCDd messages

LCDd can send messages back to the client. These messages can be
directly related to the last command, or generated for some other
reason. Because messages can be generated at any moment, the client
should read from the connection at regular intervals. A very simple
client could simply ignore all received messages. Not reading the
messages will cause trouble \!

  - `success`  
    This is the response to a command in case everything went OK.

  - `huh? error_description`  
    This is the response to a command in case something has gone wrong.
    The description is not meant to be parsed, it's only meant for the
    programmer of the client. It might be that your command has only
    been partially executed, for example if you try to reserve 3 keys,
    and one fails. Your client might need to undo its actions
    completely.

  - `listen screen_id`; `ignore screen_id`  
    The screen with the screen\_id given is now visible on the display
    (`listen`) or it is not visible anymore on the display (`ignore`).

  - `key key`  
    This message will be sent if there was a keypress that should be
    delivered to the current client.

  - ` menuevent
                                            event_type
                                            id
                                            value
                                     `  
    The user did something with a client supplied menu. The type of
    event can be:
    
      - `select` (action)  
        The item was activated.
    
      - `update` (checkbox, ring, numeric, alpha)  
        The item was modified by the user, so LCDd sends an updated
        value.
    
      - `plus` (slider); `minus` (slider)  
        The slider was moved to left (`minus`) or right (`plus`), so
        LCDd sends an updated value.
    
      - `enter`  
        This item has been entered, which means it is currently active
        on the screen. The client could now for example update the value
        of the item. If it is a menu, it may be needed to update the
        values of the items in it too, because they may be visible too.
    
      - `leave`  
        This item has been left, so it is currently not the (main)
        active item anymore.
    
    Multiple messages may be generated by one action of the user.
