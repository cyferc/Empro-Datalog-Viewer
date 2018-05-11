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
    explicit DialogPlotChannelChoose(std::vector<PointList*> vecOfPointLists, QSplitter* pSplitterPlots, QWidget* pParent = 0);
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
    QTableWidget* getCurrentTable(int index = -1);

    Ui::DialogPlotChannelChoose* m_pUi;
    int m_SelectedIndex;
    QSplitter* m_pSplitterPlots;
    std::vector<PointList*> m_VecOfPointLists;
    QStackedWidget* m_pStackedTablesWidget;

    const int cColumnChannel = 0;
    const int cColumnPlot = 1;
    const int cColumnYAxis = 2;
};
