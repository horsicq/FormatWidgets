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
#include "generictablewidget.h"
#include "ui_generictablewidget.h"

GenericTableWidget::GenericTableWidget(QWidget *pParent) : FormatWidget(pParent), ui(new Ui::GenericTableWidget)
{
    ui->setupUi(this);
    g_pModel = nullptr;
}

GenericTableWidget::~GenericTableWidget()
{
    delete ui;
}

FormatWidget::SV GenericTableWidget::_setValue(QVariant vValue, qint32 nPosition)
{
    Q_UNUSED(vValue)
    Q_UNUSED(nPosition)

    return SV_NONE;
}

void GenericTableWidget::reloadData(bool bSaveSelection)
{
    qint32 nRow = 0;

    if (bSaveSelection) {
        nRow = ui->tableViewMain->currentIndex().row();
    }

    cleanup();

    QList<FW_DEF::HEADER_RECORD> listHeaderRecords;

    DialogProcessData dialogProcessData(this);
    dialogProcessData.setGlobal(getShortcuts(), getGlobalOptions());
    dialogProcessData.setData(&g_pModel, &listHeaderRecords, getCwOptions());
    dialogProcessData.showDialogDelay();

    if (g_pModel) {
        ui->tableViewMain->setCustomModel(g_pModel, true);

        connect(ui->tableViewMain->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                SLOT(onTableView_currentRowChanged(QModelIndex, QModelIndex)));
    }

    adjustGenericTable(ui->tableViewMain, &listHeaderRecords);

    //ui->tableViewMain->resizeColumnsToContents();
    QModelIndex index = ui->tableViewMain->model()->index(nRow, 0);
    ui->tableViewMain->setCurrentIndex(index);
}

void GenericTableWidget::on_tableViewMain_customContextMenuRequested(const QPoint &pos)
{
    contextMenuGenericTableWidget(pos, ui->tableViewMain, getListRecWidgets(), getCwOptions());
}

void GenericTableWidget::onTableView_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    setTableSelection(ui->tableViewMain);
}

void GenericTableWidget::on_tableViewMain_clicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    setTableSelection(ui->tableViewMain);
}

