#pragma once

//unsigned long second = 1000;
//unsigned long minute = 60*second;
//unsigned long hour = 60*minute;
//unsigned long day = 24*hour;

class Timer
{
public:
    
    Timer(unsigned long nrOfMilliseconds);
    bool expired() const;
    void updateTimer();
    void resetTimer();
    
private:
    void updateTimer(unsigned long curTime);
    
    unsigned long timeToReset;
    unsigned long currentTimeCounter;
    unsigned long previousTimeCounter;
    bool timerExpired;
};
