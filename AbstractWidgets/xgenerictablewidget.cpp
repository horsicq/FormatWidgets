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
#include "xgenerictablewidget.h"
#include "ui_xgenerictablewidget.h"

XGenericTableWidget::XGenericTableWidget(QWidget *pParent) : XFormatWidget(pParent), ui(new Ui::XGenericTableWidget)
{
    ui->setupUi(this);
    g_pModel = nullptr;
    g_bCustomWidgetEnabled = false;

    XOptions::adjustToolButton(ui->toolButtonTableReload, XOptions::ICONTYPE_RELOAD);
    XOptions::adjustToolButton(ui->toolButtonTableSize, XOptions::ICONTYPE_SIZE);
    XOptions::adjustToolButton(ui->toolButtonTableSave, XOptions::ICONTYPE_SAVE);

    ui->toolButtonTableReload->setToolTip(tr("Reload"));
    ui->toolButtonTableSize->setToolTip(tr("Size"));
    ui->toolButtonTableSave->setToolTip(tr("Save"));
    ui->comboBoxDemangle->setToolTip(tr("Demangle"));

    const bool bBlocked1 = ui->comboBoxDemangle->blockSignals(true);

    ui->comboBoxDemangle->addItem("", XDemangle::MODE_UNKNOWN);

    //    QList<XDemangle::MODE> listModes=XDemangle::getAllModes();
    QList<XDemangle::MODE> listModes = XDemangle::getSupportedModes();

    qint32 nNumberOfModes = listModes.count();

    for (qint32 i = 0; i < nNumberOfModes; i++) {
        XDemangle::MODE mode = listModes.at(i);
        ui->comboBoxDemangle->addItem(XDemangle::modeIdToString(mode), mode);
    }

    ui->comboBoxDemangle->blockSignals(bBlocked1);

    ui->comboBoxDemangle->hide();
}

XGenericTableWidget::~XGenericTableWidget()
{
    delete ui;
}

void XGenericTableWidget::reloadData(bool bSaveSelection)
{
    qint32 nRow = 0;

    if (bSaveSelection) {
        nRow = ui->tableViewMain->currentIndex().row();
    }

    cleanup();

    g_listHeaderRecords.clear();

    XFW_DEF::CWOPTIONS cwOptions = *getCwOptions();
    cwOptions.demangleMode = getDemangleMode();

    XDialogProcessData dialogProcessData(this);
    dialogProcessData.setGlobal(getShortcuts(), getGlobalOptions());
    dialogProcessData.setData(&g_pModel, &g_listHeaderRecords, &cwOptions);
    dialogProcessData.showDialogDelay();

    if (g_pModel) {
        ui->tableViewMain->setCustomModel(g_pModel, true);

        connect(ui->tableViewMain->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                SLOT(onTableView_currentRowChanged(QModelIndex, QModelIndex)));
    }

    adjustGenericTable(ui->tableViewMain, &g_listHeaderRecords);

    qint32 nNumberOfRecords = g_listHeaderRecords.count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        if (g_listHeaderRecords.at(i).vtype & XFW_DEF::VAL_TYPE_DEMANGLE) {
            ui->comboBoxDemangle->show();
            break;
        }
    }

    // ui->tableViewMain->resizeColumnsToContents();
    QModelIndex index = ui->tableViewMain->model()->index(nRow, 0);
    ui->tableViewMain->setCurrentIndex(index);
}

void XGenericTableWidget::adjustView()
{
    XFormatWidget::adjustView();
    getGlobalOptions()->adjustTableView(ui->tableViewMain, XOptions::ID_VIEW_FONT_TABLEVIEWS);
}

void XGenericTableWidget::setCustomWidgetEnabled(bool bState)
{
    g_bCustomWidgetEnabled = bState;
}

void XGenericTableWidget::on_tableViewMain_customContextMenuRequested(const QPoint &pos)
{
    contextMenuGenericTableWidget(pos, ui->tableViewMain, &g_listHeaderRecords, getCwOptions());
}

void XGenericTableWidget::onTableView_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    setTableSelection(ui->tableViewMain, g_bCustomWidgetEnabled);
}

void XGenericTableWidget::on_tableViewMain_clicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    setTableSelection(ui->tableViewMain, g_bCustomWidgetEnabled);
}

void XGenericTableWidget::on_toolButtonTableReload_clicked()
{
    reloadData(true);
}

void XGenericTableWidget::on_toolButtonTableSize_clicked()
{
    emit followLocation(getCwOptions()->nDataOffset, XBinary::LT_OFFSET, getCwOptions()->nDataSize, XOptions::WIDGETTYPE_HEX);
}

void XGenericTableWidget::on_toolButtonTableSave_clicked()
{
    saveModel(g_pModel, getTypeTitle(getCwOptions()));
}

void XGenericTableWidget::on_tableViewMain_doubleClicked(const QModelIndex &index)
{
    tableView_doubleClicked(ui->tableViewMain, index);
}

void XGenericTableWidget::on_comboBoxDemangle_currentIndexChanged(int nIndex)
{
    Q_UNUSED(nIndex)

    setDemangleMode((XDemangle::MODE)(ui->comboBoxDemangle->currentData().toUInt()));

    reloadData(true);
}
