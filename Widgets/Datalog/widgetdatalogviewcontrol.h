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
    explicit WidgetDatalogViewControl(QWidget* pParent,
                                      MainWindow* pMainWindow,
                                      QDockWidget* pDockWidgetChannelList,
                                      QDockWidget* pDockWidgetSelection);

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
    void plotMousePressEvent(QMouseEvent* pEvent);
    void plotMouseMoveEvent(QMouseEvent* pEvent);
    void plotWheelEvent(QWheelEvent* pEvent);
    void plotResizeEvent(QResizeEvent* pEvent);

private:
    enum class eFileType
    {
        Megasquirt_MSL,
        CSV
    };

    void deleteAllIn_ListOfPointLists();
    bool processDatalogLine(QByteArray line, int lineNumber);
    bool processDatalogFirstLine(QByteArray line);
    void addPlointListsToChannelList();

    eFileType m_FileType;
    MainWindow* m_pMainWindow;
    HorizontalAxis* m_pHorizontalAxis;
    QSplitter* m_pSplitterPlots;
    QTableWidget* m_pTableChannelList;
    std::vector<PointList*> m_VecOfPointLists;
    QMenu* m_pPlotContextMenu;

    double m_XAxisBoundMin = 2.0;
    double m_XAxisBoundMax = 5.0;
    const int m_InitialXAxisBoundMax = 10; // initial zoom

    const int cLineColorBetweenA = 70;
    const int cLineColorBetweenB = 255;

    int m_TimeColumn;
    QString m_TimeColumnName;

    QPoint m_LastMousePos;
    double m_OriginalXAxisBoundMin;
    double m_OriginalXAxisBoundMax;

    bool m_AntiAliasing = false;
    bool m_ShowCurrentValueMarkers = true;
    bool m_ShowPoints = false;
};
