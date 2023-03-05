/* Copyright (c) 2019-2023 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "toolswidget.h"

#include "ui_toolswidget.h"

ToolsWidget::ToolsWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::ToolsWidget)
{
    ui->setupUi(this);

    g_pDevice = nullptr;

    connect(ui->widgetHex, SIGNAL(dataChanged(qint64, qint64)), this, SIGNAL(dataChanged(qint64, qint64)));
    connect(ui->widgetHex, SIGNAL(showOffsetDisasm(qint64)), this, SLOT(_showDisasm(qint64)));
    connect(ui->widgetHex, SIGNAL(showOffsetMemoryMap(qint64)), this, SLOT(_showMemoryMap(qint64)));
    connect(ui->widgetHex, SIGNAL(showOffsetMainHex(qint64, qint64)), this, SLOT(_showHex(qint64, qint64)));
    connect(ui->widgetStrings, SIGNAL(dataChanged(qint64, qint64)), this, SIGNAL(dataChanged(qint64, qint64)));
    connect(ui->widgetStrings, SIGNAL(showHex(qint64, qint64)), this, SLOT(_showHex(qint64, qint64)));
    connect(ui->widgetStrings, SIGNAL(showDemangle(QString)), this, SIGNAL(showDemangle(QString)));
}

void ToolsWidget::setData(QIODevice *pDevice, FW_DEF::OPTIONS options, QIODevice *pBackupDevice, bool bDisasm, bool bFollow)
{
    g_pDevice = pDevice;

    XHexView::OPTIONS hexOptions = {};
    hexOptions.nStartAddress = options.nImageBase;

    if (bFollow) {
        hexOptions.bMenu_MemoryMap = true;
        hexOptions.bMenu_Disasm = bDisasm;
        hexOptions.bMenu_MainHex = true;
    }

    if (options.bOffset) {
        hexOptions.addressMode = XDeviceTableView::MODE_OFFSET;
    } else {
        hexOptions.addressMode = XDeviceTableView::MODE_ADDRESS;
    }

    // hexOptions.bIsOffsetTitle = options.bOffset;

    //    ui->widgetHex->enableReadOnly(false);

    ui->widgetHex->setData(pDevice, hexOptions);
    ui->widgetHex->setBackupDevice(pBackupDevice);

    g_stringsOptions = {};
    g_stringsOptions.nBaseAddress = options.nImageBase;
    g_stringsOptions.bMenu_Hex = true;
    g_stringsOptions.bMenu_Demangle = true;
    g_stringsOptions.bAnsi = true;
    g_stringsOptions.bUnicode = true;
    g_stringsOptions.bCStrings = false;

    ui->widgetStrings->setData(pDevice, g_stringsOptions, false);

    reload();
}

void ToolsWidget::setDevice(QIODevice *pDevice)
{
    ui->widgetHex->setDevice(pDevice);
}

void ToolsWidget::setBackupDevice(QIODevice *pDevice)
{
    ui->widgetHex->setBackupDevice(pDevice);
}

void ToolsWidget::setEdited(qint64 nDeviceOffset, qint64 nDeviceSize)
{
    ui->widgetHex->setEdited(nDeviceOffset, nDeviceSize);

    //    emit changed();
}

void ToolsWidget::setReadonly(bool bState)
{
    ui->widgetHex->setReadonly(bState);
    ui->widgetStrings->setReadonly(bState);
}

void ToolsWidget::reload()
{
    int nIndex = ui->tabWidgetMain->currentIndex();

    if (nIndex == 0)  // Hex
    {
        ui->widgetHex->reload(true);
    } else if (nIndex == 1)  // Strings
    {
        if (!ui->widgetStrings->getInitStatus()) {
            ui->widgetStrings->reload();
        }
    }
}

qint64 ToolsWidget::getStartAddress()
{
    return ui->widgetHex->getStartAddress();
}

void ToolsWidget::setSelection(qint64 nOffset, qint64 nSize)
{
    ui->widgetHex->setDeviceSelection(nOffset, nSize);
}

ToolsWidget::~ToolsWidget()
{
    delete ui;
}

void ToolsWidget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    ui->widgetHex->setGlobal(pShortcuts, pXOptions);
    ui->widgetStrings->setGlobal(pShortcuts, pXOptions);
}

void ToolsWidget::on_tabWidgetMain_currentChanged(int nIndex)
{
    Q_UNUSED(nIndex)
    reload();
}

void ToolsWidget::_showHex(qint64 nOffset, qint64 nSize)
{
    //    XIODevice *pSubDevice=dynamic_cast<XIODevice *>(getDevice());

    //    if(pSubDevice)
    //    {
    //        nOffset+=pSubDevice->getInitOffset();
    //    }

    emit showOffsetHex(nOffset, nSize);
}

void ToolsWidget::_showDisasm(qint64 nOffset)
{
    //    XIODevice *pSubDevice=dynamic_cast<XIODevice *>(getDevice());

    //    if(pSubDevice)
    //    {
    //        nOffset+=pSubDevice->getInitOffset();
    //    }

    emit showOffsetDisasm(nOffset);
}

void ToolsWidget::_showMemoryMap(qint64 nOffset)
{
    //    XIODevice *pSubDevice=dynamic_cast<XIODevice *>(getDevice());

    //    if(pSubDevice)
    //    {
    //        nOffset+=pSubDevice->getInitOffset();
    //    }

    emit showOffsetMemoryMap(nOffset);
}

void ToolsWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
