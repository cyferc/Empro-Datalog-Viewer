#pragma once

#include <QObject>
#include <QList>
#include <QPointF>
#include <QColor>

class PointList : public QObject
{
    Q_OBJECT

public:
    PointList();
    ~PointList();

    void append(const QPointF& point);
    int size();

    int getID();

    float getMaxX();
    float getMinX();
    float getMaxY();
    float getMinY();

    double getAxisBoundsMinY();
    double getAxisBoundsMaxY();
    double getAxisBoundsMinX();
    double getAxisBoundsMaxX();

    double getAxisBoundsMinX_index();
    double getAxisBoundsMaxX_index();

    QPointF value(int index);

    QString getName();
    QString getUnits();
    QColor getColor();

public slots:
    void setAxisBoundsX(double min, double max);
    void setAxisBoundsY(double min, double max);
    void setName(QString newName);
    void setUnits(QString newUnits);
    void setColor(QColor color);

private:
    int binarySearchX(double value);
    QString m_Name;
    QString m_Units;
    QColor  m_LineColor;

    double m_AxisBoundMinX = 0.0;
    double m_AxisBoundMaxX = 10.0;

    double m_AxisBoundMinY = 0.0;
    double m_AxisBoundMaxY = 10.0;

    // Index in m_ListPointsVec
    double m_AxisBoundMinX_index = 0.0;
    double m_AxisBoundMaxX_index = 10.0;

    double m_MaxX;
    double m_MinX;
    double m_MaxY;
    double m_MinY;
    std::vector<QPointF> m_PointsVec;
};
