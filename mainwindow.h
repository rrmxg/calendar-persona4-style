#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//=============================================================================
#define BLUE_TEXT  "color: rgb( 41, 112, 164); font: 15pt \"MS Shell Dlg 2\";"
#define WHITE_TEXT "color: rgb(255, 255, 255); font: 15pt \"MS Shell Dlg 2\";"

#define BLACK_BACKGROUND  "background-color: rgb(  0,   0, 0);"
#define YELLOW_BACKGROUND "background-color: rgb(244, 232, 8);"
//=============================================================================
#define ORGANIZATION_NAME   "FreeSoft"
#define ORGANIZATION_DOMAIN "_domain_"
#define APPLICATION_NAME    "calendarpersona"
//=============================================================================
#include "accuweather.h"
//=============================================================================
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QMenu>
#include <QDate>
#include <QLabel>
#include <QSettings>
//=============================================================================
const int LEFT_DAY  = -1;
const int RIGHT_DAY = 1;
const int ZERO      = 0;
//=============================================================================
const QString APIKEY = ;
//=============================================================================
const QString CITY_NAME = "moscow";
const QString CITY_KEY  = "city_name";
//=============================================================================
const QString COUNTRY_NAME = "Russia(Moscow)";
const QString COUNTRY_KEY  = "country_name";
//=============================================================================
const QString ZERO_DEGREE = "0°";
//=============================================================================
const QString TEMPERATURE_DAY   = "temperature_day";
const QString TEMPERATURE_NIGHT = "temperature_night";
const QString TEMPERATURE_NOW   = "temperature_now";
//=============================================================================
/// = 245, 265, 285
const int prefabY[3]  =
{
    245, 265, 285
};
/// = -130, -46, 38, 122, 206, 290, 374, 458, 542, 626, 710
const int prefabX[11] =
{
    -130, -46, 38, 122, 206, 290, 374, 458, 542, 626, 710
};
//=============================================================================
struct Position
{
    int dx;
    int dy;
};
struct Prefab
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
//=============================================================================

namespace Ui
{
class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

protected:
    void contextMenuEvent (QContextMenuEvent *);
    void timerEvent       (QTimerEvent       *);
    void keyPressEvent    (QKeyEvent         *);
    void mouseMoveEvent   (QMouseEvent       *);
    void mousePressEvent  (QMouseEvent       *);

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updatesAfterClicking(const int i);

    void updatePrefabs();

    void updateTime();

    QPixmap updatePixmap(const QDate d,
                         const bool  b);

    void updateForecastFiveDays();
    void updateForecastOneHour ();

private slots:
    void menuAction(QAction *action);

private:
    Ui::MainWindow *ui;

    Position mw_position;

    QMenu     *mw_pMenu;
    QDialog   *mw_pDialog;
    QLineEdit *mw_pLineEdit;
    QComboBox *mw_pComboBox;

    QDate mw_date;
    int   mw_timer;

    Prefab mw_prefab[11];

    int mw_countDay;
    int mw_hour    ;

    QSettings *mw_pSettings;

    PixmapOther   mw_pixmapOther;
    PixmapWeather mw_pixmapWeather[7];

    AccuWeather mw_accuweatherFiveDays;
    AccuWeather mw_accuweatherOneHour;

    QMap<QString, int> mw_countries;

    int mw_locationKey{};

    QString mw_cityName   {};
    QString mw_countryArea{};

    //

    int mw_timerAnimation;
    int mw_countAnimation;
    //
    void updateAnimation();
    void resetAnimation ();
    //
    bool mw_isKey;
    //
    int mw_directionMovement;
};

#endif // MAINWINDOW_H
