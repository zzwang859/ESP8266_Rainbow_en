/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * Code: ADCInit
------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include <Arduino.h>
#include "adc.h"

AdcModule Adc;

/***********************************************************************************************************************
*   Function name: ADC_Init
*   Function description: Initialize ADC related GPIO
*   Parameter list: None
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2022.10.25 modified version V1.0
*
***********************************************************************************************************************/
void AdcModule::ADC_Init(void)
{
  pinMode(PIN_A0, INPUT);
  pinMode(ADC_CTRL,OUTPUT);
  pinMode(IR_DRIVE,OUTPUT);
  pinMode(IR_LOOP, OUTPUT);
}

/***********************************************************************************************************************
*   Function name: adc_prepare
*   Function description: Obtain the preparatory actions before ADC acquisition voltage
*   Parameter list: option: BAT_VAL Get battery voltage
*                      IRF_VAL Gets the infrared tube voltage
*                      ADC_OFF turns off ADC 
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2022.10.25 modified version V1.0
*
***********************************************************************************************************************/
void AdcModule::adc_prepare(uint8_t option )
{
   ADC_Init();                                                                                /* Initialize ADC acquisition    */
   switch(option)
    {
        /* Obtain the battery voltage and automatically turn off the ADC after each operation. */
        case BAT_VAL:
            digitalWrite(ADC_CTRL, HIGH);    /* battery access open    */
            digitalWrite(IR_DRIVE, HIGH);    /* Infrared diode goes out  */
            digitalWrite(IR_LOOP,  LOW);     /* Close infrared loop    */
        break;

        /* Obtain the infrared tube voltage, and the ADC will be automatically turned off after each operation. */
        case IRF_VAL:
            digitalWrite(ADC_CTRL, HIGH);   /* battery access open   */
            digitalWrite(IR_DRIVE, LOW);    /* Infrared diode lights up */ 
            digitalWrite(IR_LOOP,  HIGH);   /* open infrared loop   */
        break;

        /* Turn off the infrared tube*/
        case ADC_OFF:
        default:
            digitalWrite(ADC_CTRL, LOW);    /* Battery access closed   */
            digitalWrite(IR_DRIVE, HIGH);   /* Infrared diode goes out */
            digitalWrite(IR_LOOP,  LOW);    /* Close infrared loop   */
            break;
    }
}

/***********************************************************************************************************************
*   Function name: get_adc_value
*   Function description: Get ADC acquisition voltage
*   Parameter list: option: BAT_VAL Get battery voltage
*                     IRF_VAL Gets the infrared tube voltage
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2022.10.25 modified version V1.0
*
***********************************************************************************************************************/
uint16_t AdcModule::get_adc_value( uint8_t option )
{
    uint16_t adc_value = 0;
    uint32_t i,tmp;

    switch(option)
    {
        /* To obtain the battery voltage, the ADC should be turned off manually after each operation. */
        case BAT_VAL:
            tmp = 0;
            for (i = 0; i < ADC_MEAN_TIMES; i++) {
                tmp += analogRead(PIN_A0);
            }
            adc_value = (uint16_t)(tmp/ADC_MEAN_TIMES); 
        break;

        case IRF_VAL:
            tmp = 0;
            for (i = 0; i < ADC_MEAN_TIMES; i++) {
                tmp += analogRead(PIN_A0);
            }
            adc_value = (uint16_t)(tmp/ADC_MEAN_TIMES);   
        break;

        default:
            adc_value = 0;
            break;
    }

    return adc_value;
}

/***********************************************************************************************************************
*   Function name: Get_battery_voltage
*   Function description: Measure battery voltage
*   Parameter list: None
*   Hardware output: None
*   Return result: bat_val: battery voltage (0~1024)
*   History: Author/Modifier Date Remarks
*              whys 2022.10.25 modified version V1.0
*
***********************************************************************************************************************/
uint16_t  AdcModule::Get_battery_voltage(void )
{
  uint16_t bat_val = 0;

  adc_prepare( BAT_VAL);                                     /* Getting ready to measure battery voltage        */  
  delay(10);                                                 /* Wait for the voltage to stabilize            */
  bat_val = get_adc_value( BAT_VAL );                        /* Measure battery voltage            */
  adc_prepare( ADC_OFF);                                     /* Turn off measurement status to save power      */ 

  return  bat_val;                                                     
}

