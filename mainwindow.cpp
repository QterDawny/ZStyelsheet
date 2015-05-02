#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "circularprogressbar.h"

#include <QDebug>
#include <QMenu>
#include <QWidget>
#include <QListWidgetItem>

#include <QStyledItemDelegate>
// 去掉Focus时的虚边框： NOTE: item:hover的效果没了？？
class NoFocusDelegate : public QStyledItemDelegate
{
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        QStyleOptionViewItem itemOption(option);
        if (itemOption.state & QStyle::State_HasFocus) {
            itemOption.state = itemOption.state ^ QStyle::State_HasFocus;
        }
        QStyledItemDelegate::paint(painter, itemOption, index);
    }
};

class MyListWidget : public QListWidget
{
    void rowsInserted(const QModelIndex &parent, int start, int end) {
        QListView::rowsInserted(parent, start, end);
        int rc = model()->rowCount();
        qDebug() << "rc: " << rc;
        // use this all the rows have the same height. otherwise
        // you will need to iterate and sum all row heights
        resize(width(), rc ? rc * sizeHintForRow(0): height());
    }
public:
    explicit MyListWidget(QWidget *parent = 0);
};

MyListWidget::MyListWidget(QWidget *parent) :
    QListWidget(parent) {
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    circlBar(NULL)
{
    ui->setupUi(this);

    // btn
    QMenu *menu = new QMenu(this);
    menu->addAction("Menu 1");
    menu->addAction("Menu 2");
    ui->pushButton->setMenu(menu);

    // circl bar
    circlBar = new CCircularProgressBar(this);
    circlBar->resize(100, 100);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateBarValue()));
    timer->setInterval(20);
    timer->start();

    // list widget
    // TODO: 尾部总是多出一个空白行，setFixSize也不管用，怎么处理？？
//    ui->listWidget->setItemDelegate(new NoFocusDelegate);
    MyListWidget *myListWidget = new MyListWidget(this);
    myListWidget->setFixedHeight(250);
    myListWidget->setStyleSheet(ui->listWidget->styleSheet());
    for (int i = 0; i < 5; ++i) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(QSize(0, 50));  // neccessary
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, new QLabel("Item: " + QString::number(i)));

//        myListWidget->addItem(item);
//        myListWidget->setItemWidget(item, new QLabel("Item: " + QString::number(i)));
    }
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
