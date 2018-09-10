/** \file elektraconfig.h
 * TODO (kodebach): file header
 */

/* TODO (kodebach): copyright
 *
 */

#ifndef ELEKTRACONFIG_H
#define ELEKTRACONFIG_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdbool.h>

typedef struct _KeySet Config;

// TODO (kodebach): documentation
Config* econfig_open(const char* baseKeyName);
void econfig_close(Config* config);

bool econfig_exists(Config* config, const char* keyName);

char* econfig_get_string(Config* config, const char* keyName, char* default_value);
long int econfig_get_long(Config* config, const char* keyName, const long int default_value);
bool econfig_get_bool(Config* config, const char* keyName, const bool default_value);
double econfig_get_double(Config* config, const char* keyName, const double default_value);
long int econfig_get_enum(Config* config, const char* keyName, const int default_value, const long int enum_size, const char** enum_values);

Config* econfig_array_start(Config* config, const char* arrayKeyName, int* size_ptr);
char* econfig_array_next(Config* array);
void econfig_array_end(Config* array, char* lastElement);

#endif
