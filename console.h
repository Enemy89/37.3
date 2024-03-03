#ifndef CONSOLE_H
#define CONSOLE_H

#include <QMainWindow>
#include "tv.h"
#include <QPushButton>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class Console;
}
QT_END_NAMESPACE

class Console : public QMainWindow
{
    Q_OBJECT

public:
    explicit Console(QWidget *parent = nullptr);
    ~Console();

private slots:
    void on_playButton_clicked();
    void startAnimation(const QString &buttonName);
    void digitButtonClicked();
    void plusButtonClicked();
    void minusButtonClicked();
    void lastButtonClicked();
    void nextButtonClicked();

private:
    Ui::Console *ui;
    TV *tvWindow;
    QPushButton *pushButton; // Добавленная переменная pushButton
    int currentSoundLevel;
    int lastChannelIndex=-1;
};

#endif // CONSOLE_H
