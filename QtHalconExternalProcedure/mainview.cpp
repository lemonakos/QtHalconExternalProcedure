#include "mainview.h"
#include "ui_mainview.h"
#include <QDebug>

MainView::MainView(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainView)
{
    ui->setupUi(this);
    resultLabel = new QLabel(this);
    m_stop=false;

    WindowIDBuf=-1;

    ui->actionStop->setEnabled(false);

    //   Disp: Father widget for HALCON window in HBoxDispAndButtons
    Disp = new QHalconWindow(this,200,200);
    Disp->setMinimumSize(50,50);

    ui->mainVLayout->addWidget(Disp);

    ui->statusbar->addWidget(resultLabel,1);
}

MainView::~MainView()
{
    // Close all allocated HALCON resources.
    CloseWindow(WindowIDBuf);
    CloseFramegrabber(FGHandle);
    delete ui;
}

void MainView::DetectFin()
{
    try
    {


        // Create procedure and procedure call
        HDevProcedure     proc("detect_fin");
        HDevProcedureCall proc_call(proc);

        // Grab and process next image
        while (!m_stop)
        {
            // Grab next image

            HImage image;
            GrabImage(&Image,FGHandle);

            // Set input parameters for procedure call
            proc_call.SetInputIconicParamObject(1,Image);
            // Execute procedure call
            try
            {
                proc_call.Execute();
                // Get output parameters from procedure call
                HRegion fin_region = proc_call.GetOutputIconicParamObject(1);
                HTuple  fin_area;
                proc_call.GetOutputCtrlParamTuple(1,&fin_area);

                // Display results

                DispImage(Image,Disp->WindowID());
                SetColor(Disp->WindowID(),"red");
                DispRegion(fin_region,Disp->WindowID());
                SetColor(Disp->WindowID(),"white");

                resultLabel->setText("Fin area:= " + QString::number(fin_area[0].L()));

            }
            catch (HDevEngineException &hdev_exception)
            {
                // If an exception occurs during procedure call execution:
                // -> print error message, cleanup, and continue with next image

                DisplayMessage(hdev_exception.Message());

                continue;
            }
        }
    }
    catch (HDevEngineException &hdev_exception)
    {
        // If an exception occurs during loading the procedure
        // -> print error message and exit

        DisplayMessage(hdev_exception.Message());

        exit(0);
    }
}

void MainView::run()
{
    std::string halcon_examples =(std::string)HSystem::GetSystem("example_dir")[0].S();
    std::string ext_proc_path(halcon_examples);

    ext_proc_path += "/hdevengine/procedures";

    // Set external procedure path
    HDevEngine().SetProcedurePath(ext_proc_path.c_str());


    // Call action routine
    DetectFin();
}

void MainView::DisplayMessage(const QString message)
{
    qDebug()<< message;
}

void MainView::InitWin()
{
    InitFg();
    // Initialize display
    // OpenWindow(0,100,384,288,0,"buffer","",&WindowIDBuf);
    OpenWindow(0,0,575,767,0,"buffer","",&WindowIDBuf);
    SetPart(Disp->WindowID(),0,0,575,767);
    SetPart(WindowIDBuf,0,0,575,767);
    SetDraw(Disp->WindowID(),"margin");
    SetDraw(Disp->WindowID(),"margin");
    SetLineWidth(Disp->WindowID(),4);
    SetLineWidth(Disp->WindowID(),4);

}

void MainView::InitFg()
{
    Hlong disp_width, disp_height;
    const char* image_sequ_str = "fin";

    OpenFramegrabber("File",1,1,0,0,0,0,"default",-1,"default",-1,"default",image_sequ_str,"default",-1,1,&FGHandle);
    GrabImage(&Image,FGHandle);
    GetImageSize(Image,&ImageWidth,&ImageHeight);
    // Resize application to fit original image size
    disp_width = Disp->width();
    disp_height = Disp->height();
    Disp->resize(ImageWidth[0].L(),ImageHeight[0].L());
    resize(width()+ImageWidth[0].L()-disp_width,height()+ImageHeight[0].L()-disp_height);
}

void MainView::resizeEvent(QResizeEvent *)
{
    using namespace HalconCpp;

    if (WindowIDBuf>0)
    {
        // Determine the new dimensions of the QHalconWindow widget.
        // We save the new dimensions for later use in CopyRectangle.
        WindowWidth = Disp->width();
        WindowHeight = Disp->height();
        SetWindowExtents(WindowIDBuf,0,0,WindowWidth,WindowHeight);
        // Display the current image.  Note that this will slow down the resize
        // operation considerably.
        DispObj(Image,Disp->WindowID());
    }
}


void MainView::on_actionStart_triggered()
{
    ui->actionStart->setEnabled(false);
    ui->actionStop->setEnabled(true);

    m_stop=false;

    //4.Member function, call from lamba function
        workerThread = QThread::create([=](){run();});
        connect(workerThread,&QThread::started,[](){
            qDebug() << "Thread started";
        });

        connect(workerThread,&QThread::finished,workerThread,&QThread::deleteLater);
        connect(workerThread,&QThread::finished,this,&MainView::threadFinished);
        workerThread->start();


}

void MainView::on_actionStop_triggered()
{
    m_stop=true;

    ui->actionStart->setEnabled(true);
    ui->actionStop->setEnabled(false);
}

void MainView::threadFinished()
{
    qDebug() << "Thread Running " << workerThread->isRunning();
        qDebug() << "Thread Finished " << workerThread->isFinished();
        qDebug() << "Thread Finished. Current Thread : " << QThread::currentThread();
}
