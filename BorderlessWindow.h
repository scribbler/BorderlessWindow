﻿#include <windows.h>
#include <QApplication>
#include "QWinWidget.h"

class BorderlessWindow
{
    enum class Style : DWORD
    {
        windowed = ( WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN | WS_SYSMENU  ),
        aero_borderless = ( WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN )
    };
public:
    ///
    /// \brief winWidgetDestroy
    /// QWinWidget destroy
    ///
    virtual void winWidgetDestroy(){}
    ///
    /// \brief resizeWindow
    /// \param showCmd SW_MAXIMIZE,SW_MINIMIZE
    ///
    virtual void resizeWindow(int showCmd, RECT winrect){}
public:
    HWND hWnd;
    HINSTANCE hInstance;

    BorderlessWindow( QApplication *app, HBRUSH windowBackground, const int x, const int y, const int width, const int height );
    BorderlessWindow();
    ~BorderlessWindow();

    void show();
    void hide();
    bool isVisible();

    void toggleBorderless();
    void toggleShadow();
    void toggleResizeable();
    bool isResizeable();

    void setMinimumSize( const int width, const int height );
    bool isSetMinimumSize();
    void removeMinimumSize();
    int getMinimumHeight();
    int getMinimumWidth();

    void setMaximumSize( const int width, const int height );
    bool isSetMaximumSize();
    int getMaximumHeight();
    int getMaximumWidth();
    void removeMaximumSize();

    HWND winId() const;
    void setWinId(const HWND &value);
    void setBorderless(bool isBorderless);
    void setShadow(bool isShadow);
private:
    void initUI( QApplication *app, HBRUSH windowBackground, const int x, const int y, const int width, const int height );

    static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

private:
    static QApplication *a;
    HWND winId_;
    bool closed;
    bool visible;

    bool borderless;
    bool aeroShadow;
    bool borderlessResizeable;

    struct sizeType {
        sizeType() : required( false ), width( 0 ), height( 0 ) {}
        bool required;
        int width;
        int height;
    };

    sizeType minimumSize;
    sizeType maximumSize;
};
