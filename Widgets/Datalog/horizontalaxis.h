#pragma once

#include <QWidget>

class HorizontalAxis : public QWidget
{
    Q_OBJECT
public:
    explicit HorizontalAxis(QWidget* pParent = 0);

signals:

public slots:
    void setAxisBounds(double min, double max);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *event);

private:
    void dataPointMapToScreenX(double &xValue, double xAxisBoundMin, double xAxisBoundMax);
    int m_MaxSteps = 2;
    double m_AxisBoundMin = 0.0;
    double m_AxisBoundMax = 10.0;
    double m_AxisRange = 10.0;
    int m_LineWidth = 1;
    int m_MinimumHeight = 12;
    QString m_Units = "s";
};
