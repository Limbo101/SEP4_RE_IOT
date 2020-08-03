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

const uint8_t servoNo = 0;
int8_t percent = 100;
extern MessageBufferHandle_t down_link_message_buffer_handle;
void Servo_handler_task( void *pvParameters );

void ServoHandler_create(){
    rcServoCreate();	
    xTaskCreate(
    Servo_handler_task,
      (const portCHAR *)"ServoHTask",  
      configMINIMAL_STACK_SIZE+50 , 
      NULL,
      3,  
      NULL );
}

void  Servo_handler_task( void *pvParameters )
{	
  for (;;){
	xMessageBufferReceive(down_link_message_buffer_handle, &downlink_payload, sizeof(lora_payload_t), portMAX_DELAY);
	if (downlink_payload.port_no==3){ // Check that we have get contacted on right port
		percent = percent * -1;	// Makes servo turn all length opposite direction
		rcServoSet(servoNo,percent);
	}
	lora_driver_flush_buffers();
  }
}
	
	