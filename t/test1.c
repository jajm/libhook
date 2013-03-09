#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hook.h"

#define test(f) printf("%s returned %d\n", #f, f)

int callback1(int a, int b, int c, int d)
{
	printf("CALLBACK1, %d, %d, %d, %d\n", a, b, c, d);
	return 41;
}

int callback2(int a)
{
	printf("CALLBACK2, %d\n", a);
	return 42;
}

int main()
{
	test(hook_register("hook1", "callback1", callback1));
	test(hook_register("hook1", "callback1", callback1));
	test(hook_register("hook1", "callback2", callback2));
	test(hook_register("hook1", "callback2", callback2));
	test(hook_unregister("hook1", "callback1"));
	test(hook_unregister("hook1", "callback1"));
	test(hook_unregister("hook1", "callback2"));
	test(hook_unregister("hook1", "callback2"));
	test(hook_register("hook1", "callback1", callback1));
	test(hook_register("hook1", "callback1", callback1));
	test(hook_register("hook1", "callback2", callback2));
	test(hook_register("hook1", "callback2", callback2));

	gds_slist_node_t *returns = NULL;
	hook_invoke("hook1", &returns, 1, 2, 3);
	gds_iterator_t *it = gds_slist_iterator_new(returns);
	gds_iterator_reset(it);
	while (!gds_iterator_step(it)) {
		void *data = gds_iterator_get(it);
		printf("%p\n", data);
	}
	gds_iterator_free(it);
	gds_slist_free(returns, NULL);

	return 0;
}
