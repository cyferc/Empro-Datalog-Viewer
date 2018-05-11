#include "pointlist.h"
#include <cfloat>
#include <QDebug>

PointList::PointList()
{
    m_MinX        = DBL_MAX;
    m_MaxX        = DBL_MIN;
    m_MinY        = DBL_MAX;
    m_MaxY        = DBL_MIN;
    m_Name        = "";
    m_Units       = "";
    m_LineColor   = Qt::white;
}

PointList::~PointList()
{
}

void PointList::setName(QString newName)
{
    m_Name = newName;
}

void PointList::setUnits(QString newUnits)
{
    m_Units = newUnits;
}

void PointList::setColor(QColor color)
{
    m_LineColor = color;
}

void PointList::setAxisBoundsX(double min, double max)
{
    m_AxisBoundMinX = min;
    m_AxisBoundMaxX = max;

    // Binary search to find the min and max indices
    m_AxisBoundMinX_index = binarySearchX(m_AxisBoundMinX);
    m_AxisBoundMaxX_index = binarySearchX(m_AxisBoundMaxX);

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
    int right = m_PointsVec.size() - 1;

    while (left <= right)
    {
        int middle = (left + right) / 2;

        // If we're at the last index
        if (middle == (static_cast<int>(m_PointsVec.size()) - 1))
        {
            return middle + 1; // Return + 1 so that the rendered can draw the last line between points, fix this stupidity?
        }
        // If value is between middle and middle + 1, we found the index
        else if ((m_PointsVec[middle].x() < value) && (value < m_PointsVec[middle + 1].x()))
            return middle + 1;  // Return + 1 so that the rendered can draw the last line between points, fix this stupidity?
        else if (m_PointsVec[middle].x() > value)
            right = middle - 1;
        else
            left = middle + 1;
    }
    return 0;
}

void PointList::setAxisBoundsY(double min, double max)
{
    m_AxisBoundMinY = min;
    m_AxisBoundMaxY = max;
}

double PointList::getAxisBoundsMinY()
{
    return m_AxisBoundMinY;
}

double PointList::getAxisBoundsMaxY()
{
    return m_AxisBoundMaxY;
}

double PointList::getAxisBoundsMinX()
{
    return m_AxisBoundMinX;
}

double PointList::getAxisBoundsMaxX()
{
    return m_AxisBoundMaxX;
}

double PointList::getAxisBoundsMinX_index()
{
    return m_AxisBoundMinX_index;
}

double PointList::getAxisBoundsMaxX_index()
{
    return m_AxisBoundMaxX_index;
}

QString PointList::getName()
{
    return m_Name;
}

QString PointList::getUnits()
{
    return m_Units;
}

QColor PointList::getColor()
{
    return m_LineColor;
}

void PointList::append(const QPointF& point)
{
    m_PointsVec.push_back(point);

    /// X Time
    if (point.x() > m_MaxX)
    {
        m_MaxX = point.x();
    }

    if (point.x() < m_MinX)
    {
        m_MinX = point.x();
    }

    /// Y Value
    if (point.y() > m_MaxY)
    {
        m_MaxY = point.y();
    }

    if (point.y() < m_MinY)
    {
        m_MinY = point.y();
    }
}

int PointList::size()
{
    return m_PointsVec.size();
}

float PointList::getMaxX()
{
    return this->m_MaxX;
}

float PointList::getMinX()
{
    return this->m_MinX;
}

float PointList::getMaxY()
{
    return this->m_MaxY;
}

float PointList::getMinY()
{
    return this->m_MinY;
}

QPointF PointList::value(int index)
{
    return m_PointsVec[index];
}
