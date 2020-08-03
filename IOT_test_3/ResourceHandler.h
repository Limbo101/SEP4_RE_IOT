/*
 * MutexPrintf.h
 *
 * Created: 7/4/2020 5:46:10 PM
 *  Author: Karolis, Borislav, Dziugas
 */ 

#ifndef ResourceHandler.h
#define ResourceHandler.h

#include <ATMEGA_FreeRTOS.h>
#include <stddef.h>
#include "semphr.h"

SemaphoreHandle_t Mutex_CO2;
SemaphoreHandle_t Mutex_humidity;
SemaphoreHandle_t Mutex_temperature;

uint16_t getCO2 ();
float getHumidity();
float getTemperature();

void setCO2(uint16_t CO2);
void setHumidity(float humidity_level);
void setTemperature(float temperature_level);

void init_resources();


#endif