#include "pointlist.h"
#include <cfloat>
#include <QDebug>

PointList::PointList()
{
    minX        = DBL_MAX;
    maxX        = DBL_MIN;
    minY        = DBL_MAX;
    maxY        = DBL_MIN;
    name        = "";
    units       = "";
    lineColor   = Qt::white;
}

PointList::~PointList()
{
    listPoints.clear();
}

void PointList::setName(QString newName)
{
    name = newName;
}

void PointList::setUnits(QString newUnits)
{
    units = newUnits;
}

void PointList::setColor(QColor color)
{
    lineColor = color;
}

void PointList::setAxisBoundsX(double min, double max)
{
    axisBoundMinX = min;
    axisBoundMaxX = max;

    // Binary search to find the min and max indices
    axisBoundMinX_index = binarySearchX(axisBoundMinX);
    axisBoundMaxX_index = binarySearchX(axisBoundMaxX);

    /*
    // Linear Search
    // Find the new indices within listPoints
    bool minIndexFound = false;
    for (int i = 0; i < listPoints.count(); i++)
    {
        if (!minIndexFound)
        {
            if (axisBoundMinX < listPoints.at(i).x())
            {
                axisBoundMinX_index = i;
                minIndexFound = true;
            }
        }

        if (axisBoundMaxX < listPoints.at(i).x())
        {
            axisBoundMaxX_index = i;
            break;
        }
    }
    */
}

/*
* searches for a value in sorted array
*   arr is an array to search in
*   value is searched value
*   left is an index of left boundary
*   right is an index of right boundary
* returns position of searched value, if it presents in the array
* or -1, if it is absent
*/
int PointList::binarySearchX(double value)
{
    int left = 0;
    int right = listPoints.count() - 1;

    while (left <= right)
    {
        int middle = (left + right) / 2;

        // If we're at the last index
        if (middle == (listPoints.count() - 1))
        {
            return middle + 1; // Return + 1 so that the rendered can draw the last line between points, fix this stupidity?
        }
        // If value is between middle and middle + 1, we found the index
        else if ((listPoints.at(middle).x() < value) && (value < listPoints.at(middle + 1).x()))
            return middle + 1;  // Return + 1 so that the rendered can draw the last line between points, fix this stupidity?
        else if (listPoints.at(middle).x() > value)
            right = middle - 1;
        else
            left = middle + 1;
    }
    return 0;
}

void PointList::setAxisBoundsY(double min, double max)
{
    axisBoundMinY = min;
    axisBoundMaxY = max;
}

double PointList::getAxisBoundsMinY()
{
    return axisBoundMinY;
}

double PointList::getAxisBoundsMaxY()
{
    return axisBoundMaxY;
}

double PointList::getAxisBoundsMinX()
{
    return axisBoundMinX;
}

double PointList::getAxisBoundsMaxX()
{
    return axisBoundMaxX;
}

double PointList::getAxisBoundsMinX_index()
{
    return axisBoundMinX_index;
}

double PointList::getAxisBoundsMaxX_index()
{
    return axisBoundMaxX_index;
}

QString PointList::getName()
{
    return name;
}

QString PointList::getUnits()
{
    return units;
}

QColor PointList::getColor()
{
    return lineColor;
}

void PointList::append(const QPointF& point)
{
    listPoints.append(point);

    /// X Time
    if (point.x() > maxX)
    {
        maxX = point.x();
    }

    if (point.x() < minX)
    {
        minX = point.x();
    }

    /// Y Value
    if (point.y() > maxY)
    {
        //qDebug() << "setting maxY: " << t.y();
        maxY = point.y();
        //qDebug() << "maxY = " << getMaxY();
    }

    if (point.y() < minY)
    {
        minY = point.y();
    }
}

int PointList::length()
{
    return listPoints.length();
}

float PointList::getMaxX()
{
    return this->maxX;
}

float PointList::getMinX()
{
    return this->minX;
}

float PointList::getMaxY()
{
    return this->maxY;
}

float PointList::getMinY()
{
    return this->minY;
}

QPointF PointList::value(int index)
{
    return listPoints.value(index);
}
