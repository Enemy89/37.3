#include "tv.h"
#include "ui_tv.h"
#include <QScreen>


TV::TV(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TV)
{
    ui->setupUi(this);
    this->setFixedSize(680, 420);
    move(screen()->geometry().center() - frameGeometry().center());

    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    ui->tmp_channel->show();
    ui->tmp_sound->show();
}

TV::~TV()
{
    delete ui;
}
