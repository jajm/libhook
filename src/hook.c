/**
 * Copyright 2013 Julian Maurice
 *
 * This file is part of libhook.
 *
 * libhook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libhook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libhook.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgends/slist.h>
#include <libgends/hash_map.h>
#include "hook.h"
#include "log.h"

#define HOOKS_MAP_SIZE 512

struct hook_callback_s {
	char *id;
	const void *func;
};

hook_callback_t * hook_callback_new(const char *id, const void *func)
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
	hook_callback->func = func;

	return hook_callback;
}

const char * hook_callback_id(hook_callback_t *hook_callback)
{
	return hook_callback ? hook_callback->id : NULL;
}

const void * hook_callback_func(hook_callback_t *hook_callback)
{
	return hook_callback ? hook_callback->func : NULL;
}

void hook_callback_free(hook_callback_t *hook_callback)
{
	if (hook_callback) {
		free(hook_callback->id);
		free(hook_callback);
	}
}

static gds_hash_map_t * hooks = NULL;

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

int hooks_cmpkey_cb(const char *name1, const char *name2)
{
	return strcmp(name1, name2);
}

void hook_check_init()
{
	if (hooks == NULL) {
		hooks = gds_hash_map_new(HOOKS_MAP_SIZE,
			(gds_hash_cb) hooks_hash_cb,
			(gds_cmpkey_cb) hooks_cmpkey_cb);
	}
}

int hook_get_callback_position(gds_slist_node_t *callbacks, const char *callback_id)
{
	gds_iterator_t *it;
	int position = -1;

	hook_callback_t *tmp;
	int i = 0;
	it = gds_slist_iterator_new(callbacks);
	gds_iterator_reset(it);
	while (!gds_iterator_step(it)) {
		tmp = gds_iterator_get(it);
		if (tmp && !strcmp(tmp->id, callback_id)) {
			position = i;
			break;
		}
		i++;
	}
	gds_iterator_free(it);

	return position;
}

int hook_register(char *hook_name, const char *callback_id, const void *callback_func)
{
	hook_callback_t *hook_callback;
	gds_slist_node_t *callbacks;
	int ret = 0;

	hook_check_init();

	callbacks = gds_hash_map_get(hooks, hook_name);
	int pos = hook_get_callback_position(callbacks, callback_id);
	if (pos == -1) {
		hook_callback = hook_callback_new(callback_id, callback_func);
		gds_slist_add_last(&callbacks, hook_callback);
		ret = gds_hash_map_set(hooks, hook_name, callbacks, NULL);
		if (ret < 0) {
			hook_log_error("Failed to register callback '%s' for "
				"hook '%s'", callback_id, hook_name);
		} else {
			ret = 0;
			hook_log_info("Registered callback '%s' for hook '%s'",
				callback_id, hook_name);
		}
	} else {
		hook_log_warning("Callback '%s' is already registered "
			"for hook '%s'", callback_id, hook_name);
		ret = 1;
	}

	return ret;
}

int hook_unregister(char *hook_name, const char *callback_id)
{
	gds_slist_node_t *callbacks;
	int position;
	int ret = 0;

	hook_check_init();

	callbacks = gds_hash_map_get(hooks, hook_name);
	position = hook_get_callback_position(callbacks, callback_id);
	if (position < 0) {
		hook_log_warning("Callback '%s' is not registered for hook '%s'",
			callback_id, hook_name);
		ret = 1;
	} else {
		if (position == 0) {
			gds_slist_del_first(&(callbacks), (gds_free_cb)hook_callback_free);
			gds_hash_map_set(hooks, hook_name, callbacks, NULL);
		} else {
			gds_slist_node_t *slist_node = gds_slist_get_nth_node(
				callbacks, position - 1);
			ret = gds_slist_del_after(slist_node, (gds_free_cb)hook_callback_free);
		}

		if (ret != 0) {
			hook_log_error("Failed to unregister callback '%s' "
				"for hook '%s'", callback_id, hook_name);
		} else {
			hook_log_info("Unregistered callback '%s' for hook "
				"'%s'", callback_id, hook_name);
		}
	}

	return ret;
}

gds_slist_node_t * hook_callbacks(char *hook_name)
{
	hook_check_init();

	return gds_hash_map_get(hooks, hook_name);
}
