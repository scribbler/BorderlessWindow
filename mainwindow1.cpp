#include "mainwindow1.h"
#include "ui_mainwindow1.h"
#include <QPushButton>
#include <QLabel>

MainWindow1::MainWindow1(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow1)
{
    ui->setupUi(this);

    ui->tabWidget->setMinimumHeight(100);
    ui->tabWidget->setStyleSheet("QTabBar::tab { height: 60px; width: 100px; };");

    QWidget *widget = new QWidget( );
    widget->setGeometry(0,0,200,60);
    widget->setStyleSheet("background-color: rgb(255, 0, 127);");

    QPushButton * button = new QPushButton("button");
    button->setMinimumHeight(60);
    button->setMinimumWidth(100);
    QPushButton * button1 = new QPushButton("button1");
    QPushButton * button2 = new QPushButton("button2");
    QLabel * label1 = new QLabel("button2123131313");
    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->setSpacing( 0 );
    horizontalLayout->setMargin( 0 );
    horizontalLayout->addWidget(button);
    horizontalLayout->addWidget(label1);
    horizontalLayout->addWidget(button1);
    horizontalLayout->addWidget(button2);
    widget->setLayout(horizontalLayout);
  QObject::connect( button, SIGNAL( clicked() ), this, SLOT( buttonClick() ) );
    ui->tabWidget->setCornerWidget(widget,Qt::TopRightCorner);
    ui->tabWidget->cornerWidget()->setMinimumHeight(60);
    ui->tabWidget->cornerWidget()->move(0,0);

}

MainWindow1::~MainWindow1()
{
    delete ui;
}
void MainWindow1::buttonClick()
{

}
