#include <inttypes.h>

extern int
check_int(uint32_t u32) {
	unsigned char curLen = 24, u32size = 32;
	uint32_t curNum = 1u << curLen;
	if (u32 <= curNum)
		return 1;
	while (u32 > curNum && curLen != u32size) {
		curNum <<= 1;
		++curLen;
	}
	--curLen;
	curNum = 1u << curLen;
	
	uint32_t step = 1 << (curLen - 23);
	if ((u32 - curNum) % step == 0)
		return 1;
	return 0;
}