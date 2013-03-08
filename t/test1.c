#include <stdio.h>
#include "hook.h"

#define test(f) printf("%s returned %d\n", #f, f)

int main()
{
	test(hook_register("hook1", 1, 2));
	test(hook_register("hook1", 1, 2));
	test(hook_register("hook2", 1, 2));
	test(hook_register("hook2", 1, 2));
	test(hook_register("hook2", 1, 2));
	test(hook_unregister("hook2"));
	test(hook_unregister("hook2"));
	test(hook_unregister("hook1"));
	test(hook_unregister("hook1"));
	test(hook_unregister("hook0"));
	test(hook_register("hook1", 1, 2));
	test(hook_register("hook1", 1, 2));
	test(hook_register("hook2", 1, 2));
	test(hook_register("hook2", 1, 2));
	test(hook_register("hook2", 1, 2));

	return 0;
}
