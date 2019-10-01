#ifndef ACCUWEATHER_WEATHER_H
#define ACCUWEATHER_WEATHER_H
///////////////////////////////////////////////////////////////////////////////
#include "forecastdata.h"
///////////////////////////////////////////////////////////////////////////////
#include <QJsonDocument>
#include <QDate>
#include <QObject>
#include <QVector>
///////////////////////////////////////////////////////////////////////////////

const QString ACCUWEATHER = "AccuWeather";

const QString DAILY  = "daily" ;
const QString HOURLY = "hourly";

class AccuWeather : public QObject
{
    Q_OBJECT

public:
    AccuWeather () {}
    AccuWeather (const QString apiKey) : aw_apiKey(apiKey) {}
    ~AccuWeather();

    void setApiKey(const QString apikey) { aw_apiKey = apikey; }

    bool getCountriesList(const QString cityName,
                                QMap<QString, int> &countries);

    QVector<ForecastData> forecastData;

    bool getForecast(const int     locationKey,
                     const QString forecastType);

    ForecastData searchForecast(const QDate);
    ForecastData searchForecast(const QTime);

private:
    void getForecastDay ();
    void getForecastHour();

    void getData(QString&);

    QDate getDate(const QString);
    QTime getTime(const QString);

    QJsonDocument doc;

    QString aw_apiKey{};
};

#endif // ACCUWEATHER_WEATHER_H
