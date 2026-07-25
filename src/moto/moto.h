#ifndef __MOTO_H__
#define __MOTO_H__

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * Stepper motor GPIO pin declaration
------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define MOTO_A (5)
#define MOTO_B (4)
#define MOTO_C (12)
#define MOTO_D (15)

/* Stepper motor dynamic acceleration parameters */
#define MOTO_MAX_DELAY   5000.0 /* Maximum delay us*/
#define MOTO_MIN_DELAY   1800.0 /* Minimum delay us*/
#define MOTO_SPEED_RANGE   64.0 /* Acceleration phase */

/******** Function parameters *****************************/
#define MOTO_IDLE     0x00 /* Motor idle state */
#define MOTO_BUSY     0x01 /* Motor busy status */

#define MOTO_TURN_UP  0x0  /* rotate up     */
#define MOTO_TURN_DN  0x1  /* rotate down     */

#define MOTO_FIX_NULL 0x0  /* Disable motor calibration */
#define MOTO_FIX_BUSY 0x1  /* Enable motor calibration */
#define MOTO_FIX_DONE 0x2  /* Motor calibration completed */


/******** Motor data structure *********************/
typedef struct _MOTO_CTRL
{
    uint8_t  ctrl_list[8];        /* Stepper motor code               */
    uint32_t beat ;               /* Current motor beat [across life cycles] */
    uint8_t  beat_now;            /* Currently selected motor code         */
    uint8_t  turnDir ;            /* Motor rotation direction             */
    uint8_t  state ;              /* Current motor status             */
    uint8_t  enableFix;           /* Enable position calibration             */
    uint16_t steps;               /* Motor action task decrement        */
    uint16_t stepsn;              /* Motor action task increment drive delay amount */
    uint32_t standLoca;           /* Current motor position [across lifecycle] */
    uint16_t findLoca ;           /* step position increment             */
    uint16_t adc_tmp_val;         /* ADC temporary value               */
    float IR_log[4096];           /* Record IR calibration data          */

    /***********************************************************************************************************************
    *   Function name: Moto_IO_Init
    *   Function description: Motor interface initialization
    *   Parameter list: None
    *   Hardware output: None
    *   Return result: None
    ***********************************************************************************************************************/
    void IO_Init(void);

    /***********************************************************************************************************************
    *   Function name: Moto_round_Scan
    *   Function description: Motor rotation scanning
    *   Parameter list: moto: motor structure
    *   Hardware output: None
    *   Return result: None
    ***********************************************************************************************************************/
    void roundScan(void);

    /***********************************************************************************************************************
    *   Function name: Moto_Ctrl
    *   Function description: Motor control interface
    *   Parameter list: Dir: Motor rotation direction MOTO_TURN_UP MOTO_TURN_DN
    *             step: number of motor rotation steps
    *   Hardware output: None
    *   Return result: 0: Failure 1: Success
    ***********************************************************************************************************************/
    int8_t ctrl(uint8_t Dir, uint16_t step );

    /***********************************************************************************************************************
    *   Function name: Moto_Fix_Location
    *   Function description: Start motor initial position calibration
    *   Parameter list: moto: motor structure
    *   Hardware output: None
    *   Return result: None
    ***********************************************************************************************************************/
    void fixLocation(void);

    /***********************************************************************************************************************
    *   Function name: Moto_GoTo_AbsLocation
    *   Function description: The motor rotates to somewhere in the absolute coordinate system
    *   Parameter list: angle: Specify the absolute coordinate unit ° (degree)
    *   Hardware output: None
    *   Return result: None
    ***********************************************************************************************************************/
    void goToAbsLocation(float angle);

    /**********************************************************************************************************************
    *   Function name: LocationAnalyse
    *   Function description: Prism calibration program
    *   Parameter list: curv: original curve
    *             curve_len: original curve data length (<65536)
    *   Hardware output: None
    *   Return result: the position of the reflective pillar analyzed by StandLocation
    **********************************************************************************************************************/
    uint32_t LocationAnalyse(float *curv,  uint16_t curve_len);

    /**********************************************************************************************************************
    *   Function name: smooth_moving_lite
    *   Function description: Use the sliding window method to smooth curves
    *   Parameter list: curv: original curve
    *             curve_len: original curve data length (<65536)
    *   Hardware output: None
    *   Return result: None
    **********************************************************************************************************************/
    void smooth_moving_lite(float *curv,  uint16_t curve_len);

    /***********************************************************************************************************************
    *   Function name: moto_speed_up
    *   Function description: The motor gradually accelerates the beat delay
    *   Parameter list: steps
    *   Hardware output: None
    *   Return result: None
    ***********************************************************************************************************************/
    uint16_t moto_speed_up( double steps);
} _MOTO_CTRL; 


/********  *********************/
#define CURVE_LENGTH  4096                                                      /* Need smooth curve length                  */
#define SMMOOTH_SPAN  16                                                        /* The window size during smoothing filtering affects memory usage. */

#define FALLING_EDGE  0                                                         /* Falling edge macro definition                       */ 
#define RISING_EDGE   1                                                         /* Rising edge macro definition                       */

typedef struct __EdgeInfo
{
    uint16_t  type   ;                                                      /* Edge type FALLING_EDGE or RISING_EDGE */
    uint16_t  location   ;                                                  /* The position of the transition edge                         */ 
}EdgeInfo;

extern _MOTO_CTRL Moto;

#endif /* __MOTO_H__ */
