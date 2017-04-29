#ifndef VERTICALAXIS_H
#define VERTICALAXIS_H

#include <QWidget>

class VerticalAxis : public QWidget
{
    Q_OBJECT
public:
    explicit VerticalAxis(QWidget *parent = 0);

signals:

public slots:
    void setAxisBounds(double min, double max);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *event);

private:
    void dataPointMapToScreenY(double &yValue, double yAxisBoundMin, double yAxisBoundMax);
    int max_steps = 2;
    double axisBoundMin = 0.0;
    double axisBoundMax = 10.0;
    double axisRange = 10.0;
    bool antiAliasing = false;
    int lineWidth = 1;
    QString units = "s";
};

#endif // VERTICALAXIS_H
