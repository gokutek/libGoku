#ifndef IMPL_DEFINE_H
#define IMPL_DEFINE_H

#include "uv.h"

struct WriteRequest
{
	WriteRequest()
	{
		buf.base = NULL;
	}

	~WriteRequest()
	{
		if (buf.base) {
			free(buf.base);
		}
	}

	uv_write_t	req;
	uv_buf_t	buf;
};

#endif // IMPL_DEFINE_H
