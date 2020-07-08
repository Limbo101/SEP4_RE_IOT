
/*
 * HumidityTemperatureHandler.h
 *
 * Created: 23.5.2020 г. 12:10:17
 *  Author: Borislav Aleksiev
 */ 
void hum_temp_task( void *pvParameters );

void HumidityTemperatureSensor_create();
uint16_t getHumidity();
uint16_t getTemperature();
