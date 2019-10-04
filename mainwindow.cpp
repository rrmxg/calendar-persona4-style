//=============================================================================
#include "mainwindow.h"
#include "ui_mainwindow.h"
//=============================================================================
#include <QPushButton>
#include <QVBoxLayout>
#include <QRegExpValidator>
//=============================================================================
#include <QDebug>
//=============================================================================

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent, Qt::Window | Qt::FramelessWindowHint),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // SET PIXMAPS
    {
        mw_pixmapWeather[0].w
                .convertFromImage(QImage(":/images/w-sun.png"       ));
        mw_pixmapWeather[1].w
                .convertFromImage(QImage(":/images/w-sun-cloud.png" ));
        mw_pixmapWeather[2].w
                .convertFromImage(QImage(":/images/w-cloud.png"     ));
        mw_pixmapWeather[3].w
                .convertFromImage(QImage(":/images/w-rain.png"      ));
        mw_pixmapWeather[4].w
                .convertFromImage(QImage(":/images/w-rain-cloud.png"));
        mw_pixmapWeather[5].w
                .convertFromImage(QImage(":/images/w-snow.png"      ));
        mw_pixmapWeather[6].w
                .convertFromImage(QImage(":/images/w-snow-cloud.png"));

        mw_pixmapWeather[0].wActive
                .convertFromImage(QImage(":/images/w-active-sun.png"       ));
        mw_pixmapWeather[1].wActive
                .convertFromImage(QImage(":/images/w-active-sun-cloud.png" ));
        mw_pixmapWeather[2].wActive
                .convertFromImage(QImage(":/images/w-active-cloud.png"     ));
        mw_pixmapWeather[3].wActive
                .convertFromImage(QImage(":/images/w-active-rain.png"      ));
        mw_pixmapWeather[4].wActive
                .convertFromImage(QImage(":/images/w-active-rain-cloud.png"));
        mw_pixmapWeather[5].wActive
                .convertFromImage(QImage(":/images/w-active-snow.png"      ));
        mw_pixmapWeather[6].wActive
                .convertFromImage(QImage(":/images/w-active-snow-cloud.png"));

        mw_pixmapOther.question
                .convertFromImage(QImage(":/images/question.png"       ));
        mw_pixmapOther.questionActive
                .convertFromImage(QImage(":/images/question-active.png"));
        mw_pixmapOther.pointerLeft
                .convertFromImage(QImage(":/images/pointer-left.png"   ));
        mw_pixmapOther.pointerRight
                .convertFromImage(QImage(":/images/pointer-right.png"  ));
    }

    // SET OTHER PARAMETERS
    {
        ui->label_horizontal->setStyleSheet(BLACK_BACKGROUND);

//        ui->scrollArea->setStyleSheet("background-color: transparent;");
//        ui->scrollArea->setFocusPolicy(Qt::NoFocus);

        mw_date = QDate::currentDate();
        mw_hour = QTime::currentTime().hour();

        mw_countDay = 0;
    }

    mw_pSettings = new QSettings(ORGANIZATION_NAME, APPLICATION_NAME, this);
    {
        mw_cityName = mw_pSettings->value(CITY_KEY, CITY_NAME).toString();

        ui->label_cityName->setText(mw_cityName);

        mw_countryArea = mw_pSettings->value
                (COUNTRY_KEY, COUNTRY_NAME).toString();

        mw_accuweatherFiveDays.setApiKey(APIKEY);
        mw_accuweatherOneHour .setApiKey(APIKEY);

        if (mw_accuweatherOneHour.getCountriesList(mw_cityName, mw_countries))
        {
            mw_locationKey = mw_countries.find(mw_countryArea).value();
        }

        // SET POSITION PREFABS
        {
            updatePrefabs();

            for (int i = 0; i < 11; i++)
            {
                {
                    mw_prefab[i].dayOfWeek.setParent(ui->parent);
                    mw_prefab[i].dayOfWeek.setGeometry
                            (prefabX[i], prefabY[0], 60, 20);
                    mw_prefab[i].dayOfWeek.show();
                }
                {
                    mw_prefab[i].date.setParent(ui->parent);
                    mw_prefab[i].date.setGeometry
                            (prefabX[i], prefabY[1], 60, 20);
                    mw_prefab[i].date.show();
                }
                {
                    mw_prefab[i].weather.setParent(ui->parent);
                    mw_prefab[i].weather.setGeometry
                            (prefabX[i], prefabY[2], 60, 20);
                    mw_prefab[i].weather.show();
                }
            }
        }
        updateForecastOneHour ();
        updateForecastFiveDays();
    }

    mw_pMenu = new QMenu(this);
    {
        mw_pMenu->addAction("&Accuweather");
        mw_pMenu->addAction("&City name");
        mw_pMenu->addAction("&Readme");
        mw_pMenu->addAction("&Exit");

        connect(mw_pMenu, SIGNAL(triggered (QAction*)),
                this,    SLOT  (menuAction(QAction*)));
    }

    mw_pDialog = new QDialog(this);
    {
        QVBoxLayout *verLayout = new QVBoxLayout(mw_pDialog);
        {
            QHBoxLayout *horLayoutUp = new QHBoxLayout(mw_pDialog);
            {
                QLabel *label_cityName = new QLabel(mw_pDialog);
                {
                    label_cityName->setText("City name");

                    label_cityName->setMaximumSize(80, 20);
                    label_cityName->setMinimumSize(80, 20);
                }
                horLayoutUp->addWidget(label_cityName);

                mw_pLineEdit = new QLineEdit(mw_pDialog);
                {
                    mw_pLineEdit->setValidator
                            (new QRegExpValidator(QRegExp("[a-z]{0,}")));

                    mw_pLineEdit->setPlaceholderText("example: moscow");

                    mw_pLineEdit->setMinimumSize(300, 20);
                    mw_pLineEdit->setMaximumSize(300, 20);
                }
                horLayoutUp->addWidget(mw_pLineEdit);

                QPushButton *buttonSearch = new QPushButton(mw_pDialog);
                {
                    buttonSearch->setText("Search");

                    buttonSearch->setMinimumSize(50, 20);
                    buttonSearch->setMaximumSize(50, 20);

                    connect(buttonSearch, &QPushButton::clicked, [this]()
                    {
                        if (mw_pLineEdit->text().isEmpty())
                            return;

                        mw_countries.clear();

                        mw_cityName = mw_pLineEdit->text();
                        ui->label_cityName->setText(mw_cityName);
                        mw_pSettings->setValue(CITY_KEY, mw_cityName);

                        if (mw_accuweatherOneHour
                                .getCountriesList(mw_cityName, mw_countries))
                        {
                            mw_pComboBox->addItems(mw_countries.keys());

                        }
                    });
                }
                horLayoutUp->addWidget(buttonSearch);
            }
            verLayout->addLayout(horLayoutUp);

            QHBoxLayout *horLayoutDown = new QHBoxLayout(mw_pDialog);
            {
                QLabel *label_counties = new QLabel(mw_pDialog);
                {
                    label_counties->setText("Countries");

                    label_counties->setMaximumSize(80, 20);
                    label_counties->setMinimumSize(80, 20);
                }
                horLayoutDown->addWidget(label_counties);

                mw_pComboBox = new QComboBox(mw_pDialog);
                horLayoutDown->addWidget(mw_pComboBox);
            }
            verLayout->addLayout(horLayoutDown);

            QPushButton *buttonOk = new QPushButton(mw_pDialog);
            {
                buttonOk->setText("OK");

                connect(buttonOk, &QPushButton::clicked, [this]()
                {
                    if (mw_pLineEdit->text().isEmpty() || mw_countries.isEmpty())
                        return;

                    mw_countryArea = mw_pComboBox->currentText();
                    mw_pSettings->setValue(COUNTRY_KEY, mw_countryArea);

                    mw_locationKey = mw_countries.find(mw_countryArea).value();

                    updateForecastOneHour ();
                    updateForecastFiveDays();

                    mw_pDialog->close();
                });
            }
            verLayout->addWidget(buttonOk);
        }
        mw_pDialog->setLayout(verLayout);
        mw_pDialog->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    }

    ui->label_dateMonth->setText(QString::number(QDate::currentDate().month()));
    ui->label_dateYear ->setText(QString::number(QDate::currentDate().year ()));

    // TIMERS
    {
        mw_timer = startTimer(10000);

        mw_countAnimation = 0;

        mw_isKey = true;
    }
}

MainWindow::~MainWindow()
{
    killTimer(mw_timer);

    mw_countries.clear();

    delete ui;
}

void MainWindow::updateTime()
{
    ui->label_currentTime->setText(QTime::currentTime().toString("hh:mm"));

    ui->label_dateMonth->setText(QString::number(QDate::currentDate().month()));
    ui->label_dateYear ->setText(QString::number(QDate::currentDate().year ()));

    if (QTime::currentTime().toString() == "00:01:000")
    {
        updateForecastFiveDays();
    }

    if (QTime::currentTime().hour() != mw_hour)
    {
        updateForecastOneHour();
    }
}

void MainWindow::updateForecastFiveDays()
{
    if (mw_countries.isEmpty())
        return;

    if (mw_accuweatherFiveDays.getForecast(mw_locationKey, FORECASTS_DAYS[1]))
    {
        updatePrefabs();

        ui->label_temperatureDay->setText
                (QString::number(mw_accuweatherFiveDays
                                 .forecastData[0].temperatureMaxC()) + "°");

        ui->label_temperatureNight->setText
                (QString::number(mw_accuweatherFiveDays
                                 .forecastData[0].temperatureMinC()) + "°");
    }
}

void MainWindow::updateForecastOneHour()
{
    if (mw_countries.isEmpty())
        return;

    if (mw_accuweatherOneHour.getForecast(mw_locationKey, FORECASTS_HOURS[0]))
    {
        ui->label_temperatureNow->setText
                (QString::number(mw_accuweatherOneHour
                                 .forecastData[0].temperatureC()) + "°");

        mw_hour = QTime::currentTime().hour();
    }
}

void MainWindow::updatesAfterClicking(const int i)
{
    // ANIMATION START
    {
        mw_directionMovement = i * LEFT_DAY;

        mw_isKey = false;

        mw_timerAnimation = startTimer(10);
    }

    if (i == 0)
    {
        mw_date     = mw_date.currentDate();
        mw_countDay = 0;
    }
    else
    {
        mw_date      = mw_date.addDays(i);
        mw_countDay += i;
    }

    // label change date
    {
        if (mw_countDay == 0)
            ui->label_counter->setText(" ");

        if (mw_countDay == 1)
            ui->label_counter->setText("Tomorrow");

        if (mw_countDay == -1)
            ui->label_counter->setText("Yesterday");

        if (mw_countDay > 1)
            ui->label_counter->setText("After "
                                           + QString::number(mw_countDay)
                                           + " days");

        if (mw_countDay < -1)
            ui->label_counter->setText(QString::number(mw_countDay * -1)
                                           + " days ago");
    }

    // label pointer & horizontal
    {
        if (mw_countDay == 0)
        {
            ui->label_pointer   ->setPixmap(mw_pixmapOther.empty);
            ui->label_horizontal->setStyleSheet(BLACK_BACKGROUND);
        }

        if (mw_countDay > 0)
        {
            ui->label_pointer   ->setPixmap(mw_pixmapOther.pointerRight);
            ui->label_horizontal->setStyleSheet(YELLOW_BACKGROUND);
        }

        if (mw_countDay < 0)
        {
            ui->label_pointer   ->setPixmap(mw_pixmapOther.pointerLeft);
            ui->label_horizontal->setStyleSheet(YELLOW_BACKGROUND);
        }
    }

    // label date year & month
    {
        ui->label_dateYear ->setNum(mw_date.year ());
        ui->label_dateMonth->setNum(mw_date.month());
    }

    // label day & night temperatures
    {
        ForecastData fd = mw_accuweatherFiveDays.searchForecast(mw_date);

//        qDebug() << mw_date;

        ui->label_temperatureDay  ->setText
                (QString::number(fd.temperatureMaxC()));
        ui->label_temperatureNight->setText
                (QString::number(fd.temperatureMinC()));
    }

//    updatePrefabs();
}

void MainWindow::updatePrefabs()
{
    QString str;
    QDate   d;

    for (int i = 0, j = -3; i < 11; i++, j++)
    {
        d = mw_date.currentDate().addDays(j + mw_countDay);
        {
            str = QLocale().dayName(d.dayOfWeek());
            str.resize(3);

            mw_prefab[i].dayOfWeek.setText     (str.toUpper());
            mw_prefab[i].dayOfWeek.setFixedSize(60, 20);
            mw_prefab[i].dayOfWeek.setAlignment(Qt::AlignCenter);
        }
        {
            mw_prefab[i].date.setText     (QString::number(d.day()));
            mw_prefab[i].date.setFixedSize(60, 20);
            mw_prefab[i].date.setAlignment(Qt::AlignCenter);
        }
        {
            mw_prefab[i].weather.setScaledContents(true);
            mw_prefab[i].weather.setFixedSize     (60, 60);
        }

        if (j == 0)
        {
            mw_prefab[i].dayOfWeek.setStyleSheet(BLUE_TEXT);
            mw_prefab[i].date     .setStyleSheet(BLUE_TEXT);
            mw_prefab[i].weather  .setPixmap    (updatePixmap(d, true));
        }
        else
        {
            mw_prefab[i].dayOfWeek.setStyleSheet(WHITE_TEXT);
            mw_prefab[i].date     .setStyleSheet(WHITE_TEXT);
            mw_prefab[i].weather  .setPixmap    (updatePixmap(d, false));
        }
//        qDebug() << i << " " << j << " " << d;
    }
}

QPixmap MainWindow::updatePixmap(const QDate d, const bool b)
{
//    return mw_pixmapOther.question; // TEST

    if (mw_accuweatherFiveDays.forecastData.isEmpty())
    {
        return (b) ? mw_pixmapOther.questionActive
                   : mw_pixmapOther.question;
    }

    int iconNo{};

    ForecastData fd = mw_accuweatherFiveDays.searchForecast(d);

    iconNo = (QTime::currentTime().hour() > 18 ||
              QTime::currentTime().hour() < 6)
            ? fd.iconNightNo
            : fd.iconDayNo;

    // sun        1-4 24 30-36
    // sun-cloud  5-6
    // cloud      7-8 x9 x10 11 37-38
    // rain       12 15-21 41-43
    // rain-cloud 13-14 39-40
    // snow       22 25-26 x27 x28 29
    // snow-cloud 23 44

    if ((iconNo >= 1  && iconNo <= 4) ||
         iconNo == 24                 ||
        (iconNo >= 30 && iconNo <= 36))
    {
        return (b) ? mw_pixmapWeather[0].wActive
                   : mw_pixmapWeather[0].w;
    }

    if (iconNo == 5 ||
        iconNo == 6)
    {
        return (b) ? mw_pixmapWeather[1].wActive
                   : mw_pixmapWeather[1].w;
    }

    if (iconNo == 7  ||
        iconNo == 8  ||
        iconNo == 11 ||
        iconNo == 37 ||
        iconNo == 38)
    {
        return (b) ? mw_pixmapWeather[2].wActive
                   : mw_pixmapWeather[2].w;
    }

    if (iconNo == 12 ||
       (iconNo >= 15 && iconNo <= 21) ||
       (iconNo >= 41 && iconNo <= 43))
    {
        return (b) ? mw_pixmapWeather[3].wActive
                   : mw_pixmapWeather[3].w;
    }

    if (iconNo == 13 ||
        iconNo == 14 ||
        iconNo == 39 ||
        iconNo == 40)
    {
        return (b) ? mw_pixmapWeather[4].wActive
                   : mw_pixmapWeather[4].w;
    }

    if (iconNo == 22 ||
        iconNo == 25 ||
        iconNo == 26 ||
        iconNo == 29)
    {
        return (b) ? mw_pixmapWeather[5].wActive
                   : mw_pixmapWeather[5].w;
    }

    if (iconNo == 23 ||
        iconNo == 44)
    {
        return (b) ? mw_pixmapWeather[6].wActive
                   : mw_pixmapWeather[6].w;
    }

    return (b) ? mw_pixmapOther.questionActive
               : mw_pixmapOther.question;
}
