#pragma once
#include "Arduino.h"
#include "flprogUtilites.h"

//  https://github.com/GyverLibs/UnixTime/blob/main/src/UnixTime.h

class FLProgUnixTime
{
public:
    
    void setTime(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t date, uint8_t month, uint16_t year);
    void incrementUnixTime();
    uint8_t getSecond() { return _second; };
    uint8_t getMinute() { return _minute; };
    uint8_t getHour() { return _hour; };
    uint8_t getDay() { return _day; };
    uint8_t getDate() { return _date; };
    uint8_t getMonth() { return _month; };
    uint16_t getYear() { return _year; };
    uint32_t getUnix() { return _unixTime; };
    int16_t getGmt() { return _gmt; };

    void setSecond(uint8_t second);
    void setMinute(uint8_t minute);
    void setHour(uint8_t hour);
    void setDay(uint8_t day);
    void setDate(uint8_t date);
    void settMonth(uint8_t month);
    void setYear(uint16_t year);
    void setGmt(int16_t gmt);
    void setUnix(uint32_t unixTime);

private:
    void fillWeekDay();
    uint8_t ds_dim(uint8_t i);
    void fillUnixTimeFromDataTime();
    void fillDataTimeFromUnixTime();
    uint8_t _second = 0;
    uint8_t _minute = 0;
    uint8_t _hour = 0;
    uint8_t _day = 0;
    uint8_t _date = 1;
    uint8_t _month = 1;
    uint16_t _year = 2000;
    int16_t _gmt = 0;
    uint32_t _unixTime = 946684800;
};
