/* Copyright (c) 2019-2022 hors<horsicq@gmail.com>
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
#include "searchvalueswidget.h"

#include "ui_searchvalueswidget.h"

SearchValuesWidget::SearchValuesWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::SearchValuesWidget)
{
    ui->setupUi(this);
    g_pDevice = nullptr;

    g_pModel = nullptr;
    g_varValue = 0;
    g_valueType = XBinary::VT_UNKNOWN;
    g_bIsBigEndian = false;

    memset(shortCuts, 0, sizeof shortCuts);

    ui->tableViewResult->installEventFilter(this);
}

SearchValuesWidget::~SearchValuesWidget()
{
    g_watcher.waitForFinished();

    delete ui;
}

void SearchValuesWidget::setData(QIODevice *pDevice, OPTIONS options)
{
    this->g_pDevice = pDevice;
    this->g_options = options;

    if (pDevice) {
        XFormats::setFileTypeComboBox(options.fileType, g_pDevice, ui->comboBoxType);
    }
}

QIODevice *SearchValuesWidget::getDevice()
{
    return g_pDevice;
}

void SearchValuesWidget::findValue(QVariant varValue, XBinary::VT valueType, bool bIsBigEndian)
{
    g_varValue = varValue;
    g_valueType = valueType;
    g_bIsBigEndian = bIsBigEndian;

    ui->labelSearchValue->setText(QString("%1: %2").arg(XBinary::valueTypeToString(valueType), XBinary::getValueString(varValue, valueType)));

    search();
}

void SearchValuesWidget::findValue(quint64 nValue, bool bIsBigEndian)
{
    QVariant varValue = nValue;
    XBinary::VT valueType = XBinary::getValueType(nValue);

    findValue(varValue, valueType, bIsBigEndian);
}

void SearchValuesWidget::on_pushButtonSave_clicked()
{
    if (g_pModel) {
        XShortcutsWidget::saveTableModel(g_pModel, XBinary::getResultFileName(g_pDevice, QString("%1.txt").arg(tr("Values"))));
    }
}

void SearchValuesWidget::on_tableViewResult_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextMenu(this);

    QMenu menuFollowIn(tr("Follow in"), this);

    QAction actionHex(tr("Hex"), this);
    QAction actionDisasm(tr("Disasm"), this);

    if (g_options.bMenu_Hex) {
        actionHex.setShortcut(getShortcuts()->getShortcut(X_ID_FIND_FOLLOWIN_HEX));
        connect(&actionHex, SIGNAL(triggered()), this, SLOT(_hex()));

        menuFollowIn.addAction(&actionHex);
    }

    if (g_options.bMenu_Disasm) {
        actionDisasm.setShortcut(getShortcuts()->getShortcut(X_ID_FIND_FOLLOWIN_DISASM));
        connect(&actionDisasm, SIGNAL(triggered()), this, SLOT(_disasm()));
        menuFollowIn.addAction(&actionDisasm);
    }

    if (g_options.bMenu_Hex || g_options.bMenu_Disasm) {
        contextMenu.addMenu(&menuFollowIn);
    }

    contextMenu.addMenu(getShortcuts()->getRowCopyMenu(this, ui->tableViewResult));

    contextMenu.exec(ui->tableViewResult->viewport()->mapToGlobal(pos));
}

void SearchValuesWidget::search()
{
    if (g_pDevice && (g_valueType != XBinary::VT_UNKNOWN)) {

        g_pOldModel = g_pModel;

        ui->tableViewResult->setModel(nullptr);

        XBinary::FT fileType = (XBinary::FT)(ui->comboBoxType->currentData().toInt());

        MultiSearch::OPTIONS options = {};

        options.bIsBigEndian = g_bIsBigEndian;
        options.varValue = g_varValue;
        options.valueType = g_valueType;
        options.memoryMap = XFormats::getMemoryMap(fileType, g_pDevice);

        QList<XBinary::MS_RECORD> listRecords;

        QWidget *pParent = XOptions::getMainWidget(this);

        DialogMultiSearchProcess dsp(pParent);
        dsp.processSearch(g_pDevice, &listRecords, options, MultiSearch::TYPE_VALUES);
        dsp.showDialogDelay(1000);

        DialogMultiSearchProcess dmp(pParent);
        dmp.processModel(&listRecords, &g_pModel, options, MultiSearch::TYPE_VALUES);
        dmp.showDialogDelay(1000);

        ui->tableViewResult->setModel(g_pModel);

        ui->tableViewResult->setColumnWidth(0, 120);  // TODO
        ui->tableViewResult->setColumnWidth(1, 120);  // TODO
        ui->tableViewResult->setColumnWidth(2, 120);  // TODO

        QFuture<void> future = deleteOldStandardModel(&g_pOldModel);

        g_watcher.setFuture(future);

        // TODO nothing found
    }
}

void SearchValuesWidget::deleteOldModel()
{
    delete g_pOldModel;
}

void SearchValuesWidget::registerShortcuts(bool bState)
{
    if (bState) {
        // if (!shortCuts[SC_COPYSTRING]) shortCuts[SC_COPYSTRING] = new QShortcut(getShortcuts()->getShortcut(X_ID_STRINGS_COPY_STRING), this, SLOT(_copyString()));
    } else {
        for (qint32 i = 0; i < __SC_SIZE; i++) {
            if (shortCuts[i]) {
                delete shortCuts[i];
                shortCuts[i] = nullptr;
            }
        }
    }
}

void SearchValuesWidget::on_pushButtonSearchString_clicked()
{
    _search(DialogSearch::SEARCHMODE_STRING);
}

void SearchValuesWidget::on_pushButtonSearchSignature_clicked()
{
    _search(DialogSearch::SEARCHMODE_SIGNATURE);
}

void SearchValuesWidget::on_pushButtonSearchValue_clicked()
{
    _search(DialogSearch::SEARCHMODE_VALUE);
}

void SearchValuesWidget::_search(DialogSearch::SEARCHMODE mode)
{
    if (g_pDevice) {
        DialogSearch::OPTIONS options = {};
        XBinary::SEARCHDATA searchData;

        DialogSearch dialogSearch(this, getDevice(), &searchData, mode, options);

        if (dialogSearch.exec() == QDialog::Accepted)  // TODO use status
        {
            findValue(searchData.varValue, searchData.valueType, searchData.bIsBigEndian);
        }
    }
}

void SearchValuesWidget::on_pushButtonSearch_clicked()
{
    search();
}

void SearchValuesWidget::_hex()
{
    int nRow = ui->tableViewResult->currentIndex().row();

    if ((nRow != -1) && (g_pModel)) {
        QModelIndex index = ui->tableViewResult->selectionModel()->selectedIndexes().at(0);

        qint64 nOffset = ui->tableViewResult->model()->data(index, Qt::UserRole + MultiSearch::USERROLE_OFFSET).toLongLong();
        qint64 nSize = ui->tableViewResult->model()->data(index, Qt::UserRole + MultiSearch::USERROLE_SIZE).toLongLong();

        emit showHex(nOffset, nSize);
    }
}

void SearchValuesWidget::_disasm()
{
    int nRow = ui->tableViewResult->currentIndex().row();

    if ((nRow != -1) && (g_pModel)) {
        QModelIndex index = ui->tableViewResult->selectionModel()->selectedIndexes().at(0);

        qint64 nOffset = ui->tableViewResult->model()->data(index, Qt::UserRole + MultiSearch::USERROLE_OFFSET).toLongLong();

        emit showDisasm(nOffset);
    }
}
