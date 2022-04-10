#include "Timer.h"
#include <Arduino.h>

Timer::Timer(unsigned long nrOfMilliseconds)
    : timeToReset(nrOfMilliseconds)
    , currentTimeCounter(0)
    , previousTimeCounter(0)
    , timerExpired(false)
{
}

void Timer::updateTimer()
{
    updateTimer(millis());
}

void Timer::updateTimer(unsigned long curTime)
{
    currentTimeCounter = curTime;
    
    if ( abs((long)(currentTimeCounter - previousTimeCounter)) > timeToReset )
    {
        timerExpired = true;
    }
}

bool Timer::expired() const
{
    return timerExpired;
}

void Timer::resetTimer()
{
    previousTimeCounter = millis();
    timerExpired = false;
}

