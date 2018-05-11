#pragma once

#include <QWidget>

class VerticalAxis : public QWidget
{
    Q_OBJECT
public:
    explicit VerticalAxis(QWidget* pParent = 0);

signals:

public slots:
    void setAxisBounds(double min, double max);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent* pEvent);

private:
    void dataPointMapToScreenY(double &yValue, double yAxisBoundMin, double yAxisBoundMax);
    int m_MaxSteps = 2;
    double m_AxisBoundMin = 0.0;
    double m_AxisBoundMax = 10.0;
    double m_AxisRange = 10.0;
    bool m_AntiAliasing = false;
    int m_LineWidth = 1;
    QString m_Units = "s";
};
