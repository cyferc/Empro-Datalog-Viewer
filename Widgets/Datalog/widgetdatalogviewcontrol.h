#pragma once

#include <QWidget>
#include <QSplitter>
#include <QTableWidget>
#include <QToolButton>
#include <QMouseEvent>
#include <QMenu>
#include <QDockWidget>
#include "pointlist.h"
#include "plotdatalog.h"
#include "horizontalaxis.h"

class MainWindow;

class WidgetDatalogViewControl : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetDatalogViewControl(QWidget *parent,
                                      MainWindow* mainWindow,
                                      QDockWidget *dockWidgetChannelList,
                                      QDockWidget *dockWidgetSelection);

    void resetPlotVectorSizes();
    void zoom(double amount);
    void zoomIn();
    void zoomOut();

signals:
    void setXAxisBounds(double min, double max);
    void repaintPlots();

    void signalShowPoints(bool show);
    void signalShowCurrentValueMarkers(bool show);
    void signalSetAntialiasing(bool show);

public slots:
    void showPoints(bool show);
    void showCurrentValueMarkers(bool show);
    void setAntialiasing(bool show);
    void evenPlotSpacing();

    void setupClicked();
    void openDatalogClicked();
    void addPlot();
    void deleteBottomPlot();
    void plotMousePressEvent(QMouseEvent *event);
    void plotMouseMoveEvent(QMouseEvent *event);
    void plotWheelEvent(QWheelEvent *event);
    void plotResizeEvent(QResizeEvent* event);

private:

    enum FileType_t
    {
        FileType_Megasquirt_MSL,
        FileType_CSV
    } fileType;

    void deleteAllIn_ListOfPointLists();
    bool processDatalogLine(QByteArray line, int lineNumber);
    bool processDatalogFirstLine(QByteArray line);
    void addPlointListsToChannelList();

    MainWindow* _mainWindow;
    HorizontalAxis* horizontalAxis;
    QSplitter* splitterPlots;
    QTableWidget* tableChannelList;
    QList<PointList*> listOfPointLists;
    QMenu* plotContextMenu;

    double xAxisBoundMin = 2.0;
    double xAxisBoundMax = 5.0;
    const int initialXAxisBoundMax = 10; // initial zoom

    const int lineColorBetweenA = 70;
    const int lineColorBetweenB = 255;

    int timeColumn;
    QString timeColumnName;

    QPoint lastMousePos;
    double originalXAxisBoundMin;
    double originalXAxisBoundMax;

    bool _antiAliasing = false;
    bool _showCurrentValueMarkers = true;
    bool _showPoints = false;
};
