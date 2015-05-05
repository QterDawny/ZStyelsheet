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
    enum ItemProperty {
        COL_ID = 0,
        COL_PHYSICALINFO,
        COL_STATE,
        COL_TIME,
        COL_PROGRESS,
        COL_END
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void updateBarValue();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;

    CCircularProgressBar *circlBar;

    void initBtn();
    void initCirclBar();
    void initListWidget();
    void initTreeView();


};

#endif // MAINWINDOW_H
