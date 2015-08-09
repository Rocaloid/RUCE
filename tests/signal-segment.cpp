#include "signal-segment.hpp"
#include <cassert>
int main()
{
	SignalSegment a(5);
	SignalSegment b(a);
	SignalSegment c = a;
	SignalSegment d(b,1,2);
	SignalSegment e;
	assert(d[0] == 0);
	d[0] = 1;
	assert(d[0] == 0);
	d[1] = 1;
	assert(d[1] == 1);
	d.data()[1] = 2;
	assert(d[1] == 2);
	d.fill(1);
	assert(d[1] == 1);
	assert(d[2] == 1);
	d *= 2;
	assert(d[1] == 2);
	assert(d[2] == 2);
	SignalSegment e(b,3,4);
	d += e;
	d -= e;
	d *= e;
	d /= e;
	d[1] = 1;
	d[2] = 2;
	d <<= 1;
	assert(d[0] == 1);
	assert(d[1] == 2);
	d >>= 1;
	assert(d[1] == 1);
	assert(d[2] == 2);
	assert(d[0] == 0);
	return 0;
}
