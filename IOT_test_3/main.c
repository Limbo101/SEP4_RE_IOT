/*
* main.c
* Author : Karolis Gudynas, Borislav Aleksiev, Dziugas Austys
*
* 
*/

#include <stdio.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
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
#include "ResourceHandler.h"
#include "MessageBufferHandler.h"



// Prototype for LoRaWAN handler
void lora_send_task_create(UBaseType_t lora_handler_task_priority);
//void lora_receive_task_create();

//extern MessageBufferHandle_t xMessageBuffer; 


/*-----------------------------------------------------------*/
void create_tasks_and_semaphores(void)
{	
	// Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
	// because it is sharing a resource, such as the Serial port.
	// Semaphores should only be used whilst the scheduler is running, but we can set it up here.
	
	if ( Mutex_CO2 == NULL )  // Check to confirm that the Semaphore has not already been created.
	{
		Mutex_CO2 = xSemaphoreCreateMutex();  // Create a MUTEX semaphore.
		if ( ( Mutex_CO2 ) != NULL )
		{
			xSemaphoreGive( ( Mutex_CO2 ) );  // Make the MUTEX available for use, by initially "Giving" the Semaphore.
		}
	}
	
	if ( Mutex_humidity == NULL )  // Check to confirm that the Semaphore has not already been created.
	{
		Mutex_humidity = xSemaphoreCreateMutex();  // Create a MUTEX semaphore.
		if ( ( Mutex_humidity ) != NULL )
		{
			xSemaphoreGive( ( Mutex_humidity ) );  // Make the MUTEX available for use, by initially "Giving" the Semaphore.
		}
	}
	
	if ( Mutex_temperature == NULL )  // Check to confirm that the Semaphore has not already been created.
	{
		Mutex_temperature = xSemaphoreCreateMutex();  // Create a MUTEX semaphore.
		if ( ( Mutex_temperature ) != NULL )
		{
			xSemaphoreGive( ( Mutex_temperature ) );  // Make the MUTEX available for use, by initially "Giving" the Semaphore.
		}
	}
	
		init_resources();
		HumidityTemperatureSensor_create();
		CO2Sensor_create();
		
}

void create_event_groups_and_queues()
{
	//xMessageBuffer = xMessageBufferCreate(100); // change this if doesn't work
	
	Measure_event_group = xEventGroupCreate();
	if(Measure_event_group == NULL){
			printf("%s\n","#ERROR - Measure_event_group was NOT created because there was insufficient FreeRTOS heap available");
	}
	Data_event_group = xEventGroupCreate();
	if(Data_event_group == NULL) 
			printf("%s\n","#ERROR - Measure_event_group was NOT created because there was insufficient FreeRTOS heap available");
	
	/*
	Send_receive_event_group = xEventGroupCreate();
	if(Send_receive_event_group == NULL){
	//	printf("%s\n","#ERROR - Send_receive_event_group was NOT created because there was insufficient FreeRTOS heap available");
	}
	*/
}


/*-----------------------------------------------------------*/
void initialiseSystem()
{
	// Set output ports for LEDS used in the example
	DDRA |= _BV(DDA0) | _BV(DDA7);
	
	// Initialize the trace-driver to be used together with the R2R-Network
	trace_init();
	
	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdioCreate(ser_USART0);
	
	// Let's create some tasks
	create_event_groups_and_queues();
	create_tasks_and_semaphores();
	

	// Initialize the HAL layer and use 5 for LED driver priority
	hal_create(5);
	
	// Initialize the LoRaWAN driver with down-link buffer
	lora_driver_create(LORA_USART, NULL); 
	
	// Create LoRaWAN task and start it up with priority 3
	lora_send_task_create(3);
//
//	lora_receive_task_create();
//	ServoHandler_create();
}

/*-----------------------------------------------------------*/
int main(void)
{	
	
	initialiseSystem(); 
	printf("START\n");
	vTaskStartScheduler(); // Initialize and run the freeRTOS scheduler. Execution should never return from here.
	while (1)
	{
		printf("This bit of code shouldn't run!!");
	}
}
	
