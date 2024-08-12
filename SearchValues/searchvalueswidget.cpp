/* Copyright (c) 2019-2024 hors<horsicq@gmail.com>
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
    g_endian = XBinary::ENDIAN_LITTLE;

    memset(g_shortCuts, 0, sizeof g_shortCuts);

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
        XFormats::getMapModesList(options.fileType, ui->comboBoxMapMode);
    }
}

QIODevice *SearchValuesWidget::getDevice()
{
    return g_pDevice;
}

void SearchValuesWidget::findValue(QVariant varValue, XBinary::VT valueType, XBinary::ENDIAN endian)
{
    g_varValue = varValue;
    g_valueType = valueType;
    g_endian = endian;

    ui->labelSearchValue->setText(QString("%1: %2").arg(XBinary::valueTypeToString(valueType), XBinary::getValueString(varValue, valueType)));

    search();
}

void SearchValuesWidget::findValue(quint64 nValue, XBinary::ENDIAN endian)
{
    QVariant varValue = nValue;
    XBinary::VT valueType = XBinary::getValueType(nValue);

    findValue(varValue, valueType, endian);
}

void SearchValuesWidget::adjustView()
{
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
        XBinary::MAPMODE mapMode = (XBinary::MAPMODE)(ui->comboBoxMapMode->currentData().toInt());

        MultiSearch::OPTIONS options = {};

        options.endian = g_endian;
        options.varValue = g_varValue;
        options.valueType = g_valueType;
        options.memoryMap = XFormats::getMemoryMap(fileType, mapMode, g_pDevice);

        QList<XBinary::MS_RECORD> listRecords;

        QWidget *pParent = XOptions::getMainWidget(this);

        DialogMultiSearchProcess dsp(pParent);
        dsp.setGlobal(getShortcuts(), getGlobalOptions());
        dsp.processSearch(g_pDevice, &listRecords, options, MultiSearch::TYPE_VALUES);
        dsp.showDialogDelay();

        DialogMultiSearchProcess dmp(pParent);
        dmp.setGlobal(getShortcuts(), getGlobalOptions());
        dmp.processModel(&listRecords, &g_pModel, options, MultiSearch::TYPE_VALUES);
        dmp.showDialogDelay();

        ui->tableViewResult->setModel(g_pModel);

        ui->tableViewResult->setColumnWidth(MultiSearch::COLUMN_VALUE_NUMBER, 80);
        ui->tableViewResult->setColumnWidth(MultiSearch::COLUMN_VALUE_OFFSET, 120);   // TODO
        ui->tableViewResult->setColumnWidth(MultiSearch::COLUMN_VALUE_ADDRESS, 120);  // TODO
        ui->tableViewResult->setColumnWidth(MultiSearch::COLUMN_VALUE_REGION, 120);   // TODO

        QFuture<void> future = deleteOldStandardModel(&g_pOldModel);

        g_watcher.setFuture(future);

        connect(ui->tableViewResult->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this,
                SLOT(on_tableViewSelection(QItemSelection, QItemSelection)));
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
            if (g_shortCuts[i]) {
                delete g_shortCuts[i];
                g_shortCuts[i] = nullptr;
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
        dialogSearch.setGlobal(getShortcuts(), getGlobalOptions());

        XOptions::_adjustStayOnTop(&dialogSearch, true);

        if (dialogSearch.exec() == QDialog::Accepted)  // TODO use status
        {
            findValue(searchData.varValue, searchData.valueType, searchData.endian);
        }
    }
}

void SearchValuesWidget::on_pushButtonSearch_clicked()
{
    search();
}

void SearchValuesWidget::_hex()
{
    qint32 nRow = ui->tableViewResult->currentIndex().row();

    if ((nRow != -1) && (g_pModel)) {
        QModelIndex index = ui->tableViewResult->selectionModel()->selectedIndexes().at(MultiSearch::COLUMN_VALUE_NUMBER);

        qint64 nOffset = ui->tableViewResult->model()->data(index, Qt::UserRole + MultiSearch::USERROLE_OFFSET).toLongLong();
        qint64 nSize = ui->tableViewResult->model()->data(index, Qt::UserRole + MultiSearch::USERROLE_SIZE).toLongLong();

        emit showHex(nOffset, nSize);
    }
}

void SearchValuesWidget::_disasm()
{
    qint32 nRow = ui->tableViewResult->currentIndex().row();

    if ((nRow != -1) && (g_pModel)) {
        QModelIndex index = ui->tableViewResult->selectionModel()->selectedIndexes().at(MultiSearch::COLUMN_VALUE_NUMBER);

        qint64 nOffset = ui->tableViewResult->model()->data(index, Qt::UserRole + MultiSearch::USERROLE_OFFSET).toLongLong();

        emit showDisasm(nOffset);
    }
}

void SearchValuesWidget::viewSelection()
{
    QItemSelectionModel *pSelectionModel = ui->tableViewResult->selectionModel();

    if (pSelectionModel) {
        QModelIndexList listIndexes = pSelectionModel->selectedIndexes();

        if (listIndexes.count()) {
            QModelIndex indexNumber = listIndexes.at(MultiSearch::COLUMN_VALUE_NUMBER);
            XADDR nVirtualAddress = ui->tableViewResult->model()->data(indexNumber, Qt::UserRole + MultiSearch::USERROLE_ADDRESS).toULongLong();
            qint64 nOffset = ui->tableViewResult->model()->data(indexNumber, Qt::UserRole + MultiSearch::USERROLE_OFFSET).toULongLong();
            qint64 nSize = ui->tableViewResult->model()->data(indexNumber, Qt::UserRole + MultiSearch::USERROLE_SIZE).toLongLong();

            if (nVirtualAddress != (XADDR)-1) {
                emit currentLocationChanged(nVirtualAddress, XBinary::LT_ADDRESS, nSize);
            } else if (nOffset != -1) {
                emit currentLocationChanged(nOffset, XBinary::LT_OFFSET, nSize);
            }
        }
    }
}

void SearchValuesWidget::on_tableViewSelection(const QItemSelection &itemSelected, const QItemSelection &itemDeselected)
{
    Q_UNUSED(itemSelected)
    Q_UNUSED(itemDeselected)

    viewSelection();
}

void SearchValuesWidget::on_tableViewResult_clicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    viewSelection();
}

void SearchValuesWidget::on_comboBoxType_currentIndexChanged(int index)
{
    Q_UNUSED(index)

    XBinary::FT fileType = (XBinary::FT)(ui->comboBoxType->currentData().toInt());
    XFormats::getMapModesList(fileType, ui->comboBoxMapMode);
}
