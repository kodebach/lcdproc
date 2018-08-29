/** \file clients/lcdexec/menu.c
 * Menu parsing and building functions for the \c lcdexec client
 */

/* This file is part of lcdexec, an LCDproc client.
 *
 * This file is released under the GNU General Public License. Refer to the
 * COPYING file distributed with this package.
 *
 * Copyright (c) 2002, Joris Robijn
 * Copyright (c) 2006-7, Peter Marschall
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "shared/report.h"
#include "shared/elektraconfig.h"
#include "shared/sockets.h"

#include "menu.h"


/* names for boolean and tristate values */
static char *boolValueName[] = { "false", "true" };
static char *triGrayValueName[] = { "off", "on", "gray" };

/* types for command parameters */
static const char* paramTypes[] = {"slider", "ring", "numeric", "alpha", "ip", "checkbox"};

static int id = 0;

int entry_read_callback(KeySet* config, size_t index, Key* element, void* userdata);
int param_read_callback(KeySet* config, size_t index, Key* element, void* userdata);

MenuEntry* param_read(KeySet* config, Key* element);
MenuEntry* entry_read(KeySet* config, Key* element);
MenuEntry* menu_read(KeySet* config, Key* element);


MenuEntry* param_read(KeySet* config, Key* element)
{
	MenuEntry* param = calloc(1, sizeof(MenuEntry));
	if(param == NULL) {
		return NULL;
	}

	param->id = id++;
	param->name = strdup(keyBaseName(element));

	keyAddBaseName(element, "type");
	long int type = econfig_get_enum(config, keyName(element), -1, 6, paramTypes);
	switch(type) {
		case 0: // slider
			param->type = MT_ARG_SLIDER;

			keySetBaseName(element, "value");
			param->data.slider.value = econfig_get_long(config, keyName(element), 0);

			keySetBaseName(element, "minvalue");			
			param->data.slider.minval = econfig_get_long(config, keyName(element), 0);

			keySetBaseName(element, "maxvalue");			
			param->data.slider.maxval = econfig_get_long(config, keyName(element), 1000);

			char buf[35];
			snprintf(buf, 34, "%d", param->data.slider.minval);
			keySetBaseName(element, "mintext");
			param->data.slider.mintext = econfig_get_string(config, keyName(element), buf);

			snprintf(buf, 34, "%d", param->data.slider.maxval);
			keySetBaseName(element, "maxtext");			
			param->data.slider.maxtext = econfig_get_string(config, keyName(element), buf);

			keySetBaseName(element, "stepsize");			
			param->data.slider.stepsize = econfig_get_long(config, keyName(element), 1);
			break;
		case 1: // ring
			param->type = MT_ARG_RING;

			keySetBaseName(element, "value");
			param->data.ring.value = econfig_get_long(config, keyName(element), 0);
			
			keySetBaseName(element, "string");
			size_t numStrings = econfig_array_size(config, keyName(element));
			param->data.ring.strings = calloc(sizeof(char *), numStrings+1);
			param->data.ring.strings[numStrings] = NULL;

			char nameBuf[32];
			for(size_t i = 0; i < numStrings; i++)
			{
				snprintf(nameBuf, 31, "#%li", i);
				keySetBaseName(element, nameBuf);
				param->data.ring.strings[i] = econfig_get_string(config, keyName(element), NULL);
			}
			break;
		case 2: // numeric
			param->type = MT_ARG_NUMERIC;

			keySetBaseName(element, "value");
			param->data.numeric.value = econfig_get_long(config, keyName(element), 0);

			keySetBaseName(element, "minvalue");
			param->data.numeric.minval = econfig_get_long(config, keyName(element), 0);

			keySetBaseName(element, "maxvalue");
			param->data.numeric.minval = econfig_get_long(config, keyName(element), 1000);
			break;
		case 3: // alpha
			param->type = MT_ARG_ALPHA;

			keySetBaseName(element, "value");
			param->data.alpha.value = econfig_get_string(config, keyName(element), "");
			
			keySetBaseName(element, "minlength");
			param->data.alpha.minlen = econfig_get_long(config, keyName(element), 0);

			keySetBaseName(element, "maxlength");
			param->data.alpha.maxlen = econfig_get_long(config, keyName(element), 100);

			keySetBaseName(element, "allowedchars");
			param->data.alpha.allowed = econfig_get_string(config, keyName(element), "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
			break;
		case 4: // ip
			param->type = MT_ARG_IP;

			keySetBaseName(element, "value");
			param->data.ip.value = econfig_get_string(config, keyName(element), "");
			
			keySetBaseName(element, "v6");
			param->data.ip.v6 = econfig_get_bool(config, keyName(element), false);
			break;
		case 5: // checkbox
			param->type = MT_ARG_CHECKBOX;

			keySetBaseName(element, "allowgray");
			param->data.checkbox.allow_gray = econfig_get_bool(config, keyName(element), false);

			keySetBaseName(element, "value");
			const char* values[] = {"off", "on", "gray"};
			param->data.checkbox.value = econfig_get_enum(config, keyName(element), 0, 3, values);

			if(!param->data.checkbox.allow_gray && param->data.checkbox.value == 2) {
				param->data.checkbox.value = 0;
			}

			// get replacement strings for different values
			keySetBaseName(element, "offtext");
			param->data.checkbox.map[0] = econfig_get_string(config, keyName(element), NULL);

			keySetBaseName(element, "ontext");
			param->data.checkbox.map[1] = econfig_get_string(config, keyName(element), NULL);

			keySetBaseName(element, "graytext");
			param->data.checkbox.map[2] = econfig_get_string(config, keyName(element), NULL);
			break;
		default:
			report(RPT_DEBUG, "illegal parameter type");
			menu_free(param);
			return NULL;
	}

	return param;
}

int param_read_callback(KeySet* config, size_t index, Key* element, void* userdata)
{
	const char* paramName = econfig_get_string(config, keyName(element), NULL);
	if (paramName == NULL) {
		return -1;
	}

	Key* paramKey = ksLookupByName(config, paramName, 0);
	MenuEntry* param = param_read(config, paramKey);
	keyDel(paramKey);

	MenuEntry *entry = (MenuEntry*) userdata;	
	
	entry->numChildren++;
	if (entry->children == NULL) {
		entry->children = param;
		entry->lastChild = param;
	} else {
		entry->lastChild->next = param;
		entry->lastChild = entry->lastChild->next;
	}

	return 0;
}

MenuEntry* entry_read(KeySet* config, Key* element)
{
	MenuEntry* entry = calloc(1, sizeof(MenuEntry));
	if (entry == NULL) {
		return NULL;
	}

	entry->type = MT_EXEC;
	entry->id = id++;
	entry->name = strdup(keyBaseName(element));

	keyAddBaseName(element, "exec");
	char* exec = econfig_get_string(config, keyName(element), NULL);
	entry->data.exec.command = exec;

	keySetBaseName(element, "displayname");
	entry->displayname = econfig_get_string(config, keyName(element), NULL);
	if (entry->displayname == NULL) {
		entry->displayname = strdup(entry->name);
	}

	keySetBaseName(element, "feedback");
	entry->data.exec.feedback = econfig_get_bool(config, keyName(element), false);

	entry->next = NULL;
	entry->children = NULL;
	entry->numChildren = 0;

	keySetBaseName(element, "param");
	if (econfig_array_iterate(config, keyName(element), param_read_callback, entry) < 0) {
		menu_free(entry);
		return NULL;
	}

	// add an "Apply_?" action
	MenuEntry **addr = &entry->children;
	if ((entry->numChildren > 0) && (addr != NULL)) {
		MenuEntry *apply = calloc(1, sizeof(MenuEntry)); // auto-NULL elements

		if (apply == NULL) {
			menu_free(entry);
			return NULL;
		}

		apply->type = MT_ACTION;
		apply->id = id++;
		apply->name = malloc(strlen(entry->name) + 10);
		if (apply->name == NULL) {
			menu_free(apply);
			menu_free(entry);
			return NULL;
		}
		strcpy(apply->name, "Apply_");
		strcat(apply->name, entry->name);

		apply->displayname = strdup("Apply!");
		if (apply->displayname == NULL) {
			menu_free(apply);
			menu_free(entry);
			return NULL;
		}

		apply->parent = entry;
		apply->next = NULL;
		apply->children = NULL;
		apply->numChildren = 0;

		*addr = apply;
	}

	return entry;
}

int entry_read_callback(KeySet* config, size_t index, Key* element, void* userdata)
{
	MenuEntry* menu = (MenuEntry*) userdata;

	MenuEntry* entryOrSubMenu; // entry or submenu

	keyAddBaseName(element, "exec");
	if (econfig_exists(config, keyName(element))) {
		// submenu
		entryOrSubMenu = menu_read(config, element);
	} else {
		// command
		entryOrSubMenu = entry_read(config, element);
	}

	if(entryOrSubMenu == NULL) {
		return -1;
	}

	menu->numChildren++;
	if (menu->children == NULL) {
		menu->children = entryOrSubMenu;
		menu->lastChild = entryOrSubMenu;
	} else {
		menu->lastChild->next = entryOrSubMenu;
		menu->lastChild = menu->lastChild->next;
	}

	return 0;
}

MenuEntry* menu_read(KeySet* config, Key* element)
{
	MenuEntry* menu = calloc(1, sizeof(MenuEntry));
	if(menu == NULL) {
		return NULL;
	}

	menu->type = MT_MENU;
	menu->id = id++;
	menu->name = strdup(keyBaseName(element));

	keyAddBaseName(element, "displayname");
	menu->displayname = econfig_get_string(config, keyName(element), NULL);
	if (menu->displayname == NULL) {
		menu->displayname = strdup(menu->name);
	}

	keySetBaseName(element, "menu");
	if(!econfig_array_iterate(config, keyName(element), entry_read_callback, menu)) {
		menu_free(menu);
		return NULL;
	}

	return menu;
}

MenuEntry* main_menu_read(KeySet* config, const char *name)
{
	id = 0; // reset id

	MenuEntry* menu = calloc(1, sizeof(MenuEntry));
	if(menu == NULL) {
		return NULL;
	}

	menu->type = MT_MENU;
	menu->id = id++;
	menu->name = strdup(name);

	if(!econfig_array_iterate(config, name, entry_read_callback, menu)) {
		menu_free(menu);
		return NULL;
	}

	return menu;
}

/** create LCDproc commands for the menu entry hierarchy and send it to the server */
int menu_sock_send(MenuEntry *me, MenuEntry *parent, int sock)
{
	if ((me != NULL) && (sock > 0)) {
		char parent_id[12];

		// set parent_id depending on the parent given
		if ((parent != NULL) && (parent->id != 0))
			sprintf(parent_id, "%d", parent->id);
		else
			strcpy(parent_id, "");

		switch (me->type) {
			MenuEntry *entry;

			case MT_MENU:
				// don't create a separate entry for the main menu
				if ((parent != NULL) && (me->id != 0)) {
					if (sock_printf(sock, "menu_add_item \"%s\" \"%d\" menu \"%s\"\n",
							parent_id, me->id, me->displayname) < 0)
						return -1;
				}

				// recursively do it for the menu's sub-menus
				for (entry = me->children; entry != NULL; entry = entry->next) {
					if (menu_sock_send(entry, me, sock) < 0)
						return -1;
				}
				break;
			case MT_EXEC:
				if (me->children == NULL) {
					if (sock_printf(sock, "menu_add_item \"%s\" \"%d\" action \"%s\"\n",
							parent_id, me->id, me->displayname) < 0)
						return -1;

					if (sock_printf(sock, "menu_set_item \"%s\" \"%d\" -menu_result quit\n",
							parent_id, me->id) < 0)
						return -1;
				}
				else {
					if ((parent != NULL) && (me->id != 0)) {
						if (sock_printf(sock, "menu_add_item \"%s\" \"%d\" menu \"%s\"\n",
								parent_id, me->id, me->displayname) < 0)
							return -1;
					}

					// (recursively) do it for the entry's parameters
					for (entry = me->children; entry != NULL; entry = entry->next) {
						if (menu_sock_send(entry, me, sock) < 0)
							return -1;
					}
				}
				break;
			case MT_ARG_SLIDER:
				if (sock_printf(sock, "menu_add_item \"%s\" \"%d\" slider -text \"%s\""
						      " -value %d -minvalue %d -maxvalue %d"
						      " -mintext \"%s\" -maxtext \"%s\" -stepsize %d\n",
						      parent_id, me->id, me->displayname,
						      me->data.slider.value,
						      me->data.slider.minval,
						      me->data.slider.maxval,
						      me->data.slider.mintext,
						      me->data.slider.maxtext,
						      me->data.slider.stepsize) <0)
					return -1;

				if (me->next == NULL) {
					if (sock_printf(sock, "menu_set_item \"%s\" \"%d\" -next _quit_\n",
							parent_id, me->id) < 0)
						return -1;
				}
				break;
			case MT_ARG_RING:
				{
					int i;
					char *tmp = strdup("");

					// join all strings with TAB as separator
					for (i = 0; me->data.ring.strings[i] != NULL; i++) {
						tmp = realloc(tmp, strlen(tmp) + 1 +
								   strlen(me->data.ring.strings[i]) + 1);
						if (tmp[0] != '\0')
							strcat(tmp, "\t");
						strcat(tmp, me->data.ring.strings[i]);
					}

					if (sock_printf(sock, "menu_add_item \"%s\" \"%d\" ring -text \"%s\""
							      " -value %d -strings \"%s\"\n",
							      parent_id, me->id, me->displayname,
							      me->data.ring.value,
							      tmp) < 0)
						return -1;
				}

				if (me->next == NULL) {
					if (sock_printf(sock, "menu_set_item \"%s\" \"%d\" -next _quit_\n",
							parent_id, me->id) < 0)
						return -1;
				}
				break;
			case MT_ARG_NUMERIC:
				if (sock_printf(sock, "menu_add_item \"%s\" \"%d\" numeric -text \"%s\""
						      " -value %d -minvalue %d -maxvalue %d\n",
						      parent_id, me->id, me->displayname,
						      me->data.numeric.value,
						      me->data.numeric.minval,
						      me->data.numeric.maxval) < 0)
					return -1;

				if (me->next == NULL) {
					if (sock_printf(sock, "menu_set_item \"%s\" \"%d\" -next _quit_\n",
							parent_id, me->id) < 0)
						return -1;
				}
				break;
			case MT_ARG_ALPHA:
				if (sock_printf(sock, "menu_add_item \"%s\" \"%d\" alpha -text \"%s\""
						      " -value \"%s\" -minlength %d -maxlength %d"
						      " -allow_caps false -allow_noncaps false"
						      " -allow_numbers false -allowed_extra \"%s\"\n",
						      parent_id, me->id, me->displayname,
						      me->data.alpha.value,
						      me->data.alpha.minlen,
						      me->data.alpha.maxlen,
						      me->data.alpha.allowed) <0)
					return -1;

				if (me->next == NULL) {
					if (sock_printf(sock, "menu_set_item \"%s\" \"%d\" -next _quit_\n",
							parent_id, me->id) < 0)
						return -1;
				}
				break;
			case MT_ARG_IP:
				if (sock_printf(sock, "menu_add_item \"%s\" \"%d\" ip -text \"%s\""
						      " -value \"%s\" -v6 %s\n",
						      parent_id, me->id, me->displayname,
						      me->data.ip.value,
						      boolValueName[me->data.ip.v6]) < 0)
					return -1;

				if (me->next == NULL) {
					if (sock_printf(sock, "menu_set_item \"%s\" \"%d\" -next _quit_\n",
							parent_id, me->id) < 0)
						return -1;
				}
				break;
			case MT_ARG_CHECKBOX:
				if (sock_printf(sock, "menu_add_item \"%s\" \"%d\" checkbox -text \"%s\""
						      " -value %s -allow_gray %s\n",
						      parent_id, me->id, me->displayname,
						      triGrayValueName[me->data.checkbox.value],
						      boolValueName[me->data.checkbox.allow_gray]) < 0)
					return -1;

				if (me->next == NULL) {
					if (sock_printf(sock, "menu_set_item \"%s\" \"%d\" -next _quit_\n",
							parent_id, me->id) < 0)
						return -1;
				}
				break;
			case MT_ACTION:
				if (sock_printf(sock, "menu_add_item \"%s\" \"%d\" action \"%s\"\n",
						parent_id, me->id, me->displayname) < 0)
					return -1;

				if (sock_printf(sock, "menu_set_item \"%s\" \"%d\" -menu_result quit\n",
						parent_id, me->id) < 0)
					return -1;
				break;
			default:
				return -1;
		}
		return 0;
	}
	return -1;
}


/** find menu entry by its id */
MenuEntry *menu_find_by_id(MenuEntry *me, int id)
{
	if (me != NULL) {
		if (me->id == id)
			return me;

		if ((me->type == MT_MENU) || (me->type == MT_EXEC)) {
			MenuEntry *entry;

			for (entry = me->children; entry != NULL; entry = entry->next) {
				MenuEntry *result = menu_find_by_id(entry, id);

				if (result != NULL)
					return result;
			}
		}
	}
	return NULL;
}


/** return command of a menu entry */
const char *menu_command(MenuEntry *me)
{
	if ((me != NULL) && (me->type == MT_EXEC))
		return me->data.exec.command;

	return NULL;
}


/** free menu entry hierarchy */
void menu_free(MenuEntry *me)
{
	if (me != NULL) {
		MenuEntry *entry;
		int i;

		switch (me->type) {
			case MT_EXEC:
				if (me->data.exec.command != NULL)
					free(me->data.exec.command);
				me->data.exec.command = NULL;
				/* fall through */
			case MT_MENU:
				for (entry = me->children; entry != NULL; ) {
					MenuEntry *old = entry;

					entry = entry->next;
					old->next = NULL;
					menu_free(old);
				}
				me->children = NULL;
				break;
			case MT_ARG_SLIDER:
				if (me->data.slider.mintext != NULL)
					free(me->data.slider.mintext);
				me->data.slider.mintext = NULL;
				if (me->data.slider.maxtext != NULL)
					free(me->data.slider.maxtext);
				me->data.slider.maxtext = NULL;
				break;
			case MT_ARG_RING:
				if (me->data.ring.strings != NULL) {
					int i;

					for (i = 0; me->data.ring.strings[i] != NULL; i++)
						free(me->data.ring.strings[i]);

					free(me->data.ring.strings);
					me->data.ring.strings = NULL;
				}
				break;
			case MT_ARG_ALPHA:
				if (me->data.alpha.value != NULL)
					free(me->data.alpha.value);
				me->data.alpha.value = NULL;
				if (me->data.alpha.allowed != NULL)
					free(me->data.alpha.allowed);
				me->data.alpha.allowed = NULL;
				break;
			case MT_ARG_IP:
				if (me->data.ip.value != NULL)
					free(me->data.ip.value);
				me->data.ip.value = NULL;
				break;
			case MT_ARG_CHECKBOX:
				for (i = 0; i < sizeof(me->data.checkbox.map)/sizeof(me->data.checkbox.map[0]); i++) {
					if (me->data.checkbox.map[i] != NULL) {
						free(me->data.checkbox.map[i]);
						me->data.checkbox.map[i] = NULL;
					}
				}
			default:
				break;
		}

		if (me->name != NULL)
			free(me->name);
		me->name = NULL;

		if (me->displayname != NULL)
			free(me->displayname);
		me->displayname = NULL;

		me->type = MT_UNKNOWN;

		free(me);
	}
}


#if defined(DEBUG)
/** dump menu entry hierarchy to screen */
void menu_dump(MenuEntry *me)
{
	if (me != NULL) {
		/* the quick way out */
		if (me->type & MT_ACTION)
			return;

		report(RPT_DEBUG, "# menu ID: %d", me->id);
		report(RPT_DEBUG, "[%s]", me->name);
		if (me->displayname != NULL)
			report(RPT_DEBUG, "DisplayName=\"%s\"", me->displayname);

		switch (me->type) {
			MenuEntry *entry;

			case MT_MENU:
				// dump menu entry references
				for (entry = me->children; entry != NULL; entry = entry->next)
					report(RPT_DEBUG, "Entry=%s", entry->name);
				report(RPT_DEBUG, "");

				// recursively walk through sub-menus
				for (entry = me->children; entry != NULL; entry = entry->next)
					menu_dump(entry);
				break;
			case MT_EXEC:
				report(RPT_DEBUG, "Exec=\"%s\"", me->data.exec.command);
				report(RPT_DEBUG, "Feedback=%s", boolValueName[me->data.exec.feedback]);

				// dump entry's parameter referencess
				for (entry = me->children; entry != NULL; entry = entry->next)
					report(RPT_DEBUG, "Parameter=%s", entry->name);
				report(RPT_DEBUG, "");

				// dump entry's parameters
				for (entry = me->children; entry != NULL; entry = entry->next)
					menu_dump(entry);
				break;
			case MT_ARG_SLIDER:
				report(RPT_DEBUG, "Type=slider");
				report(RPT_DEBUG, "Value=%d", me->data.slider.value);
				report(RPT_DEBUG, "MinValue=%d", me->data.slider.minval);
				report(RPT_DEBUG, "MaxValue=%d", me->data.slider.maxval);
				report(RPT_DEBUG, "Stepsize=%d", me->data.slider.stepsize);
				report(RPT_DEBUG, "MinText=%s", me->data.slider.mintext);
				report(RPT_DEBUG, "MaxText=%s", me->data.slider.maxtext);
				report(RPT_DEBUG, "");
				break;
			case MT_ARG_RING:
				report(RPT_DEBUG, "Type=ring");
				report(RPT_DEBUG, "Value: %d", me->data.ring.value);
				{
					int i;

					for (i = 0; me->data.ring.strings[i] != NULL; i++)
						report(RPT_DEBUG, "String=\"%s\"", me->data.ring.strings[i]);
				}
				report(RPT_DEBUG, "");
				break;
			case MT_ARG_NUMERIC:
				report(RPT_DEBUG, "Type=numeric");
				report(RPT_DEBUG, "Value=%d", me->data.numeric.value);
				report(RPT_DEBUG, "MinValue=%d", me->data.numeric.minval);
				report(RPT_DEBUG, "MaxValue=%d", me->data.numeric.maxval);
				report(RPT_DEBUG, "");
				break;
			case MT_ARG_ALPHA:
				report(RPT_DEBUG, "Type:=lpha");
				report(RPT_DEBUG, "Value=\"%s\"", me->data.alpha.value);
				report(RPT_DEBUG, "AllowedChars=\"%s\"", me->data.alpha.allowed);
				report(RPT_DEBUG, "");
				break;
			case MT_ARG_IP:
				report(RPT_DEBUG, "Type=ip");
				report(RPT_DEBUG, "Value=\"%s\"", me->data.ip.value);
				report(RPT_DEBUG, "V6=%s", boolValueName[me->data.ip.v6]);
				report(RPT_DEBUG, "");
				break;
			case MT_ARG_CHECKBOX:
				report(RPT_DEBUG, "Type=ip");
				report(RPT_DEBUG, "Value=%s", triGrayValueName[me->data.checkbox.value]);
				report(RPT_DEBUG, "AllowGray=%s", boolValueName[me->data.checkbox.allow_gray]);
				if (me->data.checkbox.map[0] != NULL)
					report(RPT_DEBUG, "OffText=%s", me->data.checkbox.map[0]);
				if (me->data.checkbox.map[1] != NULL)
					report(RPT_DEBUG, "OnText=%s", me->data.checkbox.map[1]);
				if (me->data.checkbox.map[2] != NULL)
					report(RPT_DEBUG, "GrayText=%s", me->data.checkbox.map[2]);
				report(RPT_DEBUG, "");
				break;
			default:
				report(RPT_DEBUG, "ERROR: unknown menu entry type");
				break;
		}

	}
}
#endif

/* EOF */
