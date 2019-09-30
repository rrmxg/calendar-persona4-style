///////////////////////////////////////////////////////////////////////////////
#include "mainwindow.h"
#include "ui_mainwindow.h"
///////////////////////////////////////////////////////////////////////////////
#include <QPushButton>
#include <QVBoxLayout>
#include <QRegExpValidator>
///////////////////////////////////////////////////////////////////////////////
#include <QDebug>
///////////////////////////////////////////////////////////////////////////////

Mainwindow::Mainwindow(QWidget *parent) :
    QDialog(parent, Qt::Window | Qt::FramelessWindowHint),
    ui(new Ui::Mainwindow)
{
    ui->setupUi(this);

    // SET PIXMAPS
    {
        g_pixmapWeather[0].w
                .convertFromImage(QImage(":/images/w-sun.png"       ));
        g_pixmapWeather[1].w
                .convertFromImage(QImage(":/images/w-sun-cloud.png" ));
        g_pixmapWeather[2].w
                .convertFromImage(QImage(":/images/w-cloud.png"     ));
        g_pixmapWeather[3].w
                .convertFromImage(QImage(":/images/w-rain.png"      ));
        g_pixmapWeather[4].w
                .convertFromImage(QImage(":/images/w-rain-cloud.png"));
        g_pixmapWeather[5].w
                .convertFromImage(QImage(":/images/w-snow.png"      ));
        g_pixmapWeather[6].w
                .convertFromImage(QImage(":/images/w-snow-cloud.png"));

        g_pixmapWeather[0].wActive
                .convertFromImage(QImage(":/images/w-active-sun.png"       ));
        g_pixmapWeather[1].wActive
                .convertFromImage(QImage(":/images/w-active-sun-cloud.png" ));
        g_pixmapWeather[2].wActive
                .convertFromImage(QImage(":/images/w-active-cloud.png"     ));
        g_pixmapWeather[3].wActive
                .convertFromImage(QImage(":/images/w-active-rain.png"      ));
        g_pixmapWeather[4].wActive
                .convertFromImage(QImage(":/images/w-active-rain-cloud.png"));
        g_pixmapWeather[5].wActive
                .convertFromImage(QImage(":/images/w-active-snow.png"      ));
        g_pixmapWeather[6].wActive
                .convertFromImage(QImage(":/images/w-active-snow-cloud.png"));

        g_pixmapOther.question
                .convertFromImage(QImage(":/images/question.png"       ));
        g_pixmapOther.questionActive
                .convertFromImage(QImage(":/images/question-active.png"));
        g_pixmapOther.pointerLeft
                .convertFromImage(QImage(":/images/pointer-left.png"   ));
        g_pixmapOther.pointerRight
                .convertFromImage(QImage(":/images/pointer-right.png"  ));
    }

    // SET OTHER PARAMETERS
    {
        ui->label_horizontal->setStyleSheet(BLACK_BACKGROUND);

        ui->scrollArea->setStyleSheet("background-color: transparent;");
        ui->scrollArea->setFocusPolicy(Qt::NoFocus);

        g_date = QDate::currentDate();
        g_hour = QTime::currentTime().hour();

        g_countDay = 0;
    }

    g_pSettings = new QSettings(ORGANIZATION_NAME, APPLICATION_NAME, this);
    {
        g_cityName = g_pSettings->value(CITY_KEY, CITY_NAME).toString();

        ui->label_cityName->setText(g_cityName);

        g_countryArea = g_pSettings->value(COUNTRY_KEY, COUNTRY_NAME).toString();

        g_accuweatherFiveDays.setApiKey(APIKEY);
        g_accuweatherOneHour .setApiKey(APIKEY);

        if (g_accuweatherOneHour.getCountriesList(g_cityName, g_countries))
        {
            g_locationKey = g_countries.find(g_countryArea).value();
        }

        // UPDATE SCROLL AREA
        {
            QWidget *widget        = new QWidget    (this);
            QHBoxLayout *horLayout = new QHBoxLayout(this);
            {
                horLayout->setSpacing(24);
            }

            updateScrollArea();

            for (int i = 0; i < 13; i++)
            {
                QVBoxLayout *verLayout = new QVBoxLayout(this);
                {
                    verLayout->addWidget(&g_prefab[i].dayOfWeek);
                    verLayout->addWidget(&g_prefab[i].date     );
                    verLayout->addWidget(&g_prefab[i].weather  );

                    verLayout->setSpacing(0);
                }
                horLayout->addLayout(verLayout, i);
            }
            widget->setLayout(horLayout);

            ui->scrollArea->setWidget    (widget        );
            ui->scrollArea->show         (              );
            ui->scrollArea->ensureVisible(981, 0, 50, 50); // middle date
        }

        updateForecastOneHour ();
        updateForecastFiveDays();
    }

    g_pMenu = new QMenu(this);
    {
        g_pMenu->addAction("&Accuweather");
        g_pMenu->addAction("&City name");
        g_pMenu->addAction("&Readme");
        g_pMenu->addAction("&Exit");

        connect(g_pMenu, SIGNAL(triggered (QAction*)),
                this,    SLOT  (menuAction(QAction*)));
    }

    g_pDialog = new QDialog(this);
    {
        QVBoxLayout *verLayout = new QVBoxLayout(g_pDialog);
        {
            QHBoxLayout *horLayoutUp = new QHBoxLayout(g_pDialog);
            {
                QLabel *label_cityName = new QLabel(g_pDialog);
                {
                    label_cityName->setText("City name");

                    label_cityName->setMaximumSize(80, 20);
                    label_cityName->setMinimumSize(80, 20);
                }
                horLayoutUp->addWidget(label_cityName);

                g_pLineEdit = new QLineEdit(g_pDialog);
                {
                    g_pLineEdit->setValidator
                            (new QRegExpValidator(QRegExp("[a-z]{0,}")));

                    g_pLineEdit->setPlaceholderText("example: moscow");

                    g_pLineEdit->setMinimumSize(300, 20);
                    g_pLineEdit->setMaximumSize(300, 20);
                }
                horLayoutUp->addWidget(g_pLineEdit);

                QPushButton *buttonSearch = new QPushButton(g_pDialog);
                {
                    buttonSearch->setText("Search");

                    buttonSearch->setMinimumSize(50, 20);
                    buttonSearch->setMaximumSize(50, 20);

                    connect(buttonSearch, &QPushButton::clicked, [this]()
                    {
                        if (g_pLineEdit->text().isEmpty())
                            return;

                        g_countries.clear();

                        g_cityName = g_pLineEdit->text();
                        ui->label_cityName->setText(g_cityName);
                        g_pSettings->setValue(CITY_KEY, g_cityName);

                        if (g_accuweatherOneHour
                                .getCountriesList(g_cityName, g_countries))
                        {
                            g_pComboBox->addItems(g_countries.keys());

                        }
                    });
                }
                horLayoutUp->addWidget(buttonSearch);
            }
            verLayout->addLayout(horLayoutUp);

            QHBoxLayout *horLayoutDown = new QHBoxLayout(g_pDialog);
            {
                QLabel *label_counties = new QLabel(g_pDialog);
                {
                    label_counties->setText("Countries");

                    label_counties->setMaximumSize(80, 20);
                    label_counties->setMinimumSize(80, 20);
                }
                horLayoutDown->addWidget(label_counties);

                g_pComboBox = new QComboBox(g_pDialog);
                horLayoutDown->addWidget(g_pComboBox);
            }
            verLayout->addLayout(horLayoutDown);

            QPushButton *buttonOk = new QPushButton(g_pDialog);
            {
                buttonOk->setText("OK");

                connect(buttonOk, &QPushButton::clicked, [this]()
                {
                    if (g_pLineEdit->text().isEmpty() || g_countries.isEmpty())
                        return;

                    g_countryArea = g_pComboBox->currentText();
                    g_pSettings->setValue(COUNTRY_KEY, g_countryArea);

                    g_locationKey = g_countries.find(g_countryArea).value();

                    updateForecastOneHour ();
                    updateForecastFiveDays();

                    g_pDialog->close();
                });
            }
            verLayout->addWidget(buttonOk);
        }
        g_pDialog->setLayout(verLayout);
        g_pDialog->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    }

    ui->label_dateMonth->setText(QString::number(QDate::currentDate().month()));
    ui->label_dateYear->setText(QString::number(QDate::currentDate().year()));

    g_timer = startTimer(10000);
}

Mainwindow::~Mainwindow()
{
    killTimer(g_timer);

    g_countries.clear();

    delete ui;
}

void Mainwindow::updateTime()
{
    ui->label_currentTime->setText(QTime::currentTime().toString("hh:mm"));

    ui->label_dateMonth->setText(QString::number(QDate::currentDate().month()));
    ui->label_dateYear->setText(QString::number(QDate::currentDate().year()));

    if (QTime::currentTime().toString() == "00:01:000")
    {
        updateForecastFiveDays();
    }

    if (QTime::currentTime().hour() != g_hour)
    {
        updateForecastOneHour();
    }
}

void Mainwindow::updateForecastFiveDays()
{
    if (g_countries.isEmpty())
        return;

    if (g_accuweatherFiveDays.getForecast(g_locationKey, FORECASTS_DAYS[1]))
    {
        updateScrollArea();

        ui->label_temperatureDay->setText
                (QString::number(g_accuweatherFiveDays
                                 .forecastData[0].temperatureMaxC()) + "°");

        ui->label_temperatureNight->setText
                (QString::number(g_accuweatherFiveDays
                                 .forecastData[0].temperatureMinC()) + "°");
    }
}

void Mainwindow::updateForecastOneHour()
{
    if (g_countries.isEmpty())
        return;

    if (g_accuweatherOneHour.getForecast(g_locationKey, FORECASTS_HOURS[0]))
    {
        ui->label_temperatureNow->setText
                (QString::number(g_accuweatherOneHour
                                 .forecastData[0].temperatureC()) + "°");

        g_hour = QTime::currentTime().hour();
    }
}

void Mainwindow::updatesAfterClicking(const int i)
{
    if (i == 0)
    {
        g_date     = g_date.currentDate();
        g_countDay = 0;
    }
    else
    {
        g_date      = g_date.addDays(i);
        g_countDay += i;
    }

    // label change date
    {
        if (g_countDay == 0)
            ui->label_counter->setText(" ");

        if (g_countDay == 1)
            ui->label_counter->setText("Tomorrow");

        if (g_countDay == -1)
            ui->label_counter->setText("Yesterday");

        if (g_countDay > 1)
            ui->label_counter->setText("After "
                                           + QString::number(g_countDay)
                                           + " days");

        if (g_countDay < -1)
            ui->label_counter->setText(QString::number(g_countDay * -1)
                                           + " days ago");
    }

    // label pointer & horizontal
    {
        if (g_countDay == 0)
        {
            ui->label_pointer   ->setPixmap(g_pixmapOther.empty);
            ui->label_horizontal->setStyleSheet(BLACK_BACKGROUND);
        }

        if (g_countDay > 0)
        {
            ui->label_pointer   ->setPixmap(g_pixmapOther.pointerRight);
            ui->label_horizontal->setStyleSheet(YELLOW_BACKGROUND);
        }

        if (g_countDay < 0)
        {
            ui->label_pointer   ->setPixmap(g_pixmapOther.pointerLeft);
            ui->label_horizontal->setStyleSheet(YELLOW_BACKGROUND);
        }
    }

    // label date year & month
    {
        ui->label_dateYear ->setNum(g_date.year ());
        ui->label_dateMonth->setNum(g_date.month());
    }

    // label day & night temperatures
    {
        ForecastData fd = g_accuweatherFiveDays.searchForecast(g_date);

        qDebug() << g_date;

        ui->label_temperatureDay  ->setText
                (QString::number(fd.temperatureMaxC()));
        ui->label_temperatureNight->setText
                (QString::number(fd.temperatureMinC()));
    }

    updateScrollArea();
}

void Mainwindow::updateScrollArea()
{
    QString str;
    QDate   d;

    for (int i = 0, j = -6; i < 13; i++, j++)
    {
        d = g_date.currentDate().addDays(j + g_countDay);
        {
            str = QLocale().dayName(d.dayOfWeek());
            str.resize(3);

            g_prefab[i].dayOfWeek.setText      (str.toUpper());
            g_prefab[i].dayOfWeek.setFixedSize (60, 20);
            g_prefab[i].dayOfWeek.setAlignment (Qt::AlignCenter);
        }
        {
            g_prefab[i].date.setText      (QString::number(d.day()));
            g_prefab[i].date.setFixedSize (60, 20);
            g_prefab[i].date.setAlignment (Qt::AlignCenter);
        }
        {
            g_prefab[i].weather.setScaledContents(true);
            g_prefab[i].weather.setFixedSize     (60, 60);
        }

        if (j == 0)
        {
            g_prefab[i].dayOfWeek.setStyleSheet(BLUE_TEXT);
            g_prefab[i].date     .setStyleSheet(BLUE_TEXT);
            g_prefab[i].weather  .setPixmap    (updatePixmap(d, true));
        }
        else
        {
            g_prefab[i].dayOfWeek.setStyleSheet(WHITE_TEXT);
            g_prefab[i].date     .setStyleSheet(WHITE_TEXT);
            g_prefab[i].weather  .setPixmap    (updatePixmap(d, false));
        }
    }
}

QPixmap Mainwindow::updatePixmap(const QDate d, const bool b)
{
    if (g_accuweatherFiveDays.forecastData.isEmpty())
    {
        return (b) ? g_pixmapOther.questionActive
                   : g_pixmapOther.question;
    }

    int iconNo{};

    ForecastData fd = g_accuweatherFiveDays.searchForecast(d);

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
        return (b) ? g_pixmapWeather[0].wActive
                   : g_pixmapWeather[0].w;
    }

    if (iconNo == 5 ||
        iconNo == 6)
    {
        return (b) ? g_pixmapWeather[1].wActive
                   : g_pixmapWeather[1].w;
    }

    if (iconNo == 7  ||
        iconNo == 8  ||
        iconNo == 11 ||
        iconNo == 37 ||
        iconNo == 38)
    {
        return (b) ? g_pixmapWeather[2].wActive
                   : g_pixmapWeather[2].w;
    }

    if (iconNo == 12 ||
       (iconNo >= 15 && iconNo <= 21) ||
       (iconNo >= 41 && iconNo <= 43))
    {
        return (b) ? g_pixmapWeather[3].wActive
                   : g_pixmapWeather[3].w;
    }

    if (iconNo == 13 ||
        iconNo == 14 ||
        iconNo == 39 ||
        iconNo == 40)
    {
        return (b) ? g_pixmapWeather[4].wActive
                   : g_pixmapWeather[4].w;
    }

    if (iconNo == 22 ||
        iconNo == 25 ||
        iconNo == 26 ||
        iconNo == 29)
    {
        return (b) ? g_pixmapWeather[5].wActive
                   : g_pixmapWeather[5].w;
    }

    if (iconNo == 23 ||
        iconNo == 44)
    {
        return (b) ? g_pixmapWeather[6].wActive
                   : g_pixmapWeather[6].w;
    }

    return (b) ? g_pixmapOther.questionActive
               : g_pixmapOther.question;
}
