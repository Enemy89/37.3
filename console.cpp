#include "console.h"
#include "ui_console.h"
#include <QScreen>
#include <QPushButton>
#include <QMovie>
#include<QString>
#include <QLabel>
#include <QStringList>

int currentChannelIndex=-1;

Console::Console(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Console)
{
    ui->setupUi(this);
    tvWindow = new TV(this);
    tvWindow->show(); // Отобразить форму TV
    this->setFixedSize(240, 680);
    move(1680, 400);

    // Найти кнопки с именами "one", "two", ..., "nine" на форме "Console"
    QStringList buttonNames = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "null_num"};
    foreach (const QString &buttonName, buttonNames) {
        QPushButton *button = findChild<QPushButton*>(buttonName);
        if (button) {
            // Если кнопка найдена, связать сигнал clicked() с методом digitButtonClicked() в классе Console
            connect(button, &QPushButton::clicked, this, &Console::digitButtonClicked);
        } else {
            qDebug() << "Не удалось найти кнопку с именем" << buttonName << "на форме Console";
        }
    }

    // Также связываем сигналы clicked() кнопок с методом startAnimation() в классе Console
    QStringList animationButtonNames = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "null_num"};
    foreach (const QString &buttonName, animationButtonNames) {
        QPushButton *button = findChild<QPushButton*>(buttonName);
        if (button) {
            // Если кнопка найдена, связать сигнал clicked() с методом startAnimation() с передачей имени кнопки в качестве аргумента
            connect(button, &QPushButton::clicked, [this, buttonName]() {
                startAnimation(buttonName);
            });
        } else {
            qDebug() << "Не удалось найти кнопку с именем" << buttonName << "на форме Console";
        }
    }
    // Связываем сигналы clicked() кнопок plus и minus с соответствующими слотами
    connect(ui->plus, &QPushButton::clicked, this, &Console::plusButtonClicked);
    connect(ui->minus, &QPushButton::clicked, this, &Console::minusButtonClicked);
    currentSoundLevel = 0;
   // Устанавливаем начальное значение текста в QLabel tmp_sound
            QLabel *tmpSoundLabel = tvWindow->findChild<QLabel*>("tmp_sound");
    if (tmpSoundLabel) {
        tmpSoundLabel->setText(QString("Sound: %1%").arg(currentSoundLevel));
    } else {
        qDebug() << "Не удалось найти QLabel tmp_sound на форме TV";
    }

    // В конструкторе класса Console
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

/// Реализация слота plusButtonClicked()
void Console::plusButtonClicked() {
    // Увеличиваем уровень звука на 10%, если не превышает 100%
    if (currentSoundLevel < 100) {
        currentSoundLevel += 10;
    }

    // Обновляем текст в QLabel tmp_sound с новым значением уровня звука
    QLabel *tmpSoundLabel = tvWindow->findChild<QLabel*>("tmp_sound");
    if (tmpSoundLabel) {
        tmpSoundLabel->setText(QString("Sound: %1%").arg(currentSoundLevel));
    } else {
        qDebug() << "Не удалось найти QLabel tmp_sound на форме TV";
    }
}

// Реализация слота minusButtonClicked()
void Console::minusButtonClicked() {
    // Уменьшаем уровень звука на 10%, если не меньше 0%
    if (currentSoundLevel > 0) {
        currentSoundLevel -= 10;
    }

    // Обновляем текст в QLabel tmp_sound с новым значением уровня звука
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
        return QString(); // Пустая строка в случае неверного индекса
    }
}

void Console::nextButtonClicked() {
    // Увеличиваем индекс канала на 1, если не превышает 9
    if (lastChannelIndex < 9) {
        lastChannelIndex++;
        qDebug() <<"До передачи "<< lastChannelIndex;
        startAnimation(intToChannelName(lastChannelIndex));
        qDebug() <<"После передачи "<< intToChannelName(lastChannelIndex);
    }
}

// В методе lastButtonClicked
void Console::lastButtonClicked() {
    // Уменьшаем индекс канала на 1, если не меньше 1
    if (lastChannelIndex > 0) {
        lastChannelIndex--;
        qDebug() << lastChannelIndex;
        startAnimation(intToChannelName(lastChannelIndex));
        qDebug() <<intToChannelName(lastChannelIndex);
    }
}

void Console::digitButtonClicked() {
    // Получаем указатель на QLabel tmp_channel из формы TV
    QLabel *tmpChannelLabel = tvWindow->findChild<QLabel*>("tmp_channel");
    if (tmpChannelLabel) {
        // Получаем текст кнопки, которая вызвала этот слот
        QPushButton *senderButton = qobject_cast<QPushButton*>(sender());
        QString digitText = senderButton->text();

        // Устанавливаем текст QLabel tmp_channel равным тексту нажатой кнопки
        tmpChannelLabel->setText(digitText);
    } else {
        qDebug() << "Не удалось найти QLabel tmp_channel на форме TV";
    }
}

// В методе startAnimation
void Console::startAnimation(const QString &buttonName) {
    qDebug() << "New channel index:" << buttonName;

    // Получаем индекс канала на основе имени кнопки из словаря
    int newIndex = channelIndexMap.value(buttonName);

    qDebug() << "New channel index:" << newIndex;

    // Обновляем lastChannelIndex, если индекс канала корректен
    if (newIndex >= 0 && newIndex <= 9) {
        lastChannelIndex = newIndex;
    } else {
        qDebug() << "Ошибка: неправильный индекс канала для кнопки" << buttonName;
        return;
    }

    // Обновляем лейбл tmp_channel на форме TV
    QLabel *tmp_channel = tvWindow->findChild<QLabel*>("tmp_channel");
    if (tmp_channel) {
        tmp_channel->setText(QString::number(lastChannelIndex));
    } else {
        qDebug() << "Не удалось найти QLabel tmp_channel на форме TV";
    }

    // Строим путь к файлу GIF на основе индекса канала
    QString gifFilePath = QString("C:\\Users\\Rhan\\Documents\\untitled4\\channel\\%1.gif").arg(lastChannelIndex);

    qDebug() << "gifFilePath:" << gifFilePath;

    // Создание объекта QMovie и загрузка гиф-анимации
    QMovie *movie = new QMovie(gifFilePath);

    // Получаем указатель на QLabel tvScreen из формы TV
    QLabel *tvScreen = tvWindow->findChild<QLabel*>("tvScreen");

    if (tvScreen) {
        // Установка гиф-анимации для QLabel tvScreen
        tvScreen->setMovie(movie);

        // Растягиваем содержимое на весь размер QLabel
        tvScreen->setScaledContents(true);

        // Начать воспроизведение анимации
        movie->start();
    } else {
        qDebug() << "Не удалось найти QLabel tvScreen";
    }
}
