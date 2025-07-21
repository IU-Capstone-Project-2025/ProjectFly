#include "cmsis_os.h"
#include "main.h"
#include "user.h"
#include <stdio.h>
#include <math.h>

/// Global variables (usually auto generated)
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern USART_HandleTypeDef husart3;
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;


/// Max value ADC can output and its voltage
#define ADC_MAX_VALUE 4095
#define ADC_MAX_VOLTAGE 3.3
/// Number of measurements to store for averaging
#define ADC_WINDOW_SIZE 128
/// Number of sensor lines
#define SENSOR_LINES 2

/* Configs */
/// Number of sensors in each line
const int lineLength[SENSOR_LINES] = { 6, 6 };
/// Rescale each line to this boundaries
const float sens_boundaries[SENSOR_LINES][2] = {
    {0., 900.},
	{100., 1000.}
};
/// Max boundary value
const float sens_max = 1000.;

/// Buffer for printing
char buff[512];
/// ADC measurements go here
uint32_t adcData[SENSOR_LINES];
/// Window of last n measurements
uint32_t adcBuffer[SENSOR_LINES][ADC_WINDOW_SIZE] = {0};
/// Sum of last n measurements
uint64_t adcSum[SENSOR_LINES] = {0};


/**
 * The main loop of the program.
 */
void defaultTask() {
	for (;;) {
		// blink with green LED, dimming it
		setPWM(1000, &htim3, TIM_CHANNEL_3);
		osDelay(250);
		setPWM(10000, &htim3, TIM_CHANNEL_3);
		printMeasurements();
		osDelay(250);
	}
}

/**
 * Print message to the default (ST-Link) serial port.
 *
 * @param msg: Message to be printed
 * @param len: Length of the message
 */
void comprint(char *msg, int len) {
	HAL_USART_Transmit(&husart3, (uint8_t *)msg, len, 100);
}

/**
 * Set pulse time of a timer's channel.
 *
 * @param value: Pulse ticks
 * @param timer: Timer
 * @param channel: Channel
 */
void setPWM(uint16_t value, TIM_HandleTypeDef *timer, uint16_t channel)
{
    TIM_OC_InitTypeDef sConfigOC;

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = value;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(timer, &sConfigOC, channel); // таймер №1, канал №3
    HAL_TIM_PWM_Start(timer, channel);
}

/**
 * Callback on ADC conversion.
 *
 * @param hadc: ADC which invoked the conversion
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	if (hadc->Instance == ADC1) {
		// index to value in window to overwrite old values with new ones
		static uint16_t window_i = 0;
		for (int i = 0; i < SENSOR_LINES; ++i) {
			adcSum[i] -= adcBuffer[i][window_i];
			adcSum[i] += adcData[i];
			adcBuffer[i][window_i] = adcData[i];
		}
		window_i = (window_i + 1) % ADC_WINDOW_SIZE;
	}
}

/**
 * Convert a raw ADC measurement to real voltage.
 *
 * @param adc: Raw ADC measurement
 * @return Voltage
 */
double adcToVolts(uint32_t adc) {
	return adc / (double) ADC_MAX_VALUE * ADC_MAX_VOLTAGE;
}

/**
 * Convert voltage to index number of a triggered reed sensor.
 *
 * @param v: Voltage
 * @param N: Total number of sensors in the line
 * @return Index number of a triggered reed sensor in range [1, N]
 */
int voltToLineNumber(double v, int N) {
	const long double V_S = ADC_MAX_VOLTAGE;
	const long double R = 7440.L;
	const long double R_G = 98600.L;

	long double a = - v / V_S * R * R;
	long double b = R * R_G + v / V_S * (R*R * N + R*R);
	long double c = - R * R_G * N - R * R_G + v / V_S * (R * R_G * N + R * R_G);

	long double D = b * b - 4 * a * c;

	long double x1 = (sqrtl(D) - b) / a / 2.L;
	int n = N + 1 - ((x1 == x1) ? ((int) roundl(x1)) : 7);
	return n;
}

/**
 * Calculates measurements of each sensor line.
 * If no line is triggered now, the last measurement is used.
 *
 * @param[out] measurements: Measurements of each line
 * @param[out] voltages: Voltages on each line
 * @return Whether the sensor is triggered
 */
short getLineMeasurements(int *measurements, double *voltages) {
	static int prev[SENSOR_LINES] = { 0 };

	int active[SENSOR_LINES];
	short trig = 0;
	for (int i = 0; i < SENSOR_LINES; ++i) {
		voltages[i] = adcToVolts(adcSum[i] / (double) ADC_WINDOW_SIZE);
		active[i] = voltToLineNumber(voltages[i], lineLength[i]);
		if (active[i]) trig = 1;
	}
	for (int i = 0; i < SENSOR_LINES; ++i) {
		measurements[i] = trig ? active[i] : prev[i];
		if (trig) prev[i] = active[i];
	}
	return trig;
}

/**
 * Pretty-print current measurement state of water level into the serial port.
 */
void printMeasurements() {
	static const int BAR_LENGTH = 40;
	static const char FILL_SIGN[] = "##################################################################";
	static const char EMPTY_SIGN[] = "------------------------------------------------------------------";

	double volts[SENSOR_LINES];
	int activeN[SENSOR_LINES];
	getLineMeasurements(activeN, volts);
	int scaledC = 0;
	double scaled;
	comprint(buff, sprintf(buff, "\033[J\033[H"));
	for (int i = 0; i < SENSOR_LINES; ++i) {
		if (activeN[i]) {
			++scaledC;
			float dbound = sens_boundaries[i][1] - sens_boundaries[i][0];
			scaled += activeN[i] * dbound / (float) lineLength[i] + sens_boundaries[i][0];
		}
		comprint(buff, sprintf(
				buff,
				"Line %d: \033[1m%1d\033[m (\033[1m%1.1f\033[m/%1.1f)\033[K\r\n",
				i + 1,
				activeN[i],
				volts[i],
				ADC_MAX_VOLTAGE
		));
	}
	scaled = (scaledC == 0) ? 0 : (scaled / scaledC);
	comprint(buff, sprintf(buff, "Water level: %4.f/%.f\r\n", scaled, sens_max));
	int line_fill = (scaledC == 0) ? 0 : ((int) (scaled * BAR_LENGTH / sens_max));
	comprint(buff, sprintf(buff, "[%.*s%.*s]\r\n", line_fill, FILL_SIGN, (BAR_LENGTH - line_fill), EMPTY_SIGN));
}

void AdcRead(void *arguments) {
	for(;;)
	{
		HAL_ADC_Start_DMA(&hadc1, adcData, SENSOR_LINES);
		// osDelay(1);
	}
}
