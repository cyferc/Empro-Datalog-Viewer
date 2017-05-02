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
    void resizeEvent(QResizeEvent *event);

private:
    void dataPointMapToScreenX(double &xValue, double xAxisBoundMin, double xAxisBoundMax);
    int max_steps = 2;
    double axisBoundMin = 0.0;
    double axisBoundMax = 10.0;
    double axisRange = 10.0;
    int lineWidth = 1;
    int minimumHeight = 12;
    QString units = "s";
};

#endif // HORIZONTALAXIS_H
