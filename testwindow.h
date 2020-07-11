#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include "BorderlessWindow.h"
#include "QMainPanel.h"

class TestWindow : public BorderlessWindow
{
public:
    TestWindow();
    ~TestWindow();

private:
    QMainPanel *mainPanel;

    // BorderlessWindow interface
public:
    void winWidgetDestroy() override;
    void resizeWindow(int showCmd, RECT winrect) override;
};

#endif // TESTWINDOW_H
