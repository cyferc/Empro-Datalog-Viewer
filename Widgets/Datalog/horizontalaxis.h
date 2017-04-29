#ifndef HORIZONTALAXIS_H
#define HORIZONTALAXIS_H

#include <QWidget>

class HorizontalAxis : public QWidget
{
    Q_OBJECT
public:
    explicit HorizontalAxis(QWidget *parent = 0);

signals:

public slots:
    void setAxisBounds(double min, double max);

protected:
    void paintEvent(QPaintEvent *);

private:
    void dataPointMapToScreenX(double &xValue, double xAxisBoundMin, double xAxisBoundMax);
    double round_down_to_nearest(double value_to_round, double round_to);
    double round_up_to_nearest(double value_to_round, double round_to);
    double round_to_nearest(double value_to_round, double round_to);
    int max_steps = 20;
    double axisBoundMin = 0.0;
    double axisBoundMax = 10.0;
    double axisRange = 10.0;
    bool antiAliasing = false;
    int lineWidth = 1;
};

#endif // HORIZONTALAXIS_H
