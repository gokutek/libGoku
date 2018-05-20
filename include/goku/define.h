#ifndef GOKU_DEFINE_H
#define GOKU_DEFINE_H

#include <functional>

#define NS_GOKU_BEG namespace goku {
#define NS_GOKU_END	}

#ifdef LIBGOKU_EXPORTS
#define GOKU_API	__declspec(dllexport)
#else
#define GOKU_API	__declspec(dllimport)
#endif

typedef std::function<void(uint64_t)> on_connection_cb_t;

typedef std::function<void(uint64_t, void*, size_t)> on_read_cb_t;

typedef std::function<void(uint64_t)> on_close_cb_t;

struct uv_loop_s;

enum class LoopRunMode
{
	DEFAULT = 0,
	ONCE,
	NOWAIT
};

#endif // GOKU_DEFINE_H
