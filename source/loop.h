#ifndef LOOP_H
#define LOOP_H

#include "goku/i_loop.h"

struct uv_loop_s;

NS_GOKU_BEG

/*
===============================================================================
1. 对libuv中的简单封装；
2. 所有回调函数，只会在Run中触发；
3. 区分几种LoopRunMode；
===============================================================================
*/
class Loop : public ILoop
{
public:
	Loop();

	Loop(Loop const&) = delete;

	Loop& operator=(Loop const&) = delete;

	~Loop();

	operator uv_loop_s*();

	int Run(LoopRunMode mode) override;

	int Close() override;

	void Diagnose() override;

private:
	uv_loop_s *loop_;
};

NS_GOKU_END

#endif // LOOP_H
