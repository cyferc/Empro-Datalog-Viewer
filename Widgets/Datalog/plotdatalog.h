#pragma once

#include <QWidget>
#include <QWheelEvent>
#include "pointlist.h"

class PlotDatalog : public QWidget
{
    Q_OBJECT

public:
    PlotDatalog(QWidget *parent = 0);
    ~PlotDatalog();

    QVector<bool> vecChannelsDraw;
    QVector<bool> vecChannelsYAxis;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent* event);
    void contextMenuEvent(QContextMenuEvent *event);

public slots:
    void setDrawPoints(bool p);
    void setAntiAliasing(bool p);
    void addPointList(PointList* pList);
    void clearPointLists();
    void setDrawCurrentValueMarkers(bool draw);

signals:
    void mousePressEventSignal(QMouseEvent *event);
    void mouseMoveEventSignal(QMouseEvent *event);
    void wheelEventSignal(QWheelEvent *event);
    void resizeEventSignal(QResizeEvent* event);

private:
    void __inline__ dataPointMapToScreenY(double &yValue, double yAxisBoundMin, double yAxisBoundMax);
    void __inline__ dataPointMapToScreenX(double &xValue, double xAxisBoundMin, double xAxisBoundMax);

    const int currentValueMarkerLineThickness = 4;
    const int currentValueMarkerLineHalfLength = 5;

    const int lineWidth = 1;
    const int lineColorBetweenA = 70;
    const int lineColorBetweenB = 255;

    const int pointSize = 4;

    int marginTop = 3;
    int marginBottom = 3;
    int marginLeft = 0;
    int marginRight = 0;

    QList<PointList*> listPlots;
    bool drawPoints = false;
    bool drawCurrentValueMarkers = true;
    bool antiAliasing = false;
};
