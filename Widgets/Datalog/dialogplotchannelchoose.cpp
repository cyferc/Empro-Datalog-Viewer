#include "dialogplotchannelchoose.h"
#include "ui_dialogplotchannelchoose.h"
#include <QGridLayout>
#include <QCheckBox>
#include <QDebug>
#include "plotdatalog.h"

DialogPlotChannelChoose::DialogPlotChannelChoose(std::vector<PointList *> vecOfPointLists, QSplitter *pSplitterPlots, QWidget *pParent) :
    QDialog(pParent),
    m_pUi(new Ui::DialogPlotChannelChoose),
    m_pSplitterPlots(pSplitterPlots),
    m_VecOfPointLists(vecOfPointLists)
{
    m_pUi->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // Add stacked widget for tables
    m_pStackedTablesWidget = new QStackedWidget();
    QGridLayout *stackedLayout = new QGridLayout(m_pUi->widgetTableContainer);
    stackedLayout->setSpacing(0);
    stackedLayout->setContentsMargins(0, 0, 0, 0);
    stackedLayout->addWidget(m_pStackedTablesWidget);

    connect(m_pUi->comboBoxPlotNumber,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
            m_pStackedTablesWidget,
            &QStackedWidget::setCurrentIndex);

    // Create table for each plot
    for (int plotIndex = 0; plotIndex < pSplitterPlots->count(); plotIndex++)
    {
        PlotDatalog* plot = qobject_cast<PlotDatalog *>(pSplitterPlots->children().at(plotIndex));

        // Add plot number to drop down list
        m_pUi->comboBoxPlotNumber->addItem(QString::number(plotIndex + 1));

        // Create table for each plot
        QTableWidget *table = new QTableWidget(vecOfPointLists.size(), 3, this);
        table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        table->setSelectionMode(QAbstractItemView::NoSelection);
        table->setAlternatingRowColors(true);
        table->verticalHeader()->hide();
        table->verticalHeader()->setDefaultSectionSize(17);

        table->setHorizontalHeaderItem(cColumnChannel, new QTableWidgetItem("Channel"));
        table->setColumnWidth(cColumnChannel, 228);

        table->setHorizontalHeaderItem(cColumnPlot, new QTableWidgetItem("Plot"));
        table->setColumnWidth(cColumnPlot, 40);

        table->setHorizontalHeaderItem(cColumnYAxis, new QTableWidgetItem("Y Axis"));
        table->setColumnWidth(cColumnYAxis, 40);

        // Add channels to table
        int vecOfPointListsSize = static_cast<int>(vecOfPointLists.size());
        for (int channel = 0; channel < vecOfPointListsSize; channel++)
        {
            // Column: Channel
            PointList *pList = vecOfPointLists[channel];

            QTableWidgetItem *itemName = new QTableWidgetItem(pList->getName());
            itemName->setFlags(itemName->flags() ^ Qt::ItemIsEditable);

            table->setItem(channel, cColumnChannel, itemName);

            // Column: Plot
            QWidget   *chBoxContainerWidget = new QWidget();
            QCheckBox *chBoxDraw = new QCheckBox(chBoxContainerWidget);

            chBoxDraw->setChecked(plot->vecChannelsDraw[channel]);

            QObject::connect(chBoxDraw,
                             &QCheckBox::stateChanged,
                             this,
                             &DialogPlotChannelChoose::checkBoxPlotStateChanged);

            // Centre the checkbox
            QHBoxLayout *pLayout = new QHBoxLayout(chBoxContainerWidget);
            pLayout->addWidget(chBoxDraw);
            pLayout->setAlignment(Qt::AlignCenter);
            pLayout->setContentsMargins(0, 0, 0, 0);

            table->setCellWidget(channel, cColumnPlot, chBoxContainerWidget);

            // Column: Y Axis
            QWidget   *chBoxContainerWidgetYAxis = new QWidget();
            QCheckBox *chBoxYAxis = new QCheckBox(chBoxContainerWidgetYAxis);

            if (chBoxDraw->isChecked())
            {
                chBoxYAxis->setChecked(plot->vecChannelsYAxis[channel]);
            }
            else
            {
                chBoxYAxis->setEnabled(false);
            }

            QObject::connect(chBoxYAxis,
                             &QCheckBox::stateChanged,
                             this,
                             &DialogPlotChannelChoose::checkBoxYAxisStateChanged);

            // Centre the checkbox
            QHBoxLayout *pLayoutYAxis = new QHBoxLayout(chBoxContainerWidgetYAxis);
            pLayoutYAxis->addWidget(chBoxYAxis);
            pLayoutYAxis->setAlignment(Qt::AlignCenter);
            pLayoutYAxis->setContentsMargins(0, 0, 0, 0);

            table->setCellWidget(channel, cColumnYAxis, chBoxContainerWidgetYAxis);
        }

        table->setFocus();
        table->selectRow(0);

        // Add widget to bottom of stack
        m_pStackedTablesWidget->insertWidget(0, table);
        m_pStackedTablesWidget->setCurrentIndex(0);
    }
}

DialogPlotChannelChoose::~DialogPlotChannelChoose()
{
    delete m_pUi;
}

///
/// \brief DialogPlotChannelChoose::getCurrentTable
/// \param index
/// \return Returns table at index. If index is not provided, current table is returned.
///
QTableWidget *DialogPlotChannelChoose::getCurrentTable(int index)
{
    QWidget *table;
    if (index >= 0)
    {
        table = m_pStackedTablesWidget->widget(index);
    }
    else
    {
        table = m_pStackedTablesWidget->widget(m_pUi->comboBoxPlotNumber->currentIndex());
    }

    return qobject_cast<QTableWidget*>(table);
}

void DialogPlotChannelChoose::checkBoxPlotStateChanged(int state)
{
    int senderRow = -1;
    QCheckBox* senderCheckBox = qobject_cast<QCheckBox*>(sender());

    // Get the sender row in the table
    for (int i = 0; i < getCurrentTable()->rowCount(); i++)
    {
        if (getCurrentTable()->cellWidget(i, cColumnPlot)->children().at(0) == senderCheckBox)
        {
            senderRow = i;
            break;
        }
    }

    if (senderRow < 0)
    {
        qDebug() << "sender row not found";
        return;
    }

    QCheckBox *chBoxYAxis = qobject_cast<QCheckBox *>(getCurrentTable()->cellWidget(senderRow, cColumnYAxis)->children().at(0));

    if (state == false)
    {
        chBoxYAxis->setChecked(false);
    }
    chBoxYAxis->setEnabled(state);
}

void DialogPlotChannelChoose::checkBoxYAxisStateChanged(int /*state*/)
{
    int senderRow = -1;
    QCheckBox* senderCheckBox = qobject_cast<QCheckBox*>(sender());

    // Get the sender row in the table
    for (int i = 0; i < getCurrentTable()->rowCount(); i++)
    {
        if (getCurrentTable()->cellWidget(i, cColumnPlot)->children().at(0) == senderCheckBox)
        {
            senderRow = i;
            break;
        }
    }

    if (senderRow < 0)
    {
        qDebug() << "sender row not found";
        return;
    }
}
/*
QVector<QVector<bool>> DialogPlotChannelChoose::getSelectedPlotIndices()
{
    // Create / resize 2d vector
    QVector<QVector<bool>> isSelectedVector2d(_stackedTables->count());
    for(int outer = 0; outer < isSelectedVector2d.size(); outer++)
    {
        isSelectedVector2d[outer].resize(getCurrentTable()->rowCount());
    }

    for (int tableIndex = 0; tableIndex < _stackedTables->count(); tableIndex++)
    {
        for (int channel = 0; channel < getCurrentTable(tableIndex)->rowCount(); channel++)
        {
            QCheckBox *chBox = qobject_cast<QCheckBox *>(getCurrentTable(tableIndex)->cellWidget(channel, ColumnPlot)->children().at(0));

            if (chBox->isChecked())
            {
                isSelectedVector2d[tableIndex][channel] = true;
            }
            else
            {
                isSelectedVector2d[tableIndex][channel] = false;
            }
        }
    }

    return isSelectedVector2d;
}

QList<int> DialogPlotChannelChoose::getSelectedYAxisIndices()
{
    QList<int> selectedIndices;

    for (int row = 0; row < getCurrentTable()->rowCount(); row++)
    {
        QCheckBox *chBox = qobject_cast<QCheckBox *>(getCurrentTable()->cellWidget(row, ColumnYAxis)->children().at(0));

        if (chBox->isChecked())
        {
            //qDebug() << row;
            selectedIndices.append(row);
        }
    }

    return selectedIndices;
}
*/
void DialogPlotChannelChoose::on_btnSelectAll_clicked()
{
    for (int row = 0; row < getCurrentTable()->rowCount(); row++)
    {
        QCheckBox *chBox = qobject_cast<QCheckBox *>(getCurrentTable()->cellWidget(row, cColumnPlot)->children().at(0));
        chBox->setChecked(true);
    }
}

void DialogPlotChannelChoose::on_btnDeselectAll_clicked()
{
    for (int row = 0; row < getCurrentTable()->rowCount(); row++)
    {
        QCheckBox *chBox = qobject_cast<QCheckBox *>(getCurrentTable()->cellWidget(row, cColumnPlot)->children().at(0));
        chBox->setChecked(false);
    }
}

void DialogPlotChannelChoose::on_buttonBox_accepted()
{
    for (int plotIndex = 0; plotIndex < m_pSplitterPlots->count(); plotIndex++)
    {
        PlotDatalog* plot = qobject_cast<PlotDatalog*>(m_pSplitterPlots->widget(plotIndex));

        // Clear all points from all plots
        plot->clearPointLists();

        for (int channel = 0; channel < getCurrentTable(plotIndex)->rowCount(); channel++)
        {
            QCheckBox *chBoxDrawChannel = qobject_cast<QCheckBox *>(getCurrentTable(plotIndex)->cellWidget(channel, cColumnPlot)->children().at(0));
            QCheckBox *chBoxYAxis = qobject_cast<QCheckBox *>(getCurrentTable(plotIndex)->cellWidget(channel, cColumnYAxis)->children().at(0));

            if (chBoxDrawChannel->isChecked())
            {
                plot->addPointList(m_VecOfPointLists.at(channel));
            }

            plot->vecChannelsDraw[channel] = chBoxDrawChannel->isChecked();
            plot->vecChannelsYAxis[channel] = chBoxYAxis->isChecked();
        }
    }
}
