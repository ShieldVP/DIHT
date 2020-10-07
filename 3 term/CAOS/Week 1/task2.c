typedef enum {
    PlusZero      = 0x00,
    MinusZero     = 0x01,
    PlusInf       = 0xF0,
    MinusInf      = 0xF1,
    PlusRegular   = 0x10,
    MinusRegular  = 0x11,
    PlusDenormal  = 0x20,
    MinusDenormal = 0x21,
    SignalingNaN  = 0x30,
    QuietNaN      = 0x31
} float_class_t;

extern float_class_t
classify(double *value_ptr) {
	union {
		double double_number;
		unsigned long long bits;
	} converter;

	converter.double_number = *value_ptr;
	unsigned long long number = converter.bits;

	const unsigned char expLen = 11, mantissaLen = 52;

	unsigned long long fullExp               = (1ull << (expLen + mantissaLen)) - (1ull << mantissaLen);
	unsigned long long fullMantissa          = (1ull << mantissaLen) - 1;
	unsigned long long mantissaFirstBitIsOne = number & (1ull << (mantissaLen - 1));
	unsigned long long isNegative            = number & (1ull << (mantissaLen + expLen));


	if ((number & fullExp) == fullExp) {
		if ((number & fullMantissa) == 0)  // infinities
			return isNegative ? MinusInf : PlusInf;
		// so it is nan
		return mantissaFirstBitIsOne ? QuietNaN : SignalingNaN;
	}
	else if ((number & fullExp) == 0) {
		if ((number & fullMantissa) == 0)  // zeroes
			return isNegative ? MinusZero : PlusZero;
		// so it is denormal number
		return isNegative ? MinusDenormal : PlusDenormal;
	}
	else  // regular number
		return isNegative ? MinusRegular : PlusRegular;
}