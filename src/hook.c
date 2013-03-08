#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgends/slist.h>
#include <libgends/hash_map_keyin.h>
#include "hook.h"
#include "log.h"

#define HOOKS_MAP_SIZE 512

typedef struct {
	char *id;
	hook_callback_cb callback;
} hook_callback_t;

hook_callback_t * hook_callback_new(const char *id, hook_callback_cb callback)
{
	hook_callback_t *hook_callback;
	size_t len;

	hook_callback = malloc(sizeof(hook_callback_t));
	if (hook_callback == NULL) {
		hook_log_error("Memory allocation error");
		return NULL;
	}

	len = strlen(id);
	hook_callback->id = malloc(sizeof(char) * (len+1));
	if (hook_callback->id == NULL) {
		hook_log_error("Memory allocation error");
		free(hook_callback);
		return NULL;
	}

	strncpy(hook_callback->id, id, len+1);
	hook_callback->callback = callback;

	return hook_callback;
}

void hook_callback_free(hook_callback_t *hook_callback)
{
	if (hook_callback) {
		free(hook_callback->id);
		free(hook_callback);
	}
}

typedef struct {
	char *name;
	int has_return_value;
	int num_params;
	gds_slist_node_t * callbacks;
} hook_t;

hook_t * hook_new(const char *name, int has_return_value, int num_params)
{
	hook_t *hook;
	size_t len;

	hook = malloc(sizeof(hook_t));
	if (hook == NULL) {
		hook_log_error("Memory allocation error");
		return NULL;
	}

	len = strlen(name);
	hook->name = malloc(sizeof(char) * (len+1));
	if (hook->name == NULL) {
		hook_log_error("Memory allocation error");
		free(hook);
		return NULL;
	}

	strncpy(hook->name, name, len+1);
	hook->has_return_value = has_return_value;
	hook->num_params = num_params;
	hook->callbacks = NULL;

	return hook;
}

void hook_free(hook_t *hook)
{
	if (hook) {
		free(hook->name);
		gds_slist_free(hook->callbacks, (gds_free_cb) hook_callback_free);
		free(hook);
	}
}

static gds_hash_map_keyin_t * hooks = NULL;

unsigned int hooks_hash_cb(const char *name, unsigned int size)
{
	unsigned int hash = 0, i = 0;
	if (name != NULL) {
		while (name[i] != '\0') {
			hash += name[i] * i;
			i++;
		}
	}
	return hash % size;
}

const char * hooks_getkey_cb(hook_t *hook)
{
	if (hook) return hook->name;
	
	return NULL;
}

int hooks_cmpkey_cb(const char *name1, const char *name2)
{
	return strcmp(name1, name2);
}

void hook_init()
{
	if (hooks == NULL) {
		hooks = gds_hash_map_keyin_new(HOOKS_MAP_SIZE,
			(gds_hash_cb) hooks_hash_cb,
			(gds_getkey_cb) hooks_getkey_cb,
			(gds_cmpkey_cb) hooks_cmpkey_cb);
	}
}

int hook_register(char *hook_name, int has_return_value, int num_params)
{
	hook_t *hook = NULL;

	hook_init();

	hook = gds_hash_map_keyin_get(hooks, hook_name);
	if (hook != NULL) {
		hook_log_warning("Hook '%s' already registered", hook_name);
		return -1;
	}

	hook = hook_new(hook_name, has_return_value, num_params);
	gds_hash_map_keyin_set(hooks, hook, NULL);

	return 0;
}

int hook_unregister(char *hook_name)
{
	return gds_hash_map_keyin_unset(hooks, hook_name, (gds_free_cb) hook_free);
}
