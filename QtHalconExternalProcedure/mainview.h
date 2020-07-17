// HDevEngine/C++ example for executing an external procedure
//
// (C) 2005-2015 MVTec Software GmbH
//
// Purpose:
// This example program shows how the classes HDevEngine and
// HDevProcedureCall can be used in order to implement a fin detection
// application. Most of the application's functionality is contained in the
// HDevelop external procedure detect_fin(), which can be found in the
// procedures directory. The procedure takes an image as input object parameter
// and returns a region and its area as output parameters. In the action
// routine DetectFin() the following basic steps are executed:
// 1. The external HDevelop procedure 'detect_fin' is loaded and a call to the
//    procedure is created.
// 2. An image is grabbed and passed as input parameter to the procedure call.
// 3. The procedure call is executed.
// 4. The output parameters of the procedure call are retrieved and
//    displayed in the graphics window.
// Steps 2-4 are executed repeatedly for different images.

#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMainWindow>
#include <QThread>
#include <QLabel>

#include "HalconCpp.h"
#include "HDevThread.h"
#include "HDevEngineCpp.h"

#include "qhalconwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainView; }
QT_END_NAMESPACE

using namespace HalconCpp;
using namespace HDevEngineCpp;

class MainView : public QMainWindow
{
    Q_OBJECT

public:
    MainView(QWidget *parent = nullptr);
    ~MainView();
    void InitWin();
    void DetectFin();
    void run();
    void DisplayMessage(const QString message);

protected slots:

    void InitFg();
    void resizeEvent(QResizeEvent*);

private slots:
    void on_actionStart_triggered();
    void on_actionStop_triggered();
    void threadFinished();
private:
    Ui::MainView *ui;
    QHalconWindow* Disp;
    QThread* workerThread;
    bool m_stop;

    QLabel* resultLabel;

    // HALCON variables
    HTuple WindowIDBuf, FGHandle, ImageWidth, ImageHeight;
    HTuple WindowWidth, WindowHeight;
    HObject Image;
};
#endif // MAINVIEW_H
