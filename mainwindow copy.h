#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
//#include "tree.h"


const int Width = 740;
const int Height = 730;


QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void paintEvent(QPaintEvent *event);

private slots:

    void on_pushButton_Add_clicked();

    void on_pushButton_Delete_clicked();

    void on_lineEdit_Add_returnPressed();

    void on_lineEdit_Delete_returnPressed();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
