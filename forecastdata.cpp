//=============================================================================
#include "forecastdata.h"
//=============================================================================
#include <QLocale>
//=============================================================================

QString ForecastData::dayOfWeek()
{
    return QLocale().dayName(date.dayOfWeek());
}

int ForecastData::temperatureC()
{
    return (temperatureF - 32) * 5 / 9;
}

int ForecastData::temperatureMinC()
{
    return (temperatureMinF - 32) * 5 / 9;
}

int ForecastData::temperatureMaxC()
{
    return (temperatureMaxF - 32) * 5 / 9;
}

QString ForecastData::showData()
{
    return
      "time = " + time.toString()
    + "\n" "date = " + date.toString()

    + "\n" "day of week = " + dayOfWeek()

    + "\n" "icon number = " + QString::number(iconNo)
    + "\n" "icon phrase = " + iconPhrase
    + "\n" "day? = "        + QVariant(isDayNight).toString()

    + "\n" "temperature F = " + QString::number(temperatureF  )
    + "\n" "temperature C = " + QString::number(temperatureC())

    + "\n" "temperature min F = " + QString::number(temperatureMinF  )
    + "\n" "temperature min C = " + QString::number(temperatureMinC())

    + "\n" "temperature max F = " + QString::number(temperatureMaxF  )
    + "\n" "temperature max C = " + QString::number(temperatureMaxC())

    + "\n" "icon day number = " + QString::number(iconDayNo)
    + "\n" "icon day phrase = " + iconPhraseDay

    + "\n" "icon night number = " + QString::number(iconNightNo)
    + "\n" "icon night phrase = " + iconPhraseNight
    + "\n";
}
