#include "widgetdatalogviewcontrol.h"
#include "helpers.h"
#include "dialogplotchannelchoose.h"
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QGridLayout>
#include <QToolBar>
#include <QDebug>
#include <QHeaderView>
#include <QCheckBox>

WidgetDatalogViewControl::WidgetDatalogViewControl(QWidget *parent,
                                                   QDockWidget *dockWidgetChannelList,
                                                   QDockWidget *dockWidgetSelection) :
    QWidget(parent)
{
    tableChannelList = new QTableWidget(this);
    tableChannelList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    tableChannelList->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    tableChannelList->setSelectionMode(QAbstractItemView::NoSelection);
    tableChannelList->setAlternatingRowColors(true);
    tableChannelList->verticalHeader()->hide();
    tableChannelList->verticalHeader()->setDefaultSectionSize(15);
    tableChannelList->setColumnCount(4);
    tableChannelList->hide();

    tableChannelList->setHorizontalHeaderItem(0, new QTableWidgetItem("Channel"));
    tableChannelList->setColumnWidth(0, 100);
    tableChannelList->setHorizontalHeaderItem(1, new QTableWidgetItem("Min"));
    tableChannelList->setColumnWidth(1, 50);
    tableChannelList->setHorizontalHeaderItem(2, new QTableWidgetItem("Value"));
    tableChannelList->setColumnWidth(2, 50);
    tableChannelList->setHorizontalHeaderItem(3, new QTableWidgetItem("Max"));
    tableChannelList->setColumnWidth(3, 50);

    dockWidgetChannelList->setWidget(tableChannelList);

    splitterPlots = new QSplitter;
    splitterPlots->setOrientation(Qt::Vertical);
    splitterPlots->setHandleWidth(3);
    splitterPlots->setStyleSheet("QSplitter::handle { background-color: white };");

    QGridLayout *contentLayout = new QGridLayout(this);
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    contentLayout->addWidget(splitterPlots);

    //////// Right Click Menu ////////
    plotContextMenu = new QMenu(this);
    //plotContextMenu->addAction(plotContextMenuStrChooseChannels);

    addPlot();
}

void WidgetDatalogViewControl::addPlot()
{
    PlotDatalog *plot = new PlotDatalog;
    plot->setAntiAliasing(_antiAliasing);
    plot->setDrawCurrentValueMarkers(_showCurrentValueMarkers);
    plot->setDrawPoints(_showPoints);

    // Set vector sizes
    plot->vecChannelsDraw.clear();
    plot->vecChannelsDraw.resize(listOfPointLists.count());
    plot->vecChannelsYAxis.clear();
    plot->vecChannelsYAxis.resize(listOfPointLists.count());

    splitterPlots->addWidget(plot);

    QObject::connect(this,
                     SIGNAL(signalShowPoints(bool)),
                     plot,
                     SLOT(setDrawPoints(bool)));

    QObject::connect(this,
                     SIGNAL(signalShowCurrentValueMarkers(bool)),
                     plot,
                     SLOT(setDrawCurrentValueMarkers(bool)));

    QObject::connect(this,
                     SIGNAL(signalSetAntialiasing(bool)),
                     plot,
                     SLOT(setAntiAliasing(bool)));

    QObject::connect(plot,
                     SIGNAL(mousePressEventSignal(QMouseEvent*)),
                     this,
                     SLOT(plotMousePressEvent(QMouseEvent*)));

    QObject::connect(plot,
                     SIGNAL(mouseMoveEventSignal(QMouseEvent*)),
                     this,
                     SLOT(plotMouseMoveEvent(QMouseEvent*)));

    QObject::connect(plot,
                     SIGNAL(wheelEventSignal(QWheelEvent*)),
                     this,
                     SLOT(plotWheelEvent(QWheelEvent*)));

    QObject::connect(plot,
                     SIGNAL(resizeEventSignal(QResizeEvent*)),
                     this,
                     SLOT(plotResizeEvent(QResizeEvent*)));

    QObject::connect(this,
                     SIGNAL(repaintPlots()),
                     plot,
                     SLOT(repaint()));
}

void WidgetDatalogViewControl::evenPlotSpacing()
{
    int vertical_size = splitterPlots->height() / splitterPlots->count();

    QList<int> sizeList;
    for (int index = 0; index < splitterPlots->count(); index++)
    {
        sizeList.append(vertical_size);
    }

    splitterPlots->setSizes(sizeList);
}

void WidgetDatalogViewControl::plotResizeEvent(QResizeEvent*)
{
    emit repaintPlots();
}

void WidgetDatalogViewControl::plotMouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        float dx = (event->x() - lastMousePos.x()) * (originalXAxisBoundMax - originalXAxisBoundMin);
        dx = dx / splitterPlots->width();
        xAxisBoundMin = originalXAxisBoundMin - dx;
        xAxisBoundMax = originalXAxisBoundMax - dx;

        emit setXAxisBounds(xAxisBoundMin, xAxisBoundMax);
    }
    emit repaintPlots();
}

void WidgetDatalogViewControl::plotWheelEvent(QWheelEvent *event)
{
    double amount = (event->angleDelta().y() / 180.0);
    zoom(amount);
}

void WidgetDatalogViewControl::zoomIn()
{
    zoom(-0.666667);
}

void WidgetDatalogViewControl::zoomOut()
{
    zoom(0.666667);
}

void WidgetDatalogViewControl::zoom(double amount)
{
    double temp_xAxisBoundMin = xAxisBoundMin - amount;
    double temp_xAxisBoundMax = xAxisBoundMax + amount;

    if (temp_xAxisBoundMin > temp_xAxisBoundMax)
    {
        return;
    }

    xAxisBoundMin = temp_xAxisBoundMin;
    xAxisBoundMax = temp_xAxisBoundMax;

    // Send to all plots
    emit setXAxisBounds(xAxisBoundMin, xAxisBoundMax);
    emit repaintPlots();
}

void WidgetDatalogViewControl::plotMousePressEvent(QMouseEvent *event)
{
    //PlotDatalog* senderPlot = qobject_cast<PlotDatalog*>(sender());

    //if (event->button() == Qt::RightButton)
    //{
        //QAction* selectedItem = plotContextMenu->exec(senderPlot->mapToGlobal(event->pos()));

        // If something was pressed
        //if (selectedItem)
        //{
        //    if (selectedItem->text().compare(plotContextMenuStrChooseChannels) == 0)
        //    {

        //    }
        //}
    //}
    //else
    if (event->button() == Qt::LeftButton)
    {
        lastMousePos = event->pos();
        originalXAxisBoundMin = xAxisBoundMin;
        originalXAxisBoundMax = xAxisBoundMax;
    }
}

void WidgetDatalogViewControl::setupClicked()
{
    DialogPlotChannelChoose dialog(listOfPointLists, splitterPlots, this);
    dialog.exec();
}

void WidgetDatalogViewControl::deleteBottomPlot()
{
    if (splitterPlots->count() > 1)
    {
        splitterPlots->widget(splitterPlots->count() - 1)->deleteLater();
    }
}

void WidgetDatalogViewControl::addPlointListsToChannelList()
{
    tableChannelList->clearContents();
    tableChannelList->setRowCount(listOfPointLists.count());

    // Add channels to table
    for (int channel = 0; channel < listOfPointLists.count(); channel++)
    {
        PointList *pList = listOfPointLists.at(channel);

        QTableWidgetItem *itemName = new QTableWidgetItem(pList->getName());
        itemName->setFlags(itemName->flags() ^ Qt::ItemIsEditable);

        tableChannelList->setItem(channel, 0, itemName);

        QTableWidgetItem *itemMin = new QTableWidgetItem(QString::number(pList->getMinY()));
        itemMin->setFlags(itemMin->flags() ^ Qt::ItemIsEditable);

        tableChannelList->setItem(channel, 1, itemMin);

        QTableWidgetItem *itemMax = new QTableWidgetItem(QString::number(pList->getMaxY()));
        itemMax->setFlags(itemMax->flags() ^ Qt::ItemIsEditable);

        tableChannelList->setItem(channel, 3, itemMax);
    }
}

void WidgetDatalogViewControl::resetPlotVectorSizes()
{
    for (int plotIndex = 0; plotIndex < splitterPlots->count(); plotIndex++)
    {
        PlotDatalog* plot = qobject_cast<PlotDatalog *>(splitterPlots->children().at(plotIndex));
        plot->vecChannelsDraw.clear();
        plot->vecChannelsDraw.resize(listOfPointLists.count());

        plot->vecChannelsYAxis.clear();
        plot->vecChannelsYAxis.resize(listOfPointLists.count());
    }
}

void WidgetDatalogViewControl::openDatalogClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Log File"),
                                                    "",
                                                    tr("Files (*.csv);;Files (*.msl)"));

    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, "Error", "File is unreadable!", QMessageBox::Ok);
            return;
        }

        if (fileName.endsWith(".csv", Qt::CaseInsensitive))
        {
            fileType = FileType_CSV;
        }
        else if (fileName.endsWith(".msl", Qt::CaseInsensitive))
        {
            fileType = FileType_Megasquirt_MSL;
        }
        else
        {
            // error
            QMessageBox::warning(this, "Error", "File type is not supported!", QMessageBox::Ok);
            return;
        }

        deleteAllIn_ListOfPointLists();

        int lineNum = 1;
        while (!file.atEnd())
        {
            QByteArray line = file.readLine();
            line = line.trimmed(); // Remove start and end white spaces

            if (line.startsWith('#') || line.startsWith('"'))
            {
                continue;
            }

            if (lineNum == 1)
            {
                // First Line
                if (!processDatalogFirstLine(line))
                {
                    deleteAllIn_ListOfPointLists();
                    return;
                }
            }
            else if (lineNum == 2)
            {

            }
            else
            {
                // Rest of Lines
                if (!processDatalogLine(line, lineNum))
                {
                    deleteAllIn_ListOfPointLists();
                    return;
                }
            }

            lineNum++;
        }

        // Set initial axis bounds
        for (int i = 0; i < listOfPointLists.count(); i++)
        {
            if (i == 0)
            {
                /// Initial X axis bounds
                xAxisBoundMin = listOfPointLists.at(0)->getMinX();

                if (initialXAxisBoundMax < listOfPointLists.at(0)->getMaxX())
                {
                    xAxisBoundMax = xAxisBoundMin + initialXAxisBoundMax;
                }
                else
                {
                    xAxisBoundMax = listOfPointLists.at(0)->getMaxX();
                }

                emit setXAxisBounds(xAxisBoundMin, xAxisBoundMax);
            }

            /// Initial Y axis bounds
            listOfPointLists.at(i)->setAxisBoundsY(listOfPointLists.at(i)->getMinY(),
                                                   listOfPointLists.at(i)->getMaxY());
        }

        addPlointListsToChannelList();
        resetPlotVectorSizes();
    }
}

void WidgetDatalogViewControl::deleteAllIn_ListOfPointLists()
{
    // Delete dynamic allocated memory in container
    qDeleteAll(listOfPointLists);
    listOfPointLists.clear();

    for (int i = 0; i < splitterPlots->count(); i++)
    {
        PlotDatalog* plot = qobject_cast<PlotDatalog *>(splitterPlots->children().at(i));
        plot->clearPointLists();
    }

    // This will clear the settingTable since theres nothing in listOfPointLists
    addPlointListsToChannelList();
}

bool WidgetDatalogViewControl::processDatalogLine(QByteArray line, int lineNumber)
{
    bool conversionOK;
    int currentCol = 0;
    QList<QByteArray> array;

    if (fileType == FileType_CSV)
    {
        array = line.split(',');
    }
    else if (fileType == FileType_Megasquirt_MSL)
    {
        array = line.split('\t');
    }

    double timeValue = array[timeColumn].toDouble(&conversionOK);

    if (!conversionOK)
    {
        QString errorText("Error converting time value to double, line ");
        errorText.append(QString::number(lineNumber)).append(", column ").append(QString::number(timeColumn + 1)).append(".");

        qDebug() << errorText;
        QMessageBox::warning(this, "Error", errorText, QMessageBox::Ok);
        return conversionOK;
    }

    for (int col = 0; col < array.length(); col++)
    {
        if (col == timeColumn)
        {
            continue;
        }

        PointList *pList = listOfPointLists.value(currentCol);

        double value = array[col].toDouble(&conversionOK);

        if (conversionOK)
        {
            pList->append(QPointF(timeValue, value));
        }
        else
        {
            QString errorText("Error converting value to double, line ");
            errorText.append(QString::number(lineNumber)).append(", column ").append(QString::number(col + 1)).append(".").append(" Adding 0 instead.");

            qDebug() << errorText;
            QMessageBox::warning(this, "Error", errorText, QMessageBox::Ok);
            pList->append(QPointF(timeValue, 0));
        }

        currentCol++;
    }
    return true;
}

/**
 * @brief PlotDatalog::processDatalogFirstLine
 * @param line
 * @return true if time column was found
 *         false if time column was not found
 */
bool WidgetDatalogViewControl::processDatalogFirstLine(QByteArray line)
{
    QStringList columnNames;
    bool timeColumnFound = false;

    QList<QByteArray> array;

    if (fileType == FileType_CSV)
    {
        array = line.split(',');
    }
    else if (fileType == FileType_Megasquirt_MSL)
    {
        array = line.split('\t');
    }

    // add column names to items list for selection dialog
    for (int col = 0; col < array.length(); col++)
    {
        array[col] = array[col].trimmed(); // trim whitespaces from start and end
        columnNames << tr(array[col]);
    }

    // show dialog
    bool ok;
    timeColumnName = QInputDialog::getItem(this, tr("Time Axis Selection"),
                                           tr("Select time column:"), columnNames, 0, false, &ok);

    // if cancel pressed
    if (!ok)
    {
        return false;
    }

    // if something else went wrong
    if (timeColumnName.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Time column was not found in datalog file", QMessageBox::Ok);
        return false;
    }

    for (int col = 0; col < array.length(); col++)
    {
        // get time column number
        if (array[col] == timeColumnName)
        {
            timeColumn      = col;
            timeColumnFound = true;
            continue;
        }

        PointList *plist = new PointList;

        // We only change the x axis bounds
        QObject::connect(this,
                         SIGNAL(setXAxisBounds(double, double)),
                         plist,
                         SLOT(setAxisBoundsX(double,double)));

        plist->setName(array[col]);
        plist->setColor(QColor(Helpers::randInt(lineColorBetweenA, lineColorBetweenB),
                               Helpers::randInt(lineColorBetweenA, lineColorBetweenB),
                               Helpers::randInt(lineColorBetweenA, lineColorBetweenB)));

        listOfPointLists.append(plist);
    }

    // Check if time column was found
    if (timeColumnFound)
    {
        return true;
    }

    QMessageBox::warning(this, "Error", "Time column was not found in datalog file", QMessageBox::Ok);
    return false;
}

void WidgetDatalogViewControl::showPoints(bool show)
{
    _showPoints = show;
    emit this->signalShowPoints(show);
}

void WidgetDatalogViewControl::showCurrentValueMarkers(bool show)
{
    _showCurrentValueMarkers = show;
    emit this->signalShowCurrentValueMarkers(show);
}

void WidgetDatalogViewControl::setAntialiasing(bool show)
{
    _antiAliasing = show;
    emit this->signalSetAntialiasing(show);
}
