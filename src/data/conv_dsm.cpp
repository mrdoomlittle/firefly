# include "conv_dsm.h"
float mdl::firefly::data::conv_dsm(float __val, u8_t __conv_typ) {
	if (__conv_typ == NONE) return 0.0f;
	switch(__conv_typ) {
		case BYTE_TO_BIT:
			return __val * 8.0f;
		break;
		case BIT_TO_BYTE:
			return __val / 8.0f;
		break;
		case BIT_TO_KILOBIT:
			return __val / 1000.0f;
		break;
		case KILOBIT_TO_BIT:
			return __val * 1000.0f;
		break;
		case BIT_TO_KILOBYTE:
			return conv_dsm(conv_dsm(__val, BIT_TO_BYTE), BYTE_TO_KILOBYTE);
		break;
		case KILOBYTE_TO_BIT:
			return conv_dsm(conv_dsm(__val, KILOBYTE_TO_BYTE), BYTE_TO_BIT);
		break;
		case BYTE_TO_KILOBYTE:
			return __val / 1000.0f;
		break;
		case KILOBYTE_TO_BYTE:
			return __val * 1000.0f;
		break;
	}
	return 0;
}
