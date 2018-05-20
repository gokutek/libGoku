#ifndef GOKU_LOOP_H
#define GOKU_LOOP_H

#include "define.h"

struct uv_loop_s;

NS_GOKU_BEG

/*
===============================================================================
1. 对libuv中的简单封装；
2. 所有回调函数，只会在Run中触发；
3. 区分几种LoopRunMode；
===============================================================================
*/
class GOKU_API Loop
{
public:
	Loop();

	Loop(Loop const&) = delete;

	Loop& operator=(Loop const&) = delete;

	~Loop();

	operator uv_loop_s*();

	int Run(LoopRunMode mode);

	int Close();

	void Diagnose();

private:
	uv_loop_s *loop_;
};

NS_GOKU_END

#endif // GOKU_LOOP_H
