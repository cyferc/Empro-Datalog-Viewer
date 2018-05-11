#pragma once

#include <QWidget>
#include <QWheelEvent>
#include "pointlist.h"

class PlotDatalog : public QWidget
{
    Q_OBJECT

public:
    PlotDatalog(QWidget* pParent = 0);
    ~PlotDatalog();

    QVector<bool> vecChannelsDraw;
    QVector<bool> vecChannelsYAxis;

protected:
    void mousePressEvent(QMouseEvent* pEvent);
    void mouseMoveEvent(QMouseEvent* pEvent);
    void wheelEvent(QWheelEvent* pEvent);
    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent* pEvent);
    void contextMenuEvent(QContextMenuEvent* pEvent);

public slots:
    void setDrawPoints(bool p);
    void setAntiAliasing(bool p);
    void addPointList(PointList* pList);
    void clearPointLists();
    void setDrawCurrentValueMarkers(bool draw);

signals:
    void mousePressEventSignal(QMouseEvent* pEvent);
    void mouseMoveEventSignal(QMouseEvent* pEvent);
    void wheelEventSignal(QWheelEvent* pEvent);
    void resizeEventSignal(QResizeEvent* pEvent);

private:
    void __inline__ dataPointMapToScreenY(double &yValue, double yAxisBoundMin, double yAxisBoundMax);
    void __inline__ dataPointMapToScreenX(double &xValue, double xAxisBoundMin, double xAxisBoundMax);

    const int cCurrentValueMarkerLineThickness = 4;
    const int cCurrentValueMarkerLineHalfLength = 5;

    const double cLineWidth = 1.5;
    const int cLineColorBetweenA = 70;
    const int cLineColorBetweenB = 255;

    const int cPointSize = 4;

    int m_MarginTop = 3;
    int m_MarginBottom = 3;
    int m_MarginLeft = 0;
    int m_MarginRight = 0;

    std::vector<PointList*> m_PlotsVec;
    bool m_DrawPoints = false;
    bool m_DrawCurrentValueMarkers = true;
    bool m_AntiAliasing = false;
};
