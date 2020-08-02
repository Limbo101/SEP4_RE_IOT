
/*
 * PacketAssembly.c
 *
 * Created: 8/2/2020 10:01:31 AM
 *  Authors: Karolis Gudynas, Dziugas Austys, Borislav Aleksiev
 */ 

#include <ATMEGA_FreeRTOS.h>

#include "PacketAssembly.h"
#include "QueueHandler.h"
#include "EventGroupWrapper.h"
#include "ResourceHandler.h"
#include "bits.h"

lora_payload_t payload_to_queue;

extern QueueHandle_t Message_queue;

void packet_assembly_task();

void PacketAssembly_create()
{
	xTaskCreate(
	packet_assembly_task,
	(const portCHAR *)"PackAssembly",  // A name just for humans
	configMINIMAL_STACK_SIZE+50,  // This stack size can be checked & adjusted by reading the Stack Highwater
	NULL,
	2,  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	NULL
	);
}


void packet_assembly_task(){
	
	for(;;){
		
		EventBits_t dataBits = xEventGroupWaitBits(Data_event_group, CO2_data_bit|Hum_temp_data_bit, pdTRUE, pdTRUE, 500);
		if((dataBits & (CO2_data_bit|Hum_temp_data_bit)) == (CO2_data_bit|Hum_temp_data_bit)){
			
			uint16_t hum = (int) getHumidity();
			int16_t temp = (int) getTemperature();
			uint16_t co2_ppm = getCO2();
			
			payload_to_queue.len = 6;
			payload_to_queue.port_no = 2;
			payload_to_queue.bytes[0] = hum >> 8;
			payload_to_queue.bytes[1] = hum & 0xFF;
			payload_to_queue.bytes[2] = temp >> 8;
			payload_to_queue.bytes[3] = temp & 0xFF;
			payload_to_queue.bytes[4] = co2_ppm >> 8;
			payload_to_queue.bytes[5] = co2_ppm & 0xFF;
			//printf("%i\n",payload_to_queue.bytes[0]);
			xQueueSend(Message_queue, &payload_to_queue,portMAX_DELAY);
			
			xEventGroupSetBits(Data_event_group, Send_data_bit);
		}
	}
}