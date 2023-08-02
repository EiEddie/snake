#include <dbg/debug.h>


void set_num(int numv[], size_t numc) {
	_num.numv = numv;
	_num.numc = numc;
	_num.index = 0;
}

int get_num() {
	if(_num.index >= _num.numc)
		_num.index = 0;

	return _num.numv[_num.index++];
}
