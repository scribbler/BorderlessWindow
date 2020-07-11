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
#include "mainwindow1.h"
#include "BorderlessWindow.h"
QMainPanel::QMainPanel( HWND hWnd ) : QWinWidget( hWnd ),window1_(nullptr) {

    windowHandle = hWnd;

    setObjectName( "mainPanel" );

    // Horizontal layout
    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->setSpacing( 0 );
    horizontalLayout->setMargin( 0 );

    // Window title
    QLabel *windowTitle = new QLabel( this );
    windowTitle->setObjectName( "windowTitle" );
    windowTitle->setText( "Borderless window" );
    windowTitle->setStyleSheet( "font-size: 16px; color: #444444;" );
    windowTitle->setAttribute( Qt::WA_TransparentForMouseEvents );
    horizontalLayout->addWidget( windowTitle );
    horizontalLayout->addStretch();

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

    // Main panel layout
    QGridLayout *mainGridLayout = new QGridLayout();
    mainGridLayout->setSpacing( 0 );
    mainGridLayout->setMargin( 0 );
    setLayout( mainGridLayout );

    // Central widget
    QWidget *centralWidget = new QWidget( this );
    centralWidget->setObjectName( "centralWidget" );
    centralWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );


    // Main panel scroll area
    QScrollArea *scrollArea = new QScrollArea( this );
    scrollArea->setWidgetResizable( true );
    scrollArea->setObjectName( "mainPanelScrollArea" );
    scrollArea->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );

    // Vertical layout example
    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing( 0 );
    verticalLayout->setMargin( 0 );
    verticalLayout->setAlignment( Qt::AlignHCenter );
    verticalLayout->addLayout( horizontalLayout );

    // Label example
    QLabel *label = new QLabel( centralWidget );
    label->setObjectName( "Label" );
    label->setText( "Hello World! This is QLabel." );
    label->setStyleSheet( "font-size: 48px" );
    verticalLayout->addWidget( label );

    QPushButton *button = new QPushButton( "button", this );
    button->setObjectName( "button" );
    verticalLayout->addWidget( button );
    QObject::connect( button, SIGNAL( clicked() ), this, SLOT( buttonClick() ) );

    QPushButton *button1 = new QPushButton( "button1", this );
    button1->setObjectName( "button1" );
    verticalLayout->addWidget( button1 );
    QObject::connect( button1, SIGNAL( clicked() ), this, SLOT( buttonClick1() ) );


    // Show
    centralWidget->setLayout( verticalLayout );
    scrollArea->setWidget( centralWidget );
    mainGridLayout->addWidget( scrollArea );

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
        window1_ = new BorderlessWindow();
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
