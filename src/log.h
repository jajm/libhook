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

