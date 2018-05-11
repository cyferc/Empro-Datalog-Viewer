#include "horizontalaxis.h"
#include "helpers.h"
#include <QPainter>

#include <QDebug>

HorizontalAxis::HorizontalAxis(QWidget *pParent) : QWidget(pParent)
{
    this->setMinimumHeight(m_MinimumHeight);

    setAutoFillBackground(true);
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, Qt::black);
    setPalette(palette);
}

void HorizontalAxis::resizeEvent(QResizeEvent */*event*/)
{
    m_MaxSteps = width() / 60;
}

void HorizontalAxis::dataPointMapToScreenX(double &xValue, double xAxisBoundMin, double xAxisBoundMax)
{
    xValue = Helpers::LinearInterpolation(xAxisBoundMin,
                                          0,
                                          xAxisBoundMax,
                                          width(),
                                          xValue);
}

void HorizontalAxis::paintEvent(QPaintEvent *)
{
    QPen pen;
    QPainter painter(this);

    pen.setCosmetic(true);
    pen.setWidth(m_LineWidth);
    pen.setColor(QColor(255,255,255));

    painter.setPen(pen);

    // Top line
    painter.drawLine(QPoint(0, 0), QPoint(width(), 0));

    double step_unaligned = m_AxisRange / m_MaxSteps;
    double round_to;

    if (step_unaligned <= 0.001)
    {
        round_to = 0.001;
    }
    else if (step_unaligned <= 0.0025)
    {
        round_to = 0.0025;
    }
    else if (step_unaligned <= 0.005)
    {
        round_to = 0.005;
    }
    else if (step_unaligned <= 0.01)
    {
        round_to = 0.01;
    }
    else if (step_unaligned <= 0.025)
    {
        round_to = 0.025;
    }
    else if (step_unaligned <= 0.05)
    {
        round_to = 0.05;
    }
    else if (step_unaligned <= 0.1)
    {
        round_to = 0.1;
    }
    else if (step_unaligned <= 0.25)
    {
        round_to = 0.25;
    }
    else if (step_unaligned <= 0.5)
    {
        round_to = 0.5;
    }
    else if (step_unaligned <= 1.0)
    {
        round_to = 1.0;
    }
    else if (step_unaligned <= 2.5)
    {
        round_to = 2.5;
    }
    else if (step_unaligned <= 5.0)
    {
        round_to = 5.0;
    }
    else if (step_unaligned <= 10.0)
    {
        round_to = 10.0;
    }
    else if (step_unaligned <= 25.0)
    {
        round_to = 25;
    }
    else if (step_unaligned <= 50.0)
    {
        round_to = 50;
    }
    else if (step_unaligned <= 100.0)
    {
        round_to = 100;
    }
    else if (step_unaligned <= 250.0)
    {
        round_to = 250;
    }
    else if (step_unaligned <= 500.0)
    {
        round_to = 500;
    }
    else if (step_unaligned <= 1000.0)
    {
        round_to = 1000;
    }
    else if (step_unaligned <= 2500.0)
    {
        round_to = 2500;
    }
    else if (step_unaligned <= 5000.0)
    {
        round_to = 5000;
    }
    else if (step_unaligned <= 10000.0)
    {
        round_to = 10000;
    }
    else if (step_unaligned <= 25000.0)
    {
        round_to = 25000;
    }
    else if (step_unaligned <= 50000.0)
    {
        round_to = 50000;
    }
    else if (step_unaligned <= 100000.0)
    {
        round_to = 100000;
    }
    else
    {
        round_to = 1000000;
    }

    double start_value  = Helpers::round_down_to_nearest_d(m_AxisBoundMin, round_to);
    double step_aligned = Helpers::round_up_to_nearest_d(step_unaligned, round_to);
    if (step_aligned == 0.0)
    {
        //qDebug() << "step_u = 0\n";
        step_aligned = step_unaligned;
    }

    //qDebug() << "step_u:" << step_unaligned << "\tstep_a: " << step_aligned << "\tstart: " << start_value;

    for (double current_value = start_value - step_aligned; current_value < (m_AxisBoundMax); current_value += step_aligned)
    {
        double xValue = current_value;
        dataPointMapToScreenX(xValue, m_AxisBoundMin, m_AxisBoundMax);
        painter.drawLine(QPoint(xValue, 0), QPoint(xValue, height()));
        painter.drawText(QPoint(xValue + m_LineWidth + 2, 11), QString::number(current_value).append(" ").append(m_Units));
    }
}

void HorizontalAxis::setAxisBounds(double min, double max)
{
    if (min > max)
        return;
    m_AxisBoundMin = min;
    m_AxisBoundMax = max;
    m_AxisRange = max - min;
    repaint();
}
