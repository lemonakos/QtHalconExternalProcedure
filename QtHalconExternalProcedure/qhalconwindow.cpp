// HALCON/Qt pattern matching and measure example
//
// (c) 2004-2015 MVTec Software GmbH

// QHalconWindow.cpp : Implementation of the class QHalconWindow
//

#include "qhalconwindow.h"

#ifdef Q_WS_X11
#if QT_VERSION >= 0x040000  
#include <QX11Info>
#endif
#include <X11/Xlib.h>
#endif


QHalconWindow::QHalconWindow(QWidget *parent, long Width, long Height)
        : QWidget(parent)
{
  resize(Width,Height);
  show();
  OpenWindow();
}


QHalconWindow::~QHalconWindow(void)
{
  using namespace HalconCpp;

  CloseWindow(WindowID());
}


// Open a HALCON window inside the QHalconWindow widget
void QHalconWindow::OpenWindow(void)
{
  using namespace HalconCpp;

#ifdef Q_WS_X11
  // In Qt versions older version 3.3, it is necessary to temporarily
  // change the SubstructureRedirectMask attribute of a Qt widget 
  // in order to be able to open a HALCON window inside the widget
  XWindowAttributes attr;
#if QT_VERSION >= 0x040000  
  XGetWindowAttributes(x11Info().display(),winId(),&attr);
  if (attr.your_event_mask & SubstructureRedirectMask)
    XSelectInput(x11Info().display(),winId(),
                 attr.your_event_mask & ~SubstructureRedirectMask);
  XFlush(x11Info().display());
  XSync(x11Info().display(),False);
#else
  XGetWindowAttributes(x11Display(),winId(),&attr);
  if (attr.your_event_mask & SubstructureRedirectMask)
    XSelectInput(x11Display(),winId(),
                 attr.your_event_mask & ~SubstructureRedirectMask);
  XFlush(x11Display());
  XSync(x11Display(),False);
#endif
#endif
  SetWindowAttr("border_width",0);
  SetCheck("~father");
  // Open a HALCON window with dummy width and height
  // it will be resized soon
  HalconCpp::OpenWindow(0,0,100,100,(Hlong)winId(),"visible","",&WinID);
  SetCheck("father");
#ifdef Q_WS_X11
  // Reset widget attributes to previous settings (see above comment)
#if QT_VERSION >= 0x040000  
  if (attr.your_event_mask & SubstructureRedirectMask)
    XSelectInput(x11Info().display(),winId(),attr.your_event_mask);
#else
  if (attr.your_event_mask & SubstructureRedirectMask)
    XSelectInput(x11Display(),winId(),attr.your_event_mask);
#endif
#endif
}


// Resize the HALCON window whenever the QHalconWindow widget is resized
void QHalconWindow::resizeEvent(QResizeEvent*)
{
  using namespace HalconCpp;

#ifdef Q_WS_X11
  // See comment in ::OpenWindow()
  XWindowAttributes attr;
#if QT_VERSION >= 0x040000  
  XGetWindowAttributes(x11Info().display(),winId(),&attr);
  if (attr.your_event_mask & SubstructureRedirectMask)
    XSelectInput(x11Info().display(),winId(),
                 attr.your_event_mask & ~SubstructureRedirectMask);
  XFlush(x11Info().display());
  XSync(x11Info().display(),False);
#else
  XGetWindowAttributes(x11Display(),winId(),&attr);
  if (attr.your_event_mask & SubstructureRedirectMask)
    XSelectInput(x11Display(),winId(),
                 attr.your_event_mask & ~SubstructureRedirectMask);
  XFlush(x11Display());
  XSync(x11Display(),False);
#endif
#endif
  // Set the HALCON window to its new size.
  SetWindowExtents(WindowID(),0,0,width(),height());
#ifdef Q_WS_X11
  // See comment in ::OpenWindow()
#if QT_VERSION >= 0x040000  
  if (attr.your_event_mask & SubstructureRedirectMask)
    XSelectInput(x11Info().display(),winId(),attr.your_event_mask);
#else
  if (attr.your_event_mask & SubstructureRedirectMask)
    XSelectInput(x11Display(),winId(),attr.your_event_mask);
#endif
#endif
}

