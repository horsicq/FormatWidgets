/* Copyright (c) 2025 hors<horsicq@gmail.com>
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
#include "xgenericheaderwidget.h"
#include "ui_xgenericheaderwidget.h"

XGenericHeaderWidget::XGenericHeaderWidget(QWidget *parent)
    : XGenericAbstractWidget(parent)
    , ui(new Ui::XGenericHeaderWidget)
{
    ui->setupUi(this);

    g_nDataSize = 0;

    XOptions::adjustToolButton(ui->toolButtonTableReload, XOptions::ICONTYPE_RELOAD);
    XOptions::adjustToolButton(ui->toolButtonTableSize, XOptions::ICONTYPE_SIZE);
    XOptions::adjustToolButton(ui->toolButtonTableSave, XOptions::ICONTYPE_SAVE);

    ui->toolButtonTableReload->setToolTip(tr("Reload"));
    ui->toolButtonTableSize->setToolTip(tr("Size"));
    ui->toolButtonTableSave->setToolTip(tr("Save"));

    adjustHeaderTableWidget(ui->tableWidgetMain);
}

XGenericHeaderWidget::~XGenericHeaderWidget()
{
    delete ui;
}

void XGenericHeaderWidget::adjustView()
{
    // XGenericAbstractWidget::adjustView();
    getGlobalOptions()->adjustTableWidget(ui->tableWidgetMain, XOptions::ID_VIEW_FONT_TABLEVIEWS);
}

void XGenericHeaderWidget::reloadData(bool bSaveSelection)
{
    qint32 nCurrentRow = 0;

    if (bSaveSelection) {
        nCurrentRow = ui->tableWidgetMain->currentRow();
    }

    QList<XBinary::DATA_RECORD> listDataRecords = getRecordsOptions().dataHeader.listRecords;

    g_nDataSize = getRecordsOptions().dataHeader.nSize;

    // Example: Populate the tableWidget with the data records
    qint32 nNumberOfRecords = listDataRecords.count();

    ui->tableWidgetMain->setRowCount(nNumberOfRecords);
    ui->tableWidgetMain->clearContents();

    for (int i = 0; i < nNumberOfRecords; ++i) {
        const XBinary::DATA_RECORD &record = listDataRecords.at(i);

        QTableWidgetItem *itemName = new QTableWidgetItem(record.sName);

        ui->tableWidgetMain->setItem(i, 0, itemName);
    }

    // Optionally resize columns to fit contents
    ui->tableWidgetMain->resizeColumnsToContents();

    // Further processing can be added here as needed

    ui->tableWidgetMain->setCurrentCell(nCurrentRow, 0);
}

void XGenericHeaderWidget::on_toolButtonTableReload_clicked()
{
    reloadData(true);
}

void XGenericHeaderWidget::on_toolButtonTableSize_clicked()
{
    emit followLocation(getRecordsOptions().dataHeader.nLocation, getRecordsOptions().dataHeader.locType, g_nDataSize, XOptions::WIDGETTYPE_HEX);
}

void XGenericHeaderWidget::on_toolButtonTableSave_clicked()
{
    // saveModel(ui->tableWidgetMain->model(), getTypeTitle(getCwOptions()));
}
