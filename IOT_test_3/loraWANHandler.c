/*
* loraWANHandler.c
*
* Created: 07/01/2020 10:09:05
*  Author: Karolis, Borislav, Dziugas
*/
#include <stddef.h>
#include <stdio.h>
#include <ATMEGA_FreeRTOS.h>
#include <lora_driver.h>
#include <iled.h>

#include "queue.h"
#include "event_groups.h"
#include "bits.h"
#include "EventGroupWrapper.h"
#include "QueueHandler.h"


// Parameters for OTAA join - You have got these in a mail from IHA
#define LORA_appEUI "F2241CBDAB5FAA1E"
#define LORA_appKEY "85482418A816EBCBBDB81F0F418B6597"


static char _out_buf[100]; 

void lora_send_task( void *pvParameters );

extern QueueHandle_t Message_queue;
static lora_payload_t _uplink_payload;

void lora_send_task_create(UBaseType_t lora_handler_task_priority)
{	
	xTaskCreate(
	lora_send_task
	,  (const portCHAR *)"LRHand"  // A name just for humans
	,  configMINIMAL_STACK_SIZE+200  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  lora_handler_task_priority  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );

}


bool lora_setup(){
	e_LoRa_return_code_t return_code;
	// Factory reset the transceiver
	printf("FactoryReset >%s<\n", lora_driver_map_return_code_to_text(lora_driver_rn2483_factory_reset()));
	// Configure to EU868 LoRaWAN standards
	printf("Configure to EU868 >%s<\n", lora_driver_map_return_code_to_text(lora_driver_configure_to_eu868()));
	// Get the transceivers HW EUI
	return_code = lora_driver_get_rn2483_hweui(_out_buf);
	printf("Get HWEUI >%s<: %s\n",lora_driver_map_return_code_to_text(return_code), _out_buf);
	// Set the HWEUI as DevEUI in the LoRaWAN software stack in the transceiver
	printf("Set DevEUI: %s >%s<\n", _out_buf, lora_driver_map_return_code_to_text(lora_driver_set_device_identifier(_out_buf)));
	// Set Over The Air Activation parameters to be ready to join the LoRaWAN
	printf("Set OTAA Identity appEUI:%s appKEY:%s devEUI:%s >%s<\n", LORA_appEUI, LORA_appKEY, _out_buf, lora_driver_map_return_code_to_text(lora_driver_set_otaa_identity(LORA_appEUI,LORA_appKEY,_out_buf)));
	// Save all the MAC settings in the transceiver
	printf("Save mac >%s<\n",lora_driver_map_return_code_to_text(lora_driver_save_mac()));
	// Enable Adaptive Data Rate
	printf("Set Adaptive Data Rate: ON >%s<\n", lora_driver_map_return_code_to_text(lora_driver_set_adaptive_data_rate(LoRa_ON)));
	// Set receiver window1 delay to 500 ms - this is needed if down-link messages will be used
	printf("Set Receiver Delay: %d ms >%s<\n", 500, lora_driver_map_return_code_to_text(lora_driver_set_receive_delay(500)));
	//xMessageBuffer = xMessageBufferCreate( 10 );
	// Join the LoRaWAN
	uint8_t maxJoinTriesLeft = 3; // This is 3 because after 3rd connection attempt Loriot responds "no free channel"
	
		do {
			return_code = lora_driver_join(LoRa_OTAA);
			printf("Join Network TriesLeft: %d >%s<\n", maxJoinTriesLeft, lora_driver_map_return_code_to_text(return_code));

			if(return_code == LoRa_NO_FREE_CH){
				return 0;
			}

			else if ( return_code != LoRa_ACCEPTED && return_code != LoRa_NO_FREE_CH)
			{
				// Make the red led pulse to tell something went wrong
				led_long_puls(led_ST1); // OPTIONAL
				// Wait 5 sec and lets try again
				vTaskDelay(pdMS_TO_TICKS(5000UL));
			}
			
			else if(return_code == LoRa_ACCEPTED)
			{
				return 1;
			}
			
		} while (--maxJoinTriesLeft);
		
			return 0;
}

/*-----------------------------------------------------------*/
void lora_send_task( void *pvParameters ){
	
	while(1)
	{
		static e_LoRa_return_code_t rc;
		
		// Hardware reset of LoRaWAN transceiver
		lora_driver_reset_rn2483(1);
		vTaskDelay(2);
		lora_driver_reset_rn2483(0);
		// Give it a chance to wakeup
		vTaskDelay(150);

		lora_driver_flush_buffers(); // get rid of first version string from module after reset!
	
		bool setup_value = 0;
	
		while(setup_value == 0){
			setup_value = lora_setup();	
		}
		 TickType_t xLastWakeTime;
		 const TickType_t xFrequency = pdMS_TO_TICKS(30000UL); // Upload message every 5 minutes (300000 ms)
		 xLastWakeTime = xTaskGetTickCount();
		 
		 while(1)
		 {
				vTaskDelayUntil( &xLastWakeTime, xFrequency ); 
				xEventGroupSetBits(Measure_event_group, CO2_measure_bit|Hum_temp_measure_bit); 
				EventBits_t dataBits = xEventGroupWaitBits(Data_event_group, Send_data_bit, pdTRUE, pdTRUE, 500);
				if((dataBits & (Send_data_bit)) == (Send_data_bit)){			
					BaseType_t dequeue = xQueueReceive(Message_queue, &_uplink_payload, portMAX_DELAY);
					if(dequeue == pdTRUE){				
						rc = lora_driver_sent_upload_message(false, &_uplink_payload);
						printf("Upload Message >%s<\n", lora_driver_map_return_code_to_text(rc));
						if(rc == LoRa_NO_FREE_CH){
							break;
						}
					}
				}
		 }
	
	}	
}
