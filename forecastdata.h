#ifndef FORECAST_H
#define FORECAST_H
//=============================================================================
#include <QDate>
//=============================================================================
/// = "1day",
///   "5day",
///   "10day",
///   "15day"
const QStringList FORECASTS_DAYS =
{
    "1day",    // 0
    "5day",    // 1
    "10day",   // 2 -
    "15day"    // 3 -
};
/// = "1hour",
///   "12hour",
///   "24hour",
///   "72hour",
///   "120hour"
const QStringList FORECASTS_HOURS =
{
    "1hour",   // 0
    "12hour",  // 1
    "24hour",  // 2 -
    "72hour",  // 3 -
    "120hour"  // 4 -
};
//=============================================================================

struct ForecastData
{
    QTime time{};
    QDate date{};

    QString dayOfWeek();

    int     iconNo    {};
    QString iconPhrase{};
    bool    isDayNight{};

    int temperatureF  {};
    int temperatureC();

    int temperatureMinF{};
    int temperatureMaxF{};

    int temperatureMinC();
    int temperatureMaxC();

    int     iconDayNo      {};
    QString iconPhraseDay  {};
    int     iconNightNo    {};
    QString iconPhraseNight{};

    QString showData();
};

namespace day
{
enum IconNumber
{
    Synny            = 1,
    MostlySunny         ,
    PartlySunny         ,
    IntermittentSunny   ,
    HazySunshine        ,
    MostlyCloudy        ,
    Cloudy              ,
    Dreary              ,
    Fog             = 11,
    Showers             ,
    MostlyCloudyShowers ,
    PartlyCloudyShowers ,
    TStorms             ,
    MostlyCloudyTStorms ,
    PartlyCloudyTStroms ,
    Rain                ,
    Flurries            ,
    MostlyCloudyFlurries,
    PartlyCloudyFlurries,
    Snow                ,
    MostlyCloudySnow    ,
    Ice                 ,
    Sleet               ,
    FreezingRain        ,
    RainAndSnow     = 29,
    Hot                 ,
    Cold                ,
    Windy
};
}

namespace night
{
enum IconNumber
{
    Clear           = 33,
    MostlyClear         ,
    PartlyCloudy        ,
    IntermittentClouds  ,
    HazyMoonlight       ,
    MostlyCloudy        ,
    PartlyCloudyShowers ,
    MostlyCloudyShowers ,
    PartlyCloudyTStorms ,
    MostlyCloudyTStorms ,
    MostlyCloudyFlurries,
    MostlyCloudySnow
};
}

#endif // FORECAST_H
