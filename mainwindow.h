#ifndef MAINWINDOW_H
#define MAINWINDOW_H
///////////////////////////////////////////////////////////////////////////////
#define BLUE_TEXT  "color: rgb( 41, 112, 164); font: 15pt \"MS Shell Dlg 2\";"
#define WHITE_TEXT "color: rgb(255, 255, 255); font: 15pt \"MS Shell Dlg 2\";"

#define BLACK_BACKGROUND  "background-color: rgb(  0,   0, 0);"
#define YELLOW_BACKGROUND "background-color: rgb(244, 232, 8);"
///////////////////////////////////////////////////////////////////////////////
#define ORGANIZATION_NAME   "FreeSoft"
#define ORGANIZATION_DOMAIN "_domain_"
#define APPLICATION_NAME    "calendarpersona"
///////////////////////////////////////////////////////////////////////////////
#include "accuweather.h"
///////////////////////////////////////////////////////////////////////////////
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QMenu>
#include <QDate>
#include <QLabel>
#include <QSettings>
///////////////////////////////////////////////////////////////////////////////

const int LEFT_DAY  = -1;
const int RIGHT_DAY = 1;
const int ZERO      = 0;

const QString APIKEY = ;

const QString CITY_NAME = "moscow";
const QString CITY_KEY  = "city_name";

const QString COUNTRY_NAME = "Russia(Moscow)";
const QString COUNTRY_KEY  = "country_name";

const QString ZERO_DEGREE = "0°";

const QString TEMPERATURE_DAY   = "temperature_day";
const QString TEMPERATURE_NIGHT = "temperature_night";
const QString TEMPERATURE_NOW   = "temperature_now";

struct Position
{
    int dx;
    int dy;
};

struct ScrollAreaPrefab
{
    QLabel dayOfWeek;
    QLabel date     ;
    QLabel weather  ;
};

struct PixmapOther
{
    QPixmap empty;
    QPixmap pointerLeft;
    QPixmap pointerRight;

    QPixmap question;
    QPixmap questionActive;
};

struct PixmapWeather
{
    QPixmap w      ;
    QPixmap wActive;
};

namespace Ui
{
class Mainwindow;
}

class Mainwindow : public QDialog
{
    Q_OBJECT

protected:
    void contextMenuEvent (QContextMenuEvent *);
    void timerEvent       (QTimerEvent       *);
    void keyPressEvent    (QKeyEvent         *);
    void mouseMoveEvent   (QMouseEvent       *);
    void mousePressEvent  (QMouseEvent       *);

public:
    explicit Mainwindow(QWidget *parent = nullptr);
    ~Mainwindow();

    void updatesAfterClicking(const int i);

    void updateScrollArea();

    void updateTime();

    QPixmap updatePixmap(const QDate d,
                         const bool  b);

    void updateForecastFiveDays();
    void updateForecastOneHour ();

private slots:
    void menuAction(QAction *action);

private:
    Ui::Mainwindow *ui;

    Position g_position;

    QMenu     *g_pMenu;
    QDialog   *g_pDialog;
    QLineEdit *g_pLineEdit;
    QComboBox *g_pComboBox;

    QDate g_date;
    int   g_timer;

    ScrollAreaPrefab g_prefab[13];

    int g_countDay;
    int g_hour    ;

    QSettings *g_pSettings;

    PixmapOther   g_pixmapOther;
    PixmapWeather g_pixmapWeather[7];

    AccuWeather g_accuweatherFiveDays;
    AccuWeather g_accuweatherOneHour;

    QMap<QString, int> g_countries;

    int g_locationKey{};

    QString g_cityName   {};
    QString g_countryArea{};
};

#endif // MAINWINDOW_H
