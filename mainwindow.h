#pragma once

#include <QMainWindow>
#include "Widgets/Datalog/widgetdatalogviewcontrol.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* pParent = 0);
    ~MainWindow();

    void SetWindowTitle(QString title);

    const QString cWindowTitle = "Empro Datalog Viewer";

signals:
    void evenPlotSpacing();

private slots:
    void on_actionExit_triggered();
    void on_actionOpen_triggered();

    void on_actionAdd_Plot_triggered();
    void on_actionDelete_Plot_triggered();

    void on_actionShow_Points_triggered(bool checked);
    void on_actionShow_Markers_triggered(bool checked);
    void on_actionAnti_Aliasing_triggered(bool checked);

    void on_actionRightMouseSingleSelection_triggered(bool checked);
    void on_actionRightMouseMultiSelection_triggered(bool checked);

    void on_actionSetup_triggered();
    void on_actionFullscreen_triggered(bool checked);

    void on_actionEven_Spacing_triggered();

    void on_actionZoom_In_triggered();

    void on_actionZoom_Out_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow* m_pUi;
    WidgetDatalogViewControl* m_pWidgetDatalogViewControl;
};
