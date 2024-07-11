#include "stm32f4xx.h"                  // Device header
#include "DMA2.h"
#include "ADC1.h"
#include "Measure.h"
#include "stdio.h"

void Measure_Init(void)
{
	DMA2_ADC1_Init();
	ADC1_Init();

}

uint8_t Measure_GetPerVoltage(void)
{
	int8_t PerV = 0;
	static int8_t lastPerV, futherPerV;
	
	PerV = (double)(Measure_GetVoltage() - VOLTAG_MIN) * 100 / (VOLTAG_MAX - VOLTAG_MIN);
	PerV = (PerV + lastPerV + futherPerV) / 3;
	futherPerV = lastPerV;
	lastPerV = PerV;
	
	return PerV;
}

double Measure_GetVoltage(void)
{
	static double lastV, futherV;
	double V = 0;
	
	/**	计算电压
	 * MCU_V = ConversionValue * 3.3 / 4096
	 *	V = MCU_V / (10 / 60)
	**/
	V = (double)Measure_GetOriVoltage() *3.3/4096/(VOLTAG1_R1/(VOLTAG1_R1 + VOLTAG1_R2));
	/*	计算内阻压降并补偿*/
	V += Line_R * (V/(VOLTAG1_R1 + VOLTAG1_R2));
	V = (futherV + lastV + V) / 3;
	futherV = lastV;
	lastV = V;
	
	//	v = (double)ADC_GetConversionValue(ADC1) * 9.768 / 2048; 
	return V;
}

double Measure_GetVoltage2(void)
{
	static double lastV, futherV;
	double V = 0;
	
	/**	计算电压
	 * MCU_V = ConversionValue * 3.3 / 4096
	 *	V = MCU_V / (10 / 60)
	**/
	V = (double)Measure_GetOriVoltage2() *3.3/4096/(VOLTAG2_R1/(VOLTAG2_R1 + VOLTAG2_R2));
	/*	计算内阻压降并补偿*/
	V += Line_R * (V/(VOLTAG1_R1 + VOLTAG1_R2));
	V = (futherV + lastV + V) / 3;
	futherV = lastV;
	lastV = V;
	
	//	v = (double)ADC_GetConversionValue(ADC1) * 9.768 / 2048; 
	return V;
}

double Measure_GetTemp(void)
{
	static double lastTemp, furtherTemp;
	double temp=0;
	
	temp = (double)Measure_GetOriTemp()*(3.3/4096);
	/* format: {Vsense - V25) / Avg_Slope} + 25 */
	temp=(temp-0.76)/0.0025+25; 	 
	temp = (furtherTemp + lastTemp + temp)/3;

	lastTemp = temp;
	furtherTemp = lastTemp;

	return temp;
}

uint16_t Measure_GetOriVoltage(void)
{
	return TempDatas_ADC1[VOLTAGE_VALUE];
}

uint16_t Measure_GetOriVoltage2(void)
{
	return TempDatas_ADC1[VOLTAGE2_VALUE];
}

uint16_t Measure_GetOriTemp(void)
{
	return TempDatas_ADC1[TEMPERATURE_VALUE];
}

/* Delay about 50ms is better for data reception */
void Measure_UpdateBuf(void)
{	
	uint8_t ovr_status = 0;
	ovr_status = ADC_GetFlagStatus(ADC1, ADC_FLAG_OVR); // 必要？
	printf("ovr_status:%d\r\n", ovr_status);
	DMA2_ADC1_Start();		/* restart to convert data */	
}

void Measure_GetR(void)
{
	double voltage1_R1, voltage1_R2, voltage2_R1, voltage2_R2;
	
	voltage1_R1 = (double)Measure_GetOriVoltage() *3.3/4096;
	voltage1_R2 = (double)Measure_GetVoltage() - voltage1_R1;
	voltage2_R1 = (double)Measure_GetOriVoltage2() *3.3/4096;
	voltage2_R2 = (double)Measure_GetVoltage2() - voltage2_R1;
	printf("V1R1:%f, V1R2:%f\t", voltage1_R1, voltage1_R2);
	printf("V2R1:%f, V2R2:%f\t", voltage2_R1, voltage2_R2);
}
