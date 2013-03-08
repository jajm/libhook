#ifndef libhook_log_h_included
#define libhook_log_h_included

#include <stdlib.h>
#include <lll/lll.h>

#define hook_log(level, level_str, ...) do { \
	char *verbosity_str = getenv("LIBHOOK_VERBOSITY"); \
	int verbosity = verbosity_str ? atoi(verbosity_str) : 0; \
	if (level <= verbosity) { \
		lll_fprint(stderr, "[%T]libhook $0: %m at $1:$2 ($3)", \
			"%s", level_str, "%s", __FILE__, "%d", __LINE__, \
			"%s", __func__, NULL, NULL, __VA_ARGS__); \
	} \
} while(0)

#define hook_log_error(...)   hook_log(1, "error", __VA_ARGS__)
#define hook_log_warning(...) hook_log(2, "warning", __VA_ARGS__)
#define hook_log_info(...)    hook_log(3, "info", __VA_ARGS__)
#define hook_log_debug(...)   hook_log(4, "debug", __VA_ARGS__)

#endif /* ! libhook_log_h_include */

