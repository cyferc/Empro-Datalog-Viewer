#pragma once

#include <QDialog>
#include <QTableWidget>
#include <QSplitter>
#include <QStackedWidget>

#include "pointlist.h"

namespace Ui {
class DialogPlotChannelChoose;
}

class DialogPlotChannelChoose : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPlotChannelChoose(QList<PointList*> listOfPointLists, QSplitter *splitterPlots, QWidget *parent = 0);
    ~DialogPlotChannelChoose();
    //QVector<QVector <bool>> getSelectedPlotIndices();
    //QList<int> getSelectedYAxisIndices();

private slots:
    void on_btnSelectAll_clicked();
    void on_btnDeselectAll_clicked();
    void checkBoxPlotStateChanged(int state);
    void checkBoxYAxisStateChanged(int state);

    void on_buttonBox_accepted();

private:
    Ui::DialogPlotChannelChoose *ui;
    int selectedIndex;
    QSplitter *_splitterPlots;
    QList<PointList *> _listOfPointLists;
    QStackedWidget *_stackedTables;

    const int ColumnChannel = 0;
    const int ColumnPlot = 1;
    const int ColumnYAxis = 2;

    QTableWidget *getCurrentTable(int index = -1);
};
