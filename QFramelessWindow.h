#ifndef QFRAMELESSWINDOW_H
#define QFRAMELESSWINDOW_H

#include <windows.h>
#include <QApplication>
#include <QMouseEvent>
#include "QWinWidget.h"

class QFramelessWindow : public QWinWidget
{
    enum class Style : DWORD
    {
        windowed = ( WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN | WS_SYSMENU  ),
        aero_borderless = ( WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN )
    };

    Q_OBJECT
public:
    HINSTANCE hInstance;
    QFramelessWindow();
    ~QFramelessWindow();

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
private:
    void initUI( QApplication *app, HBRUSH windowBackground, const int x, const int y, const int width, const int height);
    void initQUI();
    void setBorderless(bool isBorderless);
    void setShadow(bool isShadow);
    static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

protected:
#if QT_VERSION >= 0x050000
    bool nativeEvent(const QByteArray &, void *msg, long *result);
#else
    bool winEvent(MSG *message, long *result);
#endif
    void mousePressEvent( QMouseEvent *event );

public slots:
    void pushButtonMinimizeClicked();
    void pushButtonMaximizeClicked();
    void pushButtonCloseClicked();
    void buttonClick();
private:
    HWND windowHandle;
    HWND framelessWinId;
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

#endif // QFRAMELESSWINDOW_H
