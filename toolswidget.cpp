/* Copyright (c) 2019-2025 hors<horsicq@gmail.com>
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

    m_pDevice = nullptr;

    connect(ui->widgetHex, SIGNAL(dataChanged(qint64, qint64)), this, SIGNAL(dataChanged(qint64, qint64)));
    connect(ui->widgetHex, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SIGNAL(followLocation(quint64, qint32, qint64, qint32)));
    connect(ui->widgetStrings, SIGNAL(dataChanged(qint64, qint64)), this, SIGNAL(dataChanged(qint64, qint64)));
    connect(ui->widgetStrings, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SIGNAL(followLocation(quint64, qint32, qint64, qint32)));
    connect(ui->widgetStrings, SIGNAL(showDemangle(QString)), this, SIGNAL(showDemangle(QString)));
}

void ToolsWidget::setData(QIODevice *pDevice, FW_DEF::OPTIONS options, bool bDisasm, bool bFollow, XInfoDB *pXInfoDB)
{
    m_pDevice = pDevice;

    XBinaryView::OPTIONS hexOptions = {};
    hexOptions.nStartOffset = options.nImageBase;

    if (bFollow) {
        hexOptions.bMenu_MemoryMap = true;
        hexOptions.bMenu_Disasm = bDisasm;
        hexOptions.bMenu_MainHex = true;
    }

    if (options.bOffset) {
        hexOptions.addressMode = XBinaryView::LOCMODE_OFFSET;
    } else {
        hexOptions.addressMode = XBinaryView::LOCMODE_ADDRESS;
    }

    // hexOptions.bIsOffsetTitle = options.bOffset;

    //    ui->widgetHex->enableReadOnly(false);

    ui->widgetHex->setXInfoDB(pXInfoDB);

    // Convert pDevice to SubDevice if necessary
    SubDevice *_pSubDevice = dynamic_cast<SubDevice *>(pDevice);

    if (_pSubDevice) {
        hexOptions.nStartOffset = _pSubDevice->getInitLocation();
        hexOptions.nTotalSize = _pSubDevice->size();
        ui->widgetHex->setData(_pSubDevice->getOrigDevice(), hexOptions, true);
    } else {
        ui->widgetHex->setData(pDevice, hexOptions, true);
    }

    m_stringsOptions = {};
    m_stringsOptions.nBaseAddress = options.nImageBase;
    m_stringsOptions.bMenu_Hex = true;
    m_stringsOptions.bMenu_Demangle = true;
    m_stringsOptions.bAnsi = true;
    m_stringsOptions.bUnicode = true;
    m_stringsOptions.bNullTerminated = false;

    ui->widgetStrings->setData(pDevice, XBinary::FT_REGION, m_stringsOptions, false);

    reload();
}

void ToolsWidget::resetWidget()
{
    ui->widgetHex->reset();
    ui->widgetHex->setXInfoDB(nullptr);
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
    qint32 nIndex = ui->tabWidgetMain->currentIndex();

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

// qint64 ToolsWidget::getStartLocation()
// {
//     return ui->widgetHex->getStartLocation();
// }

void ToolsWidget::setSelection(qint64 nOffset, qint64 nSize, bool bGoto)
{
    ui->widgetHex->setDeviceSelection(nOffset, nSize);

    if (bGoto) {
        ui->widgetHex->goToOffset(nOffset);
    }
}

void ToolsWidget::adjustView()
{
    ui->widgetHex->adjustView();
    ui->widgetStrings->adjustView();
}

void ToolsWidget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)

    reload();
}

ToolsWidget::~ToolsWidget()
{
    delete ui;
}

void ToolsWidget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    ui->widgetHex->setGlobal(pShortcuts, pXOptions);
    ui->widgetStrings->setGlobal(pShortcuts, pXOptions);

    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
}

void ToolsWidget::on_tabWidgetMain_currentChanged(int nIndex)
{
    Q_UNUSED(nIndex)
    reload();
}

void ToolsWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
