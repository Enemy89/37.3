#include "console.h"
#include "ui_console.h"
#include <QScreen>
#include <QPushButton>
#include <QMovie>
#include<QString>
#include <QLabel>
#include <QStringList>
#include <QFileInfo>

int currentChannelIndex=-1;

Console::Console(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Console)
{
    ui->setupUi(this);
    tvWindow = new TV(this);
    tvWindow->show();
    this->setFixedSize(240, 680);
    move(1680, 400);

    QStringList buttonNames = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "null_num"};
    foreach (const QString &buttonName, buttonNames) {
        QPushButton *button = findChild<QPushButton*>(buttonName);
        if (button) {
            connect(button, &QPushButton::clicked, this, &Console::digitButtonClicked);
        } else {
            qDebug() << "Не удалось найти кнопку с именем" << buttonName << "на форме Console";
        }
    }

    QStringList animationButtonNames = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "null_num"};
    foreach (const QString &buttonName, animationButtonNames) {
        QPushButton *button = findChild<QPushButton*>(buttonName);
        if (button) {
            connect(button, &QPushButton::clicked, [this, buttonName]() {
                startAnimation(buttonName);
            });
        } else {
            qDebug() << "Не удалось найти кнопку с именем" << buttonName << "на форме Console";
        }
    }
    connect(ui->plus, &QPushButton::clicked, this, &Console::plusButtonClicked);
    connect(ui->minus, &QPushButton::clicked, this, &Console::minusButtonClicked);
    currentSoundLevel = 0;
            QLabel *tmpSoundLabel = tvWindow->findChild<QLabel*>("tmp_sound");
    if (tmpSoundLabel) {
        tmpSoundLabel->setText(QString("Sound: %1%").arg(currentSoundLevel));
    } else {
        qDebug() << "Не удалось найти QLabel tmp_sound на форме TV";
    }

    connect(ui->last, &QPushButton::clicked, this, &Console::lastButtonClicked);
    connect(ui->next, &QPushButton::clicked, this, &Console::nextButtonClicked);
}


Console::~Console()
{
    delete ui;
}

void Console::on_playButton_clicked()
{
    tvWindow->show();
}

QMap<QString, int> channelIndexMap = {
    {"null_num", 0},
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9}
};

void Console::plusButtonClicked() {
    if (currentSoundLevel < 100) {
        currentSoundLevel += 10;
    }

    QLabel *tmpSoundLabel = tvWindow->findChild<QLabel*>("tmp_sound");
    if (tmpSoundLabel) {
        tmpSoundLabel->setText(QString("Sound: %1%").arg(currentSoundLevel));
    } else {
        qDebug() << "Не удалось найти QLabel tmp_sound на форме TV";
    }
}

void Console::minusButtonClicked() {
    if (currentSoundLevel > 0) {
        currentSoundLevel -= 10;
    }

    QLabel *tmpSoundLabel = tvWindow->findChild<QLabel*>("tmp_sound");
    if (tmpSoundLabel) {
        tmpSoundLabel->setText(QString("Sound: %1%").arg(currentSoundLevel));
    } else {
        qDebug() << "Не удалось найти QLabel tmp_sound на форме TV";
    }
}

QString intToChannelName(int index) {
    QStringList channelNames = {"null_num", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    if (index >= 0 && index < channelNames.size()) {
        return channelNames.at(index);
    } else {
        return QString();
    }
}

void Console::nextButtonClicked() {
    if (lastChannelIndex < 9) {
        lastChannelIndex++;
        qDebug() <<"До передачи "<< lastChannelIndex;
        startAnimation(intToChannelName(lastChannelIndex));
        qDebug() <<"После передачи "<< intToChannelName(lastChannelIndex);
    }
}

void Console::lastButtonClicked() {
    if (lastChannelIndex > 0) {
        lastChannelIndex--;
        qDebug() << lastChannelIndex;
        startAnimation(intToChannelName(lastChannelIndex));
        qDebug() <<intToChannelName(lastChannelIndex);
    }
}

void Console::digitButtonClicked() {
    QLabel *tmpChannelLabel = tvWindow->findChild<QLabel*>("tmp_channel");
    if (tmpChannelLabel) {
        QPushButton *senderButton = qobject_cast<QPushButton*>(sender());
        QString digitText = senderButton->text();
        tmpChannelLabel->setText(digitText);
    } else {
        qDebug() << "Не удалось найти QLabel tmp_channel на форме TV";
    }
}

void Console::startAnimation(const QString &buttonName) {
    qDebug() << "New channel index:" << buttonName;
    int newIndex = channelIndexMap.value(buttonName);
    qDebug() << "New channel index:" << newIndex;
    if (newIndex >= 0 && newIndex <= 9) {
        lastChannelIndex = newIndex;
    } else {
        qDebug() << "Ошибка: неправильный индекс канала для кнопки" << buttonName;
        return;
    }

    QLabel *tmp_channel = tvWindow->findChild<QLabel*>("tmp_channel");
    if (tmp_channel) {
        tmp_channel->setText(QString::number(lastChannelIndex));
    } else {
        qDebug() << "Не удалось найти QLabel tmp_channel на форме TV";
    }
    QString currentFilePath = __FILE__;
    QFileInfo currentFileInfo(currentFilePath);
    QString basePath = currentFileInfo.absolutePath() + "/channel/";
    QString gifFilePath = basePath + QString::number(lastChannelIndex) + ".gif";
    qDebug() << "gifFilePath:" << gifFilePath;
    QMovie *movie = new QMovie(gifFilePath);
    QLabel *tvScreen = tvWindow->findChild<QLabel*>("tvScreen");

    if (tvScreen) {
        tvScreen->setMovie(movie);
        tvScreen->setScaledContents(true);
        movie->start();
    } else {
        qDebug() << "Не удалось найти QLabel tvScreen";
    }
}
