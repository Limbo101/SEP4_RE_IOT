/*
 * ServoHandler.c
 *
 * Created: 5.6.2020 ?. 11:22:54
 *  Author: Borislav Aleksiev, Dziugas Austys, Karolis Gudynas
 */ 
#include <stddef.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>

#include <lora_driver.h>
#include <iled.h>
#include <hal_defs.h>
#include "bits.h"
#include "event_groups.h"
#include "rcServo.h"
#include "ServoHandler.h"
#include "lora_driver.h"

/*
//MessageBufferHandle_t down_link_message_buffer_handle = xMessageBufferCreate(sizeof(lora_payload_t)*2);

lora_payload_t downlink_payload;
const uint8_t servoNo = 0;
int8_t percent = 100;


void Servo_handler_task( void *pvParameters );

void ServoHandler_create(){
    rcServoCreate();
    xTaskCreate(
    Servo_handler_task
    ,  (const portCHAR *)"ServoHandlerTask"  // A name just for humans
    ,  configMINIMAL_STACK_SIZE+200  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

}

void  Servo_handler_task( void *pvParameters ){	

  for (;;){

		xMessageBufferReceive(down_link_message_buffer_handle, &downlink_payload, sizeof(lora_payload_t), portMAX_DELAY);
		printf("DOWN LINK: from port: %d with %d bytes received!", downlink_payload.port_no, downlink_payload.len); 
		if (4 == downlink_payload.len) // Check that we have got the expected 4 bytes
			{
				printf("%s\n", "Hello from RCServo!!");
				percent = percent * -1;	// percent=100 (defined at top)
				rcServoSet(servoNo,percent); // servoNo=0 (constant defined at top)
			}
		vTaskDelay(500);
		   }


		vTaskDelay(1000);
    }
	*/