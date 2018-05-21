#include "loop.h"
#include "uv.h"

NS_GOKU_BEG

Loop::Loop()
{
	loop_ = new uv_loop_s();
	uv_loop_init(loop_);
}


Loop::~Loop()
{
	delete loop_;
}


 Loop::operator uv_loop_s*()
{
	return loop_;
}


int Loop::Run(LoopRunMode mode)
{
	if (!loop_) { return -1; }
	int const ret = uv_run(loop_, (uv_run_mode)mode);
	return ret;
}


int Loop::Close()
{
	if (!loop_) { return -1; }
	int const ret = uv_loop_close(loop_);
	if (!ret) {
		delete loop_;
		loop_ = NULL;
	}
	return ret;
}


void Loop::Diagnose()
{
	if (loop_) {
		uv_print_all_handles(loop_, stdout);
		uv_print_active_handles(loop_, stdout);
	}
}

NS_GOKU_END
