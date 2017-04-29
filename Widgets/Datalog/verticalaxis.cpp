#include "verticalaxis.h"
#include "helpers.h"

#include <QPainter>
#include <QDebug>

VerticalAxis::VerticalAxis(QWidget *parent) : QWidget(parent)
{
    this->setMinimumWidth(30);

    setAutoFillBackground(true);
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, Qt::black);
    setPalette(palette);
}

void VerticalAxis::resizeEvent(QResizeEvent *event)
{
    max_steps = width() / 50;
}

void VerticalAxis::dataPointMapToScreenY(double &yValue, double yAxisBoundMin, double yAxisBoundMax)
{
    yValue = Helpers::LinearInterpolation(yAxisBoundMin,
                                          lineWidth,
                                          yAxisBoundMax,
                                          height() - lineWidth,
                                          yValue);
}

void VerticalAxis::paintEvent(QPaintEvent *)
{
    QPen pen;
    QPainter painter(this);

    pen.setCosmetic(true);
    pen.setWidth(lineWidth);
    pen.setColor(QColor(255,255,255));

    painter.setRenderHint(QPainter::Antialiasing, antiAliasing);
    painter.setPen(pen);

    // Top line
    painter.drawLine(QPoint(width(), 0), QPoint(width(), height()));

    double step_unaligned = axisRange / max_steps;
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
    else
    {
        round_to = 10000;
    }

    double step_aligned = Helpers::round_up_to_nearest_d(step_unaligned, round_to);
    if (step_aligned == 0.0)
    {
        qDebug() << "step_u = 0\n";
        step_aligned = step_unaligned;
    }
    double start_value = Helpers::round_down_to_nearest_d(axisBoundMin, round_to);

    qDebug() << "step_u:" << step_unaligned << "\tstep_a: " << step_aligned << "\tstart: " << start_value;
/*
    for (double current_value = start_value - step_aligned; current_value < (axisBoundMax); current_value += step_aligned)
    {
        double xValue = current_value;
        dataPointMapToScreenX(xValue, axisBoundMin, axisBoundMax);
        painter.drawLine(QPoint(xValue, 0), QPoint(xValue, height() * 0.4));
        painter.drawText(QPoint(xValue, height()), QString::number(current_value).append(" ").append(units));
    }*/
}

void VerticalAxis::setAxisBounds(double min, double max)
{
    if (min > max)
        return;
    axisBoundMin = min;
    axisBoundMax = max;
    axisRange = max - min;
    repaint();
}
