//Global top-level function configuration options

#ifndef __CONFIG_H__
#define __CONFIG_H__

/****** operating mode  **********************************************************************/
#define OFFLINE_TEST    0             /* 1 development mode or 0 official mode                       */
#define DEBUG_INFO_EN   1             /* Whether to use debug mode, debug mode will print more things       */
#define USE_EINK_SCREEN 1             /* Whether to use ink screen                                */

/****** Default voltage to trigger low battery protection  ********************************************************/
#define BAT_LOW_GATE       600        /*Detect the battery voltage, if it is lower than this value, it will enter low power mode 930=3.7v 955=4v*/


/* Ink screen model list */
#define EINK_MODEL_SSD1680_250X122    0     /* Zhongjingyuan   */
#define EINK_MODEL_UD8251_250X122     1     /* Xinchuangtianyuan */
#define EINK_MODEL_SSD1680_212X104    2     /* Low resolution SSD1680 */
#define EINK_MODEL_PERVASIVE_212X104  3     /* Longting New Technology */

/* Ink screen interface language list */
#define EINK_LANGUAGE_CHINESE  0
#define EINK_LANGUAGE_ENGLISH  1

/****** Macro definitions related to the Ink Screen hardware platform********************************************************/
#define EINK_MODEL          EINK_MODEL_SSD1680_212X104 

/****** Ink screen display interface language*************************************************************/
#define EINK_UI_LANGUAGE  EINK_LANGUAGE_CHINESE

#endif /* __CONFIG_H__ */
