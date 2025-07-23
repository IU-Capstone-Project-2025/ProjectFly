/*
 * user.h
 *
 *  Created on: Jul 21, 2025
 *      Author: Dioxider
 */

#ifndef INC_USER_H_
#define INC_USER_H_

void defaultBody();
void comprint(char *, int);
//void setPWM(uint16_t, TIM_HandleTypeDef *, uint16_t);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *);
double adcToVolts(uint32_t);
int voltToLineNumber(double, int);
short getLineMeasurements(int *, double *);
void printMeasurements();

#endif /* INC_USER_H_ */
