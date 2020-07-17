// HALCON/Qt pattern matching and measure example
//
// (c) 2004-2015 MVTec Software GmbH

// QHalconWindow.h : Class used for opening HALCON windows in Qt
//

#if QT_VERSION >= 0x050000
    #include <QtWidgets/qwidget.h>
#else
    #include <qwidget.h>
#endif
#ifndef __APPLE__
#  include "HalconCpp.h"
#else
#  ifndef HC_LARGE_IMAGES
#    include <HALCONCpp/HalconCpp.h>
#  else
#    include <HALCONCppxl/HalconCpp.h>
#  endif
#endif


class QHalconWindow: public QWidget
{
  Q_OBJECT

  public:
    QHalconWindow(QWidget *parent=0, long Width=0, long Height=0);
    virtual ~QHalconWindow(void);

    HalconCpp::HTuple WindowID(void) {return WinID;}

  protected:
    void resizeEvent(QResizeEvent*);

  private:
    HalconCpp::HTuple WinID;
    void OpenWindow(void);
};
