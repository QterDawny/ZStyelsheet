#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "circularprogressbar.h"

#include <QMenu>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    circlBar(NULL)
{
    ui->setupUi(this);

    QMenu *menu = new QMenu(this);
    menu->addAction("Menu 1");
    menu->addAction("Menu 2");
    ui->pushButton->setMenu(menu);

    circlBar = new CCircularProgressBar(this);
    circlBar->resize(100, 100);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateBarValue()));
    timer->setInterval(20);
    timer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateBarValue()
{
    static int value = 0;
    if (value >= 360) {
        value = 0;
        circlBar->setValue(0);
    }
    value++;
    circlBar->setValue(value);
}
