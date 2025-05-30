/* Copyright (c) 2024-2025 hors<horsicq@gmail.com>
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
#include "xgenerichexwidget.h"
#include "ui_xgenerichexwidget.h"

XGenericHexWidget::XGenericHexWidget(QWidget *pParent) : XFormatWidget(pParent), ui(new Ui::XGenericHexWidget)
{
    ui->setupUi(this);

    XOptions::adjustToolButton(ui->toolButtonTableReload, XOptions::ICONTYPE_RELOAD);
    XOptions::adjustToolButton(ui->toolButtonTableSize, XOptions::ICONTYPE_SIZE);

    ui->toolButtonTableReload->setToolTip(tr("Reload"));
    ui->toolButtonTableSize->setToolTip(tr("Size"));

    connect(ui->scrollAreaHex, SIGNAL(currentLocationChanged(quint64, qint32, qint64)), this, SIGNAL(currentLocationChanged(quint64, qint32, qint64)));
    connect(ui->scrollAreaHex, SIGNAL(dataChanged(qint64, qint64)), this, SIGNAL(dataChanged(qint64, qint64)));
    connect(ui->scrollAreaHex, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SIGNAL(followLocation(quint64, qint32, qint64, qint32)));
}

XGenericHexWidget::~XGenericHexWidget()
{
    delete ui;
}

void XGenericHexWidget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)

    XHexView::OPTIONS options = {};
    options.nStartOffset = getCwOptions()->nDataOffset;
    options.nTotalSize = getCwOptions()->nDataSize;
    options.addressMode = XHexView::LOCMODE_ADDRESS;  // Important!

    ui->scrollAreaHex->setData(getCwOptions()->pDevice, options, true);
}

void XGenericHexWidget::adjustView()
{
    XFormatWidget::adjustView();
    // getGlobalOptions()->adjustWidget(ui->tableWidgetMain, XOptions::ID_VIEW_FONT_TABLEVIEWS);
}

void XGenericHexWidget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    ui->scrollAreaHex->setGlobal(pShortcuts, pXOptions);
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
}

void XGenericHexWidget::setReadonly(bool bState)
{
    ui->scrollAreaHex->setReadonly(bState);
    XFormatWidget::setReadonly(bState);
}

void XGenericHexWidget::on_toolButtonTableReload_clicked()
{
    reloadData(true);
}

void XGenericHexWidget::on_toolButtonTableSize_clicked()
{
    emit followLocation(getCwOptions()->nDataOffset, XBinary::LT_OFFSET, getCwOptions()->nDataSize, XOptions::WIDGETTYPE_HEX);
}
