#ifndef libhook_hook_h_included
#define libhook_hook_h_included

#include <libgends/slist.h>
#include <libgends/foreach.h>

typedef struct hook_callback_s hook_callback_t;
const char * hook_callback_id(hook_callback_t *hook_callback);
const void * hook_callback_func(hook_callback_t *hook_callback);

int hook_register(char *hook_name, const char *callback_id, const void *callback_func);
int hook_unregister(char *hook_name, const char *callback_id);

gds_slist_node_t * hook_callbacks(char *hook_name);

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

#define hook_invoke(hook_name, returns, ...) do { \
	gds_slist_node_t **r = returns; \
	gds_slist_node_t *callbacks = hook_callbacks(hook_name); \
	hook_callback_t *callback; \
	gds_iterator_t *it = gds_slist_iterator_new(callbacks); \
	foreach (callback, it) { \
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

#endif /* ! libhook_hook_h_included */

