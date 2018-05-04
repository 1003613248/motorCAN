/*************************************************************************
* Title:    System Tick control
* Author:   @author Koko
* Usage:    @brief Routines to controle Communication used by SMBUS
==========================================================================
 We reserve all rights in this document and in the information contained
 therein. Reproduction, use or disclosure to third parties without express
 authority is strictly forbidden.
**************************************************************************/
//--------------------------------------------------------------
#ifndef __SYSTICK_H
#define __SYSTICK_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include <stdint.h>
#include "stm32f3xx_hal.h"

//--------------------------------------------------------------
// Auflösung der Systick
// (entweder 1us oder 1000us als Auflösung einstellen)
//--------------------------------------------------------------
//#define  SYSTICK_RESOLUTION   1    // 1us Auflösung
#define  SYSTICK_RESOLUTION   1000   // 1ms Auflösung


#if SYSTICK_RESOLUTION==1
  typedef enum {
    TIMER_STOP =0,  // Timer stoppen und rücksetzen
    TIMER_START_us, // Timer starten im us-Mode
    TIMER_START_ms, // Timer starten im ms-Mode
    TIMER_START_s,  // Timer starten im s-Mode
    TIMER_CHECK,     // Test ob Timer abgelaufen
  }TIMER_STATUS_t;
#else
  typedef enum {
    TIMER_STOP =0,  // Timer stoppen und rücksetzen
    TIMER_START_ms, // Timer starten im ms-Mode
    TIMER_START_s,  // Timer starten im s-Mode
    TIMER_CHECK,     // Test ob Timer abgelaufen
  }TIMER_STATUS_t;
#endif
	
	
typedef enum {
  TIMER_HOLD =0,  // Timer ist abgelaufen
  TIMER_RUN       // Timer läuft noch
}TIMER_CHECK_t;

typedef struct {
  volatile uint32_t cnt;
 } Sysbase_timer;
 
 
void Systick_Pause(volatile uint32_t pause);

void TimerSys_Init(Sysbase_timer *sTimer);
TIMER_CHECK_t Systick_Timer(Sysbase_timer *sTimer, TIMER_STATUS_t status, uint32_t wert);
 
#endif
