#ifndef CIRCULARPROGRESSBAR_H
#define CIRCULARPROGRESSBAR_H

#include <QProgressBar>
#include <QWidget>
#include <QtCore>

class CCircularProgressBar : public QProgressBar
{
    Q_OBJECT
public:
    explicit CCircularProgressBar(QWidget* parent = 0);

protected:
    void paintEvent(QPaintEvent *);

};

#endif // CIRCULARPROGRESSBAR_H
