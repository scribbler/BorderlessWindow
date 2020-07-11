#include <windows.h>

#include <QApplication>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QSpacerItem>

#include "QMainPanel.h"
#include <QDebug>
#include <windowsx.h>
#include "TestWindow.h"
QMainPanel::QMainPanel( HWND hWnd ) : QWinWidget( hWnd ),window1_(nullptr) {

    windowHandle = hWnd;

    setObjectName( "mainPanel" );
    this->setStyleSheet("background-color: rgb(175, 175, 175);");


    // actionBar
    QWidget * actioWidget = new QWidget( );
    actioWidget->setGeometry(0,0,200,60);
    actioWidget->setStyleSheet("background-color: rgb(255, 0, 127);");


    // Horizontal layout
    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->setSpacing( 0 );
    horizontalLayout->setMargin( 0 );
    actioWidget->setLayout(horizontalLayout);

    // System buttons
    // Minimize
    QPushButton *pushButtonMinimize = new QPushButton( "", this );
    pushButtonMinimize->setObjectName( "pushButtonMinimize" );
    horizontalLayout->addWidget( pushButtonMinimize );
    QObject::connect( pushButtonMinimize, SIGNAL( clicked() ), this, SLOT( pushButtonMinimizeClicked() ) );

    // Maximize
    QPushButton *pushButtonMaximize = new QPushButton( "", this );
    pushButtonMaximize->setObjectName( "pushButtonMaximize" );
    horizontalLayout->addWidget( pushButtonMaximize );
    QObject::connect( pushButtonMaximize, SIGNAL( clicked() ), this, SLOT( pushButtonMaximizeClicked() ) );

    // Close
    QPushButton *pushButtonClose = new QPushButton( "", this );
    pushButtonClose->setObjectName( "pushButtonClose" );
    horizontalLayout->addWidget( pushButtonClose );
    QObject::connect( pushButtonClose, SIGNAL( clicked() ), this, SLOT( pushButtonCloseClicked() ) );


    pushButtonMinimize->setMinimumHeight(30);
    pushButtonMinimize->setMinimumWidth(30);
    pushButtonMaximize->setMinimumHeight(30);
    pushButtonMaximize->setMinimumWidth(30);
    pushButtonClose->setMinimumHeight(30);
    pushButtonClose->setMinimumWidth(30);
    // tabWidet
    tabWidget_ = new QTabWidget();
    tabWidget_->setMinimumHeight(100);
    tabWidget_->setStyleSheet("QTabBar::tab { height: 30px; width: 100px; };");
    tabWidget_->setCornerWidget(actioWidget, Qt::TopRightCorner);
    tabWidget_->cornerWidget()->setMinimumHeight(30);

    // Vertical layout example
    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing( 0 );
    verticalLayout->setMargin( 0 );
    verticalLayout->setAlignment( Qt::AlignHCenter );
    verticalLayout->addLayout( horizontalLayout );
    verticalLayout->addWidget(tabWidget_);
    this->setLayout(verticalLayout);

    for( int i=0;i<20;i++)
    {
        QWidget * w = new QWidget;
        if( i==0){
            QPushButton *button = new QPushButton( "button", w );
            button->setObjectName( "button" );
            button->setGeometry(QRect(50, 50, 80, 20));
            QObject::connect( button, SIGNAL( clicked() ), this, SLOT( buttonClick() ) );

            QPushButton *button1 = new QPushButton( "button1", w );
            button1->setObjectName( "button1" );
            button1->setGeometry(QRect(50, 100, 80, 20));
            QObject::connect( button1, SIGNAL( clicked() ), this, SLOT( buttonClick1() ) );
        }
        tabWidget_->addTab(w,QString("tab index:%1").arg(i+1));
    }

    // Show
    show();
}
QMainPanel::~QMainPanel()
{
    qDebug()<<__FUNCTION__;

    if( window1_)
        delete window1_;
    window1_ = nullptr;
}

// Button events
void QMainPanel::pushButtonMinimizeClicked() {
    ShowWindow( parentWindow(), SW_MINIMIZE );
}

void QMainPanel::pushButtonMaximizeClicked() {
    WINDOWPLACEMENT wp;
    wp.length = sizeof( WINDOWPLACEMENT );
    GetWindowPlacement( parentWindow(), &wp );
    if ( wp.showCmd == SW_MAXIMIZE ) {
        ShowWindow( parentWindow(), SW_RESTORE );
    } else {
        ShowWindow( parentWindow(), SW_MAXIMIZE );
    }
}

void QMainPanel::pushButtonCloseClicked() {
    //    PostQuitMessage( 0 );
    PostMessage(windowHandle,WM_CLOSE,0,0);
}

#if QT_VERSION >= 0x050000
bool QMainPanel::nativeEvent( const QByteArray &, void *msg, long * ) {
#else
bool QMainPanel::winEvent( MSG *message, long * ) {
#endif
#if QT_VERSION >= 0x050000
    MSG *message = ( MSG * )msg;
#endif

    switch( message->message ) {
    case WM_SYSKEYDOWN: {
        if ( message->wParam == VK_SPACE ) {
            SendMessage( windowHandle, WM_SYSKEYDOWN, VK_SPACE, message->lParam );
            return true;
        }
        return DefWindowProc( message->hwnd, message->message, message->wParam, message->lParam );
    }
    case WM_KEYDOWN: {
        qDebug()<<__FUNCTION__<< "WM_KEYDOWN";
        if ( message->wParam == VK_F5 ||
             message->wParam == VK_F6 ||
             message->wParam == VK_F7
             ) {
            SendMessage( windowHandle, WM_KEYDOWN, message->wParam, message->lParam );
            break;
        }
    }
    }

    return false;
}

void QMainPanel::buttonClick()
{
    //    MainWindow1 * mw = new MainWindow1;
    //    mw->show();
    qDebug()<<__FUNCTION__<<window1_;
    if( window1_==nullptr)
        window1_ = new TestWindow();
    window1_->show();
}
void QMainPanel::buttonClick1()
{

}

void QMainPanel::mousePressEvent( QMouseEvent *event ) {
    if ( event->button() == Qt::LeftButton ) {
        ReleaseCapture();
        SendMessage( windowHandle, WM_NCLBUTTONDOWN, HTCAPTION, 0 );
    }

    if ( event->type() == QEvent::MouseButtonDblClick ) {
        if (event -> button() == Qt::LeftButton) {
            WINDOWPLACEMENT wp;
            wp.length = sizeof( WINDOWPLACEMENT );
            GetWindowPlacement( parentWindow(), &wp );
            if ( wp.showCmd == SW_MAXIMIZE ) {
                ShowWindow( parentWindow(), SW_RESTORE );
            } else {
                ShowWindow( parentWindow(), SW_MAXIMIZE );
            }
        }
    }
    QWidget::mousePressEvent(event);
}
