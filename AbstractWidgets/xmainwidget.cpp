/* Copyright (c) 2024 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "xmainwidget.h"
#include "ui_xmainwidget.h"

XMainWidget::XMainWidget(QWidget *pParent) : XFormatWidget(pParent), ui(new Ui::XMainWidget)
{
    ui->setupUi(this);

    g_bGlobalHexEnable = false;

    XOptions::adjustToolButton(ui->toolButtonReload, XOptions::ICONTYPE_RELOAD);
    XOptions::adjustToolButton(ui->toolButtonNext, XOptions::ICONTYPE_FORWARD, Qt::ToolButtonIconOnly);
    XOptions::adjustToolButton(ui->toolButtonPrev, XOptions::ICONTYPE_BACKWARD, Qt::ToolButtonIconOnly);
    XOptions::adjustToolButton(ui->toolButtonGlobalHex, XOptions::ICONTYPE_HEX);

    ui->toolButtonReload->setToolTip(tr("Reload"));
    ui->toolButtonNext->setToolTip(tr("Next visited"));
    ui->toolButtonPrev->setToolTip(tr("Previous visited"));
    ui->toolButtonGlobalHex->setToolTip(tr("Hex"));
    ui->checkBoxReadonly->setToolTip(tr("Readonly"));

    ui->widgetGlobalHex->setProperty("TYPE", XFW_DEF::TYPE_GLOBALHEX);
    ui->widgetGlobalHex->hide();

    ui->toolButtonGlobalHex->hide();
}

XMainWidget::~XMainWidget()
{
    delete ui;
}

void XMainWidget::setGlobalHexEnable(bool bState)
{
    g_bGlobalHexEnable = bState;

    if (bState) {
        ui->toolButtonGlobalHex->show();
        connect(ui->widgetGlobalHex, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(dataChangedSlot(qint64, qint64)));
        connect(ui->widgetGlobalHex, SIGNAL(currentLocationChanged(quint64, qint32, qint64)), this, SLOT(currentLocationChangedSlot(quint64, qint32, qint64)));
    } else {
        ui->toolButtonGlobalHex->hide();
        disconnect(ui->widgetGlobalHex, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(dataChangedSlot(qint64, qint64)));
        disconnect(ui->widgetGlobalHex, SIGNAL(currentLocationChanged(quint64, qint32, qint64)), this, SLOT(currentLocationChangedSlot(quint64, qint32, qint64)));
    }
}

void XMainWidget::clear()
{
    ui->checkBoxReadonly->setChecked(true);

    qint32 nNumberOfWidgets = ui->stackedWidgetMain->count();

    for (qint32 i = 0; i < nNumberOfWidgets; i++) {
        QWidget *pWidget = ui->stackedWidgetMain->widget(i);

        if (pWidget) {
            delete pWidget;
        }
    }

    ui->treeWidgetNavi->clear();

    XFormatWidget::reset();
}

void XMainWidget::cleanup()
{
    XMainWidget::clear();
}

void XMainWidget::reload()
{
    XMainWidget::clear();

    ui->checkBoxReadonly->setEnabled(getDevice()->isWritable());

    XBinary::FT fileType = getOptions().fileType;

    if (getOptions().fileType == XBinary::FT_UNKNOWN) {
        fileType = XBinary::getPrefFileType(getDevice());
    }

    setFileType(fileType);

    XBinary::_MEMORY_MAP _memoryMap = XFormats::getMemoryMap(fileType, XBinary::MAPMODE_UNKNOWN, getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    setMode(_memoryMap.mode);
    setEndian(_memoryMap.endian);

    XHexView::OPTIONS options = {}; // TODO move toEnable
    options.memoryMapRegion = _memoryMap;
    ui->widgetGlobalHex->setXInfoDB(getXInfoDB());
    ui->widgetGlobalHex->setData(getDevice(), options, true);
    ui->widgetGlobalHex->setBytesProLine(8);

    QTreeWidgetItem *pItem = _addBaseItems(ui->treeWidgetNavi, fileType);
    _addFileType(pItem, getDevice(), 0, getDevice()->size(), fileType, getOptions().bIsImage, getOptions().nImageBase);

    {
        QList<qint32> listSizes = ui->splitterNavi->sizes();

        // qint32 nWidgetSize = listSizes[0] + listSizes[1];
        listSizes[0] = 250;
        listSizes[1] = 250;
        ui->splitterNavi->setSizes(listSizes);
        ui->splitterNavi->setStretchFactor(0, 0);
        ui->splitterNavi->setStretchFactor(1, 1);
    }

    ui->treeWidgetNavi->expandAll();

    ui->treeWidgetNavi->setCurrentIndex(ui->treeWidgetNavi->model()->index(0, 0));

    if (ui->treeWidgetNavi->currentItem()) {
        XMainWidget::reloadData(false);
    }

    // setTreeItem(ui->treeWidgetNavi, getOptions().nStartType);
}

XFormatWidget::SV XMainWidget::_setValue(QVariant vValue, qint32 nPosition)
{
    Q_UNUSED(vValue)
    Q_UNUSED(nPosition)

    return SV_NONE;
}

void XMainWidget::setReadonly(bool bState)
{
    XFormatWidget::setReadonly(bState);

    ui->checkBoxReadonly->setChecked(bState);

    qint32 nNumberOfWidgets = ui->stackedWidgetMain->count();

    for (qint32 i = 0; i < nNumberOfWidgets; i++) {
        XShortcutsWidget *pWidget = dynamic_cast<XShortcutsWidget *>(ui->stackedWidgetMain->widget(i));

        if (pWidget) {
            pWidget->setReadonly(bState);
        }
    }

    ui->widgetGlobalHex->setReadonly(bState);
}

void XMainWidget::adjustView()
{
    getGlobalOptions()->adjustWidget(this, XOptions::ID_VIEW_FONT_CONTROLS);
    getGlobalOptions()->adjustWidget(ui->treeWidgetNavi, XOptions::ID_VIEW_FONT_TREEVIEWS);

    qint32 nNumberOfWidgets = ui->stackedWidgetMain->count();

    for (qint32 i = 0; i < nNumberOfWidgets; i++) {
        XShortcutsWidget *pWidget = dynamic_cast<XShortcutsWidget *>(ui->stackedWidgetMain->widget(i));

        if (pWidget) {
            pWidget->adjustView();
        }
    }

    if (g_bGlobalHexEnable) {
        ui->widgetGlobalHex->adjustView();
    }
}

void XMainWidget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)

    XFW_DEF::CWOPTIONS cwOptions = {};
    cwOptions.pParent = this;
    cwOptions.fileType = getFileType();
    cwOptions._type = (XFW_DEF::TYPE)(ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_TYPE).toInt());
    cwOptions.widgetMode = (XFW_DEF::WIDGETMODE)(ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_WIDGETMODE).toInt());
    cwOptions.pDevice = getDevice();
    cwOptions.bIsImage = getOptions().bIsImage;
    cwOptions.nImageBase = getOptions().nImageBase;
    cwOptions.pXInfoDB = getXInfoDB();
    cwOptions.endian = (XBinary::ENDIAN)(ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_ENDIAN).toLongLong());
    cwOptions.mode = (XBinary::MODE)(ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_MODE).toLongLong());
    cwOptions.nDataOffset = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_OFFSET).toLongLong();
    cwOptions.nDataSize = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_SIZE).toLongLong();
    cwOptions.nDataCount = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_COUNT).toLongLong();
    cwOptions.var1 = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_VAR1);
    cwOptions.var2 = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_VAR2);

    QString sInitString = getInitStringFromCwOptions(&cwOptions);

    if (isInitPresent(sInitString)) {
        qint32 nNumberOfWidgets = ui->stackedWidgetMain->count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            QWidget *pWidget = ui->stackedWidgetMain->widget(i);

            // qDebug("pWidget->property(\"INITSTRING\").toString()=%s", pWidget->property("INITSTRING").toString().toLatin1().data());

            if (pWidget->property("INITSTRING").toString() == sInitString) {
                ui->stackedWidgetMain->setCurrentIndex(i);
                break;
            }
        }
    } else {
        XShortcutsWidget *pWidget = createWidget(cwOptions);

        if (pWidget) {
            pWidget->setGlobal(getShortcuts(), getGlobalOptions());
            pWidget->setReadonly(isReadonly());
            connect(pWidget, SIGNAL(currentLocationChanged(quint64, qint32, qint64)), this, SLOT(currentLocationChangedSlot(quint64, qint32, qint64)));
            connect(pWidget, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(dataChangedSlot(qint64, qint64)));

            if ((cwOptions.widgetMode == XFW_DEF::WIDGETMODE_HEADER) || (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_TABLE)) {
                XFormatWidget *_pXFormatWidget = dynamic_cast<XFormatWidget *>(pWidget);

                if (_pXFormatWidget) {
                    connect(_pXFormatWidget, SIGNAL(showCwWidget(QString, bool)), this, SLOT(showCwWidgetSlot(QString, bool)));
                }
            }

            pWidget->setProperty("INITSTRING", sInitString);
            pWidget->setProperty("TYPE", cwOptions._type);
            pWidget->setProperty("WIDGETMODE", cwOptions.widgetMode);
            pWidget->reloadData(false);
            qint32 nPosition = ui->stackedWidgetMain->addWidget(pWidget);
            ui->stackedWidgetMain->setCurrentIndex(nPosition);
            addInit(sInitString);
        }
    }
}

void XMainWidget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XFormatWidget::setGlobal(pShortcuts, pXOptions);

    ui->widgetGlobalHex->setGlobal(pShortcuts, pXOptions);

    qint32 nNumberOfWidgets = ui->stackedWidgetMain->count();

    for (qint32 i = 0; i < nNumberOfWidgets; i++) {
        XShortcutsWidget *pWidget = dynamic_cast<XShortcutsWidget *>(ui->stackedWidgetMain->widget(i));

        if (pWidget) {
            pWidget->setGlobal(pShortcuts, pXOptions);
        }
    }
}

QTreeWidgetItem *XMainWidget::_addBaseItems(QTreeWidget *pTreeWidget, XBinary::FT fileType)
{
    Q_UNUSED(fileType)

    QTreeWidgetItem *pResult = createNewItem(XFW_DEF::TYPE_INFO, XFW_DEF::WIDGETMODE_UNKNOWN, XOptions::ICONTYPE_INFO, 0, -1, 0, 0, 0,
                                               XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN);

    pTreeWidget->addTopLevelItem(pResult);

    return pResult;
}

XShortcutsWidget *XMainWidget::createWidget(const XFW_DEF::CWOPTIONS &cwOptions)
{
    XShortcutsWidget *pResult = nullptr;

    if ((cwOptions.widgetMode == XFW_DEF::WIDGETMODE_HEADER) || (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_DIALOG_HEADER)) {
        XGenericHeaderWidget *_pWidget = new XGenericHeaderWidget(cwOptions.pParent);
        _pWidget->setCwOptions(cwOptions, false);
        pResult = _pWidget;
    } else if (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_TABLE) {
        XGenericTableWidget *_pWidget = new XGenericTableWidget(cwOptions.pParent);
        _pWidget->setCwOptions(cwOptions, false);
        pResult = _pWidget;
    }

    return pResult;
}

void XMainWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pItemCurrent, QTreeWidgetItem *pItemPrevious)
{
    Q_UNUSED(pItemPrevious)

    if (pItemCurrent) {
        reloadData(false);
        addPage(pItemCurrent);
        ui->toolButtonPrev->setEnabled(isPrevPageAvailable());
        ui->toolButtonNext->setEnabled(isNextPageAvailable());
    }
}

void XMainWidget::on_toolButtonReload_clicked()
{
    ui->toolButtonReload->setEnabled(false);
    reload();

    QTimer::singleShot(1000, this, SLOT(enableButton()));
}

void XMainWidget::enableButton()
{
    ui->toolButtonReload->setEnabled(true);
}

void XMainWidget::on_toolButtonPrev_clicked()
{
    setAddPageEnabled(false);
    ui->treeWidgetNavi->setCurrentItem(getPrevPage());
    setAddPageEnabled(true);
}

void XMainWidget::on_toolButtonNext_clicked()
{
    setAddPageEnabled(false);
    ui->treeWidgetNavi->setCurrentItem(getNextPage());
    setAddPageEnabled(true);
}

void XMainWidget::on_toolButtonGlobalHex_toggled(bool bChecked)
{
    if (bChecked) {
        ui->widgetGlobalHex->show();
        QList<qint32> listSizes = ui->splitterHex->sizes();

        qint32 nWidgetSize = 0;
        nWidgetSize += ui->widgetGlobalHex->getColumnWidth(0);
        nWidgetSize += ui->widgetGlobalHex->getColumnWidth(1);
        nWidgetSize += ui->widgetGlobalHex->getColumnWidth(2);
        nWidgetSize += ui->widgetGlobalHex->getMapWidth();
        nWidgetSize += 20;
        listSizes[0] -= nWidgetSize;
        listSizes[1] += nWidgetSize;
        ui->splitterHex->setSizes(listSizes);
        ui->splitterHex->setStretchFactor(0, 1);
        ui->splitterHex->setStretchFactor(1, 0);
    } else {
        ui->widgetGlobalHex->hide();
    }
}

void XMainWidget::on_checkBoxReadonly_stateChanged(int nArg)
{
    Q_UNUSED(nArg)

    setReadonly(ui->checkBoxReadonly->isChecked());
}

void XMainWidget::dataChangedSlot(qint64 nOffset, qint64 nSize)
{
    Q_UNUSED(nOffset)
    Q_UNUSED(nSize)

    XFW_DEF::TYPE _typeSend = (XFW_DEF::TYPE)(sender()->property("TYPE").toInt());
    XFW_DEF::WIDGETMODE _widgetModeSend = (XFW_DEF::WIDGETMODE)(sender()->property("WIDGETMODE").toInt());
    bool bIsWidgetReload = false;

    if (_typeSend == XFW_DEF::TYPE_GLOBALHEX) {
        bIsWidgetReload = true;
    } else {
        ui->widgetGlobalHex->reloadData(true);
        bIsWidgetReload = (_widgetModeSend == XFW_DEF::WIDGETMODE_DIALOG_HEADER);
    }

    if (bIsWidgetReload) {
        XShortcutsWidget *pWidget = dynamic_cast<XShortcutsWidget *>(ui->stackedWidgetMain->currentWidget());
        if (pWidget) {
            XFW_DEF::WIDGETMODE _widgetModeRecv = (XFW_DEF::WIDGETMODE)(pWidget->property("WIDGETMODE").toInt());

            if (_widgetModeRecv != XFW_DEF::WIDGETMODE_UNKNOWN) {
                pWidget->reloadData(true);
            }
        }
    }
}

void XMainWidget::currentLocationChangedSlot(quint64 nLocation, qint32 nLocationType, qint64 nSize)
{
    XFW_DEF::TYPE _type = (XFW_DEF::TYPE)(sender()->property("TYPE").toInt());
    if (_type != XFW_DEF::TYPE_GLOBALHEX) {
        if (g_bGlobalHexEnable) {
            ui->widgetGlobalHex->currentLocationChangedSlot(nLocation, nLocationType, nSize);
        }
    }
}

void XMainWidget::showCwWidgetSlot(QString sInitString, bool bNewWindow)
{
    XFW_DEF::CWOPTIONS cwOptions = {};
    cwOptions.pParent = this;
    cwOptions.fileType = getFileType();
    cwOptions._type = _getTypeFromInitString(sInitString);
    cwOptions.widgetMode = XFW_DEF::WIDGETMODE_DIALOG_HEADER;
    cwOptions.pDevice = getDevice();
    cwOptions.bIsImage = getOptions().bIsImage;
    cwOptions.nImageBase = getOptions().nImageBase;
    cwOptions.pXInfoDB = getXInfoDB();
    cwOptions.endian = getEndian();
    cwOptions.mode = getMode();
    cwOptions.nDataOffset = _getDataOffsetFromInitString(sInitString);
    cwOptions.nDataSize = _getDataSizeFromInitString(sInitString);
    cwOptions.nDataCount = _getDataCountFromInitString(sInitString);

    XShortcutsWidget *pWidget = createWidget(cwOptions);

    if (pWidget) {
        pWidget->setGlobal(getShortcuts(), getGlobalOptions());
        pWidget->setReadonly(isReadonly());
        connect(pWidget, SIGNAL(currentLocationChanged(quint64, qint32, qint64)), this, SLOT(currentLocationChangedSlot(quint64, qint32, qint64)));
        connect(pWidget, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(dataChangedSlot(qint64, qint64)));

        if ((cwOptions.widgetMode == XFW_DEF::WIDGETMODE_HEADER) || (cwOptions.widgetMode == XFW_DEF::WIDGETMODE_TABLE)) {
            connect(pWidget, SIGNAL(showCwWidget(QString, bool)), this, SLOT(showCwWidgetSlot(QString, bool)));
        }

        pWidget->setProperty("INITSTRING", sInitString);
        pWidget->setProperty("TYPE", cwOptions._type);
        pWidget->setProperty("WIDGETMODE", cwOptions.widgetMode);
        pWidget->reloadData(false);

        DialogWidget dialogWidget(this);
        dialogWidget.setGlobal(getShortcuts(), getGlobalOptions());
        dialogWidget.setModal(true);
        dialogWidget.addWidget(pWidget);

        dialogWidget.exec();
    }
}
