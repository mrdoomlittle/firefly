double ffly_ceil(double __val) {
	double ret_val;
	__asm__("movq %1, %%xmm0\n\t"
		"call __ffly_ceil\n\t"
		"movq %%xmm0, %0" : "=m"(ret_val) : "m"(__val));
    return ret_val;
}
