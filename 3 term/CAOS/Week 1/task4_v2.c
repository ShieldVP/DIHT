#include <inttypes.h>

extern int
check_int(uint32_t u32) {
	char firstBit = -1, lastBit = -1;
	for (char pointer = 0; u32 != 0; ++pointer) {
		if (u32 & 1) {
			if (firstBit < 0) firstBit = pointer;
			lastBit = pointer;
		}
		u32 >>= 1;
	}
	return (lastBit - firstBit) <= 23;
}