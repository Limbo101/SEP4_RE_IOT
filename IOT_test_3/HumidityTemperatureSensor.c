
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
		(const portCHAR *)"HumTempTask", 
		configMINIMAL_STACK_SIZE+50,  
		NULL,
		2, 
		NULL
	 );
}

void hum_temp_task( void *pvParameters )
{
	for (;;) {
		
		EventBits_t measureBits = xEventGroupWaitBits(Measure_event_group, Hum_temp_measure_bit, pdTRUE, pdTRUE, 500); // wait for permission from lorawan
		if((measureBits & (Hum_temp_measure_bit)) == (Hum_temp_measure_bit)){
			
				driver_ready_check = hih8120Wakeup();
				while (driver_ready_check != HIH8120_OK){
					vTaskDelay(50);
					driver_ready_check = hih8120Wakeup();
					
				}
				vTaskDelay(60); // necessary wait delay after calling wakeup
				driver_ready_check = hih8120Meassure();
				while (driver_ready_check != HIH8120_OK){ // check if measure went okay
					vTaskDelay(50);
					driver_ready_check = hih8120Meassure();
				}
				vTaskDelay(20); // giving it some time to get the values
				setHumidity(hih8120GetHumidity()); // set values
				setTemperature(hih8120GetTemperature());
				xEventGroupSetBits(Data_event_group, Hum_temp_data_bit); // send comfirmation for packet assembly
		}		
	}
}
