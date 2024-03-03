#ifndef TV_H
#define TV_H

#include <QMainWindow>

namespace Ui {
class TV;
}

class TV : public QMainWindow
{
    Q_OBJECT

public:
    explicit TV(QWidget *parent = nullptr);
    ~TV();

private:
    Ui::TV *ui;
};

#endif // TV_H
