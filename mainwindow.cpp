#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *pParent) :
    QMainWindow(pParent),
    m_pUi(new Ui::MainWindow)
{
    m_pUi->setupUi(this);
    setWindowTitle(cWindowTitle);

    // start Maximized
    showMaximized();

    m_pWidgetDatalogViewControl = new WidgetDatalogViewControl(this,
                                                             this,
                                                             m_pUi->dockWidgetChannelList,
                                                             m_pUi->dockWidgetSelection);
    QObject::connect(this,
                     &MainWindow::evenPlotSpacing,
                     m_pWidgetDatalogViewControl,
                     &WidgetDatalogViewControl::evenPlotSpacing);

    QGridLayout *contentLayout = new QGridLayout(m_pUi->centralWidget);
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    contentLayout->addWidget(m_pWidgetDatalogViewControl);

    // Add dock window view controls
    m_pUi->menuDock_Windows->addAction(m_pUi->dockWidgetChannelList->toggleViewAction());
    //ui->menuDock_Windows->addAction(ui->dockWidgetSelection->toggleViewAction());
    m_pUi->dockWidgetSelection->hide();
}

MainWindow::~MainWindow()
{
    delete m_pUi;
}

void MainWindow::SetWindowTitle(QString title)
{
    setWindowTitle(title + " - " + cWindowTitle);
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionOpen_triggered()
{
    m_pWidgetDatalogViewControl->openDatalogClicked();
}

void MainWindow::on_actionAdd_Plot_triggered()
{
    m_pWidgetDatalogViewControl->addPlot();
}

void MainWindow::on_actionDelete_Plot_triggered()
{
    m_pWidgetDatalogViewControl->deleteBottomPlot();
}

void MainWindow::on_actionShow_Points_triggered(bool checked)
{
    m_pWidgetDatalogViewControl->showPoints(checked);
}

void MainWindow::on_actionShow_Markers_triggered(bool checked)
{
    m_pWidgetDatalogViewControl->showCurrentValueMarkers(checked);
}

void MainWindow::on_actionAnti_Aliasing_triggered(bool checked)
{
    m_pWidgetDatalogViewControl->setAntialiasing(checked);
}

void MainWindow::on_actionRightMouseSingleSelection_triggered(bool checked)
{
    m_pUi->actionRightMouseMultiSelection->setChecked(!checked);
}

void MainWindow::on_actionRightMouseMultiSelection_triggered(bool checked)
{
    m_pUi->actionRightMouseSingleSelection->setChecked(!checked);
}

void MainWindow::on_actionSetup_triggered()
{
    m_pWidgetDatalogViewControl->setupClicked();
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
    m_pWidgetDatalogViewControl->zoomIn();
}

void MainWindow::on_actionZoom_Out_triggered()
{
    m_pWidgetDatalogViewControl->zoomOut();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About Empro Datalog Viewer", "Version: 0.1.0\n© Empro");
}
