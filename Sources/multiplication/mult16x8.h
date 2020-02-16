// multiplies signed 16 bit X unsigned 8 bit
// stores lower 16 bits
#define MultiSU16X8toL16(intRes, int16In, int8In) \
	asm volatile ( \
		"mul %A1, %2 \n\t"\
		"movw %A0, r0 \n\t"\
		"mulsu %B1, %2 \n\t"\
		"add %B0, r0 \n\t"\
		"clr r1"\
		: \
		"=&r" (intRes) \
		: \
		"a" (int16In), \
		"a" (int8In) \
	)

// multiplies unsigned 16 bit X unsigned 8 bit
// stores lower 16 bits
#define MultiUU16X8toL16(intRes, int16In, int8In) \
	asm volatile ( \
		"mul	%A1, %2 \n\t"\
		"movw	%A0, r0 \n\t"\
		"mul	%B1, %2 \n\t"\
		"add	%B0, r0 \n\t"\
		"clr	r1"\
		: \
		"=&r" (intRes) \
		: \
		"r" (int16In), \
		"r" (int8In) \
	)

// multiplies signed 16 bit number X unsigned 8 bit constant
// saves lower 16 bit
// 8 cycles
#define MultiSU16XConst8toL16(intRes, int16In, int8In) \
	asm volatile ( \
		"ldi r22, %2 \n\t"\
		"mul %A1, r22 \n\t"\
		"movw %A0, r0 \n\t"\
		"mulsu %B1, r22 \n\t"\
		"add %B0, r0 \n\t"\
		"clr r1 \n\t"\
		: \
		"=&r" (intRes) \
		: \
		"a" (int16In), \
		"M" (int8In) \
		:\
		"r22"\
	)

#define MultiUU16XConst8toL16(intRes, int16In, int8In) \
	asm volatile ( \
		"ldi r22, %2 \n\t"\
		"mul %A1, r22 \n\t"\
		"movw %A0, r0 \n\t"\
		"mul %B1, r22 \n\t"\
		"add %B0, r0 \n\t"\
		"clr r1 \n\t"\
		: \
		"=&r" (intRes) \
		: \
		"r" (int16In), \
		"M" (int8In) \
		:\
		"r22"\
	)

// multiplies unsigned 16 bit number X unsigned 8 bit and stores 24bits to
// 32 bit res
#define MultiUU16X8to24(intRes, int16In, int8In) \
	asm volatile ( \
		"clr 	%D0 \n\t"\
		"mul	%A1, %A2 \n\t"\
		"movw	%A0, r0 \n\t"\
		"mul	%B1, %A2 \n\t"\
		"add	%B0, r1 \n\t"\
		"adc	%C0, r0 \n\t"\
		"clr	r1 \n\t"\
		: \
		"=&r" (intRes) \
		: \
		"r" (int16In), \
		"r" (int8In) \
	)

// multiplies 16 bit number X 8 bit and stores 2 high bytes
#define MultiSU16X8toH16(intRes, int16In, int8In) \
	asm volatile ( \
		"mulsu %B1, %A2 \n\t"\
		"movw %A0, r0 \n\t"\
		"mul %A1, %A2 \n\t"\
		"add %A0, r1 \n\t"\
		"clr r1 \n\t"\
		"adc %B0, r1 \n\t"\
		: \
		"=&r" (intRes) \
		: \
		"a" (int16In), \
		"a" (int8In) \
	)

// multiplies unsigned 16 bit number X unsigned 8 bit and stores 2 high bytes
#define MultiUU16X8toH16(intRes, int16In, int8In) \
	asm volatile ( \
		"mul  %B1, %A2 \n\t"\
		"movw %A0, r0 \n\t"\
		"mul %A1, %A2 \n\t"\
		"add %A0, r1 \n\t"\
		"clr r1 \n\t"\
		"adc %B0, r1 \n\t"\
		: \
		"=&r" (intRes) \
		: \
		"r" (int16In), \
		"r" (int8In) \
	)

// multiplies 16 bit number X 8 bit and stores 2 high bytes
#define MultiSU16Xconst8toH16(intRes, int16In, int8In) \
	asm volatile ( \
		"ldi	r22, %2 \n\t"\
		"mulsu	%B1, r22 \n\t"\
		"movw	%A0, r0 \n\t"\
		"mul	%A1, r22 \n\t"\
		"add	%A0, r1 \n\t"\
		"clr	r1 \n\t"\
		"adc	%B0, r1 \n\t"\
		: \
		"=&r" (intRes) \
		: \
		"a" (int16In), \
		"M" (int8In) \
		: \
		"r22" \
	)

// multiplies unsigned 16 bit number X unsigned 8 bit and stores 2 high bytes
#define MultiUU16Xconst8toH16(intRes, int16In, int8In) \
	asm volatile ( \
		"ldi	r22, %2 \n\t"\
		"mul  %B1, r22 \n\t"\
		"movw %A0, r0 \n\t"\
		"mul %A1, r22 \n\t"\
		"add %A0, r1 \n\t"\
		"clr r1 \n\t"\
		"adc %B0, r1 \n\t"\
		: \
		"=&r" (intRes) \
		: \
		"r" (int16In), \
		"M" (int8In) \
		:\
		"r22" \
	)

// multiplies 16 bit signed number X 8 bit and stores 2 high bytes
// rounds the number based on the MSB of the lowest byte
#define MultiSU16X8toH16Round(intRes, int16In, int8In) \
	asm volatile ( \
		"clr r26 \n\t"\
		"mulsu %B1, %A2 \n\t"\
		"movw %A0, r0 \n\t"\
		"mul %A1, %A2 \n\t"\
		"add %A0, r1 \n\t"\
		"adc %B0, r26 \n\t"\
		"lsl r0 \n\t"\
		"adc %A0, r26 \n\t"\
		"adc %B0, r26 \n\t"\
		"clr r1 \n\t"\
		: \
		"=&r" (intRes) \
		: \
		"a" (int16In), \
		"a" (int8In) \
		:\
		"r26"\
	)
