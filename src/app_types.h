#ifndef __APP_TYPES_H__
#define __APP_TYPES_H__

#include <stdint.h>
#include "config.h"

#ifndef RAINBOW_APP_TYPES_DEFINED
#define RAINBOW_APP_TYPES_DEFINED

/******** Global configuration structure updated during initial installation*********************/
typedef struct
{
    char     ssid[48];        /* Save wifi account        */
    char password[48];        /* Save wifi password        */
    char weatherpwd[32];      /* Save the weather private key      */
    double locaEW;            /* save longitude             */
    double locaSN;            /* save latitude             */
    double height;            /* save altitude             */
    double windir;            /* Save window orientation         */
     int32_t structADJ;       /* Fine-tuning the reflective column position of the hardware structure */   /* Implicit in the decimal part of the altitude x (x-0.5)*(30.0/(360.0/4096.0)) an adjustment range of plus or minus 15 degrees */ 
     int32_t UTC;             /* Set time zone             */
     char NTPserver[32];      /* Set default NTP server    */
    uint32_t  effect;         /* Is the marker parameter valid?     */
    uint32_t  toConnectWifi;  /* Is it possible to connect to WiFi?     */
    uint32_t  needWriteFlash;  /* Whether it is necessary to write to Flash   */
    int32_t  LowPwrThreshold;  /* Low battery reminder voltage      */
} _FIXED_CONFIG; 

extern _FIXED_CONFIG Config;

 /* Is the marker parameter valid?   */
 #define FIXED_CONFIG_EXPIRE    0       /* Initial installation settings are invalid     */
 #define FIXED_CONFIG_EFFECTIVE 1       /* Initial installation settings are valid     */

 /* Is it possible to connect to WiFi?   */
 #define TODO_CAN_NOT_WIFI      0       /* Can't connect to wifi   */
 #define TODO_CONNECT_WIFI      1       /* Can connect to WiFi     */

/* Do you need to update parameters to Flash?   */
 #define DO_NOT_WRITE_FLASH     0       /* No need to update Flash  */
 #define NEED_UPDATE_FLASH      1       /* Flash needs to be updated   */

typedef struct
{
  uint8_t day;   /* daytime weather*/
  uint8_t night; /* night weather */
   int8_t high;  /* maximum temperature */
   int8_t low;   /* minimum temperature */
}_WEATHER;

// https://seniverse.yuque.com/hyper_data/api_v3/yev2c3
#define WEATHER_SUNNY_DAY      0
#define WEATHER_SUNNY_NIGHT    1
#define WEATHER_FAIR_DAY       2
#define WEATHER_FAIR_NIGHT     3
#define WEATHER_H_CLOUD        4
#define WEATHER_P_CLOUD_DAY    5
#define WEATHER_P_CLOUD_NIGHT  6
#define WEATHER_M_CLOUD_DAY    7
#define WEATHER_M_CLOUD_NIGHT  8
#define WEATHER_M_OVERCAST     9
// Other weather code viewing URLs
// Manual weather query https://api.seniverse.com/v3/weather/daily.json?key=SUfADtTVXqqdqaJ8J&location=WS10730EM8EV&language=en&unit=c&start=0&days=3


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * global state machine
------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/******** Global status structure updated daily *********************/
typedef struct
{
    uint8_t  global_state;              /* global state machine                 */
    uint8_t  wifi_state ;               /* WiFi transaction state machine             */
    uint16_t  needFixPrism;             /* Whether to calibrate prism, lighting time, weather */
    uint32_t  enter_ap_time ;           /* Time to enter AP mode           */
    _WEATHER  weatherToday ;            /* Today's weather                  */  /* Across life cycles */
    uint32_t  shine_start ;             /* Window sunshine start time           */  /* Across life cycles */
    uint32_t  shine_stop ;              /* Window sunshine end time           */  /* Across life cycles */
    uint32_t  year ;                    /* year                        */  /* Across life cycles */
    uint32_t  month ;                   /* month                        */  /* Across life cycles */
    uint32_t  day ;                     /* day                        */  /* Across life cycles */
    uint32_t  weekday ;                 /* week                      */  /* Across life cycles */
    float    best_rotate;               /* The next best prism posture (5 minutes)   */   /* Across life cycles */
    uint32_t  sleepedTime ;             /* Last sleep time              */   /* Across life cycles */
    int32_t  timemsNow ;                /* boot time              */   /* Across life cycles */
    uint32_t wifilog;                  /* Timestamp when updating time when connected to wifi  */   /* Across life cycles */
    float    rtc_adj;                   /* RTC frequency offset fine-tuning 1.0 positive and negative adjustment  */   /* Across life cycles */
    uint8_t  rst_source;                /* Record the reset source of this startup         */
    uint8_t  ds1302;                    /* Mark if there is a DS1302 IC       */
    uint8_t  Reserved2;                 /* Reserve                     */
    uint8_t  Reserved3;                 /* Reserve                     */
} _GLOBAL_MACHINE; 

/* Global state machine state declaration */
#define GLOBAL_BOOT               (0)
#define GLOBAL_WIFI               (1)
#define GLOBAL_LOCAL              (2)
#define GLOBAL_LOW_POWER          (3)

/* WiFi state machine status statement */
#define  WIFI_POWER_CONFIG        (0)
#define  WIFI_RESET_CONFIG        (1)
#define  WIFI_ENTER_SOFTAP        (2)
#define  WIFI_APWAIT_CONNECT      (3)
#define  WIFI_STATION_CONNECTED   (4)

/* RTC memory update options  */
#define RTC_UPDATE_WIFI           (0)
#define RTC_UPDATE_SLEEP          (1)

/* Whether to update the calibration prism, lighting time, and weather */
#define PRISM_NOT_FIX             (0)
#define PRISM_NEED_FIX            (1)

/* Reset source recording options */
#define RST_SOURCE_POWRON         (0)  
#define RST_SOURCE_WAKEUP         (1)

/* Reset source recording options */
#define NONE_DS1302              (0)  
#define HAVE_DS1302              (1)

#endif /* RAINBOW_APP_TYPES_DEFINED */

#endif /* __APP_TYPES_H__ */
