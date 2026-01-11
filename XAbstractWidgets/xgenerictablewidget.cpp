/* Copyright (c) 2025-2026 hors<horsicq@gmail.com>
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
#include "xgenerictablewidget.h"
#include "ui_xgenerictablewidget.h"

XGenericTableWidget::XGenericTableWidget(QWidget *parent) : XGenericAbstractWidget(parent), ui(new Ui::XGenericTableWidget)
{
    ui->setupUi(this);

    m_nDataSize = 0;

    XOptions::adjustToolButton(ui->toolButtonTableReload, XOptions::ICONTYPE_RELOAD);
    XOptions::adjustToolButton(ui->toolButtonTableSave, XOptions::ICONTYPE_SAVE);

    ui->toolButtonTableReload->setToolTip(tr("Reload"));
    ui->toolButtonTableSave->setToolTip(tr("Save"));
}

XGenericTableWidget::~XGenericTableWidget()
{
    delete ui;
}

void XGenericTableWidget::adjustView()
{
    // XGenericAbstractWidget::adjustView();
    getGlobalOptions()->adjustTableView(ui->tableViewMain, XOptions::ID_VIEW_FONT_TABLEVIEWS);
}

void XGenericTableWidget::reloadData(bool bSaveSelection)
{
    m_listDataRecordsRows.clear();

    XGetDataRecordsProcess getDataRecordsProcess;
    XDialogProcess dd(this, &getDataRecordsProcess);
    dd.setGlobal(getShortcuts(), getGlobalOptions());
    getDataRecordsProcess.setData(getDevice(), getRecordsOptions(), &m_listDataRecordsRows, &m_listTitles, dd.getPdStruct());
    dd.start();
    dd.showDialogDelay();

    XModel_Binary *pModel = new XModel_Binary(getRecordsOptions(), &m_listDataRecordsRows, &m_listTitles, this);

    ui->tableViewMain->setCustomModel(pModel, true);

    qint32 nCurrentRow = 0;

    if (bSaveSelection) {
        nCurrentRow = ui->tableViewMain->currentIndex().row();
    }

    // TODO

    // ui->tableViewMain->setCurrentIndex(ui->tableViewMain->model()->index(nCurrentRow, 0));
}

void XGenericTableWidget::on_toolButtonTableReload_clicked()
{
    reloadData(true);
}

void XGenericTableWidget::on_toolButtonTableSave_clicked()
{
    // saveModel(ui->tableWidgetMain->model(), getTypeTitle(getCwOptions()));
}

void XGenericTableWidget::on_tableViewMain_customContextMenuRequested(const QPoint &pos)
{
}
