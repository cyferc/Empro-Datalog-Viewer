#include "plotdatalog.h"

#include <QDebug>

#include <QPainter>
#include <QtMath>
#include <QInputDialog>
#include <cinttypes>
#include "helpers.h"

PlotDatalog::PlotDatalog(QWidget *pParent) : QWidget(pParent)
{
    setMouseTracking(true);

    setAutoFillBackground(true);
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, Qt::black);
    setPalette(palette);

    setMinimumSize(20,20);
}

PlotDatalog::~PlotDatalog()
{
    clearPointLists();
}

void PlotDatalog::addPointList(PointList* pList)
{
    m_PlotsVec.push_back(pList);
}

void PlotDatalog::setDrawPoints(bool p)
{
    m_DrawPoints = p;
    this->repaint();
}

void PlotDatalog::setDrawCurrentValueMarkers(bool draw)
{
    m_DrawCurrentValueMarkers = draw;
    this->repaint();
}

void PlotDatalog::setAntiAliasing(bool p)
{
    m_AntiAliasing = p;
    this->repaint();
}

void PlotDatalog::clearPointLists()
{
    // TODO: delete heap memory here?
    m_PlotsVec.clear();
}

void PlotDatalog::dataPointMapToScreenX(double &xValue, double xAxisBoundMin, double xAxisBoundMax)
{
    xValue = Helpers::LinearInterpolation(xAxisBoundMin,
                                          m_MarginLeft,
                                          xAxisBoundMax,
                                          width() - m_MarginRight, xValue);
}

void PlotDatalog::dataPointMapToScreenY(double &yValue, double yAxisBoundMin, double yAxisBoundMax)
{
    yValue = Helpers::LinearInterpolation(yAxisBoundMin,
                                          m_MarginBottom + cLineWidth,
                                          yAxisBoundMax,
                                          height() - m_MarginTop - cLineWidth, yValue);
}

void PlotDatalog::contextMenuEvent(QContextMenuEvent *event)
{
    event->accept();
}

void PlotDatalog::mousePressEvent(QMouseEvent *pEvent)
{
    emit mousePressEventSignal(pEvent);
    pEvent->accept();
}

void PlotDatalog::mouseMoveEvent(QMouseEvent *pEvent)
{
    emit mouseMoveEventSignal(pEvent);
    pEvent->accept();
}

void PlotDatalog::wheelEvent(QWheelEvent *pEvent)
{
    emit wheelEventSignal(pEvent);
    pEvent->accept();
}

void PlotDatalog::resizeEvent(QResizeEvent* pEvent)
{
    emit resizeEventSignal(pEvent);
    pEvent->accept();
}

void PlotDatalog::paintEvent(QPaintEvent *)
{
    int pointsDrawnCount = 0;
    int linesDrawnCount = 0;

    QPen pen;
    double       itemClosestToMouseCoordX = 0;
    unsigned int itemClosestToMouseIndex;
    int minDistanceToMouse = INT_MAX;
    QPoint mousePosition = this->mapFromGlobal(QCursor::pos());

    QPainter painter(this);
    painter.setClipRect(QRect(m_MarginLeft, m_MarginTop, width() - m_MarginLeft - m_MarginRight, height() - m_MarginBottom - m_MarginTop), Qt::ReplaceClip);
    painter.setRenderHint(QPainter::Antialiasing, m_AntiAliasing);

    // Translate so bottom left is (0,0), up and right is positive
    painter.scale(1, -1);
    painter.translate(0, -height());

    pen.setCosmetic(true);
    pen.setWidth(cLineWidth);
    pen.setColor(QColor(200,200,200));
    painter.setPen(pen);

    int plotsVecSize = static_cast<int>(m_PlotsVec.size());
    for (int plot = 0; plot < plotsVecSize; plot++)
    {
        PointList *p = m_PlotsVec[plot];

        // Set line style for each plot
        pen.setColor(p->getColor());
        painter.setPen(pen);

        if (p->size() > 0)
        {
            ///////////////////////////////////////
            /// First sample
            ///////////////////////////////////////
            QPointF previousPoint = p->value(p->getAxisBoundsMinX_index());

            dataPointMapToScreenX(previousPoint.rx(), p->getAxisBoundsMinX(), p->getAxisBoundsMaxX());
            dataPointMapToScreenY(previousPoint.ry(), p->getAxisBoundsMinY(), p->getAxisBoundsMaxY());

            if (m_DrawPoints)
            {
                // Set point style for each plot
                pen.setWidth(cPointSize);
                painter.setPen(pen);

                painter.drawPoint(previousPoint);

                pointsDrawnCount++;

                // Back to line style
                pen.setWidth(cLineWidth);
                painter.setPen(pen);
            }

            // Only the first plot, work out closest point to mouse for first point
            if (plot == 0)
            {
                // Calculate distance to mouse for point 1
                int distanceToMouse = abs(previousPoint.x() - mousePosition.x());

                // Save smallest distance to mouse, this is where we will draw the vertical line
                if (distanceToMouse < minDistanceToMouse)
                {
                    minDistanceToMouse = distanceToMouse;

                    // Save this point
                    itemClosestToMouseCoordX = previousPoint.x();
                    itemClosestToMouseIndex = 0;
                }
            }

            ///////////////////////////////////////
            /// The rest of the samples
            ///////////////////////////////////////
            for (int sample = p->getAxisBoundsMinX_index() + 1; sample < p->getAxisBoundsMaxX_index(); sample++)
            {
                QPointF currentPoint = p->value(sample);

                /// X Axis
                dataPointMapToScreenX(currentPoint.rx(), p->getAxisBoundsMinX(), p->getAxisBoundsMaxX());
                dataPointMapToScreenY(currentPoint.ry(), p->getAxisBoundsMinY(), p->getAxisBoundsMaxY());

                // Draw line between the points
                painter.drawLine(previousPoint, currentPoint);

                linesDrawnCount++;

                if (m_DrawPoints)
                {
                    // Set point style for each plot
                    pen.setWidth(cPointSize);
                    painter.setPen(pen);

                    painter.drawPoint(currentPoint);

                    pointsDrawnCount++;

                    // Back to line style
                    pen.setWidth(cLineWidth);
                    painter.setPen(pen);
                }

                // Only the first plot, work out closest point to mouse
                if (plot == 0)
                {
                    // Calculate distance to mouse for point 1
                    int distanceToMouse = abs(currentPoint.x() - mousePosition.x());

                    // Save smallest distance to mouse, this is where we will draw the vertical line
                    if (distanceToMouse < minDistanceToMouse)
                    {
                        minDistanceToMouse = distanceToMouse;

                        // Save this point
                        itemClosestToMouseCoordX = currentPoint.x();
                        itemClosestToMouseIndex = sample;
                    }
                }

                previousPoint.setX(currentPoint.x());
                previousPoint.setY(currentPoint.y());
            }
        }
    }

    painter.setClipping(false);

    if (m_DrawCurrentValueMarkers)
    {
        // Draw horizontal line at current point under mouse
        int plotsVecSize = static_cast<int>(m_PlotsVec.size());
        for (int plot = 0; plot < plotsVecSize; plot++)
        {
            PointList *p = m_PlotsVec[plot];
            pen.setWidth(cCurrentValueMarkerLineThickness);
            pen.setColor(p->getColor());
            painter.setPen(pen);

            // Work out Y pixel coordinates
            double itemClosestToMouseCoordY = p->value(itemClosestToMouseIndex).y();
            dataPointMapToScreenY(itemClosestToMouseCoordY, p->getAxisBoundsMinY(), p->getAxisBoundsMaxY());

            // Draw the line
            painter.drawLine(itemClosestToMouseCoordX - cCurrentValueMarkerLineHalfLength, itemClosestToMouseCoordY,     // from (x1, y1)
                             itemClosestToMouseCoordX + cCurrentValueMarkerLineHalfLength, itemClosestToMouseCoordY);    // to   (x2, y2)
        }
    }

    // Draw mouse line, snap to point
    pen.setWidth(1);
    pen.setColor(Qt::white);
    painter.setPen(pen);
    painter.drawLine(itemClosestToMouseCoordX, 0,            // from (x1, y1)
                     itemClosestToMouseCoordX, height());    // to   (x2, y2)

    // Draw text
    painter.resetTransform();

    int textHeight = 10 + m_MarginTop;
    double currentYaxisX = 0;

    for (int plot = 0; plot < plotsVecSize; plot++)
    {
        QString textNumber;
        PointList *p = m_PlotsVec[plot];

        if (plot == 0)
        {
            // Print time
            textNumber = QString::number(p->value(itemClosestToMouseIndex).x());
            QString timeStr("Time: ");

            painter.drawText(QPoint(currentYaxisX + 10, textHeight), timeStr.append(textNumber));
            textHeight += 10;
        }

        textNumber = QString::number(p->value(itemClosestToMouseIndex).y());

        pen.setColor(p->getColor());
        painter.setPen(pen);
        painter.drawText(QPoint(currentYaxisX + 10, textHeight), p->getName().append(": ").append(textNumber));
        textHeight += 10;
    }

    //qDebug() << "p: " << pointsDrawnCount << " l: " << linesDrawnCount;

    // Draw bottom axis
    //painter.fillRect(QRectF(-screenHalfWidth, 0,            // x, y
    //                        width(), bottomAxisLineHeight), // width, height
    //                 QColor(70,70,70));
    //painter.drawLine(-screenHalfWidth, bottomAxisLineHeight, screenHalfWidth, bottomAxisLineHeight);
}
