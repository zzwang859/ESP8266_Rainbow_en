#ifndef __ADC_H__
#define __ADC_H__

#include <Arduino.h>
#include "../config.h"

class AdcModule
{
public:
    /***********************************************************************************************************************
    *   Function name: adc_prepare
    *   Function description: Obtain the preparatory actions before ADC acquisition voltage
    *   Parameter list: option: BAT_VAL Get battery voltage
    *                      IRF_VAL Gets the infrared tube voltage
    *                      ADC_OFF turns off ADC
    *   Hardware output: None
    *   Return result: None
    ***********************************************************************************************************************/
    void adc_prepare(uint8_t option );

    /***********************************************************************************************************************
    *   Function name: get_adc_value
    *   Function description: Get ADC acquisition voltage
    *   Parameter list: option: BAT_VAL Get battery voltage
    *                     IRF_VAL Gets the infrared tube voltage
    *   Hardware output: None
    *   Return result: None
    ***********************************************************************************************************************/
    uint16_t get_adc_value( uint8_t option );

    /***********************************************************************************************************************
    *   Function name: Get_battery_voltage
    *   Function description: Measure battery voltage
    *   Parameter list: None
    *   Hardware output: None
    *   Return result: bat_val: battery voltage (0~1024)
    ***********************************************************************************************************************/
    uint16_t Get_battery_voltage(void );

private:
    /***********************************************************************************************************************
    *   Function name: ADC_Init
    *   Function description: Initialize ADC related GPIO
    *   Parameter list: None
    *   Hardware output: None
    *   Return result: None
    ***********************************************************************************************************************/
    void ADC_Init(void);
};

extern AdcModule Adc;


/* ADC related GPIO allocation  */
#define PIN_A0    (A0)   /* ADC sampling pin */ 
#define ADC_CTRL  (14)   /* Reuse Normally low */ 
#define IR_DRIVE  (0)    /* Reuse Normally high */ 
#define IR_LOOP   (13)   /* Reuse Normally low*/

/* ADC function parameter declaration */
#define ADC_OFF 0x0                      /* Turn off ADC acquisition     */
#define BAT_VAL 0x1                      /* Collect battery voltage     */
#define IRF_VAL 0x2                      /* Collect infrared tube voltage */

#define ADC_DEFAULT_ADJUST -4.5  /* Implicit in the decimal part of the altitude x (x-0.5)*(30.0/(360.0/4096.0)) an adjustment range of plus or minus 15 degrees */ 

#define ADC_MEAN_TIMES 10                /* ADC averaging times */
//#define ADC_FLAG_LOCATION   (2578 +16)   /* Mechanical structure reflection column position round((218.6505+7)/(360/4096)) The smaller the coordinate system, the smaller the coordinate system, the downward rotation and fine adjustment. The larger the larger the coordinate system, the upward rotation and fine adjustment. */ /* v1.3 Shell structure*/
#define ADC_FLAG_LOCATION   (3088)   /* Mechanical structure reflection column position round((270+1.4063)/(360/4096)) The smaller the coordinate system (left view), rotate downward for fine adjustment. The larger the coordinate system (left view), rotate upward for fine adjustment. */   /* v1.4 Shell structure*/

//#define BAT_LOW_GATE     1               /* Detect the battery voltage. If it is lower than this value, it will enter low power mode. Debugging value*/
// #define BAT_LOW_GATE       600              /* Detect battery voltage, enter low power mode if it is lower than this value 930=3.7v 955=4v*/

#endif /* __ADC_H__ */

