﻿

/*
 * CO2Handler.c
 *
 * Created: 23.5.2020 г. 10:49:34
 *  Author: Borislav Aleksiev, Karolis Gudynas, Dziugas Austys
 */ 

#include <stddef.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>

#include <lora_driver.h>
#include <iled.h>
#include "bits.h"
//#include "event_groups.h"
#include "hih8120.h"
#include "EventGroupWrapper.h"
#include "ResourceHandler.h"


int driver_ready_check;

	
void hum_temp_task(void *pvParameters);

void HumidityTemperatureSensor_create()
{
	hih8120Create(); 
	
	xTaskCreate(
		hum_temp_task,
		(const portCHAR *)"HumTempTask",  // A name just for humans
		configMINIMAL_STACK_SIZE+50,  // This stack size can be checked & adjusted by reading the Stack Highwater
		NULL,
		2,  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
		NULL
	 );
}


void hum_temp_task( void *pvParameters )
{

	for (;;) 
	{
		
		EventBits_t measureBits = xEventGroupWaitBits(Measure_event_group, Hum_temp_measure_bit, pdTRUE, pdTRUE, 500);
		
		if((measureBits & (Hum_temp_measure_bit)) == (Hum_temp_measure_bit)){
			
				driver_ready_check = hih8120Wakeup();
				while (driver_ready_check != HIH8120_OK)
				{
					vTaskDelay(50);
					driver_ready_check = hih8120Wakeup();
					
				}

				vTaskDelay(60); // necessary wait delay after calling wakeup
				
				driver_ready_check = hih8120Meassure();
				
				while (driver_ready_check != HIH8120_OK)
				{
					vTaskDelay(50);
					driver_ready_check = hih8120Meassure();
				}
				
				vTaskDelay(20); // giving it some time to get the values
			
				setHumidity(hih8120GetHumidity());
				setTemperature(hih8120GetTemperature());
				
				xEventGroupSetBits(Data_event_group, Hum_temp_data_bit);
		}		
		
	}
	
}
