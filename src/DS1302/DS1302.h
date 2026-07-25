/***********************************************************************************************************************
*                                 Copyright (c) 2021,*****<-All rights reserved->
*               
**----------------------------------------------------------------------------------------------------------------------
*   File name:   ds1302.h
*   Brief description: DS1302 clock IC driver header file
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
#ifndef __DS1302_H__
#define __DS1302_H__

#include <Arduino.h>

/***********************************************************************************************************************
*  Data structure declaration
***********************************************************************************************************************/
typedef struct _DS_TIME
{
    unsigned char   year   ;                                                  /* year                                         */
    unsigned char   month  ;                                                  /* month                                       */
    unsigned char   day    ;                                                  /* Date                                       */           
    unsigned char   hour   ;                                                  /* time                                         */
    unsigned char   minute ;                                                  /* points                                         */            
    unsigned char   second ;                                                  /* seconds                                         */
    unsigned char   week   ;                                                  /* week                                       */  
} _DS_TIME_T;

class DS1302Module
{
public:
    _DS_TIME_T DStime;                                                         /* Time structure year, month, day, hour, minute, second, week */

    /**********************************************************************************************************************
    *   Function name: DS_Set_Time
    *   Function description: Set DS1302 time (24-hour format)
    *   Parameter list: Global variable DStime
    *   Hardware output: None
    *   Return result: None
    **********************************************************************************************************************/
    void setTime(void );

    /**********************************************************************************************************************
    *   Function name: DS_Get_Time
    *   Function description: Read DS1302 time
    *   Parameter list: None
    *   Hardware output: None
    *   Return result: DStime / result: Determine whether DS1302 exists
    **********************************************************************************************************************/
    unsigned char getTime(void );
};

/***********************************************************************************************************************
*  Module hardware interface declaration
***********************************************************************************************************************/
#define DS1302_DAT  (0)    /* Multiplexing: Restore GPIO output after use, high level        */
#define DS1302_CE   (13)   /* Multiplexing: Restore GPIO output after use, low level        */ 
#define DS1302_CLK  (14)   /* Multiplexing: Restore GPIO output after use, low level        */


#define DS1302_CE__H   digitalWrite(DS1302_CE,  HIGH)                    /* Set CE = H                                 */ 
#define DS1302_CE__L   digitalWrite(DS1302_CE,  LOW)                     /* Set CE = L                                 */ 
#define DS1302_CLK_H   digitalWrite(DS1302_CLK, HIGH)                    /* Set CLK = H                                */ 
#define DS1302_CLK_L   digitalWrite(DS1302_CLK, LOW)                     /* Set CLK = L                                */ 
#define DS1302_DAT_H   digitalWrite(DS1302_DAT, HIGH)                    /* set DAT=H                                */ 
#define DS1302_DAT_L   digitalWrite(DS1302_DAT, LOW)                     /* set DAT=L                                */ 

#define  SET_DAT_OUTMODE   pinMode(DS1302_DAT,OUTPUT)                    /* Set DAT as output                             */
#define  SET_DAT_INPUTMODE  pinMode(DS1302_DAT,INPUT)                    /* Set DAT as input                             */
#define  GET_DAT digitalRead(DS1302_DAT)                                 /* Read DAT level status                            */

#define CLOCK_RATE 0x10                                                  /* Control communication clock rate                           */

/***********************************************************************************************************************
*  DS1302 register address definition 
***********************************************************************************************************************/
#define DS_READ   0x01                                                  /* Read operation                                     */
#define DS_WRITE  0x00                                                  /* write operation                                     */

#define DS_SECOND 0x80                                                  /* seconds                                         */
#define DS_MINUTE 0x82                                                  /* points                                         */
#define DS_HOUR   0x84                                                  /* time                                         */
#define DS_DATA   0x86                                                  /* Date                                       */
#define DS_MONTH  0x88                                                  /* month                                       */
#define DS_WEEK   0x8a                                                  /* week                                       */
#define DS_YEAR   0x8c                                                  /* year                                         */
#define DS_WP     0x8e                                                  /* write protect                                     */
#define DS_CHARGE 0x90                                                  /* charging                                       */

/***********************************************************************************************************************
* External function interface declaration 
***********************************************************************************************************************/
extern DS1302Module DS1302;

#endif //__DS1302_H__
/***********************************************************************************************************************
*  end of file
***********************************************************************************************************************/

