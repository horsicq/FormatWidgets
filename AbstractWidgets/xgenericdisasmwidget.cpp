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
#include "xgenericdisasmwidget.h"
#include "ui_xgenericdisasmwidget.h"

XGenericDisasmWidget::XGenericDisasmWidget(QWidget *pParent) : XFormatWidget(pParent), ui(new Ui::XGenericDisasmWidget)
{
    ui->setupUi(this);

    g_pSubDevice = nullptr;

    XOptions::adjustToolButton(ui->toolButtonTableReload, XOptions::ICONTYPE_RELOAD);
    XOptions::adjustToolButton(ui->toolButtonTableSize, XOptions::ICONTYPE_SIZE);

    ui->toolButtonTableReload->setToolTip(tr("Reload"));
    ui->toolButtonTableSize->setToolTip(tr("Size"));

    connect(ui->scrollAreaHex, SIGNAL(currentLocationChanged(quint64, qint32, qint64)), this, SIGNAL(currentLocationChanged(quint64, qint32, qint64)));
    connect(ui->scrollAreaHex, SIGNAL(dataChanged(qint64, qint64)), this, SIGNAL(dataChanged(qint64, qint64)));
    connect(ui->scrollAreaHex, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SIGNAL(followLocation(quint64, qint32, qint64, qint32)));
}

XGenericDisasmWidget::~XGenericDisasmWidget()
{
    delete ui;
    if (g_pSubDevice) {
        delete g_pSubDevice;
    }
}

void XGenericDisasmWidget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)

    if (g_pSubDevice) {
        delete g_pSubDevice;
    }

    g_pSubDevice = new SubDevice(getDevice(), getCwOptions()->nDataOffset, getCwOptions()->nDataSize);

    if (g_pSubDevice->open(QIODevice::ReadWrite)) {
        XHexView::OPTIONS options = {};
        options.nStartAddress = getCwOptions()->nDataOffset;
        options.addressMode = XHexView::LOCMODE_ADDRESS;

        ui->scrollAreaHex->setData(g_pSubDevice, options, true);
    }
}

void XGenericDisasmWidget::adjustView()
{
    XFormatWidget::adjustView();
    // getGlobalOptions()->adjustWidget(ui->tableWidgetMain, XOptions::ID_VIEW_FONT_TABLEVIEWS);
}

void XGenericDisasmWidget::setReadonly(bool bState)
{
    ui->scrollAreaHex->setReadonly(bState);
    XFormatWidget::setReadonly(bState);
}

void XGenericDisasmWidget::on_toolButtonTableReload_clicked()
{
    reloadData(true);
}

void XGenericDisasmWidget::on_toolButtonTableSize_clicked()
{
    emit followLocation(getCwOptions()->nDataOffset, XBinary::LT_OFFSET, getCwOptions()->nDataSize, XOptions::WIDGETTYPE_HEX);
}
