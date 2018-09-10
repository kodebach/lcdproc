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

MenuEntry* param_read(KeySet* config, const char* element);
MenuEntry* command_read(KeySet* config, const char* element);
MenuEntry* menu_read(KeySet* config, const char* element);


MenuEntry* param_read(KeySet* config, const char* paramName)
{
	char* dupName = strdup(paramName);
	char* type = strrchr(paramName, '/');
	if(type == dupName) {
		return NULL;
	}
	*type = '\0';
	type = strrchr(paramName, '/');

	MenuEntry* param = calloc(1, sizeof(MenuEntry));
	if(param == NULL) {
		return NULL;
	}

	param->id = id++;
	param->name = strdup(strrchr(paramName, '/') + 1);

	long int typeId = -1;
	for(size_t i = 0; i < 6; i++)
	{
		if(strcasecmp(paramTypes[i], type) == 0) {
			typeId = i;
			break;
		}
	}
	free(dupName);
	
	char* keyName;
	char* keyBaseNameEnd;
	switch(typeId) {
		case 0: // slider
			param->type = MT_ARG_SLIDER;

			keyName = calloc(strlen(paramName) + 10, sizeof(char));
			strcpy(keyName, paramName);
			keyBaseNameEnd = keyName + strlen(keyName);
			
			strncpy(keyBaseNameEnd, "/value", 10);
			param->data.slider.value = econfig_get_long(config, keyName, 0);

			strncpy(keyBaseNameEnd, "/minvalue", 10);			
			param->data.slider.minval = econfig_get_long(config, keyName, 0);

			strncpy(keyBaseNameEnd, "/maxvalue", 10);			
			param->data.slider.maxval = econfig_get_long(config, keyName, 1000);

			char buf[35];
			snprintf(buf, 34, "%d", param->data.slider.minval);
			strncpy(keyBaseNameEnd, "/mintext", 10);
			param->data.slider.mintext = econfig_get_string(config, keyName, buf);

			snprintf(buf, 34, "%d", param->data.slider.maxval);
			strncpy(keyBaseNameEnd, "/maxtext", 10);			
			param->data.slider.maxtext = econfig_get_string(config, keyName, buf);

			strncpy(keyBaseNameEnd, "/stepsize", 10);			
			param->data.slider.stepsize = econfig_get_long(config, keyName, 1);
			free(keyName);
			break;
		case 1: // ring
			param->type = MT_ARG_RING;

			keyName = calloc(strlen(paramName) + 8, sizeof(char));
			strcpy(keyName, paramName);
			keyBaseNameEnd = keyName + strlen(keyName);

			strncpy(keyBaseNameEnd, "/value", 8);
			param->data.ring.value = econfig_get_long(config, keyName, 0);
			
			strncpy(keyBaseNameEnd, "/string", 8);
			size_t numStrings;
			KeySet* stringsArray = econfig_array_start(config, keyName, &numStrings);
			if(stringsArray == NULL) {
				free(keyName);
				param->data.ring.strings = calloc(1, sizeof(char *));
				param->data.ring.strings[0] = NULL;
				break;
			}

			param->data.ring.strings = calloc(numStrings+1, sizeof(char *));
			param->data.ring.strings[numStrings] = NULL;

			size_t i = 0;
			char* elementName = NULL;
			while((elementName = econfig_array_next(stringsArray)) != NULL) 
			{
				param->data.ring.strings[i] = econfig_get_string(config, elementName, NULL);
				i++;
			}
			econfig_array_end(stringsArray, elementName);
			free(keyName);
			break;
		case 2: // numeric
			param->type = MT_ARG_NUMERIC;

			keyName = calloc(strlen(paramName) + 10, sizeof(char));
			strcpy(keyName, paramName);
			keyBaseNameEnd = keyName + strlen(keyName);

			strncpy(keyBaseNameEnd, "/value", 10);
			param->data.numeric.value = econfig_get_long(config, keyName, 0);

			strncpy(keyBaseNameEnd, "/minvalue", 10);			
			param->data.numeric.minval = econfig_get_long(config, keyName, 0);

			strncpy(keyBaseNameEnd, "/maxvalue", 10);			
			param->data.numeric.minval = econfig_get_long(config, keyName, 1000);
			free(keyName);
			break;
		case 3: // alpha
			param->type = MT_ARG_ALPHA;

			keyName = calloc(strlen(paramName) + 14, sizeof(char));
			strcpy(keyName, paramName);
			keyBaseNameEnd = keyName + strlen(keyName);

			strncpy(keyBaseNameEnd, "/value", 14);
			param->data.alpha.value = econfig_get_string(config, keyName, "");
			
			strncpy(keyBaseNameEnd, "/minlength", 14);
			param->data.alpha.minlen = econfig_get_long(config, keyName, 0);

			strncpy(keyBaseNameEnd, "/maxlength", 14);
			param->data.alpha.maxlen = econfig_get_long(config, keyName, 100);

			strncpy(keyBaseNameEnd, "/allowedchars", 14);
			param->data.alpha.allowed = econfig_get_string(config, keyName, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
			free(keyName);
			break;
		case 4: // ip
			param->type = MT_ARG_IP;

			keyName = calloc(strlen(paramName) + 7, sizeof(char));
			strcpy(keyName, paramName);
			keyBaseNameEnd = keyName + strlen(keyName);

			strncpy(keyBaseNameEnd, "/value", 7);
			param->data.ip.value = econfig_get_string(config, keyName, "");
			
			strncpy(keyBaseNameEnd, "/v6", 7);
			param->data.ip.v6 = econfig_get_bool(config, keyName, false);
			free(keyName);
			break;
		case 5: // checkbox
			param->type = MT_ARG_CHECKBOX;

			keyName = calloc(strlen(paramName) + 11, sizeof(char));
			strcpy(keyName, paramName);
			keyBaseNameEnd = keyName + strlen(keyName);

			strncpy(keyBaseNameEnd, "/allowgray", 11);
			param->data.checkbox.allow_gray = econfig_get_bool(config, keyName, false);

			strncpy(keyBaseNameEnd, "/value", 11);
			const char* values[] = {"off", "on", "gray"};
			param->data.checkbox.value = econfig_get_enum(config, keyName, 0, 3, values);

			if(!param->data.checkbox.allow_gray && param->data.checkbox.value == 2) {
				param->data.checkbox.value = 0;
			}

			// get replacement strings for different values
			strncpy(keyBaseNameEnd, "/offtext", 11);
			param->data.checkbox.map[0] = econfig_get_string(config, keyName, NULL);

			strncpy(keyBaseNameEnd, "/ontext", 11);
			param->data.checkbox.map[1] = econfig_get_string(config, keyName, NULL);

			strncpy(keyBaseNameEnd, "/graytext", 11);
			param->data.checkbox.map[2] = econfig_get_string(config, keyName, NULL);
			free(keyName);
			break;
		default:
			report(RPT_DEBUG, "illegal parameter type");
			menu_free(param);
			return NULL;
	}

	return param;
}

MenuEntry* command_read(KeySet* config, const char* commandName)
{
	MenuEntry* command = calloc(1, sizeof(MenuEntry));
	if (command == NULL) {
		return NULL;
	}

	command->type = MT_EXEC;
	command->id = id++;
	command->name = strdup(strrchr(commandName, ',') + 1);

	// slash + longest subkey (param + elektra array key) + \0
	char* keyName = calloc(strlen(commandName) + 1 + (5 + 1 + 20) + 1 , sizeof(char));
	strcpy(keyName, commandName);
	const char* keyBaseNameEnd = keyName + strlen(keyName);
	
	strncpy(keyBaseNameEnd, "/exec", 13);
	char* exec = econfig_get_string(config, keyName, NULL);
	command->data.exec.command = exec;

	strncpy(keyBaseNameEnd, "/displayname", 13);
	command->displayname = econfig_get_string(config, keyName, NULL);
	if (command->displayname == NULL) {
		command->displayname = strdup(command->name);
	}

	strncpy(keyBaseNameEnd, "/feedback", 13);
	command->data.exec.feedback = econfig_get_bool(config, keyName, false);

	command->next = NULL;
	command->children = NULL;
	command->numChildren = 0;

	strncpy(keyBaseNameEnd, "/param", 13);
	KeySet* array = econfig_array_start(config, keyName, NULL);
	free(keyName);

	if(array != NULL) {
		char* paramEntry = NULL;
		while((paramEntry = econfig_array_next(array)) != NULL) {
			char* referencedKey = econfig_get_string(config, paramEntry, NULL);
			MenuEntry* param = param_read(config, referencedKey);
			free(referencedKey);

			if(param == NULL) {
				menu_free(command);
				return NULL;
			}
			
			command->numChildren++;
			if (command->children == NULL) {
				command->children = param;
				command->lastChild = param;
			} else {
				command->lastChild->next = param;
				command->lastChild = command->lastChild->next;
			}
		}
		econfig_array_end(array, paramEntry);
	}

	// add an "Apply_?" action
	MenuEntry **addr = &command->children;
	if ((command->numChildren > 0) && (addr != NULL)) {
		MenuEntry *apply = calloc(1, sizeof(MenuEntry)); // auto-NULL elements

		if (apply == NULL) {
			menu_free(command);
			return NULL;
		}

		apply->type = MT_ACTION;
		apply->id = id++;
		apply->name = malloc(strlen(command->name) + 10);
		if (apply->name == NULL) {
			menu_free(apply);
			menu_free(command);
			return NULL;
		}
		strcpy(apply->name, "Apply_");
		strcat(apply->name, command->name);

		apply->displayname = strdup("Apply!");
		if (apply->displayname == NULL) {
			menu_free(apply);
			menu_free(command);
			return NULL;
		}

		apply->parent = command;
		apply->next = NULL;
		apply->children = NULL;
		apply->numChildren = 0;

		*addr = apply;
	}

	return command;
}

MenuEntry* menu_read(KeySet* config, const char* menuName)
{
	MenuEntry* menu = calloc(1, sizeof(MenuEntry));
	if(menu == NULL) {
		return NULL;
	}

	menu->type = MT_MENU;
	menu->id = id++;
	menu->name = strdup(strrchr(menuName, '/') + 1);

	char* keyName = calloc(strlen(menuName) + 1 + 11 + 1, sizeof(char));
	strcpy(keyName, menuName);
	strcat(keyName, "/displayname");
	
	menu->displayname = econfig_get_string(config, keyName, NULL);
	free(keyName);

	if (menu->displayname == NULL) {
		menu->displayname = strdup(menu->name);
	}

	if(menu_read_entries(config, menu, menuName) != 0) {
		menu_free(menu);
		menu = NULL;
	}

	return menu;
}

int menu_read_entries(KeySet* config, MenuEntry* menu, const char* menuName) {
	// length of name, /, entry, /, elektra array key, \0
	char* entryArrayName = calloc(strlen(menuName) + 1 + 5 + 1 + 20 + 1, sizeof(char));
	strcpy(entryArrayName, menuName);
	strcat(entryArrayName, "/entry");

	KeySet* array = econfig_array_start(config, entryArrayName, NULL);
	free(entryArrayName);

	if(array == NULL) {
		return 0;
	}

	char* entry = NULL;
	while((entry = econfig_array_next(array)) != NULL) {
		char* referencedKey = econfig_get_string(config, entry, NULL);
		free(referencedKey);
		MenuEntry* entry; // command or submenu
		char* lastSlash = strrchr(referencedKey, '/');

		if(strncmp(lastSlash - 8, "/command", 8) == 0) {
			entry = command_read(config, referencedKey);
		} else if(strncmp(lastSlash - 5, "/menu", 5) == 0) {
			entry = menu_read(config, referencedKey);
		} else {
			report(RPT_INFO, "unknown type of menu element %s", referencedKey);
			return -1;
		}

		if(entry == NULL) {
			return -1;
		}

		menu->numChildren++;
		if (menu->children == NULL) {
			menu->children = entry;
			menu->lastChild = entry;
		} else {
			menu->lastChild->next = entry;
			menu->lastChild = menu->lastChild->next;
		}
	}
	econfig_array_end(array, entry);

	return 0;
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

	if(menu_read_entries(config, menu, name) != 0) {
		menu_free(menu);
		menu = NULL;
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
