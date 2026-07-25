/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*                                 Copyright (c) 2018, wangzz <-All rights reserved->
*               
**----------------------------------------------------------------------------------------------------------------------
*   File name:   rainbow.c
*   Brief description: Rainbow tracking clock main program
*
**----------------------------------------------------------------------------------------------------------------------
*   Current version:   V1.0
*   Author/modifier: whys
*   Completion date: November 8, 2022
**----------------------------------------------------------------------------------------------------------------------
*   Replacement version: 
*   Original author: 
*   Completion date: 
------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <string.h>
#include <WiFiUdp.h>

#include <ArduinoJson.h>
#include <ESP8266_Seniverse.h>
#include <NTPClient.h>  /* NTP clock module */  

#include "src/config.h"                                     /* Project configuration macro                                 */
#include "src/app_types.h"                                  /* Global type and project configuration declarations                     */
#include "src/Sun/sun_position.h"                           /* Sun position calculation module declaration section                   */
#include "src/Sun/find_angle.h"                             /* Best pose finding algorithm declaration section                   */
#include "src/moto/moto.h"                                  /* Stepper motor driver declaration section                        */
#include "src/adc/adc.h"                                    /* ADC voltage acquisition statement section                       */
#include "src/DS1302/DS1302.h"                              /* DS1302 clock chip driver declaration part                */

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * Statement related to WiFi configuration
------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define CONFIG_DATA_BASEADDR         0x100000          /* Store initial installation parameters at 1M location */
#define CONFIG_DATA_SECTOR           256               /* 1M position corresponds to 256 sectors  */
#define TIME_CORRECT_PERIOD_MS       (1200000)         /* Used during the sunshine period. From the time when the window has sunshine, how often should the time be calibrated? It needs to be more than 5 minutes. The unit is ms. This item is invalid when using DS1302.*/        
#define UPDATE_TIME_PRISM            (86400000+6e5)    /* Update time and prism time are 24 hours + 10 minutes (the router may automatically reset at midnight) (the prism must be calibrated at night to avoid sunlight interference)*/
#define WIFI_APWAIT_TIMEOUT_POWERON  (10*6e4)          /* During initial installation, the AP status waiting timeout is 10 minutes.    */
#define WIFI_APWAIT_TIMEOUT_WAKEUP   (3*1e3)           /* When waking up from work, the AP status wait timeout should not be too long to avoid power consumption.  */
#define LONG_SLEEP_TIME              (60*6e4)          /* Long sleep duration */

 
const char* AP_NAME = "rainbow";       /* softAP hotspot name  */
const char* AP_PWD = "12345678";       /* softAP hotspot password  */

const byte DNS_PORT = 53;             /* DNS port number           */
IPAddress apIP(192, 168, 4, 1);       /* esp8266-AP-IP address   */
DNSServer dnsServer;                  /* Create dnsServer instance  */
ESP8266WebServer server(80);          /* Create WebServer instance  */

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp.tencent.com",60*60*8, 30*60*1000);
const char DefaultNTP[] = "ntp.tencent.com";

#if USE_EINK_SCREEN 
#include "src/Epaper/epaper.h"
#endif

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * global variables
------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#if(EINK_UI_LANGUAGE == EINK_LANGUAGE_CHINESE)
Forecast forecast; /*  Create a Forecast object to obtain informed weather information*/
#endif

#if(EINK_UI_LANGUAGE == EINK_LANGUAGE_ENGLISH)
Caiyun forecast; /*  Create a Forecast object to obtain Caiyun weather information*/
#endif

/*--------------- state machine -------------------------------------*/
/* Global configuration updated during initial installation*/
_FIXED_CONFIG Config
{
    {0},                /* Save wifi account      */
    {0},                /* Save wifi password      */
    0,                  /* save longitude           */
    0,                  /* save latitude           */
    0,                  /* save altitude           */
    0,                  /* Save window orientation       */
    0,                  /* Fine-tuning the reflective column position of the hardware structure */ 
    0,                  /* Set time zone             */
    {0},                /* Set default NTP server    */
    0,                  /* Is there an old configuration?     */
    0,                  /* Is it possible to connect to WiFi?   */
    0,                  /* Whether it is necessary to write to Flash  */
    0,                  /* Low battery reminder voltage      */
}; 

/* Global status data definition updated daily */
 _GLOBAL_MACHINE Machine ={
    0 ,               /* global state machine                 */
    0 ,               /* WiFi transaction state machine             */
    0 ,               /* Whether to calibrate prism, lighting time, weather */
    0 ,               /* Time to enter AP mode          */
    {0,0,0,0,},       /* Today's weather                  */
    0 ,               /* Window sunshine start time           */
    0 ,               /* Window sunshine end time           */
    0 ,               /* year                        */
    0 ,               /* month                        */
    0 ,               /* day                        */
    0 ,               /* week                      */
    0 ,               /* The best posture of prism in the last 5 minutes        */
    0 ,               /* Last sleep time               */
    0 ,               /* Current time in milliseconds        */
    0 ,               /* Timestamp when updating time when connected to wifi  */   
    0 ,               /* RTC frequency offset fine-tuning 1.0 positive and negative adjustment  */ 
    0 ,               /* Record the reset source of this startup         */ 
    0 ,               /* Reserve                      */ 
    0 ,               /* Reserve                      */ 
    0 ,               /* Reserve                      */ 
};    

#if OFFLINE_TEST
/*---------------  Serial console related, offline mode debugging use -------------------------------------*/
uint8_t guc_UART0cmd = 'Z';

uint32_t cnt = 0;                   /* Debugging use                  */
uint32_t user_triger = 0;           /* For debugging use, force the trigger to move.  */
uint32_t prestart = 0;              /* Debugging use, last motor action time  */

uint16_t  win_shine_start = 494;    /* Debugging use, lighting start time     */
uint16_t angle_list[] = {           /* For debugging use, look up the table to calculate the position.     */
 200, 196, 192, 190, 187, 184, 182, 180, 177, 175, 173, 171, 169, 167,
 165, 163, 161, 160, 158, 156, 155, 153, 152, 150, 149, 148, 147, 146, 145, 144,
 143, 142, 141, 140, 140, 139, 138, 138, 137, 137, 137, 136, 136, 136, 136, 136,
 136, 136, 136, 136, 137, 137, 137, 138, 138, 139, 139, 140, 140, 141, 141, 142,
 143, 143, 144, 145, 145, 146, 147, 147, 148, 149, 150, 150, 151, 152, 153, 154,
 154, 155, 156, 157, 158, 158, 159, 160, 161, 162, 162, 163, 164, 165, 166, 167,
 168, 169, 169, 170, 171, 172, 173, 174, 175, 176, 178, 179, 180, 181, 182, 184,
 185, 187, 188,};
 #endif

#if(EINK_UI_LANGUAGE == EINK_LANGUAGE_CHINESE)
const char my_weather_key[] = "SUfADtTVXqqdqaJ8J";          /* Personal private key to know the weather, backup when necessary */
#endif

#if(EINK_UI_LANGUAGE == EINK_LANGUAGE_ENGLISH)
const char my_weather_key[] = "8OyNmkKGoKRYbGfk";           /* Personal Caiyun Weather private key, backup when necessary */
#endif

#include "src/wifi_html/wifi_html.h"                        /* Web server source code statement */


/**********************************************************************************************************************
*   Function name: Offline_Test (used for debugging)
*   Function description: Drive the motor to rotate based on local time, without using WiFi and without going to sleep.
*   Parameter list: None
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
#if OFFLINE_TEST
void Offline_Test(_GLOBAL_MACHINE * machine, _MOTO_CTRL * moto )
{
  uint16_t index = 0;
  uint32_t timeCurrent = 0;
  uint16_t sum_minutes = 0;

  timeCurrent = machine->timemsNow + millis();             /* Calculate total time in milliseconds                 */
  sum_minutes = timeCurrent/60e3;                         /* Convert to minutes                     */

  if ( sum_minutes > win_shine_start) {                   /* Greater than the lighting start time                               */
      if((sum_minutes-win_shine_start) % 5 == 0 || user_triger !=0){        /* Trigger once every 5 minutes or manually trigger   */
        user_triger =0;      
        if(millis() - prestart > 61e3) {                  /* The next trigger will be performed 61 seconds after the last trigger to prevent repeated triggers. */
          prestart = millis() ;                           /* Note the time of this trigger                             */

          index = (uint16_t) ((sum_minutes-win_shine_start)/5);
          if(index < sizeof(angle_list)/2-1) {

            /* Deflecting more downward can ensure that total reflection will not occur in the next 5 minutes. */            
            if(angle_list[index +1] > angle_list[index]) {
              moto->goToAbsLocation((float) angle_list[index+1]);           /* Start motor action      */
            } else {
              moto->goToAbsLocation((float) angle_list[index]);             /* Start motor action       */              
            }

            /* Serial port displays time and action angle */
#if DEBUG_INFO_EN
            Serial.printf("time: %d:%02d moto angle: %d\r\n",  (uint16_t)((timeCurrent % 86400000) / 3600000), (uint16_t)((timeCurrent % 3600000 ) / 60000),angle_list[index]); 
#endif  
          }           
        }   
      }      
  }   
}
#endif


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * Code: state machine
------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************
*   Function name: local_calendar
*   Function description: Local perpetual calendar, when obtaining time from the Internet fails, use the local perpetual calendar
*   Parameter list: machine global variable top-level state machine
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
void local_calendar( _GLOBAL_MACHINE * machine)
{
  uint8_t month_list1[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

  /* Determine leap year*/
  if(machine->month == 2){
    if(((machine->year%4 == 0) && (machine->year%100 != 0))|| (machine->year%400 == 0) ){
       month_list1[1] = 29; 
    }
  }  

  machine->day += 1;
  if(machine->day > month_list1[machine->month -1]){ /* If it is greater than the number of days in the current month */
    machine->day = 1;
    machine->month +=1;

    if(machine->month > 12) {
        machine->month = 1;
        machine->year +=1;
    }
  }
}

/**********************************************************************************************************************
*   Function name: Press_Data_to_RTCmemory
*   Function description: Write time, date, weather and other information from RTC memory
*   Parameter list: Global_State global variable top-level state
*              moto: Motor state machine
*               sel: RTC_UPDATE_WiFi updates RTC memory after synchronizing data from WiFi
*                    RTC_UPDATE_SLEEP Update RTC memory before sleeping
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
static void Press_Data_to_RTCmemory(_GLOBAL_MACHINE * machine, _MOTO_CTRL *moto, uint8_t sel)
{  
  /* Date Weather Illumination Time Only updated after Wifi is synchronized from the network */   
  if(sel == RTC_UPDATE_WIFI) { 
    /* The RTC storage area supports a total of 128 4-byte data (that is, a total of 512 bytes of content can be stored), and the address offset value is 0 ~ 127*/
    ESP.rtcUserMemoryWrite(0, (uint32_t*)&machine->weatherToday, sizeof(machine->weatherToday));  /* Today's weather                  */
    ESP.rtcUserMemoryWrite(1, (uint32_t*)&machine->shine_start,  sizeof(machine->shine_start));   /* Window sunshine start time           */
    ESP.rtcUserMemoryWrite(2, (uint32_t*)&machine->shine_stop,   sizeof(machine->shine_stop));    /* Window sunshine end time           */
    ESP.rtcUserMemoryWrite(3, (uint32_t*)&machine->year,         sizeof(machine->year));          /* year                        */
    ESP.rtcUserMemoryWrite(4, (uint32_t*)&machine->month ,       sizeof(machine->month));         /* month                        */
    ESP.rtcUserMemoryWrite(5, (uint32_t*)&machine->day,          sizeof(machine->day));           /* day                        */
    ESP.rtcUserMemoryWrite(6, (uint32_t*)&machine->weekday,      sizeof(machine->weekday));       /* week                      */    
    machine->timemsNow = machine->timemsNow;                                                      /* Calculate Boot time            */

    ESP.rtcUserMemoryWrite(12, (uint32_t*)&machine->wifilog,      sizeof(machine->wifilog));       /* Update wifi timestamp */
    ESP.rtcUserMemoryWrite(13, (uint32_t*)&machine->rtc_adj,      sizeof(machine->rtc_adj));       /* Update rtc adjustment value   */
    /* Update RTC calibration variables */

  /* Sleep information is only updated when entering sleep mode  */ 
  } else {
    ESP.rtcUserMemoryWrite(7, (uint32_t*)&machine->best_rotate, sizeof(machine->best_rotate));    /* best angle                  */ 
    ESP.rtcUserMemoryWrite(8, (uint32_t*)&machine->sleepedTime, sizeof(machine->sleepedTime));    /* time to sleep             */
    machine->timemsNow += millis();                                                               /* Save time before sleep           */
    /* Save motor status */

    ESP.rtcUserMemoryWrite(10, (uint32_t*)&moto->beat, sizeof(moto->beat));                       /* Save current motor position           */
    ESP.rtcUserMemoryWrite(11, (uint32_t*)&moto->standLoca, sizeof(moto->standLoca));             /* Save current motor beat           */  
                                                  
  }
  /* Update time every time  */ 
  ESP.rtcUserMemoryWrite(9, (uint32_t*)&machine->timemsNow, sizeof(machine->timemsNow));          /* Save current time information           */


}

/**********************************************************************************************************************
*   Function name: Resume_Data_from_RTCmemory
*   Function description: After waking up from deep sleep, restore time, date, weather and other information from RTC memory  
*   Parameter list: Global_State global variable top-level state
*              moto: Motor state machine
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
static void Resume_Data_from_RTCmemory(_GLOBAL_MACHINE * machine, _MOTO_CTRL *moto)
{
    /* Because RAM data is lost after deep sleep, all data must be updated */   
    ESP.rtcUserMemoryRead(0, (uint32_t*)&machine->weatherToday, sizeof(machine->weatherToday));  /* Today's weather                  */
    ESP.rtcUserMemoryRead(1, (uint32_t*)&machine->shine_start,  sizeof(machine->shine_start));   /* Window sunshine start time           */
    ESP.rtcUserMemoryRead(2, (uint32_t*)&machine->shine_stop,   sizeof(machine->shine_stop));    /* Window sunshine end time           */
    ESP.rtcUserMemoryRead(3, (uint32_t*)&machine->year,         sizeof(machine->year));          /* year                        */
    ESP.rtcUserMemoryRead(4, (uint32_t*)&machine->month ,       sizeof(machine->month));         /* month                        */
    ESP.rtcUserMemoryRead(5, (uint32_t*)&machine->day,          sizeof(machine->day));           /* day                        */
    ESP.rtcUserMemoryRead(6, (uint32_t*)&machine->weekday,      sizeof(machine->weekday));       /* week                      */ 
    ESP.rtcUserMemoryRead(7, (uint32_t*)&machine->best_rotate,  sizeof(machine->best_rotate));   /* best angle                   */
    ESP.rtcUserMemoryRead(8, (uint32_t*)&machine->sleepedTime,  sizeof(machine->sleepedTime));   /* last sleep time             */
    ESP.rtcUserMemoryRead(9, (uint32_t*)&machine->timemsNow,    sizeof(machine->timemsNow));     /* The time of last sleep           */

    machine->timemsNow += machine->sleepedTime  ;                                           /* Boot time = last sleep point + sleep duration */

    /* Restore motor status */
    ESP.rtcUserMemoryRead(10, (uint32_t*)&moto->beat,            sizeof(moto->beat));             /* Restore current motor position           */
    ESP.rtcUserMemoryRead(11, (uint32_t*)&moto->standLoca,       sizeof(moto->standLoca));        /* Restore current motor beat           */   
    moto->beat%=8;                                                                                /* Prevent array subscripts from going out of bounds           */                                                     
    moto->standLoca %=4096;                                                                       /* Constraint standard numerical range           */

    ESP.rtcUserMemoryRead(12, (uint32_t*)&machine->wifilog,      sizeof(machine->wifilog));       /* Update wifi timestamp */
    ESP.rtcUserMemoryRead(13, (uint32_t*)&machine->rtc_adj,      sizeof(machine->rtc_adj));       /* Update rtc adjustment value   */
}

/**********************************************************************************************************************
*   Function name: Enter_DeepSleep
*   Function description: Clean IO power consumption status, save runtime parameters, and then enter deep sleep
*   Parameter list: Global_State global variable top-level state
*             sleepsms: the length of time to sleep in ms
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
static void Enter_DeepSleep( _GLOBAL_MACHINE * machine, uint32_t sleepms)
{
    /************* Adjust IO port status to save power *********************/
    /* Turn off all windings to prevent heating */
    digitalWrite(MOTO_A, LOW);
    digitalWrite(MOTO_B, LOW);
    digitalWrite(MOTO_C, LOW);
    digitalWrite(MOTO_D, LOW);  

    digitalWrite(ADC_CTRL, LOW);  /* ADC power path closed   */
    digitalWrite(IR_DRIVE, HIGH); /* Infrared diode goes out    */

#if USE_EINK_SCREEN 
    /************ Check whether the ink screen has finished displaying *******************/
    Epaper.sleep();                                          /* Wait up to 30 seconds for display to complete     */
    pinMode(EINK_CS,OUTPUT);        
    pinMode(EINK_DAT,OUTPUT); 
    pinMode(EINK_CLK,OUTPUT); 
                      
    digitalWrite(EINK_CS,  HIGH);                          /* Pull CS GPIO2 high to avoid power consumption     */
    digitalWrite(EINK_DAT, LOW);                           /* DATA is pulled low to start charging            */
    digitalWrite(EINK_CLK, LOW);                           /* ADC_CTRL is pulled low to turn off the ADC acquisition circuit */
#endif    

#if DEBUG_INFO_EN
    Serial.printf("Will sleep %dS\r\n", (uint32_t)(sleepms/1000));
#endif

    /********** Write the current time and timing duration into RTC memory *******/
    /* Handling situations where the battery is low for a long time and then restored*/
    if(machine->timemsNow> 20736e5 ){                             /* Time exceeds 24 days int32 maximum value   */
      /* Once the battery voltage is restored, the wifi will be reconnected next time */
        machine->timemsNow = 864e5;                               /* Time reset to more than one day to prevent overflow */
    }   
    machine->sleepedTime = sleepms;                              /* Update sleep time                */
    Press_Data_to_RTCmemory(machine, (_MOTO_CTRL*) &Moto, RTC_UPDATE_SLEEP);
    
    /********** Officially entering sleep mode ****************************/
    ESP.deepSleep(sleepms*1000); 
}

/**********************************************************************************************************************
*   Function name: Write_config_toFlash
*   Function description: Write initial installation setting data to Flash 
*   Parameter list: config: initial installation setting structure
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
static void Write_config_toFlash( _FIXED_CONFIG * config)
{
  spi_flash_erase_sector (CONFIG_DATA_SECTOR);                                       /* Sectors must be erased before writing, 4kB per sector */
  spi_flash_write(CONFIG_DATA_BASEADDR, (uint32_t*)config, sizeof(_FIXED_CONFIG));   /* Write initial installation settings to Flash           */

#if DEBUG_INFO_EN
  /* Debugging use */
    Serial.println("the config to write flash");
    Serial.println(config->ssid);  
    Serial.println(config->password);     
    Serial.printf("%.6f\r\n", config->locaEW);
    Serial.printf("%.6f\r\n", config->locaSN);
    Serial.printf("%.6f\r\n", config->height);
    Serial.printf("%.6f\r\n", config->windir);
    Serial.printf("%d\r\n", config->structADJ);
    Serial.printf("%d\r\n", config->UTC);
    Serial.println(config->NTPserver);
    Serial.printf("%d\r\n", config->LowPwrThreshold);
#endif
}

/**********************************************************************************************************************
*   Function name: Read_config_fromFlash
*   Function description: Read setting data from Flash
*   Parameter list: config: initial installation setting structure 
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
static void Read_config_fromFlash( _FIXED_CONFIG * config, uint8_t viewEN)
{
  memset((char *)config, 0, sizeof(_FIXED_CONFIG));                                /* Clear structure data                */
  spi_flash_read(CONFIG_DATA_BASEADDR, (uint32_t*)config, sizeof(_FIXED_CONFIG));  /* Read initial installation settings from Flash          */
 
  /* Check whether the parameters are valid configurations */
  config->effect = FIXED_CONFIG_EFFECTIVE;           /* Preset parameters are valid */

  if(strlen(config->ssid)== 0 || strlen(config->ssid)>63) {              /* No SSID      */
    config->effect = FIXED_CONFIG_EXPIRE;                                /* Preset parameters are invalid */
  }

  if(strlen(config->password)== 0 || strlen(config->password)>63) {     /* No password  */
    config->effect = FIXED_CONFIG_EXPIRE;                                /* Preset parameters are invalid */
  }

  if(config->locaEW<-180.0 || config->locaEW>180) {       /* Check longitude     */
    config->effect = FIXED_CONFIG_EXPIRE;            /* Preset parameters are invalid */
  }
  if(config->locaSN<-90.0 || config->locaSN>90) {        /* Check latitude    */
    config->effect = FIXED_CONFIG_EXPIRE;            /* Preset parameters are invalid */
  }
  if(config->height<0 || config->height>5000) {      /* Check altitude     */
    config->effect = FIXED_CONFIG_EXPIRE;            /* Preset parameters are invalid */
  }
  if(config->windir<0 || config->windir>360) {       /* Check window orientation  */
    config->effect = FIXED_CONFIG_EXPIRE;            /* Preset parameters are invalid  */
  }
  if(config->structADJ >= 4096) {                   /* Check reflector position */
    config->effect = FIXED_CONFIG_EXPIRE;            /* Preset parameters are invalid  */
  }  
  if(config->LowPwrThreshold> BAT_LOW_GATE ) {
    config->LowPwrThreshold = BAT_LOW_GATE;          /* Limit the maximum low battery prompt position */
    config->effect = FIXED_CONFIG_EXPIRE;            /* Preset parameters are invalid  */
  }

 
  config->toConnectWifi = TODO_CAN_NOT_WIFI ;        /* Not connected to WiFi  */
  config->needWriteFlash = DO_NOT_WRITE_FLASH ;      /* Flash not updated  */

  if (viewEN !=0) {                                  /* View configuration parameters */
#if DEBUG_INFO_EN
    /* Debugging use */
    if(config->effect == FIXED_CONFIG_EFFECTIVE) {
        Serial.println("have effect config");
        Serial.println(config->ssid);  
        Serial.println(config->password);     
        Serial.printf("%.6f\r\n", config->locaEW);
        Serial.printf("%.6f\r\n", config->locaSN);
        Serial.printf("%.6f\r\n", config->height);
        Serial.printf("%.6f\r\n", config->windir);
        Serial.printf("%d\r\n", config->structADJ);
        Serial.printf("%d\r\n", config->UTC);
        Serial.println(config->NTPserver);   
        Serial.printf("%d\r\n", config->LowPwrThreshold);  
    } else {
        Serial.println("no effect config");
    }
#endif
  }

}

/**********************************************************************************************************************
*   Function name: Connect_WiFi
*   Function description: Connect the device to WiFi
*   Parameter list: machine: global state machine structure  
*            config :Initial installation structure 
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
static void Connect_WIFI( _GLOBAL_MACHINE * machine, _FIXED_CONFIG * config)
{
  uint32_t timestart = 0;
  uint8_t  result = 0;

  if(config->toConnectWifi == TODO_CONNECT_WIFI ) {         /* Check if WiFi should be connected     */

    WiFi.mode(WIFI_STA);                                    /* Set WiFi to STATION mode  */
    WiFi.begin(config->ssid, config->password);             /* Connect to designated WiFi             */
    timestart =  millis();                                  /* Timeout start timestamp           */
#if DEBUG_INFO_EN
    Serial.println("connect to wifi...");   
#endif  
    while (WiFi.status() != WL_CONNECTED) {              
      delay(100);                                           /* Connect once every 100 ms          */
#if DEBUG_INFO_EN      
      Serial.print(".");
#endif      

      if(millis()-timestart > 10e3){                        /* If there is no connection for 10 seconds, it is considered a timeout.      */
          result = 1;                                       /* Mark connection failed             */
          break;
      }
    }

    /********* WIFI Connection successful ********************************/
    if(result == 0) {
      /* Check WiFi address and signal strength */
      IPAddress myAddress = WiFi.localIP();                 /* View IP address         */
#if DEBUG_INFO_EN
      Serial.println();                              
      Serial.print("Connected to wifi. My address:");  
      Serial.println(myAddress);
      Serial.printf("signal : %d\n", WiFi.RSSI());          /* Check wifi signal strength    */
#endif

      config->needWriteFlash = NEED_UPDATE_FLASH ;          /* Flash parameters need to be updated   */
      machine->wifi_state = WIFI_STATION_CONNECTED;         /* Enter STATION synchronization state */

    /********* WIFI Connection failed ********************************/
    } else {
      /* Prompt connection failure and turn off WiFi*/
#if DEBUG_INFO_EN
      Serial.println();                              
      Serial.print("wifi station connect failed");  
#endif
      WiFi.disconnect();
      WiFi.mode(WIFI_OFF);

      config->effect = FIXED_CONFIG_EXPIRE ;                /* Invalid tag parameter     */
      machine->wifi_state = WIFI_ENTER_SOFTAP;              /* Re-enter Soft AP */
    }
  } else {   
  }
}

/**********************************************************************************************************************
*   Function name: initSoftAP
*   Function description: Initialize ESP8266 to soft AP mode
*   Parameter list: None
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
static void initSoftAP(void)
{
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  if(WiFi.softAP(AP_NAME,AP_PWD)){
#if DEBUG_INFO_EN    
    Serial.println("ESP8266 SoftAP success");
#endif
  } else {
#if DEBUG_INFO_EN    
    Serial.println("ESP8266 SoftAP failed");
#endif
  }
}

/**********************************************************************************************************************
*   Function name: handleRoot
*   Function description: Access the homepage callback function
*   Parameter list: None
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
static void handleRoot(void) {

  if(Config.effect ==  FIXED_CONFIG_EFFECTIVE) {
    server.send(200, "text/html", page_html_oldconfig);          /* If there is an old valid configuration          */
  } else {
    server.send(200, "text/html", page_html_config);             /* There is no old configuration, enter the setting interface directly */
  }
}

/**********************************************************************************************************************
*   Function name: strfind
*   Function description: Find the position where a specific string appears in a string  
*   Parameter list: str source array pointer key matching
*   Hardware output: None
*   Return result: If the search is successful, the position where the key appears for the first time in str is returned (note that counting starts from 0)
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
static int strfind(char str[], char key[])
{
	int l1, l2, i, j, flag;
	l1 = strlen(str);
	l2 = strlen(key);
	for(i = 0; i <= l1 - l2; i++)
	{
		flag = 1;
		for(j = 0; j < l2; j++)
		{
			if (str[i + j] != key[j])
			{
				flag = 0;
				break;
			}
		}
		if (flag)
		{
			return i;//If the search is successful, the position where the key appears for the first time in str is returned (note that counting starts from 0)
		}
	}
 
	return -1;//If not found, return -1
}

/**********************************************************************************************************************
*   Function name: handleRootPost
*   Function description: Web form Post callback function
*   Parameter list: None
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
void handleRootPost(void) 
{
  char recive_buf[60] = {0};
  // char utc_tst[8]     = {0};
  // char adj_tst[16]    = {tmp0};
  char tmp_buf[32]    = {0};

  unsigned int i, find, length;
  double tmp;
#if DEBUG_INFO_EN 
  Serial.println("handleRootPost");
#endif
  /************* Old settings exist, respond to the old settings page ********************/
  if(Config.effect ==  FIXED_CONFIG_EFFECTIVE) {
    /********* Processing options *********/
    if (server.hasArg("sel")) {                         /* Determine whether there are account parameters             */
    if(strlen(server.arg("sel").c_str()) != 0){         /* Not the default value                    */
#if DEBUG_INFO_EN     
      Serial.print("got sel:");
#endif      
      memset(recive_buf, 0, 60);                        /* Clear the temporary receiving area                 */
      strcpy(recive_buf, server.arg("sel").c_str());    /* Copy the account parameters to receive_buf   */
#if DEBUG_INFO_EN       
      Serial.println(recive_buf);
#endif

      /* Reconfigure*/
      if(strcmp(recive_buf,"no") == 0) {
        Config.effect = FIXED_CONFIG_EXPIRE;              /* Mark old configuration as invalid                */
        server.send(200, "text/html", page_html_config);  /* Enter the new configuration page              */

      /* Use old configuration*/
      } else {
#if DEBUG_INFO_EN         
        Serial.println("use old config");
#endif
        Config.toConnectWifi = TODO_CONNECT_WIFI ;       /*Mark can connect to wifi              */
        server.send(200, "text/html", page_html_check);  /* Prompt to close page                 */
        delay(1500);                                     /* Wait for the page to be sent             */
      }
 
    } else {                                            /* is the default value                  */
#if DEBUG_INFO_EN       
      Serial.println("sel wrong");
#endif
      server.send(200, "text/html", page_html_error);   /* Return error page              */
      return;
    }
    } else {                                            /* no parameters                 */
#if DEBUG_INFO_EN       
      Serial.println("error, not found sel");
#endif
      server.send(200, "text/html", "<meta charset='UTF-8'>error, not found sel");//Return error page
      return;
    }

  /************* Go directly to the new settings without the old settings *********************/  
  } else {
    /********* Handle accounts *********/
    if (server.hasArg("ssid")) {                         /* Determine whether there are account parameters         */
      if(strlen(server.arg("ssid").c_str()) != 0){         /* Not the default value                */
#if DEBUG_INFO_EN       
      Serial.print("got ssid:");
#endif
      memset(Config.ssid, 0, 48);                        /* clear ssid                  */
      strcpy(Config.ssid, server.arg("ssid").c_str());   /* Copy the account parameters to sta_ssid */
#if DEBUG_INFO_EN         
      Serial.println(Config.ssid);
#endif      
      } else {                                             /* is the default value                  */
#if DEBUG_INFO_EN       
        Serial.println("ssid wrong");
#endif
        server.send(200, "text/html", page_html_error);    /* Return error page              */
        return;
      }
    } else {                                             /* no parameters  */
#if DEBUG_INFO_EN       
      Serial.println("error, not found ssid");
#endif      
      server.send(200, "text/html", "<meta charset='UTF-8'>error, not found ssid");//Return error page
      return;
    }

    /****** Handling passwords ***********/
    if (server.hasArg("password")) {
      if(strlen(server.arg("password").c_str()) != 0){ /* Not the default value                */
#if DEBUG_INFO_EN         
      Serial.print("got password:");
#endif
      memset(Config.password, 0, 48);                  /* clear password              */
      strcpy(Config.password, server.arg("password").c_str());
#if DEBUG_INFO_EN         
      Serial.println(Config.password);
#endif

      } else {                                           /* is the default value                  */
#if DEBUG_INFO_EN   
        Serial.println("password wrong");
#endif
        server.send(200, "text/html", page_html_error); /* Return error page              */
        return;
      }
    } else {
#if DEBUG_INFO_EN         
      Serial.println("error, not found password");
#endif
      server.send(200, "text/html", page_html_error); /* Return error page              */
      return;
    }

    /****** Handle private keys ***********/
    if (server.hasArg("key")) {
      if(strlen(server.arg("key").c_str()) != 0){ /* Not the default value                */
#if DEBUG_INFO_EN         
      Serial.print("got key:");
#endif
      memset(Config.weatherpwd, 0, 32);                  /* Clear key              */
      strcpy(Config.weatherpwd, server.arg("key").c_str());
#if DEBUG_INFO_EN         
      Serial.println(Config.weatherpwd);
#endif

      } else {   /* If the private key is not filled in, the default private key will be used. */                                        /* is the default value                  */
        // server.send(200, "text/html", page_html_error); /* Return error page              */
        // return;
        memset(Config.weatherpwd, 0, 32);                  /* Clear key              */
        strcpy(Config.weatherpwd, my_weather_key);         /* Fill in the default key          */
      }
    } else { /* Allow the private key to be empty and use the default private key*/
      // server.send(200, "text/html", page_html_error); /* Return error page              */
      // return;
        memset(Config.weatherpwd, 0, 32);                  /* Clear key              */
        strcpy(Config.weatherpwd, my_weather_key);         /* Fill in the default key          */
    }

    /****** Handle longitude ***********/
    if (server.hasArg("locaew")) {
      if(strlen(server.arg("locaew").c_str()) != 0){ /* Not the default value                */
#if DEBUG_INFO_EN         
      Serial.print("got locaew:");
#endif
      memset(recive_buf, 0, 60);                      /* Clear the temporary receiving area             */
      strcpy(recive_buf, server.arg("locaew").c_str());
      Config.locaEW = atof(recive_buf);               /* Convert string to double        */
#if DEBUG_INFO_EN         
      Serial.printf("sta_locaew %.6f\r\n", Config.locaEW);
#endif

      } else {                                         /* is the default value                  */
#if DEBUG_INFO_EN       
        Serial.println("locaew wrong");
#endif
        server.send(200, "text/html", page_html_error); /* Return error page              */
        return;
      }
    } else {
#if DEBUG_INFO_EN         
      Serial.println("error, not found locaew");
#endif
      server.send(200, "text/html", page_html_error); /* Return error page              */
      return;
    }

    /****** Handle latitude ***********/
    if (server.hasArg("locasn")) {
      if(strlen(server.arg("locasn").c_str()) != 0){ /* Not the default value                */
#if DEBUG_INFO_EN         
      Serial.print("got locasn:");
#endif
      memset(recive_buf, 0, 60);                     /* Clear the temporary receiving area             */    
      strcpy(recive_buf, server.arg("locasn").c_str());
      Config.locaSN= atof(recive_buf);               /* Convert string to double        */
#if DEBUG_INFO_EN         
      Serial.printf("sta_locasn %.6f\r\n", Config.locaSN);
#endif

      } else {                                         /* is the default value                  */
#if DEBUG_INFO_EN       
        Serial.println("locasn wrong");
#endif
        server.send(200, "text/html", page_html_error); /* Return error page              */
        return;
      }
    } else {
#if DEBUG_INFO_EN         
      Serial.println("error, not found locasn");
#endif
      server.send(200, "text/html", page_html_error); /* Return error page              */
      return;
    }

    /****** Handle altitude ***********/
    if (server.hasArg("height")) {
      if(strlen(server.arg("height").c_str()) != 0){ /* Not the default value                */
#if DEBUG_INFO_EN         
      Serial.print("got height:");
#endif
      memset(recive_buf, 0, 60);                     /* Clear the temporary receiving area             */      
      strcpy(recive_buf, server.arg("height").c_str());
       
      /*********** Get the altitude numeric field ***************/ 
      length = strlen(recive_buf);
      memset(tmp_buf, 0, 32);                        /* Clear the temporary receiving area            */  
      for(i=0; i<length; i++) {
        if(recive_buf[i] != '[')
        {
          tmp_buf[i] = recive_buf[i];
        } else {
          break;
        }       
      }
      Config.height = atof(tmp_buf);               /* Convert string to double        */
#if DEBUG_INFO_EN         
      Serial.printf("sta_height %.6f\r\n", Config.height);
#endif

      /*********** Get the angle trim field ***************/
      find = strfind(recive_buf, "ADJ");
      if(find != -1) {                               /* Reflective column trim field exists         */
          memset(tmp_buf, 0, 32);                    /* Clear the temporary receiving area            */    
          for(i=find+3; i<length; i++) {
            if(recive_buf[i] != ']')
            {
              tmp_buf[i-find-3] = recive_buf[i];
            } else {
              break;
            } 
          }

       tmp = atof(tmp_buf);               /* Convert string to double        */
       Config.structADJ = (int32_t)(tmp*4096/360.0);
#if DEBUG_INFO_EN         
      Serial.printf("adjust: %.6f\r\n", tmp);
#endif      
      } else {
        Config.structADJ = (int32_t)(ADC_DEFAULT_ADJUST*4096/360.0);                /* Adjust upward by 4.5 degrees by default */
      }

      /********** Get time zone setting field ****************/
      find = strfind(recive_buf, "UTC"); 
      if(find != -1) {                               /* There is manual setting of time zone          */
          memset(tmp_buf, 0, 32);                    /* Clear the temporary receiving area            */  
          for(i=find+4; i<length; i++) {
            if(recive_buf[i] != ']')
            {
              tmp_buf[i-find-4] = recive_buf[i];
            } else {
              break;
            } 
          }
          tmp = atof(tmp_buf);               /* Convert string to double        */
          if(recive_buf[find+3] == '-' ){
            tmp = -tmp;
          }
          Config.UTC = (int32_t) tmp;
#if DEBUG_INFO_EN   
      Serial.print("UTC:");
      Serial.printf("%d\r\n", Config.UTC);
#endif  
      } else {
          Config.UTC = 8;                                                            /* The default time zone is East 8 */
      }

      /********** Get time server settings field ***********/
      find = strfind(recive_buf, "NTP");
      if( find != -1) {         /* There is a manual time server setting    */
          memset(Config.NTPserver, 0, 32);                    /* Clear the temporary receiving area            */ 
          for(i=find+3; i<length; i++) {
            if(recive_buf[i] != ']')
            {
              Config.NTPserver[i-find-3] = recive_buf[i];
            } else {
              break;
            } 
          }          
#if DEBUG_INFO_EN   
      Serial.print("NTP:");
      Serial.printf("%s\r\n", Config.NTPserver);
#endif            
      } else {                                                                     
          memset(Config.NTPserver, 0, 32);                                           /* Clear the temporary receiving area            */ 
          strcpy(Config.NTPserver,  DefaultNTP);                                     /* The default time server is Alibaba server */
      }

      /********** Get the low battery threshold setting field ****************/
      find = strfind(recive_buf, "LPR"); 
      if(find != -1) {                               /* There is a manual setting of low battery threshold          */
          memset(tmp_buf, 0, 32);                    /* Clear the temporary receiving area            */  
          for(i=find+3; i<length; i++) {
            if(recive_buf[i] != ']')
            {
              tmp_buf[i-find-3] = recive_buf[i];
            } else {
              break;
            } 
          }
          tmp = atof(tmp_buf);                 /* Convert string to double        */
          if(tmp>BAT_LOW_GATE) {                       /* Limit threshold maximum  */
            tmp = BAT_LOW_GATE;
          }
          Config.LowPwrThreshold = (int32_t) tmp;
#if DEBUG_INFO_EN   
      Serial.print("LPR:");
      Serial.printf("%d\r\n", Config.LowPwrThreshold);
#endif  
      } else {
          Config.LowPwrThreshold = BAT_LOW_GATE;              /* Default low battery threshold */
      }      


      } else {                                         /* is the default value                  */
#if DEBUG_INFO_EN       
        Serial.println("height wrong");
#endif
        server.send(200, "text/html", page_html_error); /* Return error page              */
        return;
      }
    } else {
#if DEBUG_INFO_EN   
      Serial.println("error, not found height");
#endif
      server.send(200, "text/html", page_html_error); /* Return error page              */
      return;
    }

    /****** Handle window orientation  ***********/
    if (server.hasArg("windir")) {
      if(strlen(server.arg("windir").c_str()) != 0){ /* Not the default value                */
#if DEBUG_INFO_EN      
      Serial.print("got windir:");
      memset(recive_buf, 0, 60);                     /* Clear the temporary receiving area             */    
#endif 
      strcpy(recive_buf, server.arg("windir").c_str());
      Config.windir = atof(recive_buf);               /* Convert string to double        */
#if DEBUG_INFO_EN      
      Serial.printf("sta_windir %.6f\r\n", Config.windir);
#endif

      } else {                                         /* is the default value                  */
#if DEBUG_INFO_EN      
        Serial.println("windir wrong");
#endif
        server.send(200, "text/html", page_html_error); /* Return error page              */
        return;
      }
    } else {
#if DEBUG_INFO_EN      
      Serial.println("error, not found windir");
#endif
      server.send(200, "text/html", page_html_error); /* Return error page              */
      return;
    }

    /****** Return to save success page *******/
    server.send(200, "text/html", page_html_check);  
    delay(1500);                                     /* Wait for the page to be sent             */
    Config.toConnectWifi = TODO_CONNECT_WIFI ;       /*Mark can connect to wifi              */
  }
}

/**********************************************************************************************************************
*   Function name: initWebServer
*   Function description: Initialize WebServer
*   Parameter list: None
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
static void initWebServer(void)
{
  /* The above line must be written in the following format, otherwise the captive portal cannot be created */
  server.on("/", HTTP_GET, handleRoot);        /* Set the home page callback function                 */
  server.onNotFound(handleRoot);               /* Set the callback function for unresponsive http requests  */
  server.on("/", HTTP_POST, handleRootPost);   /* Set the Post request callback function             */
  server.begin();                              /* Start WebServer                   */
#if DEBUG_INFO_EN
  Serial.println("WebServer started!");      
#endif  
}

/**********************************************************************************************************************
*   Function name: initDNS
*   Function description: Initialize DNS server
*   Parameter list: None
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
static void initDNS(void)
{
  /* Determine whether mapping all addresses to esp8266's IP is successful */
  if(dnsServer.start(DNS_PORT, "*", apIP)){
#if DEBUG_INFO_EN    
    Serial.println("start dnsserver success.");
#endif
  } else {
#if DEBUG_INFO_EN    
    Serial.println("start dnsserver failed.");
#endif
  } 
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * Code: WiFi state machine
------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************
*   Function name: WiFi_PowerOn_Handle
*   Function description: Process power-on reset WiFi transactions (transient)
*   Parameter list: machine: global state machine
*             config  :Initial installation structure
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
void WIFI_PowerOn_Handle( _GLOBAL_MACHINE * machine,  _FIXED_CONFIG * config )
{
#if DEBUG_INFO_EN  
  Serial.println("WIFI PowerOn_Handle ...."); 
#endif

  Read_config_fromFlash( config, 1);          /* Read WiFi configuration from Flash and determine whether the configuration is valid*/
  machine->wifi_state = WIFI_ENTER_SOFTAP;                 /* Enter the Soft AP configuration interface */ 
}

/**********************************************************************************************************************
*   Function name: WiFi_Reset_Handle
*   Function description: Handle sleep and wake-up WiFi transactions (transient)
*   Parameter list: machine: global state machine
*             config  :Initial installation structure
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
void WIFI_Reset_Handle( _GLOBAL_MACHINE * machine , _FIXED_CONFIG * config )
{
#if DEBUG_INFO_EN  
  Serial.println("WIFI Reset_Handle ....");  
#endif

  Read_config_fromFlash( config, 1);          /* Read WiFi configuration from Flash and determine whether the configuration is valid*/

  /* The parameter format is valid in Flash */
  if(config-> effect == FIXED_CONFIG_EFFECTIVE) {
    config->toConnectWifi =  TODO_CONNECT_WIFI;           /* Mark can connect to wifi       */
    Connect_WIFI( machine, config);                       /* Connect to WiFi              */
    config->needWriteFlash = DO_NOT_WRITE_FLASH;          /* Marker does not need to update Flash       */

  /* Invalid parameter format in Flash */
  } else {
    machine->wifi_state = WIFI_ENTER_SOFTAP;              /* Enter Soft AP state and reset  */
  }
}

/**********************************************************************************************************************
*   Function name: WiFi_EnterSoftAP_Handle
*   Function description: Process transactions before WiFi enters SoftAP (transient)
*   Parameter list: machine: global state machine
*             config  :Initial installation structure
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
void WIFI_EnterSoftAP_Handle( _GLOBAL_MACHINE * machine, _FIXED_CONFIG * config )
{
#if DEBUG_INFO_EN  
  Serial.println("WIFI EnterSoftAP_Handle....");  
#endif

  initSoftAP();                                            /* Initialize ESP8266 AP mode */
  initWebServer();                                         /* InitializeWebServer      */
  initDNS();                                               /* Initialize DNS Server    */

  config->toConnectWifi = TODO_CAN_NOT_WIFI ;               /*Mark cannot connect to wifi     */
  machine->enter_ap_time =  millis();                      /* Record the time to enter AP state   */ 
                           
  /* Factory default time and date*/
  machine->year = 2022;
  machine->month = 12;
  machine->day   = 13;
  machine->timemsNow = 14*36e5;

  machine->wifi_state = WIFI_APWAIT_CONNECT;               /* Enter and wait for AP connection     */
#if DEBUG_INFO_EN  
  Serial.println("WIFI APwait_Handle ....");     
#endif
}


/**********************************************************************************************************************
*   Function name: Update_locol_calendar
*   Function description: Drive local perpetual calendar update
*   Parameter list: Global_State global variable top-level state
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
static void Update_locol_calendar ( _GLOBAL_MACHINE * machine) 
{

#if DEBUG_INFO_EN
    Serial.println("update time from Internet failed!,use local calendar or DS10302!\r\n");
#endif

  if( machine->ds1302 == HAVE_DS1302) {
    /* When there is a DS1302 clock chip, the update of the built-in software perpetual calendar of ESP8266 is abandoned. */

    machine->timemsNow = DS1302.DStime.hour*3600000 + DS1302.DStime.minute*60000 +  DS1302.DStime.second*1000 + millis(); /* machine->timemsNow Sync toDS1302 */ 
     
    /* Synchronize the DS1302 calendar to the local computer, because the ink screen still uses the local calendar to display*/
    machine->year      = DS1302.DStime.year +2000;
    machine->month     = DS1302.DStime.month;
    machine->day       = DS1302.DStime.day ;
    machine->weekday   = DS1302.DStime.week ;

    machine->rtc_adj   = 1.0;                                         /* Internal rtc error reset (actually not used) to avoid ugly printing   */

  } else { 
    if(machine->timemsNow+millis()  > 86400000 ) {
      machine->timemsNow = (machine->timemsNow+millis())%86400000 ;  /* New day now real moment                          */
      machine->timemsNow -= millis();                                /* machine->timemsNowIn fact, what is recorded is the time of this startup. */ 
      local_calendar(  machine)  ;                                   /* Update date using local perpetual calendar                        */
      machine->weekday++; machine->weekday%=7;                       /* week increment                                     */

      machine->rtc_adj   = 1.0;                                      /* Reset RTC error calibration during initial installation and early morning calibration         */
      machine->wifilog  = 0;                                         /* Record initial installation and early morning calibration time                    */ 
    } else {
      /* Not more than one day, year, month, day, startup time remains unchanged*/

      if(machine->needFixPrism == PRISM_NEED_FIX) {                   /* When I first installed it and couldn't connect to wifi in the early morning               */                             
          machine->rtc_adj  = 1.0;                                    /* Reset RTC error calibration during initial installation and early morning calibration         */
          machine->wifilog  = 0;                                      /* Record initial installation and early morning calibration time                    */  
      }else{
          /* Day calibration time only adjusts rtc_adj          */
          //machine->rtc_adj   = machine->rtc_adj ;                   /* keep machine->rtc_adj unchanged                  */
      }
    }
  }
}

/**********************************************************************************************************************
*   Function name: UpdateRTCmemory_PrismFix
*   Function description: After updating the time, calculate the starting time of solar insolation and update RTC_memory. If the prism needs to be calibrated, calibrate it.
*   Parameter list: Global_State global variable top-level state
*             config  :Initial installation structure
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2024.5.29 Original creation version V1.0
*
**********************************************************************************************************************/
static void UpdateRTCmemory_PrismFix(  _GLOBAL_MACHINE * machine, _FIXED_CONFIG * config )
{
#if DEBUG_INFO_EN     
    Serial.printf("UpdateRTCmemory_PrismFix\r\n");  
#endif
  /* Update if needed Daylight hours Prism calibration Weather*/
  if(machine->needFixPrism == PRISM_NEED_FIX) {
    /***** Calculate window sunshine start time and sunshine end time ****/   
    SunPosition.location_earth.longitude = config->locaEW;  // Longitude
    SunPosition.location_earth.latitude = config->locaSN;   // Latitude
    SunPosition.location_earth.altitude = config->height;   // altitude

    SunPosition.time_earth.UTC = (double)config->UTC;        //time zone  

    /* When you have DS1302, it will also be synchronized to the local calendar. */
    SunPosition.time_earth.year  = machine->year ;          //year
    SunPosition.time_earth.month = machine->month;          //month
    SunPosition.time_earth.day   = machine->day;            //day 

    SunPosition.sun_shine((_SUN_SHINE*) &SunPosition.Shine, (_SUN_VIEW_LOCATION*) &SunPosition.sun_pos,(_YOUR_TIME*) &SunPosition.time_earth,(_YOUR_LOCATION*) &SunPosition.location_earth, config->windir);
    machine->shine_start  = SunPosition.Shine.start*60e3;     /* Window sunshine start time ms          */
    machine->shine_stop   = SunPosition.Shine.end*60e3;       /* Window sunshine end time ms          */  
#if DEBUG_INFO_EN     
    Serial.printf("shine start %d end %d \r\n", SunPosition.Shine.start,SunPosition.Shine.end);  
#endif
  }

  machine->best_rotate  = -1;                   /* After re-timing, reset the last best angle */
  /* Write data to RTC memory */
  Press_Data_to_RTCmemory(machine, (_MOTO_CTRL*) &Moto, RTC_UPDATE_WIFI);

  /* Global state machine goes to local processing */
  machine->global_state = GLOBAL_LOCAL;                   /* Enter local action processing             */

  /* Trigger generator calibration action */
  /* Update if needed Daylight hours Prism calibration Weather*/
  if(machine->needFixPrism == PRISM_NEED_FIX) {
    delay(1e3);                                            /* Wait for serial port data output to complete */    
#if USE_EINK_SCREEN 
    Epaper.sleep();                                          /*The IR_DIV and EINK RST pins are multiplexed, so you must wait for the ink screen refresh to complete, otherwise it will cause the ink screen to be reset.*/ 
#endif

    Moto.fixLocation();  
    Moto.roundScan();                 /* Motor action scan and enter BUSY state  */
  }
}

/**********************************************************************************************************************
*   Function name: WiFi_APwait_Handle
*   Function description: Process WiFi SoftAP setting transactions (scanning state)
*   Parameter list: machine: global state machine
*             config  :Initial installation structure
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
void WIFI_APwait_Handle(_GLOBAL_MACHINE * machine, _FIXED_CONFIG * config)
{ 
  uint32_t wifi_apwait_timeout;

  if(machine->rst_source == RST_SOURCE_POWRON) {
    wifi_apwait_timeout = WIFI_APWAIT_TIMEOUT_POWERON;
  } else {
    wifi_apwait_timeout = WIFI_APWAIT_TIMEOUT_WAKEUP;
  }

  /****************** Soft AP Setting does not time out *********/
  if(millis() - machine->enter_ap_time < wifi_apwait_timeout) {             /* Timeout X minutes            */
    server.handleClient();                                  /* Respond to web server            */
    dnsServer.processNextRequest();                         /* Respond to DNS server            */
    Connect_WIFI(machine, config);                          /* Check whether connected to WiFi          */

  /****************** Soft AP Set timeout ***********/
  } else {      
#if DEBUG_INFO_EN                                                                    
    Serial.println("SoftAP Config Timeout.");   
#endif

    if(config->effect == FIXED_CONFIG_EFFECTIVE) {            /* If it works, try again even if it times out.  */
      config->toConnectWifi = TODO_CONNECT_WIFI ;             /*Mark can connect to wifi           */
      Connect_WIFI(machine, config);                          /* Check whether connected to WiFi          */

    } else { /* Flash invalid timeout and sleep directly */                     
      WiFi.disconnect();                                      /* Disconnect WiFi connection              */
      WiFi.mode(WIFI_OFF);                                    /* Turn off WiFi                   */

    if(machine->rst_source == RST_SOURCE_WAKEUP) {          /* If you wake up from sleep at work         */
        machine->weatherToday.day = WEATHER_SUNNY_DAY;       /* Force weather to be sunny               */    
        Update_locol_calendar(machine);                       /* Drive local perpetual calendar update          */

#if USE_EINK_SCREEN
    Epaper.updateRunning(machine, WIFI_CONNECT_FAILED);         /* Refresh the screen and mark WiFi failure       */
#endif
        UpdateRTCmemory_PrismFix(machine, config);            /* After updating the time, calculate the start time of solar insolation and update RTC_memory. If you need to calibrate the prism, calibrate it. */

      } else {                                                  /* If it is a power-on reset, the setting has failed at this time. */
        /* AP status prompts on the screen           */
  #if USE_EINK_SCREEN
        Epaper.dispTimeOut();
  #endif      
        /************* Go to sleep **************************/
        Enter_DeepSleep(machine, LONG_SLEEP_TIME);                     /* Go to sleep for 1 hour            */
      }

    }
  }
}

/**********************************************************************************************************************
*   Function name: WiFi_Station_Handle
*   Function description: Process date and weather related matters after connecting to WiFi and the Internet (transient)
*   Parameter list: machine: global state machine
*             config  :Initial installation structure
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
void WIFI_Station_Handle( _GLOBAL_MACHINE * machine ,_FIXED_CONFIG * config)
{
  unsigned int timeCurrent,city;
  unsigned short timeupdateLimit, timeupdateCnt;
  unsigned short i, timeupdateResult;
  struct tm *ptm;
  time_t epochTime ;
  
#if DEBUG_INFO_EN
  Serial.println("WIFI Station_Handle ....");  
#endif

  if (config->needWriteFlash = NEED_UPDATE_FLASH) {          /* Flash parameters need to be updated   */
    Write_config_toFlash( config) ;                          /* Write parameters to Flash storage area */
    config->needWriteFlash = DO_NOT_WRITE_FLASH;             /* Clear Flash pending writing flag   */
#if DEBUG_INFO_EN
    Serial.println("write config to flash done");  
#endif
  }


/********* English version-> Use Caiyun Weather  ******************************************************/
#if(EINK_UI_LANGUAGE == EINK_LANGUAGE_ENGLISH) 
  if(machine->needFixPrism == PRISM_NEED_FIX) {   /* First installation or when the weather is updated every day              */   

    forecast.config(config->weatherpwd, config->locaEW, config->locaSN);
    if(forecast.update()){  /* Caiyun weather updated successfully */

      if(3600*config->UTC != forecast.getTimeZone()) {
        config->UTC = round(forecast.getTimeZone()/3600);
        Write_config_toFlash( config) ;                          /* Write parameters to Flash storage area */
        config->needWriteFlash = DO_NOT_WRITE_FLASH;             /* Clear Flash pending writing flag   */
        #if DEBUG_INFO_EN
        Serial.println("Time Zone changed &write to flash done");  
        #endif
      } 

      machine->weatherToday.day   = forecast.getDayCode();
      machine->weatherToday.night = forecast.getNightCode();
      machine->weatherToday.high  = forecast.getHigh();
      machine->weatherToday.low   = forecast.getLow();
      epochTime = forecast.getLocalTime();                                           /* The timestamp is the Unix time + the value after the time zone offset      */             
      ptm = localtime(&epochTime);                                                   /* Convert to local time Default time zone offset=0                 */

#if DEBUG_INFO_EN
        Serial.printf("Weather update success ^_^ %d-%d,%d-%d \r\n", machine->weatherToday.day, machine->weatherToday.night, machine->weatherToday.high, machine->weatherToday.low );   
#endif

// #if DEBUG_INFO_EN
//     Serial.printf("year   =%d\r\n",  (uint8_t)((ptm->tm_year+1900)%100));   /* year                                         */
//     Serial.printf("month  =%d\r\n",  (uint8_t)(ptm->tm_mon+1));             /* month                                       */
//     Serial.printf("day    =%d\r\n",  (uint8_t)(ptm->tm_mday));              /* Date                                       */ 
//     Serial.printf("week   =%d\r\n",  (uint8_t)(ptm->tm_wday));              /* week                                       */            
//     Serial.printf("hour   =%d\r\n",  (uint8_t)(ptm->tm_hour));              /* time                                         */
//     Serial.printf("minute =%d\r\n",  (uint8_t)(ptm->tm_min));               /* points                                         */            
//     Serial.printf("second =%d\r\n",  (uint8_t)(ptm->tm_sec));               /* seconds                                         */
// #endif  

      if(machine->ds1302 == HAVE_DS1302) {
        //TOTO: Update time to DS1302
        DS1302.DStime.year   = (uint8_t)((ptm->tm_year+1900)%100);                          /* year                                         */
        DS1302.DStime.month  = (uint8_t)(ptm->tm_mon+1);                                    /* month                                       */
        DS1302.DStime.day    = (uint8_t)(ptm->tm_mday);                                     /* Date                                       */ 
        DS1302.DStime.week   = (uint8_t)(ptm->tm_wday);                                     /* week                                       */            
        DS1302.DStime.hour   = (uint8_t)(ptm->tm_hour);                                     /* time                                         */
        DS1302.DStime.minute = (uint8_t)(ptm->tm_min);                                      /* points                                         */            
        DS1302.DStime.second = (uint8_t)(ptm->tm_sec);                                      /* seconds                                         */
        DS1302.setTime();                                                               /* Update time to DS1302                            */
      }

    } else {            /* Caiyun weather update failed */
  #if DEBUG_INFO_EN      
        Serial.print("Weather update Failed. ");   
        Serial.print("Server Response: ");          // Output server response status code for users to find problems
        Serial.println(forecast.getServerCode());   // Caiyun weather server error code
  #endif             
          /* After the weather acquisition fails, it is forced to be sunny.       */
        machine->weatherToday.day = WEATHER_SUNNY_DAY;      

    }
  }
#endif     

  /********* Get time and date information from the Internet ******************************************************/
  delay(1000);
  timeClient.setTimeOffset(3600*config->UTC);       //Set time zone, offset 3600, +8 zone, offset
  timeClient.setServerName(config->NTPserver);      //Set up NTP server
  timeClient.begin();

  if(machine->needFixPrism == PRISM_NEED_FIX) {  
    timeupdateLimit = 5;            /* When initial installation or cross-day calibration, the upper limit of time calibration times is set to 5 times. */
  } else {
    timeupdateLimit = 1;            /* During initial installation or cross-day calibration, the upper limit of time calibration times is set to 1. */
  }

  for(i=0; i<timeupdateLimit; i++) {
    delay(2000);                   /* Delay 2 seconds            */

    if(timeClient.update()) {
      timeupdateResult = 1;
      Serial.printf("time update success at %d cnt\r\n", i+1);  
      break;                      /* Once the update is successful, break out of the loop*/
    } else {
      timeupdateResult = 0;
    }
  }

  if( timeupdateResult != 0) {   /* Update time from internet successfully */
    /* Extraction time */
    epochTime = timeClient.getEpochTime();                                          /* The timestamp is the Unix time + the value after the time zone offset      */
    ptm = localtime(&epochTime);                                                    /* Convert to local time Default time zone offset=0                 */

    timeCurrent =  machine->timemsNow ;                                             /* Back up the number of ms recorded by the local RTC clock when waking up, used for clock calibration. */
    machine->timemsNow = ptm->tm_hour*36e5 +  ptm->tm_min*60e3 + ptm->tm_sec*1e3 -  millis(); /* machine->timemsNowIn fact, what is recorded is the time of this startup. */ 
// #if DEBUG_INFO_EN
//     Serial.printf("year   =%d\r\n",  (uint8_t)((ptm->tm_year+1900)%100));   /* year                                         */
//     Serial.printf("month  =%d\r\n",  (uint8_t)(ptm->tm_mon+1));             /* month                                       */
//     Serial.printf("day    =%d\r\n",  (uint8_t)(ptm->tm_mday));              /* Date                                       */ 
//     Serial.printf("week   =%d\r\n",  (uint8_t)(ptm->tm_wday));              /* week                                       */            
//     Serial.printf("hour   =%d\r\n",  (uint8_t)(ptm->tm_hour));              /* time                                         */
//     Serial.printf("minute =%d\r\n",  (uint8_t)(ptm->tm_min));               /* points                                         */            
//     Serial.printf("second =%d\r\n",  (uint8_t)(ptm->tm_sec));               /* seconds                                         */
// #endif

    if(machine->ds1302 == HAVE_DS1302) {
      //TOTO: Update time to DS1302
      DS1302.DStime.year   = (uint8_t)((ptm->tm_year+1900)%100);                            /* year                                         */
      DS1302.DStime.month  = (uint8_t)(ptm->tm_mon+1);                                      /* month                                       */
      DS1302.DStime.day    = (uint8_t)(ptm->tm_mday);                                       /* Date                                       */ 
      DS1302.DStime.week   = (uint8_t)(ptm->tm_wday);                                       /* week                                       */            
      DS1302.DStime.hour   = (uint8_t)(ptm->tm_hour);                                       /* time                                         */
      DS1302.DStime.minute = (uint8_t)(ptm->tm_min);                                        /* points                                         */            
      DS1302.DStime.second = (uint8_t)(ptm->tm_sec);                                        /* seconds                                         */
      DS1302.setTime();                                                                 /* Update time to DS1302                            */
    }

    /* Update if needed Daylight hours Prism calibration Weather*/
#if(EINK_UI_LANGUAGE == EINK_LANGUAGE_CHINESE) 
    if(machine->needFixPrism == PRISM_NEED_FIX) {
      city = FindAngle.find_city(config->locaEW, config->locaSN);            /* Configure city codes based on latitude and longitude*/
#if DEBUG_INFO_EN      
      Serial.printf("City ID %s\n", FindAngle.get_city_id(city) );  
#endif

      forecast.config(config->weatherpwd, FindAngle.get_city_id(city), "c");   /*  Configure weather-knowing request information Celsius/Fahrenheit*/

      /* Get weather from internet  */
      if(forecast.update()){  // Weather information updated successfully

        machine->weatherToday.day   = forecast.getDayCode(0);
        machine->weatherToday.night = forecast.getNightCode(0);
        machine->weatherToday.high  = forecast.getHigh(0);
        machine->weatherToday.low   = forecast.getLow(0);
#if DEBUG_INFO_EN
        Serial.printf("Weather update success ^_^ %d-%d,%d-%d \r\n", machine->weatherToday.day, machine->weatherToday.night, machine->weatherToday.high, machine->weatherToday.low );   
#endif
      } else { /* Failed to get weather*/
#if DEBUG_INFO_EN      
        Serial.print("Weather update Failed. ");   
        Serial.print("Server Response: ");          // Output server response status code for users to find problems
        Serial.println(forecast.getServerCode());   // Seniverse weather server error code descriptions are available at https://docs.seniverse.com/api/start/error.html
#endif             
        /* After the weather acquisition fails, it is forced to be sunny.       */
        machine->weatherToday.day = WEATHER_SUNNY_DAY;    
      } 
    }
    #endif 
                      
    /********* Close after synchronization task is completedWIFI **********/
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);                                                              /* After WiFi is turned off, there will be serial port information output, which may not be fully displayed before the Ink Screen is refreshed. */

    /* Update local self-built year, month, day, week */
    machine->year      = ptm->tm_year+1900;
    machine->month     = ptm->tm_mon+1;
    machine->day       = ptm->tm_mday;
    machine->weekday   = ptm->tm_wday;

    if(machine->ds1302 == NONE_DS1302) {                                               /* Use local self-built perpetual calendar records when there is no DS1302 */
      if(machine->needFixPrism == PRISM_NEED_FIX) {                                        
          machine->rtc_adj   = 1.0;                                                    /* Reset RTC error calibration during initial installation and early morning calibration*/
      } else {
        if( machine->day == ptm->tm_mday) {                                            /* RTC calibration is only performed on the same day */
          /* Day calibration time only adjusts rtc_adj          */
          machine->rtc_adj   =  ((double)(machine->timemsNow  - machine->wifilog))/   /*The real elapsed time divided by the elapsed time considered by RTC */
                                ((double )(timeCurrent-machine->wifilog));                         
        } else {                                                                       /* A situation that spans the sky                           */
          machine->rtc_adj   = 1.0;                                                    /* Reset RTC error calibration during initial installation and early morning calibration  */  
        }
      }  
    } else {                                                                           /* When DS1302 is available, the update time is to DS1302      */ 
        machine->rtc_adj   = 1.0;                                                      /* Internal rtc error reset (actually not used) to avoid ugly printing   */
    }
    machine->wifilog  = machine->timemsNow + millis();                                 /* Record the initial installation and early morning calibration time */  

#if USE_EINK_SCREEN
    Epaper.updateRunning(machine, WIFI_CONNECT_SUCCESS);
#endif

#if DEBUG_INFO_EN
    Serial.println("update time from Internet success^_^");
#endif

  } else { /* Failed to obtain time, use local perpetual calendar to calculate */
    /********* Close after synchronization task is completedWIFI **********/
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    machine->weatherToday.day = WEATHER_SUNNY_DAY;                                   /* Force weather to be sunny                               */    

    Update_locol_calendar(machine);                                                  /* Drive local perpetual calendar update                           */

#if USE_EINK_SCREEN
    Epaper.updateRunning(machine, WIFI_CONNECT_FAILED);                                /* Refresh the screen and mark WiFi failure                       */
#endif
  }   

  timeCurrent = machine->timemsNow + millis();        //Current time ms    
#if DEBUG_INFO_EN             
  Serial.printf("TimeUpdate[%d:%d:%d] \r\n",(uint32_t)(timeCurrent/3600000), (uint32_t)((timeCurrent%3600000)/60000),  (uint32_t)((timeCurrent%60000)/1000));
#endif

  UpdateRTCmemory_PrismFix(machine, config);               /* After updating the time, calculate the start time of solar insolation and update RTC_memory. If you need to calibrate the prism, calibrate it. */

}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * Code: Global State Machine
------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************
*   Function name: prcess_global_wifi
*   Function description: Handle WiFi connection and synchronization related matters
*   Parameter list: Global_State global variable top-level state
*             config  :Initial installation structure
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
void prcess_global_wifi( _GLOBAL_MACHINE * machine, _FIXED_CONFIG * config) 
{
  
  switch(machine->wifi_state )
  {
    /* Power on reset and connect to wifi */
    case WIFI_POWER_CONFIG :
      WIFI_PowerOn_Handle(machine, config );
    break;

    /* Sleep wake up connection wifi */
    case WIFI_RESET_CONFIG :
      WIFI_Reset_Handle( machine, config );
    break;

    /* Prepare to enter Soft AP   */
    case WIFI_ENTER_SOFTAP :
      WIFI_EnterSoftAP_Handle( machine , config);
    break;

    /* Wait for the user to fill in the web page parameters */
    case WIFI_APWAIT_CONNECT :
      WIFI_APwait_Handle( machine, config );
    break;

    /* Connecting to the router in STATION mode */
    case  WIFI_STATION_CONNECTED  :
      WIFI_Station_Handle( machine , config);
    break;

    default:
    break;
  }     
}

/**********************************************************************************************************************
*   Function name: prcess_global_boot
*   Function description: Handle related transactions after reset (including power-on reset and sleep wake-up) (transient)
*   Parameter list: Global_State global variable top-level state
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
void prcess_global_boot( _GLOBAL_MACHINE * machine, _FIXED_CONFIG *config,  _MOTO_CTRL * moto) 
{
    uint32_t timeCurrent, BATvaltage;
    uint8_t newday_fix_prism ;

  /* Restore time, date, weather and other information from RTC memory*/
    Resume_Data_from_RTCmemory(machine, moto);

    String rst = ESP.getResetReason();  
#if DEBUG_INFO_EN  
    Serial.println(rst.c_str());   
#endif    
    /********* Power on the switch ************************************************************/
     if (strcmp(rst.c_str(), "Deep-Sleep Wake") != 0) {

      delay(10);                                                          /* Wait for stable power on                       */
      DS1302.setTime();                                                      /* Set default time to DS1302               */    
      machine->ds1302   = DS1302.getTime();                                  /* After powering on for the first time, repeatedly check whether DS1302 exists.     */
#if DEBUG_INFO_EN      
      Serial.printf("power up reset ClockIC:%d\r\n",machine->ds1302);   
#endif     
      machine->rst_source = RST_SOURCE_POWRON;                            /* Log reset source as power-on              */

      /******  Get battery voltage ********************************/
      BATvaltage = Adc.Get_battery_voltage() ;                               /* Get battery voltage timeCurrent */
      Serial.printf("BAT_V =%d\r\n", BATvaltage);    

      /******  Verify infrared tube ADC sampling value *************************/
      Adc.adc_prepare( IRF_VAL);                                               /* Also turn off the ADC when stopping spinning  */
      BATvaltage = Adc.get_adc_value( IRF_VAL);                                /* Read the infrared tube voltage value      */  
      Adc.adc_prepare( ADC_OFF);                                               /* Also turn off the ADC when stopping spinning  */
      Serial.printf("IR_val=%d\r\n", BATvaltage);     
      
      /* In order to facilitate packaging, the prism is calibrated here once and placed in a horizontal attitude.*/
      moto->fixLocation();                                                    /* Start position calibration                  */               
      while( moto->enableFix == MOTO_FIX_BUSY ) {
        moto->roundScan();                                                    /* Motor action scan and enter BUSY state    */
        yield();                                                                  /* Clear watchdog to prevent stuck           */   
      }
      moto->enableFix = MOTO_FIX_NULL;                                            /* Calibration status reset                  */
      
      moto->goToAbsLocation(90.0);                                          /* The motor starts to move and adjust to the horizontal position    */
      do {
        moto->roundScan();                                                    /* Motor action scan and enter BUSY state    */
        yield();                                                                  /* Clear watchdog to prevent stuck           */   
      }  while(moto->state != MOTO_IDLE ) ;  


      machine->wifi_state   = WIFI_POWER_CONFIG ;      /* Enter WiFi initial installation settings       */
      machine->global_state = GLOBAL_WIFI ;            /* Handle WiFi affairs           */
      machine->needFixPrism = PRISM_NEED_FIX;          /* The prism needs to be updated and calibrated during initial installation.  */
      /* Screen prompt setting tutorial  */
#if USE_EINK_SCREEN
      Epaper.dispConfig();
#endif  
                               
    /********* sleep wakeup start ************************************************************/
    } else {
      machine->ds1302   = DS1302.getTime();                               /* Check if DS1302 exists     */
      BATvaltage = Adc.Get_battery_voltage() ;                             /* Get battery voltage timeCurrent */

      Read_config_fromFlash(config, 1);             /* Read configuration parameters from Flash    */                  

      if(BATvaltage < config->LowPwrThreshold ) {                       /* Only determine whether the battery is low when waking up from sleep   */
#if DEBUG_INFO_EN  
        Serial.printf("BAT_V=%d\r\n", BATvaltage);    
        delay(30);                                        /* Wait for serial port output to complete       */
#endif

        machine-> global_state = GLOBAL_LOW_POWER;        /* State machine transitions to LOW_POWER  */

      } else {

#if DEBUG_INFO_EN  
        Serial.printf("V%d:%d\r\n", BATvaltage,machine->ds1302);    /* Check voltage and DS1302 */             
#endif


      if(machine->ds1302 == NONE_DS1302) {
        timeCurrent = (uint32_t)(((double)(machine->timemsNow + millis() - machine->wifilog))*machine->rtc_adj) + machine->wifilog;     /* Calculate current time            */     
#if DEBUG_INFO_EN             
        Serial.printf("wake up@[%d:%d:%d] \r\n",(uint32_t)(timeCurrent/3600000), (uint32_t)((timeCurrent%3600000)/60000),  (uint32_t)((timeCurrent%60000)/1000));
#endif
      }else {                                                           
        Serial.printf("wake up@%02d-%02d-%02d[%d] %02d:%02d:%02d\r\n",  \
        DS1302.DStime.year, DS1302.DStime.month, DS1302.DStime.day, DS1302.DStime.week, \
        DS1302.DStime.hour, DS1302.DStime.minute, DS1302.DStime.second );  
      
        machine->timemsNow = DS1302.DStime.hour*3600000 + DS1302.DStime.minute*60000 +  DS1302.DStime.second*1000; /* Update wake time from DS1302 */
        timeCurrent = machine->timemsNow + millis();                              /* Wake up time + running time = current time    */
      }

      machine->rst_source = RST_SOURCE_WAKEUP;                                     /* Record the reset source as sleep wake-up or manual reset */

      newday_fix_prism =0;

      if((timeCurrent > UPDATE_TIME_PRISM) && (machine->ds1302 == NONE_DS1302)) {
        newday_fix_prism = 1;
      }

      if(DS1302.DStime.hour < 2 && machine->ds1302 == HAVE_DS1302) {             /*The maximum sleep particle size is 1H. There must be a wake-up call within 0~2 hours in the morning.*/
        newday_fix_prism = 1;
      }

      if(newday_fix_prism) {                                                        /* More than 1 day             */
#if DEBUG_INFO_EN      
          Serial.println("new day update wifi date" );   
#endif
          machine->wifi_state   = WIFI_RESET_CONFIG ;      /* Enter WiFi initial installation settings       */ 
          machine->global_state = GLOBAL_WIFI ;            /* Handle WiFi affairs           */
          machine->needFixPrism = PRISM_NEED_FIX;          /* The prism needs to be updated and calibrated when crossing the sky  */

      } else {

//        machine->weatherToday.day = WEATHER_SUNNY_DAY; /*Debugging uses force to be sunny */

        if( machine->weatherToday.day < WEATHER_M_OVERCAST ) {   /* weather is sunny or cloudy      */
            /* Determine whether it is during the sunshine period */
            if(machine->shine_start< timeCurrent &&  timeCurrent < machine->shine_stop ) {
              if(((timeCurrent - machine->shine_start)% TIME_CORRECT_PERIOD_MS) < 300000  ) { /*The time until sunshine begins is an integral multiple of TIME_CORRECT_PERIOD_MS hours */
                if(machine->ds1302 == NONE_DS1302) {
#if DEBUG_INFO_EN              
                  Serial.println("same day [no DS1302] -> update wifi time");   
#endif
                  machine->wifi_state   = WIFI_RESET_CONFIG ;      /* Enter WiFi initial installation settings       */ 
                  machine->global_state = GLOBAL_WIFI ;            /* Handle WiFi affairs           */
                  machine->needFixPrism = PRISM_NOT_FIX;           /* Only update time on the hour         */
                } else {
#if DEBUG_INFO_EN              
                Serial.println("same day -> then precess local");   
#endif
                machine->global_state = GLOBAL_LOCAL ;            /* Handle local affairs           */  
                }

              } else {  /* Less than the hour*/
#if DEBUG_INFO_EN              
                Serial.println("same day -> then precess local");   
#endif
                machine->global_state = GLOBAL_LOCAL ;            /* Handle local affairs           */  
              }
            } else { /* Not during daylight hours */
#if DEBUG_INFO_EN            
              Serial.println("same day -> then precess local");   
#endif
              machine->global_state = GLOBAL_LOCAL ;            /* Handle local affairs           */              
            }
        } else { /* Not a sunny day */
#if DEBUG_INFO_EN        
            Serial.println("same day -> then precess local");   
#endif
            machine->global_state = GLOBAL_LOCAL ;            /* Handle local affairs           */             
        }
      }   
#if DEBUG_INFO_EN 
              delay(5);                                      /* Wait for serial port output to complete       */ 
#endif     
    }         
  }
}

/**********************************************************************************************************************
*   Function name: prcess_global_local
*   Function description: Process local operation transactions (scan state)
*   Parameter list: Global_State global variable top-level state
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
void prcess_global_local( _GLOBAL_MACHINE * machine, _FIXED_CONFIG * config) 
{
  uint32_t timeCurrent;

  /* Motor action has the highest priority */
//  machine->weatherToday.day = WEATHER_SUNNY_DAY;           /*Debugging uses force to be sunny */
  if(Moto.state == MOTO_IDLE) {
    if( machine->weatherToday.day < WEATHER_M_OVERCAST ) {   /* weather is sunny or cloudy      */
#if !OFFLINE_TEST    
#if DEBUG_INFO_EN
      Serial.println("sunny day"); 
#endif      
#endif

#if OFFLINE_TEST   /******************** The process of stand-alone testing **************/
     if(1 ) {
          Offline_Test((_GLOBAL_MACHINE *) &Machine,  (_MOTO_CTRL *) &Moto);
#else 
      /* Determine whether it is during the sunshine period */ 
      if(machine->ds1302 == NONE_DS1302) { 
        timeCurrent = (uint32_t)(((double)(machine->timemsNow + millis() - machine->wifilog))*machine->rtc_adj) + machine->wifilog;     /* Calculate current time            */ 
      }else{
        timeCurrent =  DS1302.DStime.hour*3600000 + DS1302.DStime.minute*60000 +  DS1302.DStime.second*1000 + millis();
      }
      if(machine->shine_start< timeCurrent &&  timeCurrent < machine->shine_stop ) {

    /******************** Formal process when connecting to WiFi **************/ 
          /* Calculate the solar altitude angle and azimuth angle at the current time Calculate the optimal dispersion position of the motor  */
          Read_config_fromFlash(config, 1);   // Read latitude and longitude, altitude, and window orientation information from Flash

          SunPosition.location_earth.longitude = config->locaEW;  // Longitude
          SunPosition.location_earth.latitude = config->locaSN;   // Latitude
          SunPosition.location_earth.altitude = config->height;   // altitude

          SunPosition.time_earth.UTC = (double)config->UTC;        //time zone  
          SunPosition.time_earth.year  = machine->year ;             //year
          SunPosition.time_earth.month = machine->month;             //month
          SunPosition.time_earth.day   = machine->day;               //day 

          /* If there was effective lighting last time */
          if(machine->ds1302 == NONE_DS1302) { 
            timeCurrent = (uint32_t)(((double)(machine->timemsNow + millis() - machine->wifilog))*machine->rtc_adj) + machine->wifilog;     /* Calculate current time            */ 
          }else{
            timeCurrent =   DS1302.DStime.hour*3600000 + DS1302.DStime.minute*60000 +  DS1302.DStime.second*1000 + millis();
          } 
          if(machine->best_rotate != -1) {
            timeCurrent = timeCurrent + millis() + 300000;   //Calculate the best angle in the next 5 minutes
          } else {
            //timeCurrent = timeCurrent ;   //current momentms
          }
          
          timeCurrent %= 86400000;                       // The constraint time is within one day
          SunPosition.time_earth.hour = floor(timeCurrent/3600000);          //time
          SunPosition.time_earth.min  = floor((timeCurrent%3600000)/60000);  //points
          SunPosition.time_earth.sec  = floor((timeCurrent%60000)/1000);     //seconds

          SunPosition.sun_position((_SUN_VIEW_LOCATION*) &SunPosition.sun_pos,(_YOUR_TIME*) &SunPosition.time_earth,(_YOUR_LOCATION*) &SunPosition.location_earth );  /* Calculate the solar altitude angle at the current time */
          FindAngle.find_best_angle((_BEST_RESULT*)&FindAngle.Best, SunPosition.sun_pos.zenith, SunPosition.sun_pos.azimuth, config->windir);                     /* Calculate optimal prism pose        */

          /* There is light and it can form a rainbow */
          if (FindAngle.Best.flag == 0 && FindAngle.Best.max_refract2_rotate != -1){

            /* If there was effective lighting last time */
            if(machine->best_rotate != -1) {
              /* Take the larger rotation angle of two adjacent lightings */
              if(machine->best_rotate > FindAngle.Best.max_refract2_rotate) {
#if DEBUG_INFO_EN                
                Serial.println("Use now angle");  
                Serial.printf("Moto Angle: %.2f \r\n",  machine->best_rotate +60   );   
#endif
                Moto.goToAbsLocation(machine->best_rotate + 60.0);      /* The motor starts to move              */    
              } else {
#if DEBUG_INFO_EN                
                Serial.println("Use future 5min angle");  
                Serial.printf("Moto Angle: %.2f \r\n",  FindAngle.Best.max_refract2_rotate +60   );   
#endif                
                Moto.goToAbsLocation(FindAngle.Best.max_refract2_rotate + 60.0);  /* The motor starts to move              */
              }
            } else {
#if DEBUG_INFO_EN              
              Serial.println("Use now angle"); 
              Serial.printf("Moto Angle: %.2f \r\n",  FindAngle.Best.max_refract2_rotate +60   );   
#endif              
              Moto.goToAbsLocation(FindAngle.Best.max_refract2_rotate + 60.0);     /* The motor starts to move              */
            }
            machine->best_rotate = (float)FindAngle.Best.max_refract2_rotate ;                     /* Back up this effective lighting and remember it during sleep */

            if(machine->ds1302 == NONE_DS1302) { 
              timeCurrent = (uint32_t)(((double)(machine->timemsNow + millis() - machine->wifilog))*machine->rtc_adj) + machine->wifilog;     /* Calculate current time            */ 
            }else{
              timeCurrent  =  DS1302.DStime.hour*3600000 + DS1302.DStime.minute*60000 +  DS1302.DStime.second*1000 + millis();
            }
#if DEBUG_INFO_EN             
            Serial.printf("TimeNow[%d:%d:%d] \r\n",(uint32_t)(timeCurrent/3600000), (uint32_t)((timeCurrent%3600000)/60000),  (uint32_t)((timeCurrent%60000)/1000));
            Serial.printf("RTC_adj=%.4f\r\n", machine->rtc_adj);
#endif            
            Moto.roundScan();                                       /* Motor action scan, enter BUSY     */ 
            
          /* There is light, but it cannot form a rainbow */
          } else {
#if DEBUG_INFO_EN              
            Serial.println("in shine time,But cnt be rainbow");
#endif
            if(machine->best_rotate != -1) { /* If there was a rainbow last time, it means it's the afternoon session */
              Enter_DeepSleep(machine, LONG_SLEEP_TIME);          /* Go to sleep 1h               */
            } else {
              Enter_DeepSleep(machine, 20*60e3);           /* Go to sleep 20min            */
            }
            machine->best_rotate = -1;                    /* Mark that this time it will not be a rainbow            */
          }
     
#endif  

      } else {
#if DEBUG_INFO_EN          
        Serial.println("not in shine time");
#endif

#if !OFFLINE_TEST     
        /*** Dynamically adjust sleep duration based on the relationship between the current time and daylight hours ****/
        if(machine->ds1302 == NONE_DS1302) { 
          timeCurrent = (uint32_t)(((double)(machine->timemsNow + millis() - machine->wifilog))*machine->rtc_adj) + machine->wifilog;     /* Calculate current time            */ 
        }else{
          timeCurrent =   DS1302.DStime.hour*3600000 + DS1302.DStime.minute*60000 +  DS1302.DStime.second*1000 + millis();
        }
        if(timeCurrent < machine->shine_start){
            if( machine->shine_start -timeCurrent < 36e5) {  /* Less than 1 hour before the light starts*/
              Enter_DeepSleep(machine, 1.2*(machine->shine_start -timeCurrent)); /* Go to sleep and wake up when light starts, covering RTC error    */
            } else {
              Enter_DeepSleep(machine, LONG_SLEEP_TIME);                          /* Go to sleep 1h              */
            }           
        } else {
            Enter_DeepSleep(machine, LONG_SLEEP_TIME);                            /* Go to sleep 1h              */
        }

#endif
      }
         
    } else {                                                   /* Sleep directly for 1 hour in rainy or snowy weather   */
      /************ The screen displays date, weather, but does not display time ********/ 
#if !OFFLINE_TEST 
#if DEBUG_INFO_EN  
      Serial.print("heavy cloudy or rain/snow day");
      Serial.printf("Weather code %d-%d,%d-%d \r\n", machine->weatherToday.day, machine->weatherToday.night, machine->weatherToday.high, machine->weatherToday.low );   
#endif

      /************* Go to sleep **************************/
      Enter_DeepSleep(machine, LONG_SLEEP_TIME);                   /* Go to sleep 1h               */
#endif
    }

    Moto.roundScan();                  /* task scan                 */
  } else {
    Moto.roundScan();                  /* Motor action scan              */
  }


  /* Action completed or no action required */
  if(Moto.state == MOTO_IDLE) {

    if(Moto.enableFix == MOTO_FIX_DONE ) {              /* If calibration is completed during this startup      */
      Moto.enableFix  = MOTO_FIX_NULL;                  /* No sleep, track the sun once in the next cycle */
      
    } else {
#if !OFFLINE_TEST 
    Enter_DeepSleep(machine, 5*60e3);                   /* Enter sleep and wake up after 5 minutes      */   
#endif
    }
  }   
}

/**********************************************************************************************************************
*   Function name: prcess_global_lowpwr
*   Function description: Handle the situation when the battery is low
*   Parameter list: Global_State global variable top-level state
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
void prcess_global_lowpwr( _GLOBAL_MACHINE * machine) 
{
    /************* Screen prompts low battery ********************/
#if USE_EINK_SCREEN
     Epaper.dispLowPwr();
#endif

   /************* Go to sleep **************************/
#if DEBUG_INFO_EN     
    Serial.println("low power enter long sleep");
#endif
    Enter_DeepSleep(machine, LONG_SLEEP_TIME);                         /* Go to sleep */
    
}

/**********************************************************************************************************************
*   Function name: global_state_machine
*   Function description: Top-level global state machine
*   Parameter list: Global_State global variable top-level state
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
void global_state_machine( _GLOBAL_MACHINE * machine, _FIXED_CONFIG *config){
  switch(machine->global_state )
  {
    /* Handling actions after reset */
    case GLOBAL_BOOT:
      prcess_global_boot(machine,  config, (_MOTO_CTRL*) &Moto);
    break;
   
   /* Handle WiFi related matters */
    case GLOBAL_WIFI:
      prcess_global_wifi(machine, config);
    break;

    /* Handle local control transactions*/
    case GLOBAL_LOCAL:
      prcess_global_local(machine,config);
    break;

    /* Handle low battery matters */
    case GLOBAL_LOW_POWER :
      prcess_global_lowpwr(machine);
    break;

    default:
#if DEBUG_INFO_EN     
    Serial.println("WARNING:no this case!! \r\n");
#endif    
    break;    
  }
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * Code: Serial console
------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************
*   Function name: UART_Console
*   Function description: Serial console framework to facilitate conditional triggering
*   Parameter list: None
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2018.6.29 Original creation version V1.0
*
**********************************************************************************************************************/
#if OFFLINE_TEST
void UART_Console(void)
{           
    unsigned int i;

    if(Serial.available()){
      guc_UART0cmd = Serial.read();
    }
    
    if(guc_UART0cmd != 'Z')
    {   
        switch (guc_UART0cmd)
        {
            case 'a':  /* Simulate the action of synchronizing data from Wifi */      
                Serial.println("Eink_Disp_Config\r");  
                delay(2e3);
                Epaper.dispConfig(); 
                Epaper.sleep();
                Serial.println("Eink_Disp_Config done\r");
                break;    


            case 'b':  /* Simulate the action of synchronizing data from Wifi */
                Serial.println("Eink_Disp_TimeOut\r");
                delay(2e3);  
                Epaper.dispTimeOut(); 
                Epaper.sleep();                
                Serial.println("Eink_Disp_TimeOut done\r");          
                break;    

            case 'm':  //  Add the action to be performed by the command
                Serial.println("Eink_Disp_LowPower\r");
                delay(2e3); 
                Epaper.dispLowPwr();
                Epaper.sleep();                
                Serial.println("Eink_Disp_LowPower done\r");
                break;   

            case 'n':  // Add the action to be performed by the command
                Serial.println("Eink_Disp Clean\r");
                delay(2e3); 
                Epaper.dispClean();
                Epaper.sleep();                
                Serial.println("Eink_Disp_Clean done\r") ;             
                break;              
            
            case 'u':  //  Add the action to be performed by the command  
                Serial.println("turn up 8");  
                Moto.ctrl(MOTO_TURN_UP, 8);
                break;    

            case 'd':  //  Add the action to be performed by the command  
                Serial.println("turn down 8");  
                Moto.ctrl(MOTO_TURN_DN, 8);
                break;  

            case 'e':  //  Add the action to be performed by the command  
                Serial.println("go to 90");  
                Moto.goToAbsLocation(90); 
                break;    

            case 'f':  /* Trigger generator calibration action */
                Serial.println("fix moto location");                
                Moto.fixLocation();  
                Moto.roundScan();            /* Motor action scan and enter BUSY state*/
                break;      

            case 't':  //  Add the action to be performed by the command
                Serial.println("Eink_Disp_Running\r");
                delay(2e3);  
                Machine.year = 2023;
                Machine.month = 2;
                Machine.day = 23;
                Machine.weekday = 4;
                Machine.weatherToday.day  = 31;//WEATHER_P_CLOUD_DAY ;
                Machine.weatherToday.night= 32;//WEATHER_M_CLOUD_NIGHT;
                Machine.weatherToday.high = 23;
                Machine.weatherToday.low  = -15;

                // Eink_UpdateRunning((_GLOBAL_MACHINE *) &Machine, WIFI_CONNECT_SUCCESS);
                Epaper.updateRunning((_GLOBAL_MACHINE *) &Machine, WIFI_CONNECT_FAILED);
                Epaper.sleep();                
                Serial.println("Eink_Disp_TimeOut done\r");                          
                break;     

            case 'j':  //  Add the action to be performed by the command
                Config.structADJ++;
                Serial.printf("UCS adjust turn up %d\r\n", Config.structADJ);  
                break;      

            case 'k':  //  Add the action to be performed by the command
                Config.structADJ--;
                Serial.printf("UCS adjust turn down %d\r\n", Config.structADJ);  
                break;   

            case 'p':  //  Add the action to be performed by the command
                Serial.println("force triger moto adjust ");  
                user_triger = 1; 
                prestart    = 0;
                break;       
                
            case 'c':  //  Add the action to be performed by the command     
                // ***** Company location ****************
                SunPosition.location_earth.longitude = 113.954266;
                SunPosition.location_earth.latitude = 22.580521;
                SunPosition.location_earth.altitude = 80;

                SunPosition.time_earth.UTC = 8;      //time zone  
                SunPosition.time_earth.year = 2022; //year
                SunPosition.time_earth.month = 12;   //month
                SunPosition.time_earth.day = 12;  //day 
                SunPosition.time_earth.hour = 12;    //time
                SunPosition.time_earth.min = 00;     //points
                SunPosition.time_earth.sec = 00;     //seconds

                printf(" start print:\n");
                for (i = 0; i < 1440; i++) {
                    yield( ); /* Feed the dog to prevent the program from getting stuck */                  
                    if (i%10 == 0) {
                        Serial.printf(" ...\r\n");
                    }

                    SunPosition.time_earth.hour = (unsigned int)(i / 60); // Change hours
                    SunPosition.time_earth.min = i % 60;  // Change minutes

                    SunPosition.sun_position((_SUN_VIEW_LOCATION*) &SunPosition.sun_pos,(_YOUR_TIME*) &SunPosition.time_earth,(_YOUR_LOCATION*) &SunPosition.location_earth );
                    FindAngle.find_best_angle((_BEST_RESULT*)&FindAngle.Best, SunPosition.sun_pos.zenith, SunPosition.sun_pos.azimuth, 178.0); // Calculate optimal prism pose
                    Serial.printf("%.3f, ", FindAngle.Best.max_refract2_rotate);

                }

                Serial.printf("program run complete\n");
                break;       

            case 'x':  //  Add the action to be performed by the command    
                // Serial.println("go to 60");  
                // Moto_GoTo_AbsLocation((_MOTO_CTRL *) &Moto, 60);         
              i =  Adc.Get_battery_voltage( );            /* Read battery voltage   */ 
              Serial.printf(" BAT_val: %d\r\n", i);
            break;

            case 'y':  //  Add the action to be performed by the command     

                // Serial.println("go to 150");  
                // Moto_GoTo_AbsLocation((_MOTO_CTRL *) &Moto, 150);     

              Adc.adc_prepare( IRF_VAL);           /* Also turn off the ADC when stopping spinning */
              delayMicroseconds(100);          /* Wait for power to stabilize        */;
              i = Adc.get_adc_value( IRF_VAL);     /* Read the infrared tube voltage value   */ 
              delayMicroseconds(5);  
              Adc.adc_prepare( ADC_OFF);           /* Also turn off the ADC when stopping spinning */
              Serial.printf(" IR_val: %d\r\n", i);            
            break;         

            case 'q':  //  Add the action to be performed by the command  
                Serial.println("go to 130");  
                Moto.goToAbsLocation(130);       
              // digitalWrite(ADC_CTRL, LOW);   /* Power path open   */
              // Serial.printf(" ADC_CTRL = LOW OFF\r\n");
            break;

            case 'o':  //  Add the action to be performed by the command 
                Serial.println("go to 100");  
                Moto.goToAbsLocation(100);     
              // digitalWrite(ADC_CTRL, HIGH);   /* power path closed   */
              // Serial.printf(" ADC_CTRL = HIGH ON\r\n");           
            break;

            case 'A':  //  Add the action to be performed by the command 
              DS1302.DStime.year   = 22;                               /* year                                         */
              DS1302.DStime.month  = 5;                               /* month                                       */
              DS1302.DStime.day    = 22;                               /* Date                                       */ 
              DS1302.DStime.week   = 7;                               /* week                                       */            
              DS1302.DStime.hour   = 23;                               /* time                                         */
              DS1302.DStime.minute = 59;                               /* points                                         */            
              DS1302.DStime.second = 47;                               /* seconds                                         */
              Serial.println("Set time to DS1302");  
              DS1302.setTime();    
            break;

            case 'B':  //  Add the action to be performed by the command 
              DS1302.DStime.year   = 89;                               /* year                                         */
              DS1302.DStime.month  = 11;                               /* month                                       */
              DS1302.DStime.day    = 25;                               /* Date                                       */ 
              DS1302.DStime.week   = 7;                               /* week                                       */            
              DS1302.DStime.hour   = 22;                               /* time                                         */
              DS1302.DStime.minute = 15;                               /* points                                         */            
              DS1302.DStime.second = 33;                               /* seconds                                         */
              Serial.println("Set time to DS1302");  
              DS1302.setTime();    
            break;           

            case 'C':  //  Add the action to be performed by the command 
              Serial.println("Read time from DS1302");  
              DS1302.getTime();  

              Serial.printf("%02d-%02d-%02d[%d] %02d:%02d:%02d\r\n",  \
              DS1302.DStime.year, DS1302.DStime.month, DS1302.DStime.day, DS1302.DStime.week, \
              DS1302.DStime.hour, DS1302.DStime.minute, DS1302.DStime.second );                          
            break;

            default:
                Serial.println("no match command");      
                break;					
        }       
        guc_UART0cmd = 'Z';
    }  
}
#endif 

/**********************************************************************************************************************
*   function name: setup()
*   Function description: Initialization phase in main function
*   Parameter list: None
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2022.10.18 Original creation version V1.0
*
**********************************************************************************************************************/
void setup( void ) {
  Serial.begin(115200);                                                 /* Initialize serial port        */
  Moto.IO_Init();                                                       /* Initialize stepper motor    */

#if DEBUG_INFO_EN
  Serial.println();
  Serial.println();
  Serial.println("************************************************************************");
  Serial.printf("Flash: %d kBytes\r\n", ESP.getFlashChipRealSize()/1024); /* View Flash size   */
#endif

  Machine.global_state = GLOBAL_BOOT;                                   /* Initialize global state machine  */
  WiFi.hostname("Rainbow_clock");                                       /* Set device name       */
  
}

/**********************************************************************************************************************
*   function name: loop() 
*   Function description: Main loop in main function
*   Parameter list: None
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2022.10.18 Original creation version V1.0
*
**********************************************************************************************************************/
void loop() {
#if OFFLINE_TEST
  //prcess_global_local((_GLOBAL_MACHINE *) &Machine, (_FIXED_CONFIG *) &Config);
  UART_Console();                                                                        /* Serial console debugging use        */
#else
  global_state_machine( (_GLOBAL_MACHINE *) &Machine, (_FIXED_CONFIG *) &Config);
#endif
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * END of file
------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/




