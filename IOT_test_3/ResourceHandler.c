/*
 * ResourceHandler.c
 *
 * Created: 7/6/2020 2:18:42 PM
 *  Author: Karolis Gudynas, Borislav Aleksiev, Dziugas Austys
 */ 

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <ATMEGA_FreeRTOS.h>

#include "task.h"
#include "ResourceHandler.h"

uint16_t local_CO2;
float local_humidity;
float local_temperature;

bool isTaken_CO2 = 0;
bool isTaken_humidity = 0;
bool isTaken_temperature = 0;

uint16_t getCO2(){ //not possible to return mutex after returning from function call, therefore we don't use it in get functions
	while(isTaken_CO2){	 // delay task while mutex is taken
		vTaskDelay(50);
	}
	return local_CO2;
}

void setCO2(uint16_t CO2){
	xSemaphoreTake(Mutex_CO2,1000);
	isTaken_CO2 = 1;
	local_CO2 = CO2;
	xSemaphoreGive(Mutex_CO2);
	isTaken_CO2 = 0;
}

float getHumidity(){
	while(isTaken_humidity){
		vTaskDelay(50);
	}
	return local_humidity;
}
void setHumidity(float humidity_level){
	xSemaphoreTake(Mutex_humidity,1000);
	isTaken_humidity = 1;
	local_humidity = humidity_level;
	xSemaphoreGive(Mutex_humidity);
	isTaken_humidity = 0;
}

float getTemperature(){
	while(isTaken_temperature){
		vTaskDelay(50);
	}
	return local_temperature;
}
void setTemperature(float temperature_level){
	xSemaphoreTake(Mutex_temperature,1000);
	isTaken_temperature = 1;
	local_temperature = temperature_level;
	xSemaphoreGive(Mutex_temperature);
	isTaken_temperature = 0;
}
