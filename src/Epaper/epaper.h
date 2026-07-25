#ifndef __EPAPER_H
#define __EPAPER_H 
#include <Arduino.h>
#include "stdlib.h"	
#include "../app_types.h"

/**********************************************************************************************************************
*  Hardware GPIO corresponding
**********************************************************************************************************************/
// //-----------------EINKPort definition---------------- 
// #define EINK_SCL_Clr() GPIO_ResetBit(GPIOA, GPIO_PIN15)//SCL
// #define EINK_SCL_Set() GPIO_SetBit( GPIOA, GPIO_PIN15)

// #define EINK_SDA_Clr() GPIO_ResetBit(GPIOA, GPIO_PIN14)//SDA
// #define EINK_SDA_Set() GPIO_SetBit( GPIOA, GPIO_PIN14)

// #define EINK_RES_Clr() GPIO_ResetBit(GPIOA, GPIO_PIN13)//RES
// #define EINK_RES_Set() GPIO_SetBit( GPIOA, GPIO_PIN13)

// #define EINK_DC_Clr()  GPIO_ResetBit(GPIOA, GPIO_PIN12)//DC
// #define EINK_DC_Set()  GPIO_SetBit( GPIOA, GPIO_PIN12)

// #define EINK_CS_Clr()  GPIO_ResetBit(GPIOA, GPIO_PIN9)//CS
// #define EINK_CS_Set()  GPIO_SetBit( GPIOA, GPIO_PIN9)

// #define EINK_BUSY()    GPIO_ReadInputbit(GPIOA,GPIO_PIN7)

/* Display related GPIO pins */
#define EINK_RST  (0)  /* Reuse */ 
#define EINK_DC   (1) 
#define EINK_CS   (2) 
#define EINK_BSY  (3) 
#define EINK_DAT  (13)  /* Reuse */ 
#define EINK_CLK  (14)  /* Reuse */ 

#define EINK_SCL_Clr() digitalWrite(EINK_CLK, LOW)//SCL
#define EINK_SCL_Set() digitalWrite(EINK_CLK, HIGH)

#define EINK_SDA_Clr() digitalWrite(EINK_DAT, LOW)//SDA
#define EINK_SDA_Set() digitalWrite(EINK_DAT, HIGH)

#define EINK_RES_Clr() digitalWrite(EINK_RST, LOW)//RES
#define EINK_RES_Set() digitalWrite(EINK_RST, HIGH)

#define EINK_DC_Clr()  digitalWrite(EINK_DC,  LOW)//DC
#define EINK_DC_Set()  digitalWrite(EINK_DC,  HIGH)

#define EINK_CS_Clr()  digitalWrite(EINK_CS,  LOW)//CS
#define EINK_CS_Set()  digitalWrite(EINK_CS,  HIGH)

#define EINK_BUSY()    digitalRead(EINK_BSY)


/**********************************************************************************************************************
*  Function parameter macro declaration
**********************************************************************************************************************/
/* Ink screen model list */
// #define EINK_MODEL_SSD1680_250X122    0     /* Zhongjingyuan   */
// #define EINK_MODEL_UD8251_250X122     1     /* Xinchuangtianyuan */
// #define EINK_MODEL_SSD1680_212X104    2     /* Xianyu Procurement */
// #define EINK_MODEL_PERVASIVE_212X104  3     /* Longting New Technology */

/* Ink screen interface language list */
// #define EINK_LANGUAGE_CHINESE  0
// #define EINK_LANGUAGE_ENGLISH  1

/****** Macro definitions related to the Ink Screen hardware platform********************************************************/
// #define EINK_MODEL          EINK_MODEL_SSD1680_212X104 

/****** Ink screen display interface language*************************************************************/
// #define EINK_UI_LANGUAGE  EINK_LANGUAGE_CHINESE

                              
/****** SSD1680 Zhongjingyuan ****************************************************/
#if EINK_MODEL== EINK_MODEL_SSD1680_250X122
  #define EINK_WIDTH         250
  #define EINK_HEIGH         122
  #define EINK_HEIGH_BYTE    16   /* Align height by bytes */

  #define EINK_BUSY_STATE    0x1  /* The value of the BUSY pin when in the BUSY state  */

  #define EINK_LAYER_BW     0x24  /* Layer 1 Data Transfer Command */
  #define EINK_LAYER_RE     0x26  /* Layer 2 Data Transfer Commands */

  #define BW_COLOR_BLACE    0x0   /* The black layer is ink text 0= black*/
  #define BW_COLOR_WHITE    0xff  /* The black layer is the ink text 1= white*/
  #define RE_COLOR_RED      0xff  /* The red layer is Yangwen 1= red*/
  #define RE_COLOR_EMPTY    0x0   /* The red layer is Yangwen 0= white*/
#endif

/****** UC8251 Xinchuangtianyuan **************************************************/
#if EINK_MODEL== EINK_MODEL_UD8251_250X122
  #define EINK_WIDTH         250
  #define EINK_HEIGH         122
  #define EINK_HEIGH_BYTE    16   /* Align height by bytes */  

  #define EINK_BUSY_STATE    0x0 /* The value of the BUSY pin when in the BUSY state  */

  #define EINK_LAYER_BW     0x10  /* Layer 1 Data Transfer Command */
  #define EINK_LAYER_RE     0x13  /* Layer 2 Data Transfer Commands */

  #define BW_COLOR_BLACE    0x0   /* The black layer is ink text 0= black*/
  #define BW_COLOR_WHITE    0xff  /* The black layer is the ink text 1= white*/
  #define RE_COLOR_RED      0xff  /* The red layer is Yangwen 1= red*/
  #define RE_COLOR_EMPTY    0x0   /* The red layer is Yangwen 0= white*/
#endif

/****** SSD1680 Xianyu Procurement ****************************************************/
#if EINK_MODEL== EINK_MODEL_SSD1680_212X104
  #define EINK_WIDTH         212
  #define EINK_HEIGH         104
  #define EINK_HEIGH_BYTE    13   /* Align height by bytes */

  #define EINK_BUSY_STATE    0x1  /* The value of the BUSY pin when in the BUSY state  */

  #define EINK_LAYER_BW     0x24  /* Layer 1 Data Transfer Command */
  #define EINK_LAYER_RE     0x26  /* Layer 2 Data Transfer Commands */

  #define BW_COLOR_BLACE    0x0   /* The black layer is ink text 0= black*/
  #define BW_COLOR_WHITE    0xff  /* The black layer is the ink text 1= white*/
  #define RE_COLOR_RED      0xff  /* The red layer is Yangwen 1= red*/
  #define RE_COLOR_EMPTY    0x0   /* The red layer is Yangwen 0= white*/
#endif

/****** UC8251 Longting New Technology **************************************************/
#if EINK_MODEL== EINK_MODEL_PERVASIVE_212X104
  #define EINK_WIDTH        212
  #define EINK_HEIGH        104
  #define EINK_HEIGH_BYTE    13   /* Align height by bytes */

  #define EINK_BUSY_STATE    0x0 /* The value of the BUSY pin when in the BUSY state  */

  #define EINK_LAYER_BW     0x10  /* Layer 1 Data Transfer Command */
  #define EINK_LAYER_RE     0x13  /* Layer 2 Data Transfer Commands */

  #define BW_COLOR_BLACE    0xff   /* The black layer is ink text 0= black*/
  #define BW_COLOR_WHITE    0x0  /* The black layer is the ink text 1= white*/
  #define RE_COLOR_RED      0xff  /* The red layer is Yangwen 1= red*/
  #define RE_COLOR_EMPTY    0x0   /* The red layer is Yangwen 0= white*/
#endif


/**** Software pictures display related macro definitions ****************************************************************/
#define EINK_STAGE_INIT     0x0   /* initialization phase */
#define EINK_STAGE_DISP     0x1   /* display stage   */

#define EINK_BWPART_REPLACE 0x00
#define EINK_BWPART_CLEAN   0x01
#define EINK_BWPART_SET     0x02

#define EINK_REPART_REPLACE 0x10
#define EINK_REPART_CLEAN   0x11
#define EINK_REPART_SET     0x12

#define ICON_BW_LAYER       0x00
#define ICON_RE_LAYER       0x10

#define WIFI_CONNECT_FAILED  0   /* WiFi connection failed */
#define WIFI_CONNECT_SUCCESS 1   /* WiFi connection successful */

/* Reset source recording options */
#define CLOCK_IC_EXIST_FALSE 0   /* Clock chip not found (DS1302)  */
#define CLOCK_IC_EXIST_TURE  1   /* There is a clock chip (DS1302)    */


/**** UI texture layout with different resolutions ****************************************************************/
#if (EINK_MODEL== EINK_MODEL_SSD1680_250X122)||(EINK_MODEL== EINK_MODEL_UD8251_250X122)
#define LPOW_ICON_X  182    /* Normal view low battery icon x coordinate  */
#define LPOW_ICON_Y   17    /* Normal view low battery icon y coordinate  */
#define LPOW_ICON_W   30    /* Low battery icon width (pixels)      */
#define LPOW_ICON_H    9    /* Low battery icon height (Byte)      */

#define WIFI_ICON_X   3    /* Normal view WiFi icon x coordinate   */
#define WIFI_ICON_Y 105    /* Normal view WiFi icon y coordinate   */
#define WIFI_ICON_W  22    /* WIFIIcon width (pixels)        */
#define WIFI_ICON_H   3    /* WIFIicon height(Byte)       */

#define CLOCK_ICON_X  35    /* Normal view WiFi icon x coordinate   */
#define CLOCK_ICON_Y 105    /* Normal view WiFi icon y coordinate   */
#define CLOCK_ICON_W  22    /* WIFIIcon width (pixels)        */
#define CLOCK_ICON_H   3    /* WIFIicon height(Byte)       */

#define TODAY_ICON_X 163   /* Normal view TODAY icon x coordinate  */
#define TODAY_ICON_Y 105   /* Normal view TODAY icon y coordinate  */
#define TODAY_ICON_W  48   /* TODAYIcon width (pixels)      */
#define TODAY_ICON_H   3   /* TODAYicon height(Byte)     */

#define WORK_ICON_X 211    /* Normal view WORK icon x coordinate  */
#define WORK_ICON_Y 105    /* Normal view WORK icon y coordinate  */
#define WORK_ICON_W  32    /* WORKIcon width (pixels)       */
#define WORK_ICON_H   3    /* WORKicon height(Byte)       */

#define SLEEP_ICON_X 211   /* Normal view SLEEP icon x coordinate */
#define SLEEP_ICON_Y 105   /* Normal view SLEEP icon y coordinate */
#define SLEEP_ICON_W  40   /* SLEEPIcon width (pixels)      */
#define SLEEP_ICON_H   3   /* SLEEPicon height(Byte)      */

#define DATE_BG_X   2      /* red date background x coordinate       */
#define DATE_BG_Y  17      /* Red date background y coordinate       */
#define DATE_BG_W 118      /* Red date background width (pixels)  */
#define DATE_BG_H  11      /* Red Date Background Height(Byte)  */

#define DATE_NUM_S_X  35      /* Red date single number x coordinate       */
#define DATE_NUM_S_Y  17      /* Red date single number y coordinate       */
#define DATE_NUM_S_W  52      /* Red date single number width (pixels)  */
#define DATE_NUM_S_H  11      /* Red date single number height (Byte)  */

#define DATE_NUM_D1_X  10     /* red date double digit x coordinate       */
#define DATE_NUM_D1_Y  17     /* red date double digit y coordinate       */
#define DATE_NUM_D1_W  52     /* Red date double digits width (pixels)  */
#define DATE_NUM_D1_H  11     /* Red date double digit height (Byte)  */

#define DATE_NUM_D2_X  63     /* red date double digit x coordinate       */
#define DATE_NUM_D2_Y  17     /* red date double digit y coordinate       */
#define DATE_NUM_D2_W  52     /* Red date double digits width (pixels)  */
#define DATE_NUM_D2_H  11     /* Red date double digit height (Byte)  */

#define WEATHER_ICON_Y      33     /* Weather icon Y coordinate*/
#define EINK_WEATHER_NIGHT  139    /* Night weather icon showing x starting location*/
#define EINK_WEATHER_DAY    199    /* Daytime weather icon showing x starting location*/

#define TEMPTURE_ICON_L_X   155    /* Temperature low temperature icon x coordinate*/
#define TEMPTURE_ICON_L_Y    89    /* Temperature low temperature icon y coordinate*/
#define TEMPTURE_ICON_H_X   194    /* Temperature high temperature icon x coordinate*/
#define TEMPTURE_ICON_H_Y    89    /* Temperature high temperature icon y coordinate*/
#define TEMPTURE_ICON_W      8     /* Temperature icon width (pixels)  */
#define TEMPTURE_ICON_H      2     /* Temperature icon altitude (Byte)  */

#if (EINK_UI_LANGUAGE ==EINK_LANGUAGE_CHINESE)
#define WEEK_ICON_X 106   /* normal view day of the week icon x coordinate */
#define WEEK_ICON_Y   1   /* Normal view week icon y coordinate */
#define WEEK_ICON_W  12   /* Day of the week icon width (pixels)      */
#define WEEK_ICON_H   2   /* Day icon height (Byte)      */

#define MONTH_ICON_X  48   /* Normal view monthly icon x coordinate */
#define MONTH_ICON_Y   1   /* Normal view monthly icon y coordinate */
#define MONTH_ICON_W   8   /* Monthly icon width (pixels)      */
#define NONTH_ICON_H   2   /* Monthly icon height (Byte)      */

#define YEAR_ICON_X    3   /* normal view year icon x coordinate */
#define YEAR_ICON_Y    1   /* Normal view year icon y coordinate */
#define YEAR_ICON_W    8   /* Year icon width (pixels)      */
#define YEAR_ICON_H    2   /* Year icon height (Byte)      */
#endif

#if (EINK_UI_LANGUAGE ==EINK_LANGUAGE_ENGLISH)
#define WEEK_ICON_X  84   /* normal view day of the week icon x coordinate */
#define WEEK_ICON_Y   1   /* Normal view week icon y coordinate */
#define WEEK_ICON_W  27   /* Day of the week icon width (pixels)      */
#define WEEK_ICON_H   2   /* Day icon height (Byte)      */

#define MONTH_ICON_X   5   /* Normal view monthly icon x coordinate */
#define MONTH_ICON_Y   1   /* Normal view monthly icon y coordinate */
#define MONTH_ICON_W  27   /* Monthly icon width (pixels)      */
#define NONTH_ICON_H   2   /* Monthly icon height (Byte)      */

#define YEAR_ICON_X   44   /* normal view year icon x coordinate */
#define YEAR_ICON_Y    1   /* Normal view year icon y coordinate */
#define YEAR_ICON_W    8   /* Year icon width (pixels)      */
#define YEAR_ICON_H    2   /* Year icon height (Byte)      */
#endif

#endif

/************** 212x104 Resolution material map layout *****************************************************/
#if (EINK_MODEL== EINK_MODEL_SSD1680_212X104)||(EINK_MODEL== EINK_MODEL_PERVASIVE_212X104)
#define LPOW_ICON_X  164   /* Normal view low battery icon x coordinate  */
#define LPOW_ICON_Y   17    /* Normal view low battery icon y coordinate  */
#define LPOW_ICON_W   23    /* Low battery icon width (pixels)      */
#define LPOW_ICON_H    8    /* Low battery icon height (Byte)      */

#define WIFI_ICON_X   1    /* Normal view WiFi icon x coordinate   */
#define WIFI_ICON_Y  89    /* Normal view WiFi icon y coordinate   */
#define WIFI_ICON_W  22    /* WIFIIcon width (pixels)        */
#define WIFI_ICON_H   2    /* WIFIicon height(Byte)       */

#define CLOCK_ICON_X  33    /* Normal view WiFi icon x coordinate   */
#define CLOCK_ICON_Y  89    /* Normal view WiFi icon y coordinate   */
#define CLOCK_ICON_W  22    /* WIFIIcon width (pixels)        */
#define CLOCK_ICON_H   2    /* WIFIicon height(Byte)       */

#define TODAY_ICON_X 125   /* Normal view TODAY icon x coordinate  */
#define TODAY_ICON_Y  89   /* Normal view TODAY icon y coordinate  */
#define TODAY_ICON_W  48   /* TODAYIcon width (pixels)      */
#define TODAY_ICON_H   2   /* TODAYicon height(Byte)     */

#define WORK_ICON_X  173    /* Normal view WORK icon x coordinate  */
#define WORK_ICON_Y  89    /* Normal view WORK icon y coordinate  */
#define WORK_ICON_W  32    /* WORKIcon width (pixels)       */
#define WORK_ICON_H   2    /* WORKicon height(Byte)       */

#define SLEEP_ICON_X 173   /* Normal view SLEEP icon x coordinate */
#define SLEEP_ICON_Y  89   /* Normal view SLEEP icon y coordinate */
#define SLEEP_ICON_W  40   /* SLEEPIcon width (pixels)      */
#define SLEEP_ICON_H   2   /* SLEEPicon height(Byte)      */

#define DATE_BG_X   2      /* red date background x coordinate       */
#define DATE_BG_Y  17      /* Red date background y coordinate       */
#define DATE_BG_W  84      /* Red date background width (pixels)  */
#define DATE_BG_H   9      /* Red Date Background Height(Byte)  */

#define DATE_NUM_S_X   26     /* Red date single number x coordinate       */
#define DATE_NUM_S_Y   17     /* Red date single number y coordinate       */
#define DATE_NUM_S_W   36     /* Red date single number width (pixels)  */
#define DATE_NUM_S_H   9      /* Red date single number height (Byte)  */

#define DATE_NUM_D1_X   6     /* red date double digit x coordinate       */
#define DATE_NUM_D1_Y  17     /* red date double digit y coordinate       */
#define DATE_NUM_D1_W  36     /* Red date double digits width (pixels)  */
#define DATE_NUM_D1_H   9     /* Red date double digit height (Byte)  */

#define DATE_NUM_D2_X  46     /* red date double digit x coordinate       */
#define DATE_NUM_D2_Y  17     /* red date double digit y coordinate       */
#define DATE_NUM_D2_W  36     /* Red date double digits width (pixels)  */
#define DATE_NUM_D2_H   9     /* Red date double digit height (Byte)  */

#define WEATHER_ICON_Y      25     /* Weather icon Y coordinate*/
#define EINK_WEATHER_NIGHT  101    /* Night weather icon showing x starting location*/
#define EINK_WEATHER_DAY    161    /* Daytime weather icon showing x starting location*/

#define TEMPTURE_ICON_L_X   117    /* Temperature low temperature icon x coordinate*/
#define TEMPTURE_ICON_L_Y    73    /* Temperature low temperature icon y coordinate*/
#define TEMPTURE_ICON_H_X   156    /* Temperature high temperature icon x coordinate*/
#define TEMPTURE_ICON_H_Y    73    /* Temperature high temperature icon y coordinate*/
#define TEMPTURE_ICON_W      8     /* Temperature icon width (pixels)  */
#define TEMPTURE_ICON_H      2     /* Temperature icon altitude (Byte)  */

#define WEEK_ICON_X  129   /* Normal view week icon x coordinate */
#define WEEK_ICON_Y    1   /* Normal view week icon y coordinate */
#define WEEK_ICON_W   12   /* Weekday icon width (pixels)      */
#define WEEK_ICON_H    2   /* Day icon height (Byte)      */

#define MONTH_ICON_X  51   /* Normal view monthly icon x coordinate */
#define MONTH_ICON_Y   1   /* Normal view monthly icon y coordinate */
#define MONTH_ICON_W   8   /* Monthly icon width (pixels)      */
#define NONTH_ICON_H   2   /* Monthly icon height (Byte)      */

#define YEAR_ICON_X    5   /* normal view year icon x coordinate */
#define YEAR_ICON_Y    1   /* Normal view year icon y coordinate */
#define YEAR_ICON_W    8   /* Year icon width (pixels)      */
#define YEAR_ICON_H    2   /* Year icon height (Byte)      */

#endif


/**********************************************************************************************************************
*   External function declaration
**********************************************************************************************************************/
// extern void Eink_UpdateRunning( _GLOBAL_MACHINE * machine, uint8_t wifi_success);
// extern void Eink_Disp_TimeOut(void);
// extern void Eink_Disp_Config(void);
// extern void Eink_Disp_LowPwr(void);
// extern void Eink_Disp_Clean(void);
// extern void EINK_sleep(void);

class EpaperModule
{
public:
    /**********************************************************************************************************************
    *  Global variable definition
    **********************************************************************************************************************/
    uint8_t  Eink_Cache[EINK_WIDTH][EINK_HEIGH_BYTE];            /* EINK_WIDTH *EINK_HEIGH_BYTE bytes of virtual video memory    */

    /**********************************************************************************************************************
    *   Function name: EINK_sleep
    *   Function description: The Ink Screen chip enters sleep mode (power saving). After exiting sleep, the settings need to be re-initialized.
    *   Parameter list: None 
    *   Hardware output: None
    *   Return result: None
    *   History: Author/Modifier Date Remarks
    *              whys 2018.6.29 Original creation version V1.0
    *
    **********************************************************************************************************************/
    void sleep(void);
    /**********************************************************************************************************************
    *   Function name: Eink_UpdateRunning
    *   Function description: Refresh the weather calendar display
    *   Parameter list: Global_State global variable top-level state
    *             wifi_success: Whether the WiFi update time is successful
    *                     \ref WiFi_CONNECT_FAILED Failed to connect and update time = red WiFi icon
    *                     \ref WiFi_CONNECT_SUCCESS Successfully connected and updated time = black WiFi icon
    *
    *   Hardware output: None
    *   Return result: None
    *   History: Author/Modifier Date Remarks
    *
    **********************************************************************************************************************/
    void updateRunning( _GLOBAL_MACHINE * machine, uint8_t wifi_success);
    /**********************************************************************************************************************
    *   Function name: Eink_Disp_TimeOut
    *   Function description: Display the setting timeout interface
    *   Parameter list: None
    *   Hardware output: None
    *   Return result: None
    *   History: Author/Modifier Date Remarks
    *
    **********************************************************************************************************************/
    void dispTimeOut(void);
    /**********************************************************************************************************************
    *   Function name: Eink_Disp_Config
    *   Function description: Display the setting interface
    *   Parameter list: None
    *   Hardware output: None
    *   Return result: None
    *   History: Author/Modifier Date Remarks
    *
    **********************************************************************************************************************/
    void dispConfig(void);
    /**********************************************************************************************************************
    *   Function name: Eink_Disp_LowPwr
    *   Function description: Display low battery interface
    *   Parameter list: None
    *   Hardware output: None
    *   Return result: None
    *   History: Author/Modifier Date Remarks
    *
    **********************************************************************************************************************/
    void dispLowPwr(void);
    /**********************************************************************************************************************
    *   Function name: Eink_Disp_Clean
    *   Function description: Refresh the screen to white
    *   Parameter list: None
    *   Hardware output: None
    *   Return result: None
    *   History: Author/Modifier Date Remarks
    *
    **********************************************************************************************************************/
    void dispClean(void);

private:
    /**********************************************************************************************************************
    *   Function name: EINK_GPIOInit
    *   Function description: GPIO initialization function related to MCU platform
    *   Parameter list: None
    *   Hardware output: None
    *   Return result: None
    *   History: Author/Modifier Date Remarks
    *
    **********************************************************************************************************************/
    void EINK_GPIOInit(void);
    /**********************************************************************************************************************
    *   Function name: EINK_GPIOdeInit
    *   Function description: Restore the reused IO port status to avoid affecting other functions
    *   Parameter list: None
    *   Hardware output: None
    *   Return result: None
    *   History: Author/Modifier Date Remarks
    *
    **********************************************************************************************************************/
    void EINK_GPIO_deInit(void);
    /**********************************************************************************************************************
    *   Function name: EINK_WriteDATA
    *   Function description: Pass 1 byte data through GPIO simulation SPI bus
    *   Parameter list: dat: data passed through the SPI bus
    *   Hardware output: None
    *   Return result: None
    *   History: Author/Modifier Date Remarks
    *
    **********************************************************************************************************************/
    void EINK_WriteDATA(uint8_t dat);
    /**********************************************************************************************************************
    *   Function name: EINK_WriteCMD
    *   Function description: Write a register command to SPI
    *   Parameter list: reg: register address
    *   Hardware output: None
    *   Return result: None
    *   History: Author/Modifier Date Remarks
    *
    **********************************************************************************************************************/
    void EINK_WriteCMD(uint8_t reg);
    /**********************************************************************************************************************
    *   Function name: Epaper_WaitBusy
    *   Function description: Read the BUSY pin status to determine whether the operation is completed
    *   Parameter list: timeOutms sets the timeout time in ms 0 means infinite waiting
    *   Hardware output: None
    *   Return result: None
    *   History: Author/Modifier Date Remarks
    *
    **********************************************************************************************************************/
    void Epaper_WaitBusy(uint32_t timeOutms);
    /**********************************************************************************************************************
    *   Function name: EINK_GUIInit
    *   Function description: Initialize screen hardware IO and internal related registers
    *   Parameter list: None
    *   Hardware output: None
    *   Return result: None
    *   History: Author/Modifier Date Remarks
    *
    **********************************************************************************************************************/
    void EINK_GUIInit(void);
    /**********************************************************************************************************************
    *   Function name: EINK_Fresh
    *   Function description: Data is updated to the display screen
    *   Parameter list: Cache: image data source
    *                 layer: Which layer the data is filled into
    *                        EINK_LAYER_BW black and white layer
    *                        EINK_LAYER_RE red and white layer 
    *                 stage: image data source
    *                        EINK_STAGE_INIT needs to initialize the interface stage
    *                        EINK_STAGE_DISP The final displayed stage
    * EINK_STAGE_INIT
    *   Hardware output: None
    *   Return result: None
    *   History: Author/Modifier Date Remarks
    *
    **********************************************************************************************************************/
    void EINK_Fresh(uint8_t *Cache, uint8_t layer, uint8_t stage);
    /**********************************************************************************************************************
    *   Function name: Eink_BWadd_pictur
    *   Function description: Integrate a sub-picture on the black layer into virtual video memory
    *   Parameter list: x pixel coordinate column subscript starts from 1
    *              y must be an integer of 8, indexed starting from 1
    *              part_w image width in pixels
    *              part_hbytes image height in bytes (an integer multiple of 8 pixels)
    *                 part: image data source
    *               method: filling method
    *                       EINK_BWPART_REPLACE replace
    *                       EINK_BWPART_CLEAN Clear
    *                       EINK_BWPART_SET set
    *   Hardware output: None
    *   Return result: None
    *   History: Author/Modifier Date Remarks
    *
    **********************************************************************************************************************/
    void Eink_BWadd_picture(uint16_t y, uint16_t x, uint16_t part_w, uint16_t part_hbytes, const uint8_t *part, uint8_t method);
    /**********************************************************************************************************************
    *   Function name: Eink_REadd_picture
    *   Function description: Integrate a sub-picture into virtual video memory on the red layer
    *   Parameter list: x pixel coordinate column subscript starts from 1
    *              y must be an integer of 8, indexed starting from 1
    *              part_w image width in pixels
    *              part_hbytes image height in bytes (an integer multiple of 8 pixels)
    *                 part: image data source
    *               method: filling method
    *                       EINK_REPART_REPLACE replace
    *                       EINK_REPART_CLEAN Clear
    *                       EINK_REPART_SET set
    *   Hardware output: None
    *   Return result: None
    *   History: Author/Modifier Date Remarks
    *
    **********************************************************************************************************************/
    void Eink_REadd_picture(uint16_t y, uint16_t x, uint16_t part_w, uint16_t part_hbytes, const uint8_t *part, uint8_t method);
    /**********************************************************************************************************************
    *   Function name: Eink_fill_rectangular
    *   Function description: Fill a rectangle in virtual video memory
    *   Parameter list: x pixel coordinate column subscript starts from 1
    *              y must be an integer of 8, indexed starting from 1
    *              part_w image width in pixels
    *              part_hbytes image height in bytes (an integer multiple of 8 pixels)
    *                 part: image data source
    *                color: 
    *                     BW_COLOR_WHITE black layer is ink text 1= white
    *                     BW_COLOR_BLACE black layer is black text 0= black
    *                     RE_COLOR_EMPTY The red layer is Yangwen 0= white
    *                     RE_COLOR_RED The red layer is Yangwen 1= red
    *   Hardware output: None
    *   Return result: None
    *   History: Author/Modifier Date Remarks
    *
    **********************************************************************************************************************/
    void Eink_fill_rectangular(uint16_t y, uint16_t x, uint16_t part_w, uint16_t part_hbytes,  uint8_t color);
    /**********************************************************************************************************************
    *   Function name: Eink_FillWeather_Ico
    *   Function description: Populate icons based on weather codes
    *   Parameter list:     wcode: Weather code, see https://seniverse.yuque.com/hyper_data/api_v3/yev2c3
    *             day_night: EINK_WEATHER_NIGHT Night weather icon shows x starting position
    *                        EINK_WEATHER_DAY Daytime weather icon shows x starting location
    *                 layer: Which layer the data is filled into
    *                        EINK_LAYER_BW black and white layer
    *                        EINK_LAYER_RE red and white layer 
    *   Hardware output: None
    *   Return result: None
    *   History: Author/Modifier Date Remarks
    *
    **********************************************************************************************************************/
    void Eink_FillWeather_Icom( uint8_t wcode, uint8_t day_night, uint8_t layer );
};

extern EpaperModule Epaper;

#endif // __EPAPER_H 

/**********************************************************************************************************************
*   end of file
**********************************************************************************************************************/

