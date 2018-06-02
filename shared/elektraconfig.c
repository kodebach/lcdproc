#include "shared/elektraconfig.h"

#include "shared/report.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

KeySet* econfig_open(const char* baseKeyName)
{
    KeySet* config = ksNew(0, KS_END);
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

void econfig_close(KeySet* config) {
    ksDel(config);
}

// TODO (kodebach): documentation; name == NULL or returned by malloc/realloc/calloc/econfig_name MUST free
char* econfig_name(char* name, const char* fmt, ...) {
    // TODO (kodebach): error handling
    va_list args, argsCopy;
    va_start(args, fmt);
    va_copy(argsCopy, args);

    size_t len = vsnprintf(NULL, 0, fmt, args);
    name = realloc(name, len + 1);
    va_end(args);

    vsnprintf(name, len, fmt, argsCopy);
    va_end(argsCopy);

    return name;
}

char* econfig_get_string(KeySet* config, const char* keyName, char* default_value)
{
	Key* key = ksLookupByName(config, keyName, 0);

	if (key == NULL) {
		keyDel(key);
		return default_value;
	}

	char* value = strdup(keyString(key));
	keyDel(key);
	
	return value;
}

long int econfig_get_long(KeySet* config, const char* keyName, const long int default_value)
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

bool econfig_get_bool(KeySet* config, const char* keyName, const bool default_value)
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


double econfig_get_double(KeySet* config, const char* keyName, const double default_value)
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

long int econfig_get_enum(KeySet* config, const char* keyName, const int default_value, const long int enum_size, const char** enum_values)
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
