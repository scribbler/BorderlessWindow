#ifndef QMAINPANEL_H
#define QMAINPANEL_H

#include <QMouseEvent>
#include "qwinwidget.h"
class BorderlessWindow;
class QMainPanel : public QWinWidget {
  Q_OBJECT

public:
  QMainPanel( HWND hWnd );
  ~QMainPanel();
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
    void buttonClick1();
private:
    HWND windowHandle;
    BorderlessWindow * window1_;
protected:

};

#endif // QMAINPANEL_H
