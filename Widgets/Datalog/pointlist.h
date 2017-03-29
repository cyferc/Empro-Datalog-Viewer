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
    int length();

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
    QString name;
    QString units;
    QColor  lineColor;

    double axisBoundMinX = 0.0;
    double axisBoundMaxX = 10.0;

    double axisBoundMinY = 0.0;
    double axisBoundMaxY = 10.0;

    // Index in listPoints
    double axisBoundMinX_index = 0.0;
    double axisBoundMaxX_index = 10.0;

    double maxX;
    double minX;
    double maxY;
    double minY;
    QList<QPointF> listPoints;
};
