#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // start Maximized
    showMaximized();

    _widgetDatalogViewControl = new WidgetDatalogViewControl(this,
                                                             ui->dockWidgetChannelList,
                                                             ui->dockWidgetSelection);
    QObject::connect(this,
                     SIGNAL(evenPlotSpacing()),
                     _widgetDatalogViewControl,
                     SLOT(evenPlotSpacing()));

    QGridLayout *contentLayout = new QGridLayout(ui->centralWidget);
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    contentLayout->addWidget(_widgetDatalogViewControl);

    // Add dock window view controls
    ui->menuDock_Windows->addAction(ui->dockWidgetChannelList->toggleViewAction());
    //ui->menuDock_Windows->addAction(ui->dockWidgetSelection->toggleViewAction());
    ui->dockWidgetSelection->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionOpen_triggered()
{
    _widgetDatalogViewControl->openDatalogClicked();
}

void MainWindow::on_actionAdd_Plot_triggered()
{
    _widgetDatalogViewControl->addPlot();
}

void MainWindow::on_actionDelete_Plot_triggered()
{
    _widgetDatalogViewControl->deleteBottomPlot();
}

void MainWindow::on_actionShow_Points_triggered(bool checked)
{
    _widgetDatalogViewControl->showPoints(checked);
}

void MainWindow::on_actionShow_Markers_triggered(bool checked)
{
    _widgetDatalogViewControl->showCurrentValueMarkers(checked);
}

void MainWindow::on_actionAnti_Aliasing_triggered(bool checked)
{
    _widgetDatalogViewControl->setAntialiasing(checked);
}

void MainWindow::on_actionRightMouseSingleSelection_triggered(bool checked)
{
    ui->actionRightMouseMultiSelection->setChecked(!checked);
}

void MainWindow::on_actionRightMouseMultiSelection_triggered(bool checked)
{
    ui->actionRightMouseSingleSelection->setChecked(!checked);
}

void MainWindow::on_actionSetup_triggered()
{
    _widgetDatalogViewControl->setupClicked();
}

void MainWindow::on_actionFullscreen_triggered(bool checked)
{
    if (checked)
    {
        this->showFullScreen();
    }
    else
    {
        this->showMaximized();
    }
}

void MainWindow::on_actionEven_Spacing_triggered()
{
    evenPlotSpacing();
}

void MainWindow::on_actionZoom_In_triggered()
{
    _widgetDatalogViewControl->zoomIn();
}

void MainWindow::on_actionZoom_Out_triggered()
{
    _widgetDatalogViewControl->zoomOut();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About Empro Datalog Viewer", "Version: 0.1.0\n© Empro");
}
