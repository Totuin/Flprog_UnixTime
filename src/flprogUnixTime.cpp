#include "flprogUnixTime.h"

void FLProgUnixTime::fillWeekDay()
{
    uint16_t tempYear = _year;
    if (tempYear >= 2000)
    {
        tempYear -= 2000;
    }
    uint16_t days = _date;
    for (uint8_t i = 1; i < _month; ++i)
    {
        days += ds_dim(i - 1);
    }
    if (_month > 2 && tempYear % 4 == 0)
    {
        days++;
    }
    _day = (days + 365 * tempYear + (tempYear + 3) / 4 + 4) % 7 + 1;
}

uint8_t FLProgUnixTime::ds_dim(uint8_t i)
{
    return (i < 7) ? ((i == 1) ? 28 : ((i & 1) ? 30 : 31)) : ((i & 1) ? 31 : 30);
}

void FLProgUnixTime::fillUnixTimeFromDataTime()
{
    int16_t tempGmt = _gmt;
    if (abs(tempGmt) <= 12)
    {
        tempGmt *= 60;
    }
    int8_t my = (_month >= 3) ? 1 : 0;
    uint16_t y = _year + my - 1970;
    uint16_t dm = 0;
    for (int i = 0; i < _month - 1; i++)
    {
        dm += ds_dim(i);
    }
    _unixTime = (((_date - 1 + dm + ((y + 1) / 4) - ((y + 69) / 100) + ((y + 369) / 100 / 4) + 365 * (y - my)) * 24ul + _hour) * 60ul + _minute - tempGmt) * 60ul + _second;
}

void FLProgUnixTime::fillDataTimeFromUnixTime()
{
    // http://howardhinnant.github.io/date_algorithms.html#civil_from_days

    int16_t tempGmt = _gmt;
    uint32_t tempUnix = _unixTime;
    if (abs(tempGmt) <= 12)
    {
        tempGmt *= 60;
    }
    tempUnix += tempGmt * 3600ul;
    _second = tempUnix % 60ul;
    tempUnix /= 60ul;
    _minute = tempUnix % 60ul;
    tempUnix /= 60ul;
    _hour = tempUnix % 24ul;
    tempUnix /= 24ul;
    _day = (tempUnix + 4) % 7;
    if (!_day)
        _day = 7;
    uint32_t z = tempUnix + 719468;
    uint8_t era = z / 146097ul;
    uint16_t doe = z - era * 146097ul;
    uint16_t yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    uint16_t y = yoe + era * 400;
    uint16_t doy = doe - (yoe * 365 + yoe / 4 - yoe / 100);
    uint16_t mp = (doy * 5 + 2) / 153;
    _date = doy - (mp * 153 + 2) / 5 + 1;
    _month = mp + (mp < 10 ? 3 : -9);
    y += (_month <= 2);
    _year = y;
    /*
    //https://www.oryx-embedded.com/doc/date__time_8c_source.html
    uint32_t a, b, c, d, e, f;
    int h, j, k;
    tempUnix += _gmt * 3600ul;
    _second = tempUnix % 60ul;
    tempUnix /= 60;
    _minute = tempUnix % 60ul;
    tempUnix /= 60;
    _hour = tempUnix % 24ul;
    tempUnix /= 24;
    a = (uint32_t)((4ul * tempUnix + 102032) / 146097 + 15);
    b = (uint32_t)(tempUnix + 2442113 + a - (a / 4));
    c = (20 * b - 2442) / 7305;
    d = b - 365 * c - (c / 4);
    e = d * 1000 / 30601;
    f = d - e * 30 - e * 601 / 1000;
    if (e <= 13) {
        c -= 4716;
        e -= 1;
    } else {
        c -= 4715;
        e -= 13;
    }
    _year = c;
    _month = e;
    _date = f;
    if (e <= 2) {
        e += 12;
        c -= 1;
    }
    j = c / 100;
    k = c % 100;
    h = f + (26 * (e + 1) / 10) + k + (k / 4) + (5 * j) + (j / 4);  // Уравнение Зеллера
    _day = ((h + 5) % 7) + 1;
    */
}

void FLProgUnixTime::setUnix(uint32_t unixTime)
{
    if (unixTime == _unixTime)
    {
        return;
    }
    _unixTime = unixTime;
    fillDataTimeFromUnixTime();
    fillWeekDay();
}

void FLProgUnixTime::setTime(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t date, uint8_t month, uint8_t year)
{
    _second = seconds;
    _minute = minutes;
    _hour = hours;
    _date = date;
    _month = month;
    _year = year;
    fillUnixTimeFromDataTime();
    fillWeekDay();
}

void FLProgUnixTime::setTime(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t date, uint8_t month, uint8_t year, uint8_t day)
{
    _second = seconds;
    _minute = minutes;
    _hour = hours;
    _date = date;
    _month = month;
    _year = year;
    _day = day;
    fillUnixTimeFromDataTime();
}

void FLProgUnixTime::incrementUnixTime()
{
    _unixTime++;
    fillDataTimeFromUnixTime();
    fillWeekDay();
}

void FLProgUnixTime::setSecond(uint8_t second)
{
    if (_second == second)
    {
        return;
    }
    _second = second;
    fillUnixTimeFromDataTime();
}

void FLProgUnixTime::setMinute(uint8_t minute)
{
    if (_minute == minute)
    {
        return;
    }
    _minute = minute;
    fillUnixTimeFromDataTime();
}

void FLProgUnixTime::setHour(uint8_t hour)
{
    if (_hour == hour)
    {
        return;
    }
    _hour = hour;
    fillUnixTimeFromDataTime();
}

void FLProgUnixTime::setDay(uint8_t day)
{
    if (_day == day)
    {
        return;
    }
    _day = day;
}

void FLProgUnixTime::setDate(uint8_t date)
{
    if (_date == date)
    {
        return;
    }
    _date = date;
    fillUnixTimeFromDataTime();
    fillWeekDay();
}

void FLProgUnixTime::settMonth(uint8_t month)
{
    if (_month == month)
    {
        return;
    }
    _month = month;
    fillUnixTimeFromDataTime();
    fillWeekDay();
}

void FLProgUnixTime::setYear(uint16_t year)
{
    if (_year == year)
    {
        return;
    }
    _year = year;
    fillUnixTimeFromDataTime();
    fillWeekDay();
}

void FLProgUnixTime::settGmt(int16_t gmt)
{
    if (_gmt == gmt)
    {
        return;
    }
    _gmt = gmt;
    fillDataTimeFromUnixTime();
    fillWeekDay();
}