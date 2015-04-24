#include "circularprogressbar.h"
#include <QPainter>

CCircularProgressBar::CCircularProgressBar(QWidget *parent) :
    QProgressBar(parent)
{
}

void CCircularProgressBar::paintEvent(QPaintEvent */*event*/)
{
    int progress = this->value();
    int progressInDegrees = (double)(progress * 360) / 100;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::cyan, 3, Qt::SolidLine, Qt::RoundCap));
    painter.drawArc(10, 10, this->width() - 20, this->height()-20, 3016, progressInDegrees * 16);
}
