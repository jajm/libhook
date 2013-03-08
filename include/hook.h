#ifndef libhook_hook_h_included
#define libhook_hook_h_included

#include <libgends/slist.h>

typedef void * (*hook_callback_cb) (void *, ...);

int hook_register(char *hook_name, int has_return_value, int num_params);
int hook_unregister(char *hook_name);

void hook_register_callback(const char *hook_name, const char *callback_id, hook_callback_cb callback);
void hook_unregister_callback(const char *hook_name, const char *callback_id);

gds_slist_node_t * hook_invoke(const char *hook_name, ...);

#endif /* ! libhook_hook_h_included */

