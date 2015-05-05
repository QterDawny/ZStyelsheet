#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "circularprogressbar.h"

#include <QDebug>
#include <QMenu>
#include <QWidget>
#include <QListWidgetItem>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QPainter>

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

// 自定义ListWidget
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

// Prograss Delegate
class ProgressDelegate : QStyledItemDelegate
{
public:
    ProgressDelegate::ProgressDelegate(QObject *parent) :
        QStyledItemDelegate(parent) {
    }

    void ProgressDelegate::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const {
        QStyledItemDelegate::paint(painter, option, index);

        if (index.column() == MainWindow::COL_PROGRESS) {
            QProgressBar renderer;
            const QAbstractItemModel *itemModel = index.model();
            int progressPercentage = itemModel->data(index, Qt::UserRole + 1).toInt();

            // Customize style using style-sheet...
            QString color = "#83CE2A";
            QString bgColor = "#E6E6E6";
            bool bFailed = itemModel->data(index, Qt::UserRole + 2).toBool();
            if (bFailed) {
                color = "#CDCDCD";
                bgColor = "#FF0000";
            } else if (100 == progressPercentage) {
                color = "#83CE2A";
                bgColor = "#E6E6E6";
            }
            QString style = QString("QProgressBar {border: 0px solid grey; border-radius: 5px; background: %1; }").arg(bgColor);
            style += QString("QProgressBar::chunk {background-color: %1;}").arg(color);

            int width = option.rect.width() - 20;
            int height = 16;
            QSize size = QSize(width, height);
            QPoint topLeft = QPoint(option.rect.x() + (option.rect.width() - width) / 2,
                                    option.rect.y() + (option.rect.height() - height) / 2);

            renderer.resize(size);
            renderer.setMinimum(0);
            renderer.setMaximum(100);
            renderer.setValue(progressPercentage);
            renderer.setAlignment(Qt::AlignCenter);

            renderer.setStyleSheet(style);
            painter->save();
            painter->translate(topLeft);
            renderer.render(painter);
            painter->restore();
        }
        else
            QStyledItemDelegate::paint(painter, option, index);
    }
};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    circlBar(NULL)
{
    ui->setupUi(this);

    initBtn();
    initCirclBar();
    initListWidget();
    initTreeView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initBtn()
{
    QMenu *menu = new QMenu(this);
    menu->addAction("Menu 1");
    menu->addAction("Menu 2");
    ui->pushButton->setMenu(menu);
}

void MainWindow::initCirclBar()
{
    circlBar = new CCircularProgressBar(this);
    circlBar->resize(100, 100);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateBarValue()));
    timer->setInterval(20);
    timer->start();
}

void MainWindow::initListWidget()
{
    // TODO: 尾部总是多出一个空白行，setFixSize也不管用，怎么处理？？
    //    ui->listWidget->setItemDelegate(new NoFocusDelegate);
    //    MyListWidget *myListWidget = new MyListWidget(this);
    //    myListWidget->setFixedHeight(250);
    //    myListWidget->setStyleSheet(ui->listWidget->styleSheet());
    for (int i = 0; i < 5; ++i) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(QSize(0, 50));  // neccessary
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, new QLabel("Item: " + QString::number(i)));

        //        myListWidget->addItem(item);
        //        myListWidget->setItemWidget(item, new QLabel("Item: " + QString::number(i)));
    }
}

void MainWindow::initTreeView()
{
    // init model
    QStandardItemModel *model = new QStandardItemModel(5, COL_END, this);
    model->setHorizontalHeaderLabels(QStringList() << tr("ID") << tr("COM-PORT-HUB") << tr("State") << tr("Time") << tr("Progress"));
    for (int i = 0; i < model->rowCount(); ++i) {
        model->setData(model->index(i, COL_ID), i + 1, Qt::DisplayRole);
        model->setData(model->index(i, COL_ID), QSize(100, 40), Qt::SizeHintRole);  // set size
        model->item(i, COL_ID)->setTextAlignment(Qt::AlignCenter);
        model->item(i, COL_ID)->setFont(QFont("Times New Roman", 18, 600, true));
        model->item(i, COL_ID)->setForeground(QBrush(QColor(102, 102, 102)));

        model->setData(model->index(i, COL_PHYSICALINFO), QString("--"), Qt::DisplayRole);
        model->setData(model->index(i, COL_PHYSICALINFO), QSize(100, 40), Qt::SizeHintRole);
        model->item(i, COL_PHYSICALINFO)->setTextAlignment(Qt::AlignCenter);
        model->item(i, COL_PHYSICALINFO)->setFont(QFont("Times New Roman", 12));
        model->item(i, COL_PHYSICALINFO)->setForeground(QBrush(QColor(102, 102, 102)));

        model->setData(model->index(i, COL_STATE), QString(tr("Disconnect")), Qt::DisplayRole);
        model->setData(model->index(i, COL_STATE), QSize(100, 40), Qt::SizeHintRole);
        model->item(i, COL_STATE)->setTextAlignment(Qt::AlignCenter);
        model->item(i, COL_STATE)->setFont(QFont("Times New Roman", 12, 600));
        model->item(i, COL_STATE)->setForeground(QBrush(QColor(168, 168, 168)));

        model->setData(model->index(i, COL_PROGRESS), 0, Qt::UserRole + 1);
        model->setData(model->index(i, COL_PROGRESS), QSize(100, 40), Qt::SizeHintRole);
        model->setData(model->index(i, COL_PROGRESS), false, Qt::UserRole + 2);

        model->setData(model->index(i, COL_TIME), QString(""), Qt::DisplayRole);
        model->setData(model->index(i, COL_TIME), QSize(100, 40), Qt::SizeHintRole);
        model->item(i, COL_TIME)->setTextAlignment(Qt::AlignCenter);
        model->item(i, COL_TIME)->setFont(QFont("Times New Roman", 12, 600));
        model->item(i, COL_TIME)->setForeground(QBrush(QColor(51, 51, 51)));
    }
    ui->treeView->setModel(model);

    // init treeview
    ProgressDelegate *progressDel = new ProgressDelegate(this);
    ui->treeView->setItemDelegateForColumn(COL_PROGRESS, (QAbstractItemDelegate *)progressDel);

    // header
    ui->treeView->header()->setDefaultAlignment(Qt::AlignCenter);
    ui->treeView->header()->setMovable(false);
    ui->treeView->header()->setResizeMode(QHeaderView::Fixed);

    ui->treeView->setColumnWidth(COL_ID, 50);

    // enable menu
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

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


void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
