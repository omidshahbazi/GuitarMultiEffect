#include <assert.h>

#define CHECK_CALL(Expression) \
	if (!Expression)           \
		assert("Call failed" && Expression);