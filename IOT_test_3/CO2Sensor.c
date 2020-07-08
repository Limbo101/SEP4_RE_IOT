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
#include "event_groups.h"
#include "mh_z19.h"
#include "EventGroupWrapper.h"
#include "ResourceHandler.h"

mh_z19_return_code_t rc;
	
void CO2_handler_task( void *pvParameters );

void CO2Sensor_create()
{
	mh_z19_create(ser_USART3, setCO2);
	xTaskCreate(
		CO2_handler_task,
		(const portCHAR *)"CO2HandTask",  
		configMINIMAL_STACK_SIZE+200,
		NULL,
		3,
		NULL 
	);
}


void CO2_handler_task( void *pvParameters )
{

	for (;;) 
	{
		
		EventBits_t measureBits = xEventGroupWaitBits(Measure_event_group, CO2_measure_bit, pdTRUE, pdTRUE, 500);
		if((measureBits & (CO2_measure_bit)) == (CO2_measure_bit))
		{
				rc = mh_z19_take_meassuring();
				if (rc != MHZ19_OK)
				{
					printf("%s\n","Arduino measurement taking from sensor went wrong!"); 
				}
				 printf("%s%i\n","Arduino measurement CO2: ", (int)getCO2());	
				xEventGroupSetBits(Data_event_group, CO2_data_bit);
				vTaskDelay(200);
		}
	

	}
}

//mh_z19_return_code_t