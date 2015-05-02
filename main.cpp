#include "mainwindow.h"
#include <QApplication>


#include <QProxyStyle>
// 去掉获取焦点部件的矩形点阵虚框
class MyProxyStyle : public QProxyStyle
{
public:
    virtual void drawPrimitive(PrimitiveElement element, const QStyleOption * option,
                               QPainter * painter, const QWidget * widget = 0) const
    {
        if (PE_FrameFocusRect == element) {
            // do not draw focus rectangle
        } else {
            QProxyStyle::drawPrimitive(element, option,painter, widget);
        }
    }
};


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    a.setStyle(new MyProxyStyle);

    return a.exec();
}
