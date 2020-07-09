/*
 * ResourceHandler.c
 *
 * Created: 7/6/2020 2:18:42 PM
 *  Author: karol
 */ 

#include "ResourceHandler.h"
#include <stddef.h>
#include <stdio.h>
#include "task.h"
#include <stdbool.h>

uint16_t local_CO2;
float local_humidity;
float local_temperature;

bool isTaken_CO2 = 0;
bool isTaken_humidity = 0;
bool isTaken_temperature = 0;



uint16_t getCO2(){
	while(isTaken_CO2){	
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



void init_resources(){
	
	 local_CO2 = (uint16_t)pvPortMalloc(sizeof(uint16_t));
		 
}
