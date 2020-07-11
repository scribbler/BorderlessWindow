#include <windows.h>
#include <dwmapi.h>
#include <windowsx.h>
#include <stdexcept>

#include <QPushButton>
#include <QFile>
#include <QApplication>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QSpacerItem>
#include <QDebug>


#include "QFramelessWindow.h"

QFramelessWindow::QFramelessWindow()
    : QWinWidget(),
      framelessWinId( 0 ),
      windowHandle(0),
      hInstance( GetModuleHandle( NULL ) ),
      borderless( true ),
      borderlessResizeable( true ),
      aeroShadow( false ),
      closed( false ),
      visible( false )
{

    HBRUSH windowBackground = CreateSolidBrush( RGB( 255, 255, 255 ) );

    int x = 100;
    int y = 100;
    int width = 1024;
    int height = 768;
    initUI(nullptr, windowBackground, x, y, width, height);
}
QFramelessWindow::~QFramelessWindow()
{
    qDebug()<<__FUNCTION__;

}
void QFramelessWindow::initUI( QApplication *app, HBRUSH windowBackground, const int x, const int y, const int width, const int height)
{
    WNDCLASSEX wcx = { 0 };
    wcx.cbSize = sizeof( WNDCLASSEX );
    wcx.style = CS_HREDRAW | CS_VREDRAW;
    wcx.hInstance = hInstance;
    wcx.lpfnWndProc = WndProc;
    wcx.cbClsExtra	= 0;
    wcx.cbWndExtra	= 0;
    wcx.lpszClassName = L"WindowClass";
    wcx.lpszMenuName	= NULL;
    wcx.hIconSm	= NULL;
    wcx.hbrBackground = windowBackground;
    wcx.hCursor = LoadCursor( hInstance, IDC_ARROW );
    RegisterClassEx( &wcx );
    if ( FAILED( RegisterClassEx( &wcx ) ) ) throw std::runtime_error( "Couldn't register window class" );

    windowHandle = CreateWindow( L"WindowClass", L"QFramelessWindow", static_cast<DWORD>( Style::windowed ), x, y, width, height, 0, 0, hInstance, nullptr );

    if ( !windowHandle ) throw std::runtime_error( "Couldn't create window because of reasons" );

    SetWindowLongPtr( windowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( this ) );

    QWinWidget::init(windowHandle);

    initQUI();

    framelessWinId = ( HWND )this->winId();
    setBorderless(borderless);
}
void QFramelessWindow::initQUI()
{
//    windowHandle = hWnd;

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


    // Show
    centralWidget->setLayout( verticalLayout );
    scrollArea->setWidget( centralWidget );
    mainGridLayout->addWidget( scrollArea );

    show();
}
LRESULT CALLBACK QFramelessWindow::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    QFramelessWindow *window = reinterpret_cast<QFramelessWindow*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
    if ( !window ) return DefWindowProc( hWnd, message, wParam, lParam );

    switch ( message )
    {
    case WM_KEYDOWN:
    {
        if( window)
        {
            switch ( wParam )
            {
            case VK_F5: {
                window->borderlessResizeable = !window->borderlessResizeable;
                break;
            }
            case VK_F6: {
                window->toggleShadow();
                window->toggleBorderless();
                SetFocus( window->framelessWinId );
                break;
            }
            case VK_F7: {
                window->toggleShadow();
                break;
            }
            }

            if ( wParam != VK_TAB ) return DefWindowProc( hWnd, message, wParam, lParam );

            SetFocus(  window->framelessWinId );
        }
        break;
    }
    // ALT + SPACE
    case WM_SYSKEYDOWN:
    {
        if ( wParam == VK_SPACE)
        {
            HMENU hMenu = GetSystemMenu( hWnd, FALSE );
            if ( hMenu )
            {
                MENUITEMINFO mii;
                mii.cbSize = sizeof( MENUITEMINFO );
                mii.fMask = MIIM_STATE;
                mii.fType = 0;

                mii.fState = MF_ENABLED;
                SetMenuItemInfo( hMenu, SC_RESTORE, FALSE, &mii );
                SetMenuItemInfo( hMenu, SC_SIZE, FALSE, &mii );
                SetMenuItemInfo( hMenu, SC_MOVE, FALSE, &mii );
                SetMenuItemInfo( hMenu, SC_MAXIMIZE, FALSE, &mii );
                SetMenuItemInfo( hMenu, SC_MINIMIZE, FALSE, &mii );

                mii.fState = MF_GRAYED;

                WINDOWPLACEMENT wp;
                GetWindowPlacement( hWnd, &wp );

                switch ( wp.showCmd ) {
                case SW_SHOWMAXIMIZED:
                    SetMenuItemInfo( hMenu, SC_SIZE, FALSE, &mii );
                    SetMenuItemInfo( hMenu, SC_MOVE, FALSE, &mii );
                    SetMenuItemInfo( hMenu, SC_MAXIMIZE, FALSE, &mii );
                    SetMenuDefaultItem( hMenu, SC_CLOSE, FALSE );
                    break;
                case SW_SHOWMINIMIZED:
                    SetMenuItemInfo( hMenu, SC_MINIMIZE, FALSE, &mii );
                    SetMenuDefaultItem( hMenu, SC_RESTORE, FALSE );
                    break;
                case SW_SHOWNORMAL:
                    SetMenuItemInfo( hMenu, SC_RESTORE, FALSE, &mii );
                    SetMenuDefaultItem( hMenu, SC_CLOSE, FALSE );
                    break;
                }

                RECT winrect;
                GetWindowRect( hWnd, &winrect );

                LPARAM cmd = TrackPopupMenu( hMenu, (TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD ),
                                             winrect.left, winrect.top, NULL, hWnd, NULL );

                if ( cmd ) PostMessage( hWnd, WM_SYSCOMMAND, cmd, 0 );
            }
            return 0;
        }
        return DefWindowProc( hWnd, message, wParam, lParam );
    }
    case WM_COMMAND:
    {
        SendMessage( hWnd, WM_SYSCOMMAND, wParam, lParam );
        return DefWindowProc( hWnd, message, wParam, lParam );
    }
    case WM_SETFOCUS:
    {
        if(window)
        {
            SetFocus( window->framelessWinId );
        }
        break;
    }
    case WM_NCCALCSIZE: {
        //this kills the window frame and title bar we added with
        //WS_THICKFRAME and WS_CAPTION
        if ( window->borderless )
        {
            return 0;
        }
        break;
    }
    case WM_DESTROY:
    {
        qDebug()<<__FUNCTION__<<"WM_DESTROY";
        HWND windowHandle = window->windowHandle;
//        if( window )
//        {
//            delete( window );
//        }
//        window = nullptr;
        //        PostQuitMessage(0);

        if(windowHandle)
        {
            DestroyWindow( windowHandle );
        }
        break;
    }
    case WM_CLOSE:
    {
        qDebug()<<__FUNCTION__<<"WM_CLOSE";
        break;
    }
    case WM_QUIT:
    {
        qDebug()<<__FUNCTION__<<"WM_QUIT";
        break;
    }
    case WM_NCHITTEST:
    {
        if ( window->borderless )
        {
            if ( window->borderlessResizeable )
            {
                const LONG borderWidth = 8; //in pixels
                RECT winrect;
                GetWindowRect( hWnd, &winrect );
                long x = GET_X_LPARAM( lParam );
                long y = GET_Y_LPARAM( lParam );

                //bottom left corner
                if ( x >= winrect.left && x < winrect.left + borderWidth &&
                     y < winrect.bottom && y >= winrect.bottom - borderWidth )
                {
                    return HTBOTTOMLEFT;
                }
                //bottom right corner
                if ( x < winrect.right && x >= winrect.right - borderWidth &&
                     y < winrect.bottom && y >= winrect.bottom - borderWidth )
                {
                    return HTBOTTOMRIGHT;
                }
                //top left corner
                if ( x >= winrect.left && x < winrect.left + borderWidth &&
                     y >= winrect.top && y < winrect.top + borderWidth )
                {
                    return HTTOPLEFT;
                }
                //top right corner
                if ( x < winrect.right && x >= winrect.right - borderWidth &&
                     y >= winrect.top && y < winrect.top + borderWidth )
                {
                    return HTTOPRIGHT;
                }
                //left border
                if ( x >= winrect.left && x < winrect.left + borderWidth )
                {
                    return HTLEFT;
                }
                //right border
                if ( x < winrect.right && x >= winrect.right - borderWidth )
                {
                    return HTRIGHT;
                }
                //bottom border
                if ( y < winrect.bottom && y >= winrect.bottom - borderWidth )
                {
                    return HTBOTTOM;
                }
                //top border
                if ( y >= winrect.top && y < winrect.top + borderWidth )
                {
                    return HTTOP;
                }
            }

            return HTCAPTION;
        }
        break;
    }

    case WM_SIZE: {
        RECT winrect;
        GetClientRect( hWnd, &winrect );

        WINDOWPLACEMENT wp;
        wp.length = sizeof( WINDOWPLACEMENT );
        GetWindowPlacement( hWnd, &wp );
        if(window)
        {
            if ( wp.showCmd == SW_MAXIMIZE ) {
                QPushButton* pushButtonMaximize = window->findChild<QPushButton*>( "pushButtonMaximize" );
                pushButtonMaximize->setStyleSheet( "#pushButtonMaximize {image: url(:/SystemMenu/Icons/Restore.png);} #pushButtonMaximize:hover { image: url(:/SystemMenu/Icons/RestoreHover.png); }" );
                window->setGeometry( 16, 16, winrect.right - 32, winrect.bottom - 32 );
            } else {
                QPushButton* pushButtonMaximize = window->findChild<QPushButton*>( "pushButtonMaximize" );
                pushButtonMaximize->setStyleSheet( "#pushButtonMaximize {image: url(:/SystemMenu/Icons/Maximize.png);} #pushButtonMaximize:hover { image: url(:/SystemMenu/Icons/MaximizeHover.png); }" );
                window->setGeometry( 8, 8, winrect.right - 16, winrect.bottom - 16 );
            }
        }
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    case WM_GETMINMAXINFO: {
        MINMAXINFO* minMaxInfo = ( MINMAXINFO* )lParam;
        if( window )
        {
            if ( window->minimumSize.required ) {
                minMaxInfo->ptMinTrackSize.x = window->getMinimumWidth();;
                minMaxInfo->ptMinTrackSize.y = window->getMinimumHeight();
            }

            if ( window->maximumSize.required ) {
                minMaxInfo->ptMaxTrackSize.x = window->getMaximumWidth();
                minMaxInfo->ptMaxTrackSize.y = window->getMaximumHeight();
            }
        }
        return 0;
    }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void QFramelessWindow::setBorderless(bool isBorderless)
{
    Style newStyle = ( isBorderless ) ?  Style::aero_borderless : Style::windowed;
    SetWindowLongPtr( windowHandle, GWL_STYLE, static_cast<LONG>( newStyle ) );
    this->borderless = isBorderless;
    if ( newStyle == Style::aero_borderless ) {
        setShadow(true);
    }
    //redraw frame
    SetWindowPos( windowHandle, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE );
}

void QFramelessWindow::setShadow(bool isShadow)
{
    if ( borderless )
    {
        this->aeroShadow = isShadow;
        const MARGINS shadow_on = { 1, 1, 1, 1 };
        const MARGINS shadow_off = { 0, 0, 0, 0 };
        DwmExtendFrameIntoClientArea( windowHandle, ( aeroShadow ) ? ( &shadow_on ) : ( &shadow_off ) );
    }
}

void QFramelessWindow::toggleBorderless() {
    if ( visible )
    {
        setBorderless(!this->borderless);
        show();
    }
}

void QFramelessWindow::toggleShadow() {
    setShadow(!this->borderless);
}

void QFramelessWindow::toggleResizeable() {
    borderlessResizeable = borderlessResizeable ? false : true;
}

bool QFramelessWindow::isResizeable() {
    return borderlessResizeable ? true : false;
}

void QFramelessWindow::show() {
    ShowWindow( windowHandle, SW_SHOW );
    visible = true;
    QWinWidget::show();
}

void QFramelessWindow::hide() {
    ShowWindow( windowHandle, SW_HIDE );
    visible = false;
    QWinWidget::hide();
}

bool QFramelessWindow::isVisible() {
    return visible ? true : false;
}

// Minimum size
void QFramelessWindow::setMinimumSize( const int width, const int height ) {
    this->minimumSize.required = true;
    this->minimumSize.width = width;
    this->minimumSize.height = height;
}

bool QFramelessWindow::isSetMinimumSize() {
    return this->minimumSize.required;
}

void QFramelessWindow::removeMinimumSize() {
    this->minimumSize.required = false;
    this->minimumSize.width = 0;
    this->minimumSize.height = 0;
}

int QFramelessWindow::getMinimumWidth() {
    return minimumSize.width;
}

int QFramelessWindow::getMinimumHeight() {
    return minimumSize.height;
}

// Maximum size
void QFramelessWindow::setMaximumSize( const int width, const int height ) {
    this->maximumSize.required = true;
    this->maximumSize.width = width;
    this->maximumSize.height = height;
}

bool QFramelessWindow::isSetMaximumSize() {
    return this->maximumSize.required;
}

void QFramelessWindow::removeMaximumSize() {
    this->maximumSize.required = false;
    this->maximumSize.width = 0;
    this->maximumSize.height = 0;
}

int QFramelessWindow::getMaximumWidth() {
    return maximumSize.width;
}

int QFramelessWindow::getMaximumHeight() {
    return maximumSize.height;
}


// Button events
void QFramelessWindow::pushButtonMinimizeClicked() {
    ShowWindow( parentWindow(), SW_MINIMIZE );
}

void QFramelessWindow::pushButtonMaximizeClicked() {
    WINDOWPLACEMENT wp;
    wp.length = sizeof( WINDOWPLACEMENT );
    GetWindowPlacement( parentWindow(), &wp );
    if ( wp.showCmd == SW_MAXIMIZE ) {
        ShowWindow( parentWindow(), SW_RESTORE );
    } else {
        ShowWindow( parentWindow(), SW_MAXIMIZE );
    }
}

void QFramelessWindow::pushButtonCloseClicked() {
//    PostQuitMessage( 0 );
    PostMessage(windowHandle,WM_CLOSE,0,0);
}
void QFramelessWindow::buttonClick()
{
    //    MainWindow1 * mw = new MainWindow1;
    //    mw->show();
    QFramelessWindow * window2_ = new QFramelessWindow();
    window2_->show();
}

#if QT_VERSION >= 0x050000
bool QFramelessWindow::nativeEvent( const QByteArray &, void *msg, long * ) {
#else
bool QFramelessWindow::winEvent( MSG *message, long * ) {
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


void QFramelessWindow::mousePressEvent( QMouseEvent *event ) {
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
