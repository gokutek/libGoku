#ifndef GOKU_I_LOOP_H
#define GOKU_I_LOOP_H

#include "define.h"

NS_GOKU_BEG

/*
===============================================================================
1. 对libuv中的简单封装；
2. 所有回调函数，只会在Run中触发；
3. 区分几种LoopRunMode；
===============================================================================
*/
class ILoop
{
public:
	virtual ~ILoop() = default;

	virtual int Run(LoopRunMode mode) = 0;

	virtual int Close() = 0;

	virtual void Diagnose() = 0;
};

NS_GOKU_END

#endif // GOKU_I_LOOP_H
