/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * Code: stepper motor
------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include <Arduino.h>
#include <math.h>
#include "moto.h"
#include "../adc/adc.h"
#include "../app_types.h"

/*---------------  Motor data definition-------------------------------------*/
_MOTO_CTRL Moto ={
    { 0x8, 0xc, 0x4, 0x6, 0x2, 0x3, 0x1, 0x9 },  /* Stepper motor code               */
    0,                                          /* Currently selected motor code          */
    0,                                          /* Current motor beat             */
    0,                                          /* Motor rotation direction             */
    MOTO_IDLE,                                  /* Current motor status             */
    MOTO_FIX_NULL,                              /* Enable position calibration             */
    0,                                          /* motor action tasks             */
    0,                                          /* Current motor position             */
    0,                                          /* step position increment             */
    0,                                          /* ADC temporary value              */   
    {0},                                        /* Record IR calibration data          */
};


/**********************************************************************************************************************
*   Function name: smooth_moving_lite
*   Function description: Use the sliding window method to smooth curves
*   Parameter list: curv: original curve
*             curve_len: original curve data length (<65536)
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2024.04.09 Original creation version V1.0
*
**********************************************************************************************************************/
void _MOTO_CTRL::smooth_moving_lite(float *curv,  uint16_t curve_len)
{
  uint16_t i, j, width;
  uint16_t begin_end_len, center_len;
  double sumbegin, sumend;
  float begin_end_cache[SMMOOTH_SPAN];                                              /* Cache new sequence of start and end parts     */
  float Center_FIFO[SMMOOTH_SPAN];                                                  /* Partial caching of intermediate segments            */
  uint16_t FIFO_wt_Idx ;                                                            /* FIFO write pointer                   */
  uint16_t FIFO_rd_Idx ;                                                            /* FIFO read pointer                   */

  width = (SMMOOTH_SPAN > curve_len)? curve_len: SMMOOTH_SPAN;                      /* The sliding window length must be less than or equal to the sequence length    */

  if(SMMOOTH_SPAN>2) {
    width = (width-1) + (width&0x1);                                                /* Force the sliding window length to be odd  */
    begin_end_len =(width-1)/2;
    center_len = curve_len-width+1;
    
    j = 0;
    sumbegin = 0;
    sumend   = 0;

    for (i=0; i<width-2; i++) { 
        sumbegin += curv[i] ;
        sumend   += curv[curve_len-1-i];

        if( (i&0x1) == 0)  {        
            begin_end_cache[j]                = (float) (sumbegin/(i+1));
            begin_end_cache[SMMOOTH_SPAN-1-j] = (float) (sumend/(i+1));
            j = j +1;
        }    
    }

    FIFO_wt_Idx = 0;                                                                 /* FIFO write pointer initialization            */
    FIFO_rd_Idx = 0;                                                                 /* FIFO read pointer initialization            */
    for (i=0; i<center_len; i++ ) {
        sumbegin = 0;
        for (j=i; j<i+width; j++) {
          sumbegin += curv[j];
        }
        Center_FIFO[FIFO_wt_Idx++] = (float)( sumbegin/width);                       /* The filtering results are first cached in the FIFO       */
        FIFO_wt_Idx %= SMMOOTH_SPAN;   

        if (i > begin_end_len) {                                                     
          curv[i-1] = Center_FIFO[FIFO_rd_Idx++];                                    /* After the original data no longer participates in the calculation, it will be overwritten and updated.*/
          FIFO_rd_Idx  %= SMMOOTH_SPAN; 
        }
    }


    for (i = (center_len-begin_end_len-1); i<center_len; i++) {                      /* FIFO end data output            */                            
          curv[begin_end_len+i] = Center_FIFO[FIFO_rd_Idx++];                        
          FIFO_rd_Idx  %= SMMOOTH_SPAN; 
    }

    for (i = 0; i<begin_end_len; i++) {                                             /* Fill in header and tail data                  */                            
      curv[i]             = begin_end_cache[i] ;
      curv[curve_len-1-i] = begin_end_cache[SMMOOTH_SPAN-1-i];
    }

  } else {                                                                   
                                                                                    /* When the window length is less than 3, it is equivalent to no filtering.  */
  }
}

/**********************************************************************************************************************
*   Function name: LocationAnalyse
*   Function description: Prism calibration program
*   Parameter list: curv: original curve
*             curve_len: original curve data length (<65536)
*   Hardware output: None
*   Return results: The position of the reflective pillar analyzed by StandLocation
*   History: Author/Modifier Date Remarks
*              whys 2024.04.09 Original creation version V1.0
*
**********************************************************************************************************************/
uint32_t _MOTO_CTRL::LocationAnalyse(float *curv,  uint16_t curve_len)
{
    uint16_t i;
    float  minVal, maxVal, threshold;                                           /* Bubble Maximum Minimum Intermediate Threshold         */
    uint16_t  Lmin, Lmax;                                                       /* Bubble maximum value minimum value position               */

    uint16_t timeFilterCnt       ;                                              /* Pulse filter time count                   */
    uint16_t timeFilterThreshold ;                                              /* Pulse filter filter time threshold              */
    uint16_t timeFilterEnable    ;                                              /* Enable pulse filtering                   */

    uint16_t edgeCnt;                                                           /* edge counter                        */
    EdgeInfo edge[6];                                                           /* Record edge type and location               */

    uint16_t NagtivePulseCnt = 0;                                               /* low pulse count                        */
    float    NagtivePulseMid[2];                                                /* Low pulse center recording                    */

    uint32_t StandLocation ;                                                    /* The analyzed position of the reflecting pillar             */            

    /****************step1 Perform cubic smoothing filtering *************************************/
    smooth_moving_lite(curv,  CURVE_LENGTH);
    smooth_moving_lite(curv,  CURVE_LENGTH);
    smooth_moving_lite(curv,  CURVE_LENGTH);

// #if DEBUG_INFO_EN
//     Serial.printf("******smoothed data*********\r\n");
//     for(i = 0; i<CURVE_LENGTH; i++){
//       Serial.printf("%.4f\r\n", curv[i] );
//     }
// #endif

    /************step2 Use the bubble method to find the maximum, minimum and threshold values of the stable segment*******************/
    minVal = 65535;
    maxVal = 0;
    Lmin = 0;
    Lmax = 0;
    for (i=1365; i<CURVE_LENGTH; i++) {
        if (curv[i] < minVal) {
            minVal = curv[i] ;
            Lmin = i;
        }
        
        if (curv[i] > maxVal) {
            maxVal = curv[i] ;
            Lmax = i;
        }
    }
    threshold = (maxVal+minVal)/2;                                              /* Find the threshold Threshold = (maximum value - minimum value)/2 */
#if DEBUG_INFO_EN    
    Serial.printf("Min_Loction=%d Max_Location=%d\r\n", Lmin, Lmax);
#endif    

    /************step3 Find transition edges based on threshold and store ******************************/
    timeFilterCnt       = 0;
    timeFilterThreshold = 160;
    timeFilterEnable    = 0;                                                     /* The temporal filter is initially disabled              */
    edgeCnt             = 0;                                                     /* Edge count cleared                   */

    for (i=0;i<4095;i++) {
        timeFilterCnt = timeFilterCnt+1;                                         /* Time filter timing increases automatically              */
        
        /********* Falling edge judgment *****************************/
        if( curv[i]>=threshold && curv[i+1]<threshold ) {                        /* Determine whether it is a falling edge                */
            if(timeFilterEnable == 1) {                                     /* An edge has been captured and the temporal filter has been activated */
                if(timeFilterCnt > timeFilterThreshold) {
                    edge[edgeCnt].type = FALLING_EDGE;                           /* mark*/
                    edge[edgeCnt].location = i;

                    timeFilterCnt    = 0;                                        /* filter reset                    */     
                    timeFilterEnable = 1;                                        /* Filter enable                    */
                    edgeCnt = edgeCnt +1;                                        /* edge count                      */                    
                }

            } else{                                                             /* There was no edge before, this is the first edge       */
                edge[edgeCnt].type = FALLING_EDGE;
                edge[edgeCnt].location = i;
                
                timeFilterCnt    = 0;                                           /* filter reset                     */     
                timeFilterEnable = 1;                                           /* Filter enable                     */
                edgeCnt = edgeCnt +1;                                           /* edge count                       */  
            }
        }       
        if (edgeCnt>=6) { break; }                                              /* Find up to 6 transition edges            */
        
        /********* Rising edge judgment *****************************/
        if( curv[i]<=threshold && curv[i+1]>threshold ) {                   /* Determine whether it is a rising edge                */
            if(timeFilterEnable == 1)  {                                   /* An edge has been captured and the temporal filter has been activated */
                if(timeFilterCnt > timeFilterThreshold) { 
                    edge[edgeCnt].type = RISING_EDGE;
                    edge[edgeCnt].location = i;

                    timeFilterCnt    = 0;                                        /* filter reset                    */     
                    timeFilterEnable = 1;                                        /* Filter enable                    */
                    edgeCnt = edgeCnt +1;                                        /* edge count                      */   
                }

            } else {                                                             /* There was no edge before, this is the first edge       */
                edge[edgeCnt].type = RISING_EDGE;
                edge[edgeCnt].location = i;

                timeFilterCnt    = 0;                                           /* filter reset                     */     
                timeFilterEnable = 1;                                           /* Filter enable                     */
                edgeCnt = edgeCnt +1;                                           /* edge count                       */  
            }

        }        
        if (edgeCnt>=6) { break; }                                              /* Find up to 6 transition edges            */
    }

#if DEBUG_INFO_EN    
    for(i=0; i<edgeCnt; i++) {
      Serial.printf("edge%d@%04d Type%d\r\n", i, edge[i].location, edge[i].type);
    }
#endif  

    /************step4 Find the low pulse center point from the edge ******************************/
    NagtivePulseCnt = 0;
    for(i=(edgeCnt-1); i>0; i--) {
      if (edge[i].type == RISING_EDGE) {
          if (edge[i-1].type == FALLING_EDGE) {
              NagtivePulseMid[NagtivePulseCnt] = ((float)(edge[i].location + edge[i-1].location))/2.0;           
              NagtivePulseCnt ++;                                              /*  Store low pulse midpoint                 */
          }        
      }           
      if (NagtivePulseCnt >= 2 ){ break; }                                     /*  Exit after finding enough two low pulses            */ 
    }

    /************step5 Calculate Flag position based on ground pulse midpoint **************************/
    if(NagtivePulseCnt >= 2) {
      minVal = (NagtivePulseMid[0]-NagtivePulseMid[1]) -1365.3333;             /* Get the position difference of the two pulses and compare it with 120°  */
      if(minVal < 0) {minVal = -minVal;}                                       /* Take absolute value                        */
      if( minVal < 40) {                                                       /* The interval between two pulses is 120° ±3.5°       */
        maxVal =  (NagtivePulseMid[0]+NagtivePulseMid[1])/2 - 682.6667;        /* The median value of the two pulses returns 60°               */
        StandLocation = (uint32_t)(maxVal);                                    /* The median value of the two pulses returns 60°               */
        StandLocation = StandLocation+1;
#if DEBUG_INFO_EN    
        Serial.printf("%.4f %.4f %.4f\r\n", NagtivePulseMid[0],NagtivePulseMid[1], maxVal);
        Serial.printf("Good  FlagLoction=%d\r\n",  StandLocation);
#endif 

      } else {                                                                 /* Take the middle low pulse midpoint               */
        StandLocation = (uint32_t)NagtivePulseMid[1];  
        StandLocation = StandLocation+1;   
#if DEBUG_INFO_EN    
        Serial.printf("Case2  FlagLoction=%d\r\n",  StandLocation);
#endif 
      }

    } else if(NagtivePulseCnt == 1) {
      StandLocation = (uint32_t)NagtivePulseMid[0] ;    
      StandLocation = StandLocation+1; 
#if DEBUG_INFO_EN    
      Serial.printf("Case3  FlagLoction=%d\r\n",  StandLocation);
#endif 

    } else {
      StandLocation = 1024;     
#if DEBUG_INFO_EN    
      Serial.printf("error:  FlagLoction=default(1024)\r\n");
#endif       
    }

    return StandLocation;
}

/***********************************************************************************************************************
*   Function name: Moto_IO_Init
*   Function description: Motor interface initialization
*   Parameter list: None
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2022.10.25 modified version V1.0
*
***********************************************************************************************************************/
void _MOTO_CTRL::IO_Init(void) {
  /* Initialize the stepper motor GPIO port */
  pinMode(MOTO_A,OUTPUT);
  pinMode(MOTO_B,OUTPUT);
  pinMode(MOTO_C,OUTPUT);
  pinMode(MOTO_D,OUTPUT);  

  /* Turn off all windings to prevent heating */
  digitalWrite(MOTO_A, LOW);
  digitalWrite(MOTO_B, LOW);
  digitalWrite(MOTO_C, LOW);
  digitalWrite(MOTO_D, LOW);  
}

/***********************************************************************************************************************
*   Function name: moto_speed_up
*   Function description: The motor gradually accelerates the beat delay
*   Parameter list: steps
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2022.10.25 modified version V1.0
*
***********************************************************************************************************************/
uint16_t _MOTO_CTRL::moto_speed_up( double steps)
{
  if(steps < MOTO_SPEED_RANGE) {
    return (uint16_t) (pow(1.0 - steps/MOTO_SPEED_RANGE,2)*(MOTO_MAX_DELAY - MOTO_MIN_DELAY) + MOTO_MIN_DELAY) ;
  }else {
    return (uint16_t) (MOTO_MIN_DELAY) ;
  }
}

/***********************************************************************************************************************
*   Function name: Moto_round_Scan
*   Function description: Motor rotation scanning
*   Parameter list: moto: motor structure
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2022.10.25 modified version V1.0
*
***********************************************************************************************************************/
void _MOTO_CTRL::roundScan(void)
{
    _MOTO_CTRL * moto = this;

    if(moto->steps != 0) {
      moto->state = MOTO_BUSY;
      moto->steps--;

      if(moto->turnDir == MOTO_TURN_UP) {
        moto->beat++;
      }else{     
        moto->beat--;      
      }   
      moto->beat%=8;  
      moto->beat_now = moto->ctrl_list[moto->beat];  

      digitalWrite(MOTO_A, moto->beat_now  & 0x8);
      digitalWrite(MOTO_B, moto->beat_now  & 0x4);
      digitalWrite(MOTO_C, moto->beat_now  & 0x2);
      digitalWrite(MOTO_D, moto->beat_now  & 0x1);      
      delayMicroseconds(moto_speed_up((double)moto->stepsn)); 
      moto->stepsn++;

      /* If the motor calibration action is enabled */   
      if( moto->enableFix == MOTO_FIX_BUSY ) {
          delayMicroseconds(100);                          /* Wait for power to stabilize         */

          moto->adc_tmp_val = Adc.get_adc_value( IRF_VAL);     /* Read the infrared tube voltage value   */ 
          moto->IR_log[ moto->findLoca++ ] = (float)moto->adc_tmp_val; /* store      */

// #if DEBUG_INFO_EN
//           Serial.printf("L%d-V%d\r\n",  moto->findLoca , moto->adc_tmp_val);
// #endif
          delayMicroseconds(5); 
      } else {
        if(moto->turnDir == MOTO_TURN_UP) {
          moto->standLoca--;                               /* rotate up           */
        }else{
          moto->standLoca++;                               /* rotate down           */
        }  
        moto->standLoca %= 4096;                           /* Unwind             */
      }
    }else {

      if(moto->enableFix == MOTO_FIX_BUSY){
        moto->enableFix = MOTO_FIX_DONE;
        Adc.adc_prepare( ADC_OFF);                                                              /* Also turn off the ADC when stopping spinning */
        moto->standLoca = moto->LocationAnalyse(moto->IR_log,  CURVE_LENGTH) ;                  /* Calibrating coordinates using filter analysis method */
        moto->standLoca = (ADC_FLAG_LOCATION + Config.structADJ + 4096-moto->standLoca) % 4096;  /* Get the current absolute angle */
#if DEBUG_INFO_EN
        Serial.printf("standLoca =  %d， ADJ = %d\r\n", moto->standLoca,  Config.structADJ );
        delay(2e3);                                                                              /* Wait for serial port data output to complete */  
#endif
      }

      if(moto->state != MOTO_IDLE) {
        moto->state = MOTO_IDLE;

        /* Turn off all windings to prevent heating */
        digitalWrite(MOTO_A, LOW);
        digitalWrite(MOTO_B, LOW);
        digitalWrite(MOTO_C, LOW);
        digitalWrite(MOTO_D, LOW);  
#if DEBUG_INFO_EN
        Serial.println("Moto task done!");
#endif        
      } else {
        // When the motor is idle, there is no action 
      }
    }
}

/***********************************************************************************************************************
*   Function name: Moto_Ctrl
*   Function description: Motor control interface
*   Parameter list: moto: motor structure
*              Dir: Motor rotation direction MOTO_TURN_UP MOTO_TURN_DN
*             step: number of motor rotation steps
*   Hardware output: None
*   Return result: 0: Failure 1: Success
*   History: Author/Modifier Date Remarks
*              whys 2022.10.25 modified version V1.0
*
***********************************************************************************************************************/
int8_t _MOTO_CTRL::ctrl(uint8_t Dir, uint16_t step )
{
      _MOTO_CTRL * moto = this;

      if(moto->state == MOTO_BUSY) {
          //Add wait action
          //After waiting, still busy
          return 0;
      } else {
        moto->turnDir =  Dir;
        moto->steps   =  step;
        moto->stepsn  = 0;
#if DEBUG_INFO_EN        
        Serial.printf("access new task %d>> %d\r\n", Dir, step);
#endif
        return 1;
      }
}

/***********************************************************************************************************************
*   Function name: Moto_Fix_Location
*   Function description: Start motor initial position calibration
*   Parameter list: moto: motor structure
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2022.10.25 modified version V1.0
*
***********************************************************************************************************************/
void _MOTO_CTRL::fixLocation(void)
{
  _MOTO_CTRL * moto = this;

  moto->findLoca    = 0;
  moto->standLoca   = 0;        
  moto->beat        = 0;    
  moto->enableFix   = MOTO_FIX_BUSY ;    

  Adc.adc_prepare( IRF_VAL);                                 /* Prepare the status of infrared tube     */                                                       
  moto->ctrl(MOTO_TURN_DN, 4096);                            /* Rotate once to find the ADC minimum value */
}

/***********************************************************************************************************************
*   Function name: Moto_GoTo_AbsLocation
*   Function description: The motor rotates to somewhere in the absolute coordinate system
*   Parameter list: moto: motor structure
*             angle: Specify the absolute coordinates in ° (degree) with the vertex of surface C pointing to the positive direction of the Z axis (directly above is the absolute coordinate zero point)
*   Hardware output: None
*   Return result: None
*   History: Author/Modifier Date Remarks
*              whys 2022.10.25 modified version V1.0
*
***********************************************************************************************************************/
void _MOTO_CTRL::goToAbsLocation(float angle)
{
  _MOTO_CTRL * moto = this;

  uint16_t angle_step = 0;

  angle_step = (uint16_t)(angle*4096.0/360.0);

  if(angle_step >  moto->standLoca) {
     moto->ctrl(MOTO_TURN_DN, (angle_step - moto->standLoca)&0xfffe);    /* Rotate downward to reach the position, limited to even steps   */
  } else {
     moto->ctrl(MOTO_TURN_UP, (moto->standLoca - angle_step)&0xfffe);    /* Rotate upward to reach the position, limited to even steps */
  }
}
