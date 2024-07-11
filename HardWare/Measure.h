#ifndef __MEASURE_H
#define __MEASURE_H

/* R1-4 Unit:kO */
/* Limit condition: R1/R2 must less than 0.275 */
#define VOLTAG1_R1		10.0
#define VOLTAG1_R2		50.0
#define VOLTAG2_R1		10.02
#define VOLTAG2_R2		50.9

#define VOLTAG_MIN		11
#define VOLTAG_MAX		12.3

void Measure_Init(void);
uint8_t Measure_GetPerVoltage(void);
double Measure_GetVoltage(void);
double Measure_GetVoltage2(void);
double Measure_GetTemp(void);
uint16_t Measure_GetOriVoltage(void);
uint16_t Measure_GetOriVoltage2(void);
uint16_t Measure_GetOriTemp(void);
void Measure_UpdateBuf(void);

#endif
