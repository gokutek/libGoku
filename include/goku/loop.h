#ifndef GOKU_LOOP_H
#define GOKU_LOOP_H

#include "define.h"

NS_GOKU_BEG

class GOKU_API Loop
{
public:
	Loop();

	~Loop();

	operator uv_loop_s*();

	int Run(LoopRunMode mode);

	void Diagnose();

	int Close();

private:
	uv_loop_s	*loop_;
};

NS_GOKU_END

#endif // GOKU_LOOP_H
