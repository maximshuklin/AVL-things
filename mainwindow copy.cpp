#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tree.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    this->setWindowState(Qt::WindowMaximized);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap));

    for (auto circle : Circles) {
        QRect rect = QRect(circle.x - circle.R, circle.y - circle.R, 2 * circle.R, 2 * circle.R);
        painter.drawEllipse(rect);
        std::string name = convertToCharX(circle.number);
        const char *chr_name = name.c_str();
        painter.drawText(rect, Qt::AlignCenter, tr(chr_name));
    }
    for (auto seg : segments) {
        painter.drawLine(seg.first.x, seg.first.y, seg.second.x, seg.second.y);
    }

    QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        timer->start(100);
}


void MainWindow::on_pushButton_Add_clicked()
{
    QString text = ui->lineEdit_Add->text();
    if (text.size() == 0) {
        // empty query
        return ;
    }
    ui->lineEdit_Add->clear();

    addNode(convertToInt(text));
}

void MainWindow::on_pushButton_Delete_clicked()
{
    QString text = ui->lineEdit_Delete->text();
    if (text.size() == 0) {
        // empty query
        return ;
    }
    ui->lineEdit_Delete->clear();
    deleteNode(convertToInt(text));
}


void MainWindow::on_lineEdit_Add_returnPressed()
{
    QString text = ui->lineEdit_Add->text();
    if (text.size() == 0) {
        // empty query
        return ;
    }
    ui->lineEdit_Add->clear();
    addNode(convertToInt(text));
}

void MainWindow::on_lineEdit_Delete_returnPressed()
{
    QString text = ui->lineEdit_Delete->text();
    if (text.size() == 0) {
        // empty query
        return ;
    }
    ui->lineEdit_Delete->clear();
    deleteNode(convertToInt(text));
}

