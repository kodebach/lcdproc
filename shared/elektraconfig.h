/** \file elektraconfig.h
 * TODO (kodebach): file header
 */

/* TODO (kodebach): copyright
 *
 */

#ifndef ELEKTRACONFIG_H
#define ELEKTRACONFIG_H

// TODO (kodebach): why?
//#ifdef HAVE_CONFIG_H
//#include "config.h"
//#endif

#include <elektra/kdb.h>
#include <stdbool.h>

// TODO (kodebach): documentation, rename

KeySet* econfig_open(const char* baseKeyName);
void econfig_close(KeySet* config);

char* econfig_name(char* name, const char* fmt, ...);

char* econfig_get_string(KeySet* config, const char* keyName, char* default_value);
long int econfig_get_long(KeySet* config, const char* keyName, const long int default_value);
bool econfig_get_bool(KeySet* config, const char* keyName, const bool default_value);
double econfig_get_double(KeySet* config, const char* keyName, const double default_value);
long int econfig_get_enum(KeySet* config, const char* keyName, const int default_value, const long int enum_size, const char** enum_values);

#endif
