/*
* main.c
* Author : Karolis Gudynas, Borislav Aleksiev, Dziugas Austys
*/

#include <stdio.h>
#include <hal_defs.h>
#include <ihal.h>

#include <ATMEGA_FreeRTOS.h>

#include <FreeRTOSTraceDriver.h>
#include <stdio_driver.h>
#include <serial.h>

// Needed for LoRaWAN
#include <lora_driver.h>
#include "bits.h"
#include "EventGroupWrapper.h"
#include "DownlinkWrapper.h"
#include "QueueHandler.h"
#include "PacketAssembly.h"
#include "ResourceHandler.h"

extern QueueHandle_t Message_queue;

void lora_send_task_create(UBaseType_t lora_handler_task_priority);
extern MessageBufferHandle_t Messadown_link_message_buffer_handle;

/*-----------------------------------------------------------*/
void create_tasks_and_resources(void)
{	
		ServoHandler_create();
		HumidityTemperatureSensor_create();
		CO2Sensor_create();
		PacketAssembly_create();
}

void create_controllers() // controllers - influences how tasks and resources interact
{
	down_link_message_buffer_handle = xMessageBufferCreate(sizeof(lora_payload_t)*2);
	if(down_link_message_buffer_handle == NULL){
		printf("%s\n","Message buffer not created!!");
	}
	if ( Mutex_CO2 == NULL ) {  // Check to confirm that the Semaphore has not already been created.
	
		Mutex_CO2 = xSemaphoreCreateMutex();  // Create a MUTEX semaphore.
		if ( ( Mutex_CO2 ) != NULL ){
			xSemaphoreGive( ( Mutex_CO2 ) );  // Make the MUTEX available for use, by initially "Giving" the Semaphore.
		}
	}
	
	if ( Mutex_humidity == NULL ) {
		Mutex_humidity = xSemaphoreCreateMutex(); 
		if ( ( Mutex_humidity ) != NULL ){
			xSemaphoreGive( ( Mutex_humidity ) ); 
		}
	}
	
	if ( Mutex_temperature == NULL ) {
		Mutex_temperature = xSemaphoreCreateMutex();
		if ( ( Mutex_temperature ) != NULL ){
			xSemaphoreGive( ( Mutex_temperature ) );
		}
	}
	Measure_event_group = xEventGroupCreate();
	if(Measure_event_group == NULL)
			printf("%s\n","#ERROR - Measure_event_group was NOT created because there was insufficient FreeRTOS heap available");
	Data_event_group = xEventGroupCreate();
	if(Data_event_group == NULL) 
			printf("%s\n","#ERROR - Measure_event_group was NOT created because there was insufficient FreeRTOS heap available");
	Message_queue = xQueueCreate(10,sizeof(lora_payload_t));
}


/*-----------------------------------------------------------*/
void initialiseSystem()
{
	// Initialize the trace-driver to be used together with the R2R-Network
	trace_init();
	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdioCreate(ser_USART0);
	create_controllers();
	create_tasks_and_resources();
	// Initialize the HAL layer and use 5 for LED driver priority
	hal_create(5);
	// Create LoRaWAN task and start it up with priority 3
	lora_send_task_create(3);
}

/*-----------------------------------------------------------*/
int main(void)
{	
	initialiseSystem(); 
	printf("START\n");
	vTaskStartScheduler(); // Initialize and run the freeRTOS scheduler. Execution should never return from here.
	while (1)
	{
		printf("Task Scheduler crashed!");
	}
}
	
