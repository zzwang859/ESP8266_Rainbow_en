/***********************************************************************************************************************
*                                 Copyright (c) 2021,*****<-All rights reserved->
*               
**----------------------------------------------------------------------------------------------------------------------
*   File name:   ds1302.c
*   Brief description: DS1302 clock IC driver
*
**----------------------------------------------------------------------------------------------------------------------
*   Current version:   V1.0
*   Author/modifier: Wzz
*   Completion date: June 16, 2021
**----------------------------------------------------------------------------------------------------------------------
*   Replacement version:
*   Original author:   
*   Completion date:
***********************************************************************************************************************/
// #include "ds1302.h"

#include <Arduino.h>
#include "../app_types.h"
#include "DS1302.h"

/**********************************************************************************************************************
* Global variable definition
**********************************************************************************************************************/
 /* Time structure year, month, day, hour, minute, second, week               */
DS1302Module DS1302 = 
{
  {
    // 22, /* years */
    //  4, /* month */
    // 17, /* day */
    // 13, /* hour*/
    // 58, /* points */
    // 47, /* seconds */
    //  7        /**/

    24,       /* year */
     7,       /* month */
    17,       /* day */
    23,       /* time*/
    59,       /* points */
    47,       /* seconds */
     7        /* week */
  }
};                                                     

static void DS1302_GPIO_Init(void) 
{
    pinMode(DS1302_DAT,OUTPUT);
    pinMode(DS1302_CE, OUTPUT);
    pinMode(DS1302_CLK,OUTPUT);

    digitalWrite(DS1302_DAT, HIGH);
    digitalWrite(DS1302_CE,  LOW);  
    digitalWrite(DS1302_CLK, LOW);    
}


/**********************************************************************************************************************
*   Function name: ds_delay
*   Function description: DS1302 delayed use
*   Parameter list: Delay amount
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              Terry 2021.6.16 Original creation version V1.0
*
**********************************************************************************************************************/
static void ds_delay(unsigned int delay) 
{
    while(delay--);
}



/**********************************************************************************************************************
*   Function name: input_byte
*   Function description: The bottom layer of timing transmits 1 byte to DS1302
*   Parameter list: dat transmission content
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              Terry 2021.6.16 Original creation version V1.0
*
**********************************************************************************************************************/
static void input_byte(unsigned char dat) 
{ 
    unsigned char i;
    
    SET_DAT_OUTMODE ;                                                   /* Set IO as output                             */
    
    for(i=0; i<8; i++) {
        (dat&0x1)?DS1302_DAT_H:DS1302_DAT_L;                            /* Prepare data levels                              */                          
        ds_delay(CLOCK_RATE); 
        DS1302_CLK_H;                                                   /* Rising edge latches data                            */
        ds_delay(CLOCK_RATE);
        DS1302_CLK_L;                   
        dat =dat>> 1;                                                   /* Prepare for next data                              */
    }
}

/**********************************************************************************************************************
*   Function name: out_byte
*   Function description: Transmit 1 byte from the bottom layer of DS1302
*   Parameter list: None
*   Hardware output: None
*   Return result: transmitted value
*   History: Author/Modifier Date Remarks
*              Terry 2021.6.16 Original creation version V1.0
*
**********************************************************************************************************************/
static unsigned char out_byte(void) 
{ 
    unsigned char i;
    unsigned char res = 0;
    
    SET_DAT_INPUTMODE ;                                                 /* Set data IO as input                          */

    for(i=0; i<8; i++) {   
        ds_delay(CLOCK_RATE); 
        res |= (GET_DAT)?0x80:0x00;                                     /* Read data level                              */

        if(i<7) {
            DS1302_CLK_H;                                               /* Rising edge latches data                            */        
            ds_delay(CLOCK_RATE);
            DS1302_CLK_L;   
            res =res>> 1;                                               /* Prepare for next data                              */
        }            
        
    }   
    return  res;
}

/**********************************************************************************************************************
*   Function name: set_reg
*   Function description: Set a register
*   Parameter list: addr register address val register value
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              Terry 2021.6.16 Original creation version V1.0
*
**********************************************************************************************************************/
static void set_reg(unsigned char addr, unsigned char val)
{
    DS1302_CE__H;   
    input_byte(addr|DS_WRITE);
    input_byte(val);    
    DS1302_CE__L;
}

/**********************************************************************************************************************
*   Function name: get_reg
*   Function description: Read a register
*   Parameter list: None
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              Terry 2021.6.16 Original creation version V1.0
*
**********************************************************************************************************************/
static unsigned char get_reg(unsigned char addr)
{
    unsigned char res;
    
    DS1302_CE__H; 
    input_byte(addr|DS_READ);
    res = out_byte();
    DS1302_CE__L; 
    
    return  res;  
}

/**********************************************************************************************************************
*   Function name: DS_Set_Time
*   Function description: Set DS1302 time (24-hour format)
*   Parameter list: Global variable DStime
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              Terry 2021.6.16 Original creation version V1.0
*
**********************************************************************************************************************/
void DS1302Module::setTime(void )
{
    DS1302_GPIO_Init() ;                                                            /* Prepare GPIO                     */

    set_reg(DS_WP,     0x00);                                                       /* Remove write protection                    */
    set_reg(DS_SECOND, 0x80);                                                       /* Stop the clock                  */  
                                                                                                                     
    set_reg(DS_YEAR,    (((unsigned char)(DStime.year/10))<<4)   | (DStime.year%10) );    /* Decimal to BCD year             */
    set_reg(DS_MONTH,   (((unsigned char)(DStime.month/10))<<4)  | (DStime.month%10) );   /* Decimal to BCD code month             */
    set_reg(DS_DATA,    (((unsigned char)(DStime.day/10))<<4)    | (DStime.day%10) );     /* Decimal to BCD day             */
    set_reg(DS_WEEK,    (((unsigned char)(DStime.week/10))<<4)   | (DStime.week%10) );    /* Decimal to BCD code day of the week           */
    set_reg(DS_HOUR,    (((unsigned char)(DStime.hour/10))<<4)   | (DStime.hour%10) );    /* When converting decimal to BCD code             */
    set_reg(DS_MINUTE,  (((unsigned char)(DStime.minute/10))<<4) | (DStime.minute%10) );  /* Decimal to BCD code minutes             */
    set_reg(DS_SECOND,  (((unsigned char)(DStime.second/10))<<4) | (DStime.second%10) );  /* Decimal to BCD code seconds + start        */
    
    set_reg(DS_WP, 0x80 );                                                          /* Start register protection                */

    DS1302_GPIO_Init() ;                                                            /* Restore GPIO                     */
}

/**********************************************************************************************************************
*   Function name: DS_Get_Time
*   Function description: Read DS1302 time
*   Parameter list: None
*   Hardware output: None
*   Return result: global variable DStime
*              result: Determine whether DS1302 exists 
*                     \ref  NONE_DS1302 
*                     \ref   HAVE_DS1302  
*   History: Author/Modifier Date Remarks
*              Terry 2021.6.16 Original creation version V1.0
*
**********************************************************************************************************************/
unsigned char DS1302Module::getTime(void )
{

    DS1302_GPIO_Init() ;                                                            /* Prepare GPIO                      */  

    DStime.year   = get_reg(DS_YEAR   );                               /* year                                         */
    DStime.month  = get_reg(DS_MONTH  );                               /* month                                       */
    DStime.day    = get_reg(DS_DATA   );                               /* Date                                       */ 
    DStime.week   = get_reg(DS_WEEK   );                               /* week                                       */            
    DStime.hour   = get_reg(DS_HOUR   );                               /* time                                         */
    DStime.minute = get_reg(DS_MINUTE );                               /* points                                         */            
    DStime.second = get_reg(DS_SECOND );                               /* seconds                                         */


    if(DStime.year > 0x99) {                                           /* If DS1302 does not exist, it will read 0xFF                */        
#if DEBUG_INFO_EN                                                         /* DS1302 BCD code maximum 0x99                     */
    Serial.println("can not find DS1302");         
#endif            
      return NONE_DS1302;                                                            
    }
    
    
    /* Convert BCD code to decimal */
    DStime.year   &= 0xff; DStime.year  = (DStime.year  >>4)*10 + (DStime.year&0xf);
    DStime.month  &= 0x1f; DStime.month = (DStime.month >>4)*10 + (DStime.month&0xf);
    DStime.day    &= 0x3f; DStime.day   = (DStime.day   >>4)*10 + (DStime.day&0xf);
    DStime.week   &= 0x0f;
    DStime.hour   &= 0x3f; DStime.hour   = (DStime.hour   >>4)*10 + (DStime.hour&0xf);    
    DStime.minute &= 0x7f; DStime.minute = (DStime.minute >>4)*10 + (DStime.minute&0xf);
    DStime.second &= 0x7f; DStime.second = (DStime.second >>4)*10 + (DStime.second&0xf);    

    DS1302_GPIO_Init() ;                                                            /* Restore GPIO                     */    
    
    return HAVE_DS1302;    
}
 
/***********************************************************************************************************************
*  end of file
***********************************************************************************************************************/






