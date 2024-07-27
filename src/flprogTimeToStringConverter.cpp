#include "flprogTimeToStringConverter.h"

FLProgTimeToStringConverter::FLProgTimeToStringConverter(String string, FLProgUnixTime *time)
{
    _time = time;
    _inString = string;
    init();
}

void FLProgTimeToStringConverter::setFormatString(String inString)
{
    if (inString.equals(_inString))
    {
        return;
    }
    _inString = inString;
    init();
    convertTime();
}

void FLProgTimeToStringConverter::init()
{
    _charStringSize = _inString.length() + 1;
    free(_charString);
    _charString = (char *)malloc(_charStringSize);
    _inString.toCharArray(_charString, _charStringSize);
    _charString[_charStringSize] = 0;
    setMemoryForString();
}

void FLProgTimeToStringConverter::setMemoryForString()
{
    uint8_t charIndex;
    free(_charReturn);
    _charReturnSize = _charStringSize + 1;
    for (charIndex = 0; charIndex < strlen(_charString); charIndex++)
    {
        for (uint8_t k = 0; k < strlen(_charInput); k++)
        {
            if (_charString[charIndex] == _charInput[k])
            {
                if (k > 0)
                {
                    _charReturnSize++;
                }
                if (k > 9)
                {
                    _charReturnSize++;
                }
                if (k > 11)
                {
                    _charReturnSize++;
                }
            }
        }
    }
    _charReturn = (char *)malloc(_charReturnSize);
}

String FLProgTimeToStringConverter::getTimeString()
{
    if (_time->getUnix() != _unixCash)
    {
        convertTime();
    }
    return String(_charReturn);
}

void FLProgTimeToStringConverter::convertTime()
{
    uint8_t newStringIndex = 0;
    for (uint8_t charIndex = 0; charIndex < _charStringSize; charIndex++)
    {
        newStringIndex = checkChar(_charString[charIndex], newStringIndex);
    }
    _charReturn[newStringIndex] = '\0';
}

uint8_t FLProgTimeToStringConverter::checkChar(char val, uint8_t index)
{

    if (val == _charInput[0] /*	w	*/)
    {
        funcFillChar(_time->getDay(), FLPROG_RTC_ONE_SYMBOL_TYPE, index, FLPROG_RTC_FLASH_DAY_SYMBOL);
        return index + 1;
    }
    if (val == _charInput[1] /*	a	*/)
    {
        funcFillChar(((_time->getHour() < 12) ? 0 : 1) * 2, FLPROG_RTC_AM_PM_SYMBOL_TYPE, index, FLPROG_RTC_FLASH_AM_PM_SYMBOL);
        return index + 2;
    }
    if (val == _charInput[2] /*	A	*/)
    {
        funcFillChar((((_time->getHour() < 12) ? 0 : 1) + 2) * 2, FLPROG_RTC_AM_PM_SYMBOL_TYPE, index, FLPROG_RTC_FLASH_AM_PM_SYMBOL);
        return index += 2;
    }
    if (val == _charInput[3] /*	d	*/)
    {
        funcFillChar(_time->getDate(), FLPROG_RTC_TWO_SYMBOL_TYPE, index, FLPROG_RTC_FLASH_DATE_SYMBOL);
        return index + 2;
    }
    if (val == _charInput[4] /*	h	*/)
    {
        funcFillChar(((_time->getHour() % 12) ? (_time->getHour() % 12) : 12), FLPROG_RTC_TWO_SYMBOL_TYPE, index, FLPROG_RTC_FLASH_HOUR_SYMBOL);
        return index + 2;
    }
    if (val == _charInput[5] /*	H	*/)
    {
        funcFillChar(_time->getHour(), FLPROG_RTC_TWO_SYMBOL_TYPE, index, FLPROG_RTC_FLASH_HOUR_SYMBOL);
        return index + 2;
    }
    if (val == _charInput[6] /*	i	*/)
    {
        funcFillChar(_time->getMinute(), FLPROG_RTC_TWO_SYMBOL_TYPE, index, FLPROG_RTC_FLASH_MINUTE_SYMBOL);
        return index + 2;
    }
    if (val == _charInput[7] /*	m	*/)
    {
        funcFillChar(_time->getMonth(), FLPROG_RTC_TWO_SYMBOL_TYPE, index, FLPROG_RTC_FLASH_MONTH_SYMBOL);
        return index + 2;
    }
    if (val == _charInput[8] /*	s	*/)
    {
        funcFillChar((_time->getSecond()), FLPROG_RTC_TWO_SYMBOL_TYPE, index, FLPROG_RTC_FLASH_SECOND_SYMBOL);
        return index + 2;
    }
    if (val == _charInput[9] /*	y	*/)
    {
        funcFillChar((_time->getYear()- 2000), FLPROG_RTC_TWO_SYMBOL_TYPE, index, FLPROG_RTC_FLASH_YEAR_SYMBOL);
        return index + 2;
    }
    if (val == _charInput[10] /*	M	*/)
    {
        funcFillChar((_time->getMonth() + 6) * 3, FLPROG_RTC_DAY_MONTH_SYMBOL_TYPE, index, FLPROG_RTC_FLASH_MONTH_SYMBOL);
        return index + 3;
    }
    if (val == _charInput[11] /*	D	*/)
    {
        funcFillChar(_time->getDay() * 3, FLPROG_RTC_DAY_MONTH_SYMBOL_TYPE, index, FLPROG_RTC_FLASH_DAY_SYMBOL);
        return index + 3;
    }
    if (val == _charInput[12] /*	Y	*/)
    {
        funcFillChar((_time->getYear() - 2000), FLPROG_RTC_FOUR_SYMBOL_TYPE, index, FLPROG_RTC_FLASH_YEAR_SYMBOL);
        return index + 4;
    }
    _charReturn[index] = val;
    return index + 1;
}

void FLProgTimeToStringConverter::funcFillChar(uint8_t value, uint8_t type, uint8_t position, uint8_t blinkVal)
{
    //	(данные, тип данных, позиция для вставки, мигание)
    bool blink = _valBlink == blinkVal;
    if ((millis() % _valFrequency) < (_valFrequency / 2))
    {
        blink = false;
    } //	Устанавливаем флаг мигания, если значение valBlink равно значению параметра k
    switch (type)
    {
    /* 1 знак	*/ case FLPROG_RTC_ONE_SYMBOL_TYPE:
        if (value > 6)
        {
            value = 6;
        }
        _charReturn[position] = blink ? 32 : value + 48;
        break;
    /* 2 знака	*/ case FLPROG_RTC_TWO_SYMBOL_TYPE:
        if (value > 99)
        {
            value = 99;
        }
        _charReturn[position] = blink ? 32 : value / 10 + 48;
        _charReturn[position + 1] = blink ? 32 : value % 10 + 48;
        break;
    /* AM / PM	*/ case FLPROG_RTC_AM_PM_SYMBOL_TYPE:
        if (value > 6)
        {
            value = 6;
        }
        _charReturn[position] = blink ? 32 : _charMidday[value];
        _charReturn[position + 1] = blink ? 32 : _charMidday[value + 1];
        break;
    /* дн / мес	*/ case FLPROG_RTC_DAY_MONTH_SYMBOL_TYPE:
        if (value > 54)
        {
            value = 54;
        }
        _charReturn[position] = blink ? 32 : _charDayMon[value];
        _charReturn[position + 1] = blink ? 32 : _charDayMon[value + 1];
        _charReturn[position + 2] = blink ? 32 : _charDayMon[value + 2];
        break;
    /* 4 знака	*/ case FLPROG_RTC_FOUR_SYMBOL_TYPE:
        if (value > 99)
        {
            value = 99;
        }
        _charReturn[position] = blink ? 32 : 2 + 48;
        _charReturn[position + 1] = blink ? 32 : 0 + 48;
        _charReturn[position + 2] = blink ? 32 : value / 10 + 48;
        _charReturn[position + 3] = blink ? 32 : value % 10 + 48;

        break;
    }
}

void FLProgTimeToStringConverter::blinktime(uint8_t blinkRazrad, float blinkPeriod)
{
    _valBlink = blinkRazrad;
    _valFrequency = uint32_t(1000 / blinkPeriod);
}