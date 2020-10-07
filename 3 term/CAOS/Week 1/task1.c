uint16_t satsum(uint16_t x, uint16_t y) {
	uint16_t ones =       ~0, 
		 _x   = ones ^ x,
		 _y   = ones ^ y;
	if (_x > x && _y > y) return x + y;
	else if (_x < x && _y < y) return ones;
	else if (_x > x && _y < y) {
		uint16_t mask = ones ^ (ones >> 1);
		if ((y - mask) + x >= mask)
			return ones;
		else return x + y;
	}
	else return satsum(y, x);
}
