#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class CCircularProgressBar;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void updateBarValue();

private:
    Ui::MainWindow *ui;

    CCircularProgressBar *circlBar;



};

#endif // MAINWINDOW_H
