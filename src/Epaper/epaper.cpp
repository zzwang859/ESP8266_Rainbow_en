#include <Arduino.h>
#include <SPI.h>
#include <string.h>
#include "epaper.h"
#include "epaper_bmp.h"   /* Overall picture material     */
#include "weather_icon.h" /* Run interface picture material */
#include "../moto/moto.h"

/**********************************************************************************************************************
*  Global variable definition
**********************************************************************************************************************/
EpaperModule Epaper;

/**********************************************************************************************************************
*   Function name: EINK_GPIOInit
*   Function description: GPIO initialization function related to MCU platform
*   Parameter list: None
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*
**********************************************************************************************************************/
void EpaperModule::EINK_GPIOInit(void)
{   
    /* SPI CLK DAT uses IP */
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.setDataMode(SPI_MODE0);

    /*Keep GPIO12 GPIO15 at 0, does not affect the motor */
    pinMode(MOTO_C,OUTPUT);
    pinMode(MOTO_D,OUTPUT);  

    digitalWrite(MOTO_C, LOW);
    digitalWrite(MOTO_D, LOW);  

    /* Other screen interfaces use GPIO */
    pinMode(EINK_RST,OUTPUT);
    pinMode(EINK_DC,OUTPUT);
    pinMode(EINK_CS,OUTPUT);
    pinMode(EINK_BSY,INPUT); 

    digitalWrite(EINK_DC,  HIGH);
    digitalWrite(EINK_CS,  HIGH);
    digitalWrite(EINK_RST, HIGH);

    delay(100);
}

/**********************************************************************************************************************
*   Function name: EINK_GPIOdeInit
*   Function description: Restore the reused IO port status to avoid affecting other functions
*   Parameter list: None
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*
**********************************************************************************************************************/
void EpaperModule::EINK_GPIO_deInit(void)
{   
    SPI.end();                                                                           /* initialization               */
    Serial.begin(115200);                              /* If the screen is not refreshed, the serial port will not be initialized and there will be no output.    */

    pinMode(EINK_RST,OUTPUT);
    //pinMode(EINK_CS,OUTPUT);
    pinMode(EINK_DAT,OUTPUT); 
    pinMode(EINK_CLK,OUTPUT); 

    digitalWrite(EINK_RST, HIGH);
    //digitalWrite(EINK_CS,  HIGH);
    digitalWrite(EINK_DAT, LOW); 
    digitalWrite(EINK_CLK, LOW); 

   /*Keep GPIO12 GPIO15 at 0, does not affect the motor */
    pinMode(MOTO_C,OUTPUT);
    pinMode(MOTO_D,OUTPUT);  

    digitalWrite(MOTO_C, LOW);
    digitalWrite(MOTO_D, LOW);  
}

/**********************************************************************************************************************
*   Function name: EINK_WriteDATA
*   Function description: Pass 1 byte data through GPIO simulation SPI bus
*   Parameter list: dat: data passed through the SPI bus
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*
**********************************************************************************************************************/
void EpaperModule::EINK_WriteDATA(uint8_t dat)  
{
	EINK_CS_Clr();
  EINK_DC_Set();
  SPI.transfer(dat);
	EINK_CS_Set();
  EINK_DC_Set();  
}

/**********************************************************************************************************************
*   Function name: EINK_WriteCMD
*   Function description: Write a register command to SPI
*   Parameter list: reg: register address
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*
**********************************************************************************************************************/
void EpaperModule::EINK_WriteCMD(uint8_t reg)  
{
    EINK_CS_Clr();
    EINK_DC_Clr();
    SPI.transfer(reg);  
    EINK_CS_Set(); 
    EINK_DC_Set();     
}

/**********************************************************************************************************************
*   Function name: Epaper_WaitBusy
*   Function description: Read the BUSY pin status to determine whether the operation is completed
*   Parameter list: timeOutms sets the timeout time in ms 0 means infinite waiting
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*
**********************************************************************************************************************/
void EpaperModule::Epaper_WaitBusy(uint32_t timeOutms)
{
    uint32_t start_time = 0;

    pinMode(EINK_BSY,INPUT);                /* Set the BSY pin as input IO */

    start_time  = millis();                 /* start timestamp          */
    while(EINK_BUSY()== EINK_BUSY_STATE)
    {
        yield();                             /* Clear watchdog to prevent stuck */         
        
        if(timeOutms > 0) {
          if(millis() - start_time  > timeOutms){
              break;
          } 
        }      
    }
}

/**********************************************************************************************************************
*   Function name: EINK_GUIInit
*   Function description: Initialize screen hardware IO and internal related registers
*   Parameter list: None
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*
**********************************************************************************************************************/
void EpaperModule::EINK_GUIInit(void)
{
  
  EINK_GPIOInit();
  EINK_RES_Clr();     // Module reset      
  delay(20);       //At least 10ms delay 
  EINK_RES_Set(); 
  delay(20);       //At least 10ms delay  
    
  Epaper_WaitBusy(2e3);  

#if (EINK_MODEL== EINK_MODEL_SSD1680_250X122)||(EINK_MODEL== EINK_MODEL_SSD1680_212X104)
  EINK_WriteCMD(0x12);  //SWRESET
  Epaper_WaitBusy(100);   
    
  EINK_WriteCMD(0x01);  //Driver output control      
  EINK_WriteDATA(0x27);
  EINK_WriteDATA(0x01);
  EINK_WriteDATA(0x01);
	
  EINK_WriteCMD(0x11);   //data entry mode       
  EINK_WriteDATA(0x01);

#if EINK_MODEL== EINK_MODEL_SSD1680_250X122
  EINK_WriteCMD(0x44);   //set Ram-X address start/end position   
  EINK_WriteDATA(0x00);
  EINK_WriteDATA(0x0F);   //0x0F-->(15+1)*8=128

  EINK_WriteCMD(0x45);   //set Ram-Y address start/end position          
  EINK_WriteDATA(0xF9);   //0xF9-->(249+1)=250
  EINK_WriteDATA(0x00);
  EINK_WriteDATA(0x00);
  EINK_WriteDATA(0x00); 
#endif

#if EINK_MODEL== EINK_MODEL_SSD1680_212X104
  EINK_WriteCMD(0x44);   //set Ram-X address start/end position   
  EINK_WriteDATA(0x00);
  EINK_WriteDATA(0x0C);   //0x0F-->(12+1)*8=104

  EINK_WriteCMD(0x45);   //set Ram-Y address start/end position          
  EINK_WriteDATA(0xD3);   //0xD3-->(211+1)=212
  EINK_WriteDATA(0x00);
  EINK_WriteDATA(0x00);
  EINK_WriteDATA(0x00); 
#endif

  EINK_WriteCMD(0x3C);   //BorderWavefrom
  EINK_WriteDATA(0x05);       
  
  EINK_WriteCMD(0x21);   //  Display update control
  EINK_WriteDATA(0x00);  
  EINK_WriteDATA(0x80);  

  EINK_WriteCMD(0x4E);   // set RAM x address count to 0;
  EINK_WriteDATA(0x00);
  EINK_WriteCMD(0x4F);   // set RAM y address count to 0X199;    
  EINK_WriteDATA(0x27);
  EINK_WriteDATA(0x01); 
#endif

#if EINK_MODEL== EINK_MODEL_UD8251_250X122
  EINK_WriteCMD(0x00); //Driver output control UC8251D
	EINK_WriteDATA(0x8F);   
#endif	

  Epaper_WaitBusy(2e3);	
}

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
void EpaperModule::EINK_Fresh(uint8_t *Cache, uint8_t layer, uint8_t stage)
{
  uint16_t i,j;
  
  if (stage == EINK_STAGE_INIT){                                                   /* If it is a stage that includes initialization          */
    EINK_GUIInit();
  }

  EINK_WriteCMD(layer);                                                            /* Refresh data in virtual memory to the display  */
  for(i= 0; i<EINK_WIDTH; i++) {
      for(j=0; j<EINK_HEIGH_BYTE; j++) {
        EINK_WriteDATA(*(Cache + i*EINK_HEIGH_BYTE + j));
      }   
  }

  if (stage == EINK_STAGE_DISP){                                                   /* If it reaches the stage of enabling display       */

#if (EINK_MODEL==EINK_MODEL_SSD1680_250X122) || (EINK_MODEL== EINK_MODEL_SSD1680_212X104)
    EINK_WriteCMD(0x18);                                                           /* Read built-in temperature sensor */
    EINK_WriteDATA(0x80); 

    EINK_WriteCMD(0x22);  //Load waveform LUT from OTP 
    EINK_WriteDATA(0xB1);     
    EINK_WriteCMD(0x20);  //Activate Display Update Sequence
    Epaper_WaitBusy(100);  

    EINK_WriteCMD(0x22); //Display Update Control
    EINK_WriteDATA(0xF7);     
    EINK_WriteCMD(0x20);  //Activate Display Update Sequence
    Epaper_WaitBusy(100);   
#endif  

#if EINK_MODEL==EINK_MODEL_UD8251_250X122
    EINK_WriteCMD (0x04);     // Power on DC/DC
    Epaper_WaitBusy(20e3);
    
    EINK_WriteCMD (0x12);     // Refresh
    Epaper_WaitBusy(20e3);         
#endif
  }
}

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
void EpaperModule::sleep(void)
{
    EINK_GPIOInit();                                                                            /* Reconfigure IO        */
    Epaper_WaitBusy(30e3);                                                                      /* Wait up to 30 seconds      */
    
#if (EINK_MODEL== EINK_MODEL_SSD1680_250X122 ) || (EINK_MODEL== EINK_MODEL_SSD1680_212X104)     /* Zhongjingyuan Company Screen & Xianyu Procurement Screen   */
    EINK_WriteCMD(0x10);     //deep sleep
    EINK_WriteDATA(0x01); 
#endif 
    
#if EINK_MODEL== EINK_MODEL_UD8251_250X122                                                      /* Xinchuangtianyuan company screen */
    EINK_WriteCMD(0x02);     // Turn-off DC/DC
    Epaper_WaitBusy(100);  

    EINK_WriteCMD(0x07);      //deep sleep
    EINK_WriteDATA(0xA5); 
#endif
    
#if EINK_MODEL==  EINK_MODEL_PERVASIVE_212X104                                                /* Longting New Technology Company Screen */
    EINK_WriteCMD (0x07);            //deep sleep
    EINK_WriteDATAA(0xA5);
#endif  
    EINK_GPIO_deInit();                                         /* Release IO port      */
}


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
void EpaperModule::Eink_BWadd_picture(uint16_t y, uint16_t x, uint16_t part_w, uint16_t part_hbytes, const uint8_t *part, uint8_t method)
{
    uint8_t h, w;
   
    y = y-1;
    y=y>>3;
    
    switch (method) 
    {
        case EINK_BWPART_CLEAN:
            for(w=0; w< part_w; w++){
                for(h=0; h< part_hbytes; h++) { 
                    Eink_Cache[EINK_WIDTH- x - w][ y + h] |= ~*(part+w*part_hbytes + h);
                }
            }            
            break;
        case EINK_BWPART_SET:
            for(w=0; w< part_w; w++){
                for(h=0; h< part_hbytes; h++) { 
                    Eink_Cache[EINK_WIDTH- x - w][ y + h] &= *(part+w*part_hbytes + h);
                }
            }            
            break;
        case EINK_BWPART_REPLACE :                   
            for(w=0; w< part_w; w++){
                for(h=0; h< part_hbytes; h++) { 
                    Eink_Cache[EINK_WIDTH- x - w][ y + h] = *(part+w*part_hbytes + h);
                }
            }
            break;
        default:   
        break;
    }
}

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
void EpaperModule::Eink_REadd_picture(uint16_t y, uint16_t x, uint16_t part_w, uint16_t part_hbytes, const uint8_t *part, uint8_t method)
{
    uint8_t h, w;
   
    y = y-1;
    y=y>>3;
    
    switch (method) 
    {
        case EINK_REPART_CLEAN:
            for(w=0; w< part_w; w++){
                for(h=0; h< part_hbytes; h++) { 
                    Eink_Cache[EINK_WIDTH- x - w][ y + h] &= *(part+w*part_hbytes + h);
                }
            }            
            break;
        case EINK_REPART_SET:
            for(w=0; w< part_w; w++){
                for(h=0; h< part_hbytes; h++) { 
                    Eink_Cache[EINK_WIDTH- x - w][ y + h] |= ~*(part+w*part_hbytes + h);
                }
            }            
            break;
        case EINK_REPART_REPLACE :                      
            for(w=0; w< part_w; w++){
                for(h=0; h< part_hbytes; h++) { 
                    Eink_Cache[EINK_WIDTH- x - w][ y + h] = *(part+w*part_hbytes + h);
                }
            }
            break;
        default:
        break;
    }
}

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
void EpaperModule::Eink_fill_rectangular(uint16_t y, uint16_t x, uint16_t part_w, uint16_t part_hbytes,  uint8_t color)
{
    uint8_t h, w;  
    y = y-1;
    y=y>>3;
    
    for(w=0; w< part_w; w++){
        for(h=0; h< part_hbytes; h++) { 
            Eink_Cache[EINK_WIDTH- x - w][ y + h] = color;
        }
    }            
}

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
void EpaperModule::Eink_FillWeather_Icom( uint8_t wcode, uint8_t day_night, uint8_t layer ) {

    uint8_t method = EINK_BWPART_SET| layer;
    
    if(wcode == 0 || wcode ==2){      
        Eink_REadd_picture(WEATHER_ICON_Y, day_night, 40, 5, (uint8_t *)&icon_whole[0][0], method);                  /* Filled red daytime sunny icon    */  

    } else if(wcode == 1 || wcode == 3) {
        Eink_BWadd_picture(WEATHER_ICON_Y, day_night, 40, 5, (uint8_t *)&icon_whole[1][0], method);                  /* filled black night sunny icon   */  
        
    } else if ( wcode == 4) {   
        Eink_BWadd_picture(WEATHER_ICON_Y, day_night, 40, 5, (uint8_t *)&icon_whole[2][0], method);                  /* filled black night sunny icon   */  

    } else if( wcode == 5 || wcode ==7){
        Eink_BWadd_picture(WEATHER_ICON_Y, day_night, 40, 5, (uint8_t *)&icon_whole[3][0], method);                  /* filled black cloud icon        */  
        if(wcode == 5) {
            Eink_REadd_picture(WEATHER_ICON_Y, day_night, 40, 4, (uint8_t *)&icon_top[0][0], method);                /* fill red sun rays        */  
        } else {
            Eink_REadd_picture(WEATHER_ICON_Y, day_night, 40, 4, (uint8_t *)&icon_top[1][0], method);                /* fill red sun            */  
        }        
        
    } else if ( wcode == 6 || wcode == 8) {   
        Eink_BWadd_picture(WEATHER_ICON_Y, day_night, 40, 5, (uint8_t *)&icon_whole[4][0], method);                  /* fill black icon    */  

    } else if ( wcode == 9) {   
        Eink_BWadd_picture(WEATHER_ICON_Y, day_night, 40, 4, (uint8_t *)&icon_top[4][0], method);                    /* fill cloudy days         */  
        
    } else if ( wcode >25 && wcode <30) {   
        Eink_BWadd_picture(WEATHER_ICON_Y, day_night, 40, 5, (uint8_t *)&icon_whole[5][0], method);                  /* fill black icon    */  
        
    } else if ( wcode == 30) {   
        Eink_BWadd_picture(WEATHER_ICON_Y, day_night, 40, 5, (uint8_t *)&icon_whole[6][0], method);                  /* fill black icon    */  
        
    } else if ( wcode == 31) {   
        Eink_BWadd_picture(WEATHER_ICON_Y, day_night, 40, 5, (uint8_t *)&icon_whole[7][0], method);                  /* fill black icon    */  
        
    } else if (wcode ==10 || wcode ==21) {
        Eink_BWadd_picture(WEATHER_ICON_Y, day_night, 40, 4, (uint8_t *)&icon_top[2][0], method);                    /* fill cloudy         */  
        if(wcode == 10) { 
            Eink_BWadd_picture(WEATHER_ICON_Y+24, day_night, 40, 2, (uint8_t *)&icon_bot[0][0], method);                /* fill shower         */  
        } else {
            Eink_BWadd_picture(WEATHER_ICON_Y+24, day_night, 40, 2, (uint8_t *)&icon_bot[11][0], method);               /* Filling snow showers         */  
        }

    } else if (wcode ==11 || wcode == 12) {
        Eink_BWadd_picture(WEATHER_ICON_Y, day_night, 40, 4, (uint8_t *)&icon_top[3][0], method);                    /* fill in white clouds         */ 
        Eink_BWadd_picture(WEATHER_ICON_Y+24, day_night, 40, 2, (uint8_t *)&icon_bot[wcode-10][0], method);          /* Filling thundershowers/hail   */  

    } else if(wcode>12&&wcode<21) {
        Eink_BWadd_picture(WEATHER_ICON_Y, day_night, 40, 4, (uint8_t *)&icon_top[4][0], method);                    /* fill in the dark clouds         */ 
        Eink_BWadd_picture(WEATHER_ICON_Y+24, day_night, 40, 2, (uint8_t *)&icon_bot[wcode-10][0], method);             /* padding             */  

    } else if(wcode>31&&wcode<37) {                                                                                  /* fill wind           */
        Eink_BWadd_picture(WEATHER_ICON_Y, day_night, 40, 5, (uint8_t *)&icon_whole[8][0], method);                  /* fill black icon    */  

    } else if(wcode>21&&wcode<26) {
        Eink_BWadd_picture(WEATHER_ICON_Y, day_night, 40, 4, (uint8_t *)&icon_top[4][0], method);                    /* fill in the dark clouds         */ 
        Eink_BWadd_picture(WEATHER_ICON_Y+24, day_night, 40, 2, (uint8_t *)&icon_bot[wcode-10][0], method);             /* padding             */  

    } else {   /* Other undefined codes are uniformly displayed as N/A.*/

        Eink_BWadd_picture(WEATHER_ICON_Y+8, day_night+2, 36, 3,  icon_none,  method);                                 /* FillN/A         */ 
    }
}

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
void EpaperModule::updateRunning( _GLOBAL_MACHINE * machine, uint8_t wifi_success )
{
    int8_t high, low;
    
    /* ************************ Fill black layer *****************************************/
    memset(    (uint8_t *)&Eink_Cache[0][0], BW_COLOR_WHITE, EINK_WIDTH *EINK_HEIGH_BYTE );                    /* Clear the black and white layer         */
    memcpy(    (uint8_t *)&Eink_Cache[0][0],(uint8_t *)&img_runing[0][0], EINK_WIDTH *EINK_HEIGH_BYTE );       /* Copy display content         */

    if(wifi_success == WIFI_CONNECT_SUCCESS) {
        Eink_BWadd_picture(WIFI_ICON_Y,   WIFI_ICON_X, WIFI_ICON_W, WIFI_ICON_H,  part_wifi, EINK_BWPART_SET);       /* Filled black wifi icon    */  
    }

    if(machine->ds1302 == CLOCK_IC_EXIST_TURE) {
        Eink_BWadd_picture(CLOCK_ICON_Y,   CLOCK_ICON_X, CLOCK_ICON_W, CLOCK_ICON_H,  part_clock, EINK_BWPART_SET);  /* Filled black clock icon    */ 
    }

    Eink_BWadd_picture(TODAY_ICON_Y, TODAY_ICON_X, TODAY_ICON_W, TODAY_ICON_H, part_today, EINK_BWPART_SET);         /* Fill black Today:       */   
    /* weather is sunny or cloudy      */
    if(machine->weatherToday.day < WEATHER_M_OVERCAST) {    
        Eink_BWadd_picture(WORK_ICON_Y, WORK_ICON_X, WORK_ICON_W, WORK_ICON_H,  part_work, EINK_BWPART_SET);         /* Fill black Work         */  

    } else {
        Eink_BWadd_picture(SLEEP_ICON_Y, SLEEP_ICON_X, SLEEP_ICON_W , SLEEP_ICON_H, part_sleep, EINK_BWPART_SET);    /* fill black sleep        */     
    }

    /* Fill in year and month */
    Eink_BWadd_picture(YEAR_ICON_Y, YEAR_ICON_X,    YEAR_ICON_W, YEAR_ICON_H, (const uint8_t*)&part_litNum[(uint16_t)(machine->year/1000)  ][0],  EINK_BWPART_CLEAN);      /* fill black year        */  
    Eink_BWadd_picture(YEAR_ICON_Y, YEAR_ICON_X+8,  YEAR_ICON_W, YEAR_ICON_H, (const uint8_t*)&part_litNum[(uint16_t)(machine->year/100)%10][0],  EINK_BWPART_CLEAN);      /* fill black year        */  
    Eink_BWadd_picture(YEAR_ICON_Y, YEAR_ICON_X+16, YEAR_ICON_W, YEAR_ICON_H, (const uint8_t*)&part_litNum[(uint16_t)(machine->year/10)%10 ][0],  EINK_BWPART_CLEAN);      /* fill black year        */  
    Eink_BWadd_picture(YEAR_ICON_Y, YEAR_ICON_X+24, YEAR_ICON_W, YEAR_ICON_H, (const uint8_t*)&part_litNum[(uint16_t)(machine->year%10)    ][0],  EINK_BWPART_CLEAN);      /* fill black year        */  
    
#if (EINK_UI_LANGUAGE  == EINK_LANGUAGE_CHINESE )
    if (machine->month < 10){ /* Numbers less than 10 are centered */
        Eink_BWadd_picture(MONTH_ICON_Y, MONTH_ICON_X+5, MONTH_ICON_W, NONTH_ICON_H, (const uint8_t*)&part_litNum[machine->month][0],                  EINK_BWPART_CLEAN);    /* fill black month        */  
    } else {        /* Numbers greater than 10 are displayed in two digits*/
        Eink_BWadd_picture(MONTH_ICON_Y, MONTH_ICON_X , MONTH_ICON_W, NONTH_ICON_H, (const uint8_t*)&part_litNum[(uint16_t)(machine->month/10) ][0],  EINK_BWPART_CLEAN);     /* fill black month        */  
        Eink_BWadd_picture(MONTH_ICON_Y, MONTH_ICON_X+8, MONTH_ICON_W, NONTH_ICON_H, (const uint8_t*)&part_litNum[machine->month%10             ][0],  EINK_BWPART_CLEAN);    /* fill black month        */  
    }
#endif

#if (EINK_UI_LANGUAGE  == EINK_LANGUAGE_ENGLISH)
        Eink_BWadd_picture(MONTH_ICON_Y, MONTH_ICON_X, MONTH_ICON_W, NONTH_ICON_H, (const uint8_t*)&part_month[machine->month-1][0],                  EINK_BWPART_CLEAN);    /* fill black month          */
#endif

    /* fill week */  
    Eink_BWadd_picture(WEEK_ICON_Y, WEEK_ICON_X, WEEK_ICON_W, WEEK_ICON_H, (const uint8_t*)&part_weekday[machine->weekday%7 ][0],  EINK_BWPART_CLEAN);                       /* fill black day of the week        */   

    /* filling temperature */
    low = machine->weatherToday.low;
    if(low < 0){
        Eink_BWadd_picture(TEMPTURE_ICON_L_Y, TEMPTURE_ICON_L_X, TEMPTURE_ICON_W, TEMPTURE_ICON_H, (const uint8_t*)&part_litNum[13][0],  EINK_BWPART_SET);                   /* fill black temperature        */    
        low = -low;
    } 
    Eink_BWadd_picture(TEMPTURE_ICON_L_Y, TEMPTURE_ICON_L_X+8, TEMPTURE_ICON_W, TEMPTURE_ICON_H, (const uint8_t*)&part_litNum[(uint8_t)(low/10)][0],  EINK_BWPART_SET);      /* fill black temperature        */  
    Eink_BWadd_picture(TEMPTURE_ICON_L_Y, TEMPTURE_ICON_L_X+16, TEMPTURE_ICON_W, TEMPTURE_ICON_H, (const uint8_t*)&part_litNum[low%10][0],  EINK_BWPART_SET);                /* fill black temperature        */  

    high = low = machine->weatherToday.high;
    if(high < 0){
        Eink_BWadd_picture(TEMPTURE_ICON_H_Y, TEMPTURE_ICON_H_X, TEMPTURE_ICON_W, TEMPTURE_ICON_H, (const uint8_t*)&part_litNum[13][0],  EINK_BWPART_SET);                   /* fill black temperature        */    
        high = -high;
    } 
    Eink_BWadd_picture(TEMPTURE_ICON_H_Y, TEMPTURE_ICON_H_X+8, TEMPTURE_ICON_W, TEMPTURE_ICON_H,  (const uint8_t*)&part_litNum[(uint8_t)(high/10)][0],  EINK_BWPART_SET);    /* fill black temperature        */  
    Eink_BWadd_picture(TEMPTURE_ICON_H_Y, TEMPTURE_ICON_H_X+16, TEMPTURE_ICON_W, TEMPTURE_ICON_H,  (const uint8_t*)&part_litNum[high%10][0],  EINK_BWPART_SET);              /* fill black temperature        */ 
                    
    /* Update black layer weather icon */
    Eink_FillWeather_Icom( machine->weatherToday.day,     EINK_WEATHER_DAY, ICON_BW_LAYER) ;
    Eink_FillWeather_Icom( machine->weatherToday.night, EINK_WEATHER_NIGHT, ICON_BW_LAYER) ;


    EINK_Fresh((uint8_t *)&Eink_Cache[0][0], EINK_LAYER_BW, EINK_STAGE_INIT);          /* Update the black and white layer and initialize it */

    /* ************************ Fill red layer *****************************************/
    memset(    (uint8_t *)&Eink_Cache[0][0], RE_COLOR_EMPTY, EINK_WIDTH *EINK_HEIGH_BYTE );                        /* Clear the red layer          */   
    Eink_fill_rectangular(DATE_BG_Y, DATE_BG_X , DATE_BG_W, DATE_BG_H ,  RE_COLOR_RED);                            /* fill red area          */  

    if(machine->day <10) {
        Eink_REadd_picture(DATE_NUM_S_Y , DATE_NUM_S_X , DATE_NUM_S_W , DATE_NUM_S_H, (const uint8_t*)&part_bigNum[machine->day][0]  , EINK_REPART_CLEAN);                 /* Fill in calendar numbers          */    
    } else {
        Eink_REadd_picture(DATE_NUM_D1_Y, DATE_NUM_D1_X, DATE_NUM_D1_W, DATE_NUM_D1_H, (const uint8_t*)&part_bigNum[(uint8_t)(machine->day/10)][0]  , EINK_REPART_CLEAN);  /* Fill in calendar numbers          */ 
        Eink_REadd_picture(DATE_NUM_D2_Y, DATE_NUM_D2_X, DATE_NUM_D2_W, DATE_NUM_D2_H, (const uint8_t*)&part_bigNum[          machine->day%10 ][0]  , EINK_REPART_CLEAN);  /* Fill in calendar numbers          */    
    }

    if(wifi_success == WIFI_CONNECT_FAILED) {
        Eink_REadd_picture(WIFI_ICON_Y,   WIFI_ICON_X, WIFI_ICON_W, WIFI_ICON_H,  part_wifi, EINK_REPART_SET);    /* Filled red wifi icon    */ 
    }

    if(machine->ds1302 == CLOCK_IC_EXIST_FALSE ) {
        Eink_REadd_picture(CLOCK_ICON_Y,   CLOCK_ICON_X, CLOCK_ICON_W, CLOCK_ICON_H,  part_clock, EINK_REPART_SET);  /* Filled red clock icon    */ 
    }

  
    /* Update red layer weather icon */
    Eink_FillWeather_Icom( machine->weatherToday.day,     EINK_WEATHER_DAY, ICON_RE_LAYER) ;
    Eink_FillWeather_Icom( machine->weatherToday.night, EINK_WEATHER_NIGHT, ICON_RE_LAYER) ;

    EINK_Fresh((uint8_t *)&Eink_Cache[0][0], EINK_LAYER_RE, EINK_STAGE_DISP);                                    /* Update the red layer and display   */

    EINK_GPIO_deInit();
}

/**********************************************************************************************************************
*   Function name: Eink_Disp_TimeOut
*   Function description: Display the setting timeout interface
*   Parameter list: None
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*
**********************************************************************************************************************/
void EpaperModule::dispTimeOut(void)
{
  memset(    (uint8_t *)&Eink_Cache[0][0], BW_COLOR_WHITE, EINK_WIDTH *EINK_HEIGH_BYTE );                /* Clear the black and white layer         */
  EINK_Fresh((uint8_t *)&Eink_Cache[0][0], EINK_LAYER_BW, EINK_STAGE_INIT);                              /* Update the black and white layer and initialize it */
  
  memset(    (uint8_t *)&Eink_Cache[0][0], RE_COLOR_EMPTY, EINK_WIDTH *EINK_HEIGH_BYTE );                /* Clear the red layer         */
  memcpy(    (uint8_t *)&Eink_Cache[0][0],(uint8_t *)&img_timeout[0][0], EINK_WIDTH *EINK_HEIGH_BYTE );  /* Copy display content         */
  EINK_Fresh((uint8_t *)&Eink_Cache[0][0], EINK_LAYER_RE, EINK_STAGE_DISP);                              /* Update the red layer and display   */

  EINK_GPIO_deInit();
}

/**********************************************************************************************************************
*   Function name: Eink_Disp_Config
*   Function description: Display the setting interface
*   Parameter list: None
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*
**********************************************************************************************************************/
void EpaperModule::dispConfig(void)
{
  /* Display red and black two-color interface */
  memset(    (uint8_t *)&Eink_Cache[0][0], BW_COLOR_WHITE, EINK_WIDTH *EINK_HEIGH_BYTE );                     /* Clear the black and white layer         */
  memcpy(    (uint8_t *)&Eink_Cache[0][0],(uint8_t *)&img_config_black[0][0], EINK_WIDTH *EINK_HEIGH_BYTE );  /* Copy display content         */
  EINK_Fresh((uint8_t *)&Eink_Cache[0][0], EINK_LAYER_BW, EINK_STAGE_INIT);                                   /* Update the black and white layer and initialize it */

  memset(    (uint8_t *)&Eink_Cache[0][0], RE_COLOR_EMPTY, EINK_WIDTH *EINK_HEIGH_BYTE );                     /* Clear the red layer         */
  memcpy(    (uint8_t *)&Eink_Cache[0][0],(uint8_t *)&img_config_red[0][0], EINK_WIDTH *EINK_HEIGH_BYTE );    /* Copy display content         */
  EINK_Fresh((uint8_t *)&Eink_Cache[0][0], EINK_LAYER_RE, EINK_STAGE_DISP);                                   /* Update the red layer and display   */  

  EINK_GPIO_deInit();
}

/**********************************************************************************************************************
*   Function name: Eink_Disp_LowPwr
*   Function description: Display low battery interface
*   Parameter list: None
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*
**********************************************************************************************************************/
void EpaperModule::dispLowPwr(void)
{
  memset(    (uint8_t *)&Eink_Cache[0][0], BW_COLOR_WHITE, EINK_WIDTH *EINK_HEIGH_BYTE );               /* Clear the black and white layer         */
  memcpy(    (uint8_t *)&Eink_Cache[0][0],(uint8_t *)&img_lowpwr[0][0], EINK_WIDTH *EINK_HEIGH_BYTE );  /* Copy display content         */
  EINK_Fresh((uint8_t *)&Eink_Cache[0][0], EINK_LAYER_BW, EINK_STAGE_INIT);                             /* Update the black and white layer and initialize it */

  memset(    (uint8_t *)&Eink_Cache[0][0], RE_COLOR_EMPTY, EINK_WIDTH *EINK_HEIGH_BYTE );               /* Clear the red layer         */ 
  Eink_fill_rectangular(LPOW_ICON_Y, LPOW_ICON_X, LPOW_ICON_W, LPOW_ICON_H ,  RE_COLOR_RED);            /* fill red area         */   

  EINK_Fresh((uint8_t *)&Eink_Cache[0][0], EINK_LAYER_RE, EINK_STAGE_DISP);                             /* Update the red layer and display   */

  EINK_GPIO_deInit();
}


#define LPOW_ICON_X  164   /* Normal view low battery icon x coordinate  */
#define LPOW_ICON_Y   17    /* Normal view low battery icon y coordinate  */
#define LPOW_ICON_W   23    /* Low battery icon width (pixels)      */
#define LPOW_ICON_H    8    /* Low battery icon height (Byte)      */


/**********************************************************************************************************************
*   Function name: Eink_Disp_Clean
*   Function description: Refresh the screen to white
*   Parameter list: None
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*
**********************************************************************************************************************/
void EpaperModule::dispClean(void)
{
  memset(    (uint8_t *)&Eink_Cache[0][0], BW_COLOR_WHITE, EINK_WIDTH *EINK_HEIGH_BYTE );               /* Clear the black and white layer         */
  EINK_Fresh((uint8_t *)&Eink_Cache[0][0], EINK_LAYER_BW, EINK_STAGE_INIT);                             /* Update the black and white layer and initialize it */

  memset(    (uint8_t *)&Eink_Cache[0][0], RE_COLOR_EMPTY, EINK_WIDTH *EINK_HEIGH_BYTE );               /* Clear the red layer         */
  EINK_Fresh((uint8_t *)&Eink_Cache[0][0], EINK_LAYER_RE, EINK_STAGE_DISP);                             /* Update the red layer and display   */

  EINK_GPIO_deInit();
}

/**********************************************************************************************************************
*   end of file
**********************************************************************************************************************/
