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

#ifndef libhook_hook_h_included
#define libhook_hook_h_included

#include <string.h>
#include <libgends/slist.h>

/**
 * Register a callback function for a hook.
 *
 * Parameters:
 * - hook_name is the hook name.
 * - callback_id is the identifier (name) of callback.
 * - callback_func is a pointer to the callback function.
 */
int hook_register(char *hook_name, const char *callback_id, const void *callback_func);

/**
 * Unregister a callback function for a hook.
 *
 * Parameters:
 * - hook_name is the hook name.
 * - callback_id is the identifier (name) of callback.
 */
int hook_unregister(char *hook_name, const char *callback_id);

/* Helper macros for hook_invoke(). */
#define hook_callback_ptr(name, param1, ...) \
	void * (*name)(__typeof__(param1), ...)
#define hook_invoke_call(callback, func_ptr, returns, ...) do { \
	func_ptr = hook_callback_func(callback); \
	if (returns != NULL) { \
		gds_slist_add_last(returns, func_ptr(__VA_ARGS__)); \
	} else { \
		func_ptr(__VA_ARGS__); \
	} \
} while(0)

/**
 * Invokes a hook.
 *
 * Invokes all callback functions registered to this hook.
 * Parameters:
 * - hook_name is the hook name.
 * - returns must be a (gds_slist_node_t **) pointer. Values returned by
 *   callback functions will be appended to this list. If NULL, return
 *   values will not be used.
 * - All other parameters are passed to callback functions.
 */
#define hook_invoke(hook_name, returns, ...) do { \
	gds_slist_node_t **r = returns; \
	gds_slist_node_t *callbacks = hook_callbacks(hook_name); \
	hook_callback_t *callback; \
	gds_iterator_t *it = gds_slist_iterator_new(callbacks); \
	gds_iterator_reset(it); \
	while (!gds_iterator_step(it)) { \
		callback = gds_iterator_get(it); \
		if (strcmp(#__VA_ARGS__, "") == 0) { \
			void *(*func)(void); \
			hook_invoke_call(callback, func, r); \
		} else { \
			hook_callback_ptr(func, ##__VA_ARGS__, NULL); \
			hook_invoke_call(callback, func, r, ##__VA_ARGS__, NULL); \
		} \
	} \
	gds_iterator_free(it); \
} while(0)

/* These functions should not be used outside of this file. */
typedef struct hook_callback_s hook_callback_t;
const char * hook_callback_id(hook_callback_t *hook_callback);
const void * hook_callback_func(hook_callback_t *hook_callback);
gds_slist_node_t * hook_callbacks(char *hook_name);

#endif /* ! libhook_hook_h_included */

