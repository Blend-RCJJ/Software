#ifndef _RTOSKIT_H_
#define _RTOSKIT_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <STM32FreeRTOS.h>

#define App void *rtosKit
#define maximumAppCount 30

#define firstPriority 4
#define secondPriority 3
#define defaultPriority 2
#define lowPriority 1

class RTOS_Kit {
   public:
    RTOS_Kit();
    void create(TaskFunction_t funcPtr, int priority = defaultPriority);
    void start(TaskFunction_t funcPtr);
    void stop(TaskFunction_t funcPtr);

    void startRTOS(void);

    void delay(unsigned long ms);

    int mainAppNumber  = 0;

    TaskFunction_t funcPtrArray[maximumAppCount];
    TaskHandle_t handlerArray[maximumAppCount];

   private:
};

#endif