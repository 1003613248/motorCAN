/*************************************************************************
* Title:    System Tick control
* Author:   @author Koko
* Usage:    @brief Pausen- Timer- und Counter-Funktionen, Zeiten im [us,ms,s] Raster
==========================================================================
 We reserve all rights in this document and in the information contained
 therein. Reproduction, use or disclosure to third parties without express
 authority is strictly forbidden.
**************************************************************************/


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "systick.h"

//--------------------------------------------------------------
// Globale Pausen-Variabeln
//--------------------------------------------------------------
static volatile uint32_t Systick_Delay;  // Global pause counter

//--------------------------------------------------------------
// Globale Timer-Variabeln
//--------------------------------------------------------------
Sysbase_timer timebase_10ms;
Sysbase_timer timebase_100ms;
Sysbase_timer timebase_1s;


/**
  * @brief  Pause fuction
  * @param  pause: value for pause in us (if SYSTICK_RESOLUTION==1) else ms
  * @retval none
  */
void Systick_Pause(volatile uint32_t pause)
{
 Systick_Delay = pause;

 while(Systick_Delay != 0);
}

/**
  * @brief  Init of System dedicated SysteTimer
  * @param  sTimer: Systimer handle
  * @retval none
  */
void TimerSys_Init(Sysbase_timer *sTimer)
{
  sTimer->cnt=0;
}


/**
  * @brief  Timer can be started with a specific timer and cyclic checked if timer elapsed
  * @param  sTimer: Systimer handle
  * @param  status: set timer status [TIMER_STOP, TIMER_START_us/ms/s, TIMER_CHECK]
  * @param  wert : Timer value
  * @retval Timer status [TIMER_HOLD, TIMER_RUN]
  */
TIMER_CHECK_t Systick_Timer(Sysbase_timer *sTimer, TIMER_STATUS_t status, uint32_t wert)
{
  TIMER_CHECK_t ret_wert=TIMER_RUN;

  if(status==TIMER_STOP) {
    // Timer1 schnell anhalten
    sTimer->cnt=1;
    ret_wert=TIMER_HOLD;
  }
  #if SYSTICK_RESOLUTION==1
    else if(status==TIMER_START_us) {
      // Timer1 im us-Mode starten
      sTimer->cnt=wert;
    }
    else if(status==TIMER_START_ms) {
      // Timer1 im ms-Mode starten
      sTimer->cnt=1000*wert;
    }
    else if(status==TIMER_START_s) {
      // Timer1 im s-Mode starten
      if(wert>4293) wert=4293;
      sTimer->cnt=1000*1000*wert;
    }
  #else
    else if(status==TIMER_START_ms) {
      // Timer1 im ms-Mode starten
      sTimer->cnt=wert;
    }
    else if(status==TIMER_START_s) {
      // Timer1 im s-Mode starten
      sTimer->cnt=1000*wert;
    }
  #endif
  else {
    // Staus zurückgeben
    if(sTimer->cnt!=0x00) {
      // Timer läuft noch
      ret_wert=TIMER_RUN;
    }
    else {
      // Timer ist abgelaufen
      ret_wert=TIMER_HOLD;
    }
  }
  return(ret_wert);
}


//--------------------------------------------------------------
// Interrupt Callback Functions
//--------------------------------------------------------------
/**
  * @brief  IRQ Handler for dedicated SystickTimer
  * @param  htim : TIM OC handle
  * @retval None
  */
void Systimer_IRQHandler(Sysbase_timer *timer)
{
    if (timer->cnt != 0x00){
       timer->cnt--;
    }
}

//--------------------------------------------------------------
// Systic-Interrupt
//--------------------------------------------------------------
void HAL_SYSTICK_Callback(void)
{
  Systimer_IRQHandler(&timebase_10ms);
  Systimer_IRQHandler(&timebase_100ms);
  Systimer_IRQHandler(&timebase_1s);
  
  // Tick for break
  if(Systick_Delay != 0x00) {
    Systick_Delay--;
  }
}
