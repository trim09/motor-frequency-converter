#ifndef DIV16_H_
#define DIV16_H_

#include "../multiplication/mult16x8.h"

#define DivU16_2(intRes, uint16In) \
		MultiUU16Xconst8toH16(intRes, uint16In, 256 / 2)

#define DivU16_4(intRes, uint16In) \
		MultiUU16Xconst8toH16(intRes, uint16In, 256 / 4)

#define DivU16_8(intRes, uint16In) \
		MultiUU16Xconst8toH16(intRes, uint16In, 256 / 8)

#define DivU16_16(intRes, uint16In) \
		MultiUU16Xconst8toH16(intRes, uint16In, 256 / 16)

#define DivU16_32(intRes, uint16In) \
		MultiUU16Xconst8toH16(intRes, uint16In, 256 / 32)

#define DivU16_64(intRes, uint16In) \
		MultiUU16Xconst8toH16(intRes, uint16In, 256 / 64)

#define DivU16_128(intRes, uint16In) \
		MultiUU16Xconst8toH16(intRes, uint16In, 256 / 128)


#define DivS16_2(intRes, int16In) \
		MultiSU16Xconst8toH16(intRes, int16In, 256 / 2)

#define DivS16_4(intRes, int16In) \
		MultiSU16Xconst8toH16(intRes, int16In, 256 / 4)

#define DivS16_8(intRes, int16In) \
		MultiSU16Xconst8toH16(intRes, int16In, 256 / 8)

#define DivS16_16(intRes, int16In) \
		MultiSU16Xconst8toH16(intRes, int16In, 256 / 16)

#define DivS16_32(intRes, int16In) \
		MultiSU16Xconst8toH16(intRes, int16In, 256 / 32)

#define DivS16_64(intRes, int16In) \
		MultiSU16Xconst8toH16(intRes, int16In, 256 / 64)

#define DivS16_128(intRes, int16In) \
		MultiSU16Xconst8toH16(intRes, int16In, 256 / 128)


#endif /* DIV16_H_ */
