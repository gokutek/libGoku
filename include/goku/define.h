#ifndef GOKU_DEFINE_H
#define GOKU_DEFINE_H

#include <functional>
#include <stdint.h>

#define NS_GOKU_BEG namespace goku {
#define NS_GOKU_END	}

#ifdef LIBGOKU_EXPORTS
#define GOKU_API	__declspec(dllexport)
#else
#define GOKU_API	__declspec(dllimport)
#endif

NS_GOKU_BEG

typedef uint64_t peer_t;

// 服务端：收到客户端连接回调
typedef std::function<void(peer_t)> on_connection_cb_t;

// 收到对方数据
typedef std::function<void(peer_t, void*, size_t)> on_read_cb_t;

// 服务端：客户端连接断开
typedef std::function<void(peer_t)> on_close_cb_t;

// 客户端：与服务器连接成功/失败
typedef std::function<void(int)> on_connect_cb_t;

enum class LoopRunMode
{
	DEFAULT = 0,
	ONCE,
	NOWAIT
};

NS_GOKU_END

#endif // GOKU_DEFINE_H
