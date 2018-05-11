#include "widgetdatalogviewcontrol.h"
#include "helpers.h"
#include "dialogplotchannelchoose.h"
#include "verticalaxis.h"
#include "mainwindow.h"
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QGridLayout>
#include <QToolBar>
#include <QDebug>
#include <QHeaderView>
#include <QCheckBox>


WidgetDatalogViewControl::WidgetDatalogViewControl(QWidget *pParent,
                                                   MainWindow* pMainWindow,
                                                   QDockWidget *pDockWidgetChannelList,
                                                   QDockWidget */*pDockWidgetSelection*/) :
    QWidget(pParent),
    m_pMainWindow(pMainWindow)
{
    m_pTableChannelList = new QTableWidget(this);
    m_pTableChannelList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_pTableChannelList->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_pTableChannelList->setSelectionMode(QAbstractItemView::NoSelection);
    m_pTableChannelList->setAlternatingRowColors(true);
    m_pTableChannelList->verticalHeader()->hide();
    m_pTableChannelList->verticalHeader()->setDefaultSectionSize(15);
    m_pTableChannelList->setColumnCount(4);
    m_pTableChannelList->hide();

    m_pTableChannelList->setHorizontalHeaderItem(0, new QTableWidgetItem("Channel"));
    m_pTableChannelList->setColumnWidth(0, 100);
    m_pTableChannelList->setHorizontalHeaderItem(1, new QTableWidgetItem("Min"));
    m_pTableChannelList->setColumnWidth(1, 50);
    m_pTableChannelList->setHorizontalHeaderItem(2, new QTableWidgetItem("Value"));
    m_pTableChannelList->setColumnWidth(2, 50);
    m_pTableChannelList->setHorizontalHeaderItem(3, new QTableWidgetItem("Max"));
    m_pTableChannelList->setColumnWidth(3, 50);

    pDockWidgetChannelList->setWidget(m_pTableChannelList);

    m_pSplitterPlots = new QSplitter;
    m_pSplitterPlots->setOrientation(Qt::Vertical);
    m_pSplitterPlots->setHandleWidth(3);
    m_pSplitterPlots->setStyleSheet("QSplitter::handle { background-color: white };");

    m_pHorizontalAxis = new HorizontalAxis();
    QObject::connect(this,
                     &WidgetDatalogViewControl::setXAxisBounds,
                     m_pHorizontalAxis,
                     &HorizontalAxis::setAxisBounds);

    QGridLayout *contentLayout = new QGridLayout(this);
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    contentLayout->addWidget(m_pSplitterPlots);
    contentLayout->addWidget(m_pHorizontalAxis);

    //////// Right Click Menu ////////
    m_pPlotContextMenu = new QMenu(this);
    //plotContextMenu->addAction(plotContextMenuStrChooseChannels);

    addPlot();
}

void WidgetDatalogViewControl::addPlot()
{
    PlotDatalog *plot = new PlotDatalog;
    plot->setAntiAliasing(m_AntiAliasing);
    plot->setDrawCurrentValueMarkers(m_ShowCurrentValueMarkers);
    plot->setDrawPoints(m_ShowPoints);

    // Set vector sizes
    plot->vecChannelsDraw.clear();
    plot->vecChannelsDraw.resize(m_VecOfPointLists.size());
    plot->vecChannelsYAxis.clear();
    plot->vecChannelsYAxis.resize(m_VecOfPointLists.size());


    m_pSplitterPlots->addWidget(plot);

    QObject::connect(this,
                     &WidgetDatalogViewControl::signalShowPoints,
                     plot,
                     &PlotDatalog::setDrawPoints);

    QObject::connect(this,
                     &WidgetDatalogViewControl::signalShowCurrentValueMarkers,
                     plot,
                     &PlotDatalog::setDrawCurrentValueMarkers);

    QObject::connect(this,
                     &WidgetDatalogViewControl::signalSetAntialiasing,
                     plot,
                     &PlotDatalog::setAntiAliasing);

    QObject::connect(plot,
                     &PlotDatalog::mousePressEventSignal,
                     this,
                     &WidgetDatalogViewControl::plotMousePressEvent);

    QObject::connect(plot,
                     &PlotDatalog::mouseMoveEventSignal,
                     this,
                     &WidgetDatalogViewControl::plotMouseMoveEvent);

    QObject::connect(plot,
                     &PlotDatalog::wheelEventSignal,
                     this,
                     &WidgetDatalogViewControl::plotWheelEvent);

    QObject::connect(plot,
                     &PlotDatalog::resizeEventSignal,
                     this,
                     &WidgetDatalogViewControl::plotResizeEvent);

    QObject::connect(this,
                     &WidgetDatalogViewControl::repaintPlots,
                     plot,
                     static_cast<void (PlotDatalog::*)()>(&PlotDatalog::repaint));
}

void WidgetDatalogViewControl::evenPlotSpacing()
{
    int vertical_size = m_pSplitterPlots->height() / m_pSplitterPlots->count();

    QList<int> sizeList;
    for (int index = 0; index < m_pSplitterPlots->count(); index++)
    {
        sizeList.append(vertical_size);
    }

    m_pSplitterPlots->setSizes(sizeList);
}

void WidgetDatalogViewControl::plotResizeEvent(QResizeEvent* /*pEvent*/)
{
    emit repaintPlots();
}

void WidgetDatalogViewControl::plotMouseMoveEvent(QMouseEvent *pEvent)
{
    if (pEvent->buttons() & Qt::LeftButton)
    {
        float dx = (pEvent->x() - m_LastMousePos.x()) * (m_OriginalXAxisBoundMax - m_OriginalXAxisBoundMin);
        dx = dx / m_pSplitterPlots->width();
        m_XAxisBoundMin = m_OriginalXAxisBoundMin - dx;
        m_XAxisBoundMax = m_OriginalXAxisBoundMax - dx;

        emit setXAxisBounds(m_XAxisBoundMin, m_XAxisBoundMax);
    }
    emit repaintPlots();
}

void WidgetDatalogViewControl::plotWheelEvent(QWheelEvent *pEvent)
{
    double amount = (pEvent->angleDelta().y() / 180.0);
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
    double temp_xAxisBoundMin = m_XAxisBoundMin - amount;
    double temp_xAxisBoundMax = m_XAxisBoundMax + amount;

    if (temp_xAxisBoundMin > temp_xAxisBoundMax)
    {
        return;
    }

    m_XAxisBoundMin = temp_xAxisBoundMin;
    m_XAxisBoundMax = temp_xAxisBoundMax;

    // Send to all plots
    emit setXAxisBounds(m_XAxisBoundMin, m_XAxisBoundMax);
    emit repaintPlots();
}

void WidgetDatalogViewControl::plotMousePressEvent(QMouseEvent *pEvent)
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
    if (pEvent->button() == Qt::LeftButton)
    {
        m_LastMousePos = pEvent->pos();
        m_OriginalXAxisBoundMin = m_XAxisBoundMin;
        m_OriginalXAxisBoundMax = m_XAxisBoundMax;
    }
}

void WidgetDatalogViewControl::setupClicked()
{
    DialogPlotChannelChoose dialog(m_VecOfPointLists, m_pSplitterPlots, this);
    dialog.exec();
}

void WidgetDatalogViewControl::deleteBottomPlot()
{
    if (m_pSplitterPlots->count() > 1)
    {
        m_pSplitterPlots->widget(m_pSplitterPlots->count() - 1)->deleteLater();
    }
}

void WidgetDatalogViewControl::addPlointListsToChannelList()
{
    m_pTableChannelList->clearContents();
    m_pTableChannelList->setRowCount(m_VecOfPointLists.size());

    // Add channels to table
    int vecOfPointListsSize = static_cast<int>(m_VecOfPointLists.size());
    for (int channel = 0; channel < vecOfPointListsSize; channel++)
    {
        PointList *pList = m_VecOfPointLists[channel];

        QTableWidgetItem *itemName = new QTableWidgetItem(pList->getName());
        itemName->setFlags(itemName->flags() ^ Qt::ItemIsEditable);

        m_pTableChannelList->setItem(channel, 0, itemName);

        QTableWidgetItem *itemMin = new QTableWidgetItem(QString::number(pList->getMinY()));
        itemMin->setFlags(itemMin->flags() ^ Qt::ItemIsEditable);

        m_pTableChannelList->setItem(channel, 1, itemMin);

        QTableWidgetItem *itemMax = new QTableWidgetItem(QString::number(pList->getMaxY()));
        itemMax->setFlags(itemMax->flags() ^ Qt::ItemIsEditable);

        m_pTableChannelList->setItem(channel, 3, itemMax);
    }
}

void WidgetDatalogViewControl::resetPlotVectorSizes()
{
    for (int plotIndex = 0; plotIndex < m_pSplitterPlots->count(); plotIndex++)
    {
        PlotDatalog* plot = qobject_cast<PlotDatalog *>(m_pSplitterPlots->children().at(plotIndex));
        plot->vecChannelsDraw.clear();
        plot->vecChannelsDraw.resize(m_VecOfPointLists.size());

        plot->vecChannelsYAxis.clear();
        plot->vecChannelsYAxis.resize(m_VecOfPointLists.size());
    }
}

void WidgetDatalogViewControl::openDatalogClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Log File"),
                                                    "",
                                                    tr("Files (*.csv);;Files (*.msl)"));

    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "File is unreadable!", QMessageBox::Ok);
        return;
    }

    if (fileName.endsWith(".csv", Qt::CaseInsensitive))
    {
        m_FileType = eFileType::CSV;
    }
    else if (fileName.endsWith(".msl", Qt::CaseInsensitive))
    {
        m_FileType = eFileType::Megasquirt_MSL;
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
    int vecOfPointListsSize = static_cast<int>(m_VecOfPointLists.size());
    for (int i = 0; i < vecOfPointListsSize; i++)
    {
        if (i == 0)
        {
            /// Initial X axis bounds
            m_XAxisBoundMin = m_VecOfPointLists[0]->getMinX();

            if (m_InitialXAxisBoundMax < m_VecOfPointLists[0]->getMaxX())
            {
                m_XAxisBoundMax = m_XAxisBoundMin + m_InitialXAxisBoundMax;
            }
            else
            {
                m_XAxisBoundMax = m_VecOfPointLists[0]->getMaxX();
            }

            emit setXAxisBounds(m_XAxisBoundMin, m_XAxisBoundMax);
        }

        /// Initial Y axis bounds
        m_VecOfPointLists[i]->setAxisBoundsY(m_VecOfPointLists[i]->getMinY(),
                                               m_VecOfPointLists[i]->getMaxY());
    }

    addPlointListsToChannelList();
    resetPlotVectorSizes();

    QString fileNameNoDir;
    for (QString::reverse_iterator i = fileName.rbegin(); i != fileName.rend(); ++i )
    {
        if (*i == '/')
        {
            break;
        }
        fileNameNoDir.prepend(*i);
    }

    m_pMainWindow->SetWindowTitle(fileNameNoDir);
}

void WidgetDatalogViewControl::deleteAllIn_ListOfPointLists()
{
    // Delete dynamic allocated memory in container
    qDeleteAll(m_VecOfPointLists);
    m_VecOfPointLists.clear();

    for (int i = 0; i < m_pSplitterPlots->count(); ++i)
    {
        PlotDatalog* plot = qobject_cast<PlotDatalog *>(m_pSplitterPlots->children().at(i));
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

    if (m_FileType == eFileType::CSV)
    {
        array = line.split(',');
    }
    else if (m_FileType == eFileType::Megasquirt_MSL)
    {
        array = line.split('\t');
    }

    double timeValue = array[m_TimeColumn].toDouble(&conversionOK);

    if (!conversionOK)
    {
        QString errorText("Error converting time value to double, line ");
        errorText.append(QString::number(lineNumber)).append(", column ").append(QString::number(m_TimeColumn + 1)).append(".");

        qDebug() << errorText;
        QMessageBox::warning(this, "Error", errorText, QMessageBox::Ok);
        return conversionOK;
    }

    for (int col = 0; col < array.length(); col++)
    {
        if (col == m_TimeColumn)
        {
            continue;
        }

        PointList *pList = m_VecOfPointLists[currentCol];

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

    if (m_FileType == eFileType::CSV)
    {
        array = line.split(',');
    }
    else if (m_FileType == eFileType::Megasquirt_MSL)
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
    m_TimeColumnName = QInputDialog::getItem(this, tr("Time Axis Selection"),
                                           tr("Select time column:"), columnNames, 0, false, &ok);

    // if cancel pressed
    if (!ok)
    {
        return false;
    }

    // if something else went wrong
    if (m_TimeColumnName.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Time column was not found in datalog file", QMessageBox::Ok);
        return false;
    }

    for (int col = 0; col < array.length(); col++)
    {
        // get time column number
        if (array[col] == m_TimeColumnName)
        {
            m_TimeColumn      = col;
            timeColumnFound = true;
            continue;
        }

        PointList *plist = new PointList;

        // We only change the x axis bounds
        QObject::connect(this,
                         &WidgetDatalogViewControl::setXAxisBounds,
                         plist,
                         &PointList::setAxisBoundsX);

        plist->setName(array[col]);
        plist->setColor(QColor(Helpers::randInt(cLineColorBetweenA, cLineColorBetweenB),
                               Helpers::randInt(cLineColorBetweenA, cLineColorBetweenB),
                               Helpers::randInt(cLineColorBetweenA, cLineColorBetweenB)));

        m_VecOfPointLists.push_back(plist);
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
    m_ShowPoints = show;
    emit this->signalShowPoints(show);
}

void WidgetDatalogViewControl::showCurrentValueMarkers(bool show)
{
    m_ShowCurrentValueMarkers = show;
    emit this->signalShowCurrentValueMarkers(show);
}

void WidgetDatalogViewControl::setAntialiasing(bool show)
{
    m_AntiAliasing = show;
    emit this->signalSetAntialiasing(show);
}
