/*
 * =====================================================================================
 *
 *       Filename:  config.h
 *    Description:  parses INI files
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2018
 *
 * =====================================================================================
 */

#ifndef LA_CONFIG_H
#define LA_CONFIG_H

#include <la/boolean.h>

typedef struct la_config config_t;

/**
 * Constructor of config_t which creates object-instance.
 *
 * @return              initiated config-object; otherwise NULL
 * @errno ENOMEM        if object doesn't fit into the heap
 * @see                 stringmap
 * @see                 stringlist
 */
config_t *config_init();

/**
 * Destructs the object and frees all included objhects.
 *
 * @param self          object-instance
 * @return
 */
void config_free(config_t *self);

/**
 * Sets/adds an entry to the configuration.
 *
 * @param self          object-instance
 * @param section       category
 * @param key           identifier inside the given section
 * @param value         value of the entry
 * @return
 * @errno EINVAL        size of value
 * @see                 getKeyId
 */
void config_set(config_t *self, const char *section, const char *key, const char *value);

/**
 * Returns the value of the identified entry.
 *
 * @param self          instance
 * @param section       section/category
 * @param key           key-identifier for the entry
 * @return              value if the entry has been found; otherwise NULL
 * @see                 getKeyId
 */
char *config_get(config_t *self, const char *section, const char *key);

/**
 * Checks if key (in the given section) exists.
 *
 * @param self          instance
 * @param section       section/category
 * @param key           unique key-identifier
 * @return              true if there is a value; otherwies false
 */
boolean_t config_exists(config_t *self, const char *section, const char *key);

/**
 * Removes the given key of the given section.
 *
 * @param self          instance
 * @param section       section/category
 * @param key           key/value to remove from section
 */
void config_remove(config_t *self, const char *section, const char *key);

/**
 * Loads all valid entries from the given filename into the config-instance.
 *
 * @param self          object
 * @param filename      name of the configuration-file
 * @return
 * @errno EINVAL        filename is empty
 * @errno EINVAL        malformed section
 * @errno EINVAL        a line has no separator between key and value
 * @errno ENOENT        file doesn't exists
 */
void config_load(config_t *self, const char *filename);

/**
 * Saves all valid entries to a file.
 * If the file exists, it will be created; otherwise it will be overwritten.
 *
 * @param self          instance
 * @param filename      filename of the configuration
 * @errno EINVAL        invalid internal format
 * @errno EPERM         permission denied
 * @errno EIO           unable to write file
 */
void config_save(config_t *self, const char *filename);

#endif
