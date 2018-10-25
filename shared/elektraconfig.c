#include "shared/elektraconfig.h"

#include "shared/report.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <elektra/kdbease.h>
#include <elektra/kdb.h>

Config* econfig_open(const char* baseKeyName)
{
    Config* config = ksNew(0, KS_END);
    Key* parent_key = keyNew(baseKeyName, KEY_END);
	KDB* kdb = kdbOpen(parent_key);
	if (kdbGet(kdb, config, parent_key) < 0) {
		report( RPT_DEBUG, "error with kdbGet: %s", keyString(keyGetMeta(parent_key, "error/description")));
		return NULL;
	}

	if(kdbClose(kdb, parent_key) < 0) {
		report( RPT_DEBUG, "error with kdbClose: %s", keyString(keyGetMeta(parent_key, "error/description")));
		return NULL;
	}
	keyDel(parent_key);

    return config;
}

void econfig_close(Config* config)
{
    ksDel(config);
}

bool econfig_exists(Config* config, const char* keyName)
{
	Key* key = ksLookupByName(config, keyName, 0);

	if (key == NULL) {
		return false;
	}

	const char* value = keyString(key);
	keyDel(key);
	
	return value != NULL;
}

char* econfig_get_string(Config* config, const char* keyName, char* default_value)
{
	Key* key = ksLookupByName(config, keyName, 0);

	if (key == NULL) {
		keyDel(key);
		return default_value == NULL ? NULL : strdup(default_value);
	}

	char* value = strdup(keyString(key));
	keyDel(key);
	
	return value;
}

long int econfig_get_long(Config* config, const char* keyName, const long int default_value)
{
	Key* key = ksLookupByName(config, keyName, 0);
	if (key == NULL) {
		keyDel(key);
		return default_value;
	}

	const char* str = keyString(key);
	keyDel(key);

	char *end;
	long int v = strtol(str, &end, 0);

	if ((end != NULL) && (end != str) && (*end == '\0')) {
		/* Conversion successful */
		return v;
    }

	return default_value;
}

bool econfig_get_bool(Config* config, const char* keyName, const bool default_value)
{
	Key* key = ksLookupByName(config, keyName, 0);
	if (key == NULL) {
		keyDel(key);
		return default_value;
	}

	const char* str = keyString(key);
	keyDel(key);

	// TODO (kodebach): use boolean plugin to normalize values to 0/1
	if ((strcasecmp(str, "0") == 0) || (strcasecmp(str, "false") == 0) ||
	    (strcasecmp(str, "n") == 0) || (strcasecmp(str, "no") == 0) ||
	    (strcasecmp(str, "off") == 0)) {
		return 0;
	}
	if ((strcasecmp(str, "1") == 0) || (strcasecmp(str, "true") == 0) ||
	    (strcasecmp(str, "y") == 0) || (strcasecmp(str, "yes") == 0) ||
	    (strcasecmp(str, "on") == 0)) {
		return 1;
	}
	return default_value;
}


double econfig_get_double(Config* config, const char* keyName, const double default_value)
{
	Key* key = ksLookupByName(config, keyName, 0);
	if (key == NULL) {
		keyDel(key);
		return default_value;
	}

	const char* str = keyString(key);
	keyDel(key);

	char* end;
	long int v = strtod(str, &end);

	if ((end != NULL) && (end != str) && (*end == '\0')) {
		/* Conversion successful */
		return v;
    }

	return default_value;
}

long int econfig_get_enum(Config* config, const char* keyName, const int default_value, const long int enum_size, const char** enum_values)
{
	Key* key = ksLookupByName(config, keyName, 0);
	if (key == NULL) {
		keyDel(key);
		return default_value;
	}

	const char* str = keyString(key);
	keyDel(key);

	for(long int i = 0; i < enum_size; ++i)
	{
		if(strcasecmp(str, enum_values[i]) == 0) {
			return i;
		}
	}
	

	return default_value;
}

Config* econfig_array_start(Config* config, const char* arrayKeyName, int* size_ptr)
{
	Key* key = ksLookupByName(config, arrayKeyName, 0);
	if (key == NULL) {
		return NULL;
	}
	
	Config* array = elektraArrayGet(key, config);
	keyDel(key);
	
	if(array == NULL) {
		return NULL;
	}
	
	if(size_ptr != NULL) {
		*size_ptr = (int) ksGetSize(array);
	}

	ksRewind(array);
	return array;
}

char* econfig_array_next(Config* array) {
	Key* current = ksNext(array);
	if (current == NULL) {
		return NULL;
	}

	char* name = strdup(keyName(current));
	keyDel(current);
	return name;
}

void econfig_array_end(Config* array, char* lastElement) {
	free(lastElement);
	ksDel(array);
}
