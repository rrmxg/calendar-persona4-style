//=============================================================================
#include "accuweather.h"
//=============================================================================
#include <QDebug>
//=============================================================================
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <QEventLoop>
//=============================================================================
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
//=============================================================================

AccuWeather::~AccuWeather() { forecastData.clear(); }

/// forecastKind = daily or hourly
bool AccuWeather::getForecast(const int     locationKey,
                              const QString forecastType)
{
    if (locationKey == 0)
    {
        qDebug() << "error --- aw_locationKey = 0";
        return false;
    }

    QString forecastKind
            = (forecastType == FORECASTS_DAYS[0] ||
               forecastType == FORECASTS_DAYS[1] ||
               forecastType == FORECASTS_DAYS[2] ||
               forecastType == FORECASTS_DAYS[3])
            ? DAILY
            : HOURLY;

    QString url
            = "http://dataservice.accuweather.com/forecasts/v1//"
            + forecastKind
            + "/"
            + forecastType
            + "/"
            + QString::number(locationKey)
            + "?apikey="
            + aw_apiKey;

    getData(url);

    if (url == "error")
    {
        qDebug() << "error --- getForecast - getData";
        return false;
    }

    if (forecastKind == DAILY)
    {
        getForecastDay();
    }

    if (forecastKind == HOURLY)
    {
        getForecastHour();
    }
    return true;
}

void AccuWeather::getForecastDay()
{
    forecastData.clear();

    QJsonObject obj = aw_doc.object();
    QJsonArray  arr = obj.value("DailyForecasts").toArray();

    ForecastData fd;

    QString s;

    for (const QJsonValue value : arr)
    {
        s = value.toObject().value("Date").toString();

        fd.date = getDate(s);

        fd.temperatureMinF = value.toObject().value("Temperature")
                                             .toObject().value("Minimum")
                                             .toObject().value("Value")
                                             .toInt();
        fd.temperatureMaxF = value.toObject().value("Temperature")
                                             .toObject().value("Maximum")
                                             .toObject().value("Value")
                                             .toInt();
        fd.iconDayNo       = value.toObject().value("Day")
                                             .toObject().value("Icon")
                                             .toInt();
        fd.iconPhraseDay   = value.toObject().value("Day")
                                             .toObject().value("IconPhrase")
                                             .toString();
        fd.iconNightNo     = value.toObject().value("Night")
                                             .toObject().value("Icon")
                                             .toInt();
        fd.iconPhraseNight = value.toObject().value("Night")
                                             .toObject().value("IconPhrase")
                                             .toString();
        forecastData.push_back(fd);
    }
}

void AccuWeather::getForecastHour()
{
    forecastData.clear();

    QJsonArray arr = aw_doc.array();

    ForecastData fd;

    QString s;

    for (const QJsonValue value : arr)
    {
        s = value.toObject().value("DateTime").toString();

        fd.date = getDate(s);
        fd.time = getTime(s);

        fd.iconNo       = value.toObject().value("WeatherIcon")
                                          .toInt();
        fd.iconPhrase   = value.toObject().value("IconPhrase")
                                          .toString();
        fd.isDayNight   = value.toObject().value("IsDaylight")
                                          .toBool();
        fd.temperatureF = value.toObject().value("Temperature")
                                          .toObject().value("Value")
                                          .toInt();
        forecastData.push_back(fd);
    }
}

bool AccuWeather::getCountriesList(const QString cityName,
                                         QMap<QString, int> &countries)
{
    QString url
            = "http://dataservice.accuweather.com"
              "/locations/v1/cities/autocomplete?apikey="
            + aw_apiKey
            + "&q="
            + cityName;

    getData(url);

    if (url == "error")
    {
        qDebug() << "error --- getCountryList - getData";
        return false;
    }

    QJsonArray arr = aw_doc.array();

    for (const QJsonValue value : arr)
    {
        QString key     = value.toObject().value("Key")
                               .toString();
        QString country = value.toObject().value("Country")
                               .toObject().value("LocalizedName")
                               .toString();
        QString administrativeArea
                        = value.toObject().value("AdministrativeArea")
                               .toObject().value("LocalizedName")
                               .toString();
        countries[country
                + "("
                + administrativeArea
                + ")"]
                = key.toInt();
     }
    return true;
}

ForecastData AccuWeather::searchForecast(const QDate d)
{
    for (ForecastData fd : forecastData)
    {
        if (fd.date == d)
            return fd;
    }
    return ForecastData();
}

ForecastData AccuWeather::searchForecast(const QTime t)
{
    for (ForecastData fd : forecastData)
    {
        if (fd.time == t)
            return fd;
    }
    return ForecastData();
}

void AccuWeather::getData(QString &url)
{
    QNetworkAccessManager networkAccessManager;
    QEventLoop            eventLoop;

    QObject::connect(&networkAccessManager, SIGNAL(finished(QNetworkReply*)),
                     &eventLoop,            SLOT  (quit    (              )));

    QNetworkReply *reply =
            networkAccessManager.get(QNetworkRequest(QUrl(url)));

    eventLoop.exec();

    if (reply->error())
        url = "error";
    else
        aw_doc = QJsonDocument::fromJson(reply->readAll());

    reply->deleteLater();
}

QDate AccuWeather::getDate(const QString s)
{
    // yyyy-mm-ddThh:mm:ss+hh:mm;

    return QDate::fromString(s.left(10), "yyyy-MM-dd");
}

QTime AccuWeather::getTime(const QString s)
{
    // yyyy-mm-ddThh:mm:ss+hh:mm;

    return QTime::fromString(s.mid(11, 8), "HH:mm:ss");
}
