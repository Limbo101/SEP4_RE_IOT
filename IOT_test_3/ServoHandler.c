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
#include "EventGroupWrapper.h"
#include "DownlinkWrapper.h"
#include "lora_driver.h"
//#include "MessageBufferHandler.h"


//lora_payload_t downlink_payload;
const uint8_t servoNo = 0;
int8_t percent = 100;
//extern MessageBufferHandle_t xMessageBuffer; // NOTE: Make message buffer header file and initialize it in main

void Servo_handler_task( void *pvParameters );

void ServoHandler_create(){
    rcServoCreate();
	//down_link_message_buffer_handle = xMessageBufferCreate(sizeof(lora_payload_t)*4);
	
    xTaskCreate(
    Servo_handler_task
    ,  (const portCHAR *)"ServoHTask"  // A name just for humans
    ,  configMINIMAL_STACK_SIZE+50  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );
}

void flushDownlink(){
downlink_payload.port_no = 0;
downlink_payload.len = 0;
}

void  Servo_handler_task( void *pvParameters )
{	
	 if(down_link_message_buffer_handle == NULL){
		 printf("%s\n","Message buffer is null!");
	 }
	 else if(down_link_message_buffer_handle != NULL){
	//	 printf("%s\n","Message buffer is GOOD!");
	 }
  for (;;)
  {

		xMessageBufferReceive(down_link_message_buffer_handle, &downlink_payload, sizeof(lora_payload_t), portMAX_DELAY);
		
		//printf("DOWN LINK: from port: %d with %d bytes received!\n", downlink_payload.port_no, downlink_payload.len); 
		if (downlink_payload.port_no==3) // Check that we have got the expected 8 bytes
			{
				/*
				uint16_t _newValue;
				printf("%s\n", "Hello from RCServo!!");
				_newValue = downlink_payload.bytes[0] << 8;
				_newValue |= downlink_payload.bytes[1];
				printf("byte1 %d \n", _newValue);
				_newValue = downlink_payload.bytes[2] << 8;
				_newValue |= downlink_payload.bytes[3];
				printf("byte2 %d \n", _newValue);
				_newValue = downlink_payload.bytes[4] << 8;
				_newValue |= downlink_payload.bytes[5];
				printf("byte3 %d \n", _newValue);
				*/
				
				percent = percent * -1;	// percent=100 (defined at top)
				rcServoSet(servoNo,percent); // servoNo=0 (constant defined at top)
				flushDownlink();
			}
		lora_driver_flush_buffers();
  }
}
	
	