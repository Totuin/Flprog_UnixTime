#pragma once
#include "Arduino.h"
#include "flprogUtilites.h"
#include "flprogUnixTime.h"

#define FLPROG_RTC_ONE_SYMBOL_TYPE 0
#define FLPROG_RTC_TWO_SYMBOL_TYPE 1
#define FLPROG_RTC_AM_PM_SYMBOL_TYPE 2
#define FLPROG_RTC_DAY_MONTH_SYMBOL_TYPE 3
#define FLPROG_RTC_FOUR_SYMBOL_TYPE 4

#define FLPROG_RTC_NO_FLASH_SYMBOL 0
#define FLPROG_RTC_FLASH_SECOND_SYMBOL 1
#define FLPROG_RTC_FLASH_MINUTE_SYMBOL 2
#define FLPROG_RTC_FLASH_HOUR_SYMBOL 3
#define FLPROG_RTC_FLASH_DATE_SYMBOL 4
#define FLPROG_RTC_FLASH_MONTH_SYMBOL 5
#define FLPROG_RTC_FLASH_YEAR_SYMBOL 6
#define FLPROG_RTC_FLASH_DAY_SYMBOL 7
#define FLPROG_RTC_FLASH_AM_PM_SYMBOL 8

// const char *_charInput = "waAdhHimsyMDY"; //	Определяем константу-строку с символами требующими замены			(данные символы заменяются функцией gettime на значение времени)
// const char *_charMidday = "ampmAMPM";     //	Определяем константу-строку для вывода полудня						(am / pm / AM / PM)
// const char *_charDayMon = "SunMonTueWedThuFriSatJanFebMarAprMayJunJulAugSepOctNovDec";

class FLProgTimeToStringConverter
{
public:
    FLProgTimeToStringConverter(String string, FLProgUnixTime *time);
    String getTimeString();
    String getFormatString() { return _inString; };
    void setFormatString(String inString);
    void blinktime(uint8_t blinkRazrad, float blinkPeriod = 1);

private:
    void init();
    void convertTime();
    void setMemoryForString();
    uint8_t checkChar(char val, uint8_t index);
    void funcFillChar(uint8_t value, uint8_t type, uint8_t position, uint8_t blinkVal);
    FLProgUnixTime *_time = 0;
    char *_charReturn = 0;
    char *_charString = 0;
    String _inString = "";
    uint32_t _unixCash = 0;
    uint32_t _valFrequency = 1000;
    uint8_t _valBlink = 255;
    uint8_t _charStringSize = 0;
    uint8_t _charReturnSize = 0;

    static constexpr char *_charInput = (char *)"waAdhHimsyMDY"; //	Определяем константу-строку с символами требующими замены
    static constexpr char *_charMidday = (char *)"ampmAMPM";     //	Определяем константу-строку для вывода полудня (am / pm / AM / PM)
    static constexpr char *_charDayMon = (char *)"SunMonTueWedThuFriSatJanFebMarAprMayJunJulAugSepOctNovDec";
};
