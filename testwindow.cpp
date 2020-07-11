#include "testwindow.h"
#include <QDebug>
#include <QPushButton>

TestWindow::TestWindow():BorderlessWindow(),mainPanel(nullptr)
{
    mainPanel = new QMainPanel( hWnd );
    setWinId((HWND)mainPanel->winId());
    mainPanel->show();
}

TestWindow::~TestWindow()
{
    if( mainPanel)
    {
        delete mainPanel;
    }
    mainPanel = nullptr;
}


void TestWindow::winWidgetDestroy()
{
    if( mainPanel)
    {
        delete mainPanel;
    }
    mainPanel = nullptr;
}

void TestWindow::resizeWindow(int showCmd,RECT winrect)
{
    if(mainPanel)
    {
        if ( showCmd == SW_MAXIMIZE ) {
            QPushButton* pushButtonMaximize = mainPanel->findChild<QPushButton*>( "pushButtonMaximize" );
            pushButtonMaximize->setStyleSheet( "#pushButtonMaximize {image: url(:/SystemMenu/Icons/Restore.png);} #pushButtonMaximize:hover { image: url(:/SystemMenu/Icons/RestoreHover.png); }" );
            mainPanel->setGeometry( 16, 16, winrect.right - 32, winrect.bottom - 32 );
        } else {
            QPushButton* pushButtonMaximize = mainPanel->findChild<QPushButton*>( "pushButtonMaximize" );
            pushButtonMaximize->setStyleSheet( "#pushButtonMaximize {image: url(:/SystemMenu/Icons/Maximize.png);} #pushButtonMaximize:hover { image: url(:/SystemMenu/Icons/MaximizeHover.png); }" );
            mainPanel->setGeometry( 8, 8, winrect.right - 16, winrect.bottom - 16 );
        }
    }
}
