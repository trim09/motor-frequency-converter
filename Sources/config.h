#ifndef __COMMON_H
#define __COMMON_H

/* needed for TAB_SIN_SIZE */
#include "space_vector/table_sin.h"

#define BOOTLOADER_START 0x1800

/* UART's baud rate */
#define BAUD                   57600UL /* or 19200UL, 38400UL, ...*/
#define SPEED_MAX              512     /* The largest possible speed value. */
#define COMMAND_MAX            512     /* The largest possible command value. */

#define CURRENT_TRESHOLD       120 /* we don't care about current when is lower than this value */
#define CURRENT_MAX            180 /* The current must be kept under this value */

#define VOLTAGE_TRESHOLD_LO    600
#define VOLTAGE_TRESHOLD_HI    650
#define VOLTAGE_MAX            700 /* The coltage must be kept under this value */

/* TIMER0 setup */
/* one interruption every OCR0A_VAL * TIMER0_DIVIDER / F_CPU second.
 * 64 * 64 / 16Mhz = 0.256ms */
/* OCR0A_VAL = x - 1; There is "- 1" because the timer's comparator clears
 * timer courter at/instead of value  "x + 1"
 * When x == 4, the timer's counter will contain the following values:
 * 0 -> 1 -> 2 -> 3 -> 4 -> 0 -> 1 -> ...
 * therefore when we need only 4 tick per period, we must set OCR0A to 3. :-/ */
#define OCR0A_VAL              63U /* Comparator value when interrupt will be generated */
#define TIMER0_DIVIDER         64U /* Clock divider of timer0 */

/* Maximal frequency that can be achieved on stator. */
#define F_MAX                  125U /* 125Hz */

#define DEADTIME               128U      /* 64 / PSC freq = 1.0 µs */
#define	MAX_PWM		           2048U // 64MHz (PLL frequency) / 2048 / 2 = 15 kHz (PWM frequency)
#define AMPLITUDE_MAX          MAX_PWM

/* Maximal possible difference between two positions of stator that can be
 * achieved between two recalculations.
 *
 * ANGLE_INCREMENT_MAX = (F_MAX * recalculation delay) * (360 / sin table unit)
 * ANGLE_INCREMENT_MAX = [second ^ -1] * [second] * ([degree] / [degree / sin table unit])
 * ANGLE_INCREMENT_MAX = [sin table unit]
 * recalculation delay = 2 * (MAX_PWM + DEADTIME) / 64Mhz   [second]
 * sin table unit = (360 / (6 * TAB_SIN_SIZE))    [degree / sin table unit]
 *
 * ANGLE_INCREMENT_MAX = ((6ULL * TAB_SIN_SIZE * F_MAX * 2 * (MAX_PWM + DEADTIME)) / 64Mhz)
 */
//#define ANGLE_INCREMENT_MAX_NUMERATOR   (6ULL * TAB_SIN_SIZE * F_MAX * (OCR0A_VAL + 1) * TIMER0_DIVIDER)
//#define ANGLE_INCREMENT_MAX_DENOMINATOR (F_CPU)
#define ANGLE_INCREMENT_MAX_NUMERATOR   (6UL * TAB_SIN_SIZE * F_MAX * 2 * (MAX_PWM + DEADTIME))
#define ANGLE_INCREMENT_MAX_DENOMINATOR (64000000UL)
#define ANGLE_INCREMENT_MAX             (ANGLE_INCREMENT_MAX_NUMERATOR / ANGLE_INCREMENT_MAX_DENOMINATOR)
#define ANGLE_INCREMENT_SCALED_MAX      (ANGLE_INCREMENT_MAX * ANGLE_SCALE)

/* The current position of the stator is calculated in units of sin table
 * multiplied with this scale value. */
#define ANGLE_SCALE 256

#endif
