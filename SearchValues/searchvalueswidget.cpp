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
#include "searchvalueswidget.h"

#include "ui_searchvalueswidget.h"

SearchValuesWidget::SearchValuesWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::SearchValuesWidget)
{
    ui->setupUi(this);
    m_pDevice = nullptr;
    g_options = {};

    addShortcut(X_ID_TABLE_FOLLOWIN_HEX, this, SLOT(_hex()));
    addShortcut(X_ID_TABLE_FOLLOWIN_DISASM, this, SLOT(_disasm()));

    XOptions::adjustToolButton(ui->toolButtonSearch, XOptions::ICONTYPE_SEARCH);
    XOptions::adjustToolButton(ui->toolButtonSave, XOptions::ICONTYPE_SAVE);
    XOptions::adjustToolButton(ui->toolButtonSearchSignature, XOptions::ICONTYPE_SIGNATURE);
    XOptions::adjustToolButton(ui->toolButtonSearchString, XOptions::ICONTYPE_STRING);
    XOptions::adjustToolButton(ui->toolButtonSearchValue, XOptions::ICONTYPE_VALUE);

    ui->comboBoxType->setToolTip(tr("Type"));
    ui->comboBoxMapMode->setToolTip(tr("Mode"));
    ui->toolButtonSearchString->setToolTip(tr("Search string"));
    ui->toolButtonSearchSignature->setToolTip(tr("Search signature"));
    ui->toolButtonSearchValue->setToolTip(tr("Search value"));
    ui->toolButtonSearch->setToolTip(tr("Search"));
    ui->toolButtonSave->setToolTip(tr("Save"));
    ui->tableViewResult->setToolTip(tr("Result"));

    g_varValue = 0;
    g_valueType = XBinary::VT_UNKNOWN;
    g_endian = XBinary::ENDIAN_LITTLE;

    // ui->tableViewResult->installEventFilter(this);
}

SearchValuesWidget::~SearchValuesWidget()
{
    delete ui;
}

void SearchValuesWidget::setData(QIODevice *pDevice, OPTIONS options)
{
    this->m_pDevice = pDevice;
    this->g_options = options;

    if (pDevice) {
        XFormats::setFileTypeComboBox(options.fileType, m_pDevice, ui->comboBoxType);
        XFormats::getMapModesList(options.fileType, ui->comboBoxMapMode);
    }

    if (options.bScan) {
        ui->toolButtonSearchSignature->hide();
        ui->toolButtonSearchString->hide();
        ui->toolButtonSearchValue->hide();

        g_varValue = options.varValue;
        g_valueType = options.valueType;
        g_endian = options.endian;

        search();
    }
}

QIODevice *SearchValuesWidget::getDevice()
{
    return m_pDevice;
}

void SearchValuesWidget::findValue(QVariant varValue, XBinary::VT valueType, XBinary::ENDIAN endian)
{
    g_varValue = varValue;
    g_valueType = valueType;
    g_endian = endian;

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
    getGlobalOptions()->adjustWidget(this, XOptions::ID_VIEW_FONT_CONTROLS);
    getGlobalOptions()->adjustTableView(ui->tableViewResult, XOptions::ID_VIEW_FONT_TABLEVIEWS);

    ui->tableViewResult->adjust();
}

void SearchValuesWidget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)
    search();
}

void SearchValuesWidget::on_toolButtonSave_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableViewResult->getProxyModel(), XBinary::getResultFileName(m_pDevice, QString("%1.txt").arg(tr("Values"))));
}

void SearchValuesWidget::on_tableViewResult_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextMenu(this);

    QList<XShortcuts::MENUITEM> listMenuItems;

    getShortcuts()->_addMenuItem_CopyRow(&listMenuItems, ui->tableViewResult);

    if (g_options.bMenu_Hex) {
        getShortcuts()->_addMenuItem(&listMenuItems, X_ID_TABLE_FOLLOWIN_HEX, this, SLOT(_hex()), XShortcuts::GROUPID_FOLLOWIN);
    }

    if (g_options.bMenu_Disasm) {
        getShortcuts()->_addMenuItem(&listMenuItems, X_ID_TABLE_FOLLOWIN_DISASM, this, SLOT(_disasm()), XShortcuts::GROUPID_FOLLOWIN);
    }

    getShortcuts()->adjustContextMenu(&contextMenu, &listMenuItems);

    contextMenu.exec(ui->tableViewResult->viewport()->mapToGlobal(pos));
}

void SearchValuesWidget::search()
{
    ui->labelSearchValue->setText(QString("%1: %2").arg(XBinary::valueTypeToString(g_valueType, 0), XBinary::getValueString(g_varValue, g_valueType)));

    if (m_pDevice && (g_valueType != XBinary::VT_UNKNOWN)) {
        // ui->tableViewResult->setModel(nullptr);
        XBinary::FT fileType = (XBinary::FT)(ui->comboBoxType->currentData().toInt());
        XBinary::MAPMODE mapMode = (XBinary::MAPMODE)(ui->comboBoxMapMode->currentData().toInt());

        MultiSearch::OPTIONS options = {};

        options.endian = g_endian;
        options.varValue = g_varValue;
        options.valueType = g_valueType;
        options.memoryMap = XFormats::getMemoryMap(fileType, mapMode, m_pDevice);

        QWidget *pParent = XOptions::getMainWidget(this);

        ui->tableViewResult->clear();

        MultiSearch multiSearch;
        XDialogProcess dsp(pParent, &multiSearch);
        dsp.setGlobal(getShortcuts(), getGlobalOptions());
        multiSearch.setSearchData(m_pDevice, &g_listRecords, options, MultiSearch::TYPE_VALUES, dsp.getPdStruct());
        dsp.start();
        dsp.showDialogDelay();

        XModel_MSRecord *pModel = new XModel_MSRecord(m_pDevice, options.memoryMap, &g_listRecords, XBinary::VT_VALUE, this);
        pModel->setValue(options.endian, options.valueType, options.varValue);

        ui->tableViewResult->setCustomModel(pModel, true);

        connect(ui->tableViewResult->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this,
                SLOT(on_tableViewSelection(QItemSelection, QItemSelection)));
        // TODO nothing found
    }
}

void SearchValuesWidget::on_toolButtonSearchString_clicked()
{
    _search(XBinary::SEARCHMODE_STRING);
}

void SearchValuesWidget::on_toolButtonSearchSignature_clicked()
{
    _search(XBinary::SEARCHMODE_SIGNATURE);
}

void SearchValuesWidget::on_toolButtonSearchValue_clicked()
{
    _search(XBinary::SEARCHMODE_VALUE);
}

void SearchValuesWidget::_search(XBinary::SEARCHMODE mode)
{
    if (m_pDevice) {
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

void SearchValuesWidget::on_toolButtonSearch_clicked()
{
    search();
}

void SearchValuesWidget::_hex()
{
    if (g_options.bMenu_Hex) {
        qint32 nRow = ui->tableViewResult->currentIndex().row();

        if ((nRow != -1) && (g_listRecords.count())) {
            QModelIndex index = ui->tableViewResult->selectionModel()->selectedIndexes().at(XModel_MSRecord::COLUMN_NUMBER);

            qint64 nOffset = ui->tableViewResult->model()->data(index, Qt::UserRole + XModel_MSRecord::USERROLE_OFFSET).toLongLong();
            qint64 nSize = ui->tableViewResult->model()->data(index, Qt::UserRole + XModel_MSRecord::USERROLE_SIZE).toLongLong();

            emit followLocation(nOffset, XBinary::LT_OFFSET, nSize, XOptions::WIDGETTYPE_HEX);
        }
    }
}

void SearchValuesWidget::_disasm()
{
    if (g_options.bMenu_Disasm) {
        qint32 nRow = ui->tableViewResult->currentIndex().row();

        if ((nRow != -1) && (g_listRecords.count())) {
            QModelIndex index = ui->tableViewResult->selectionModel()->selectedIndexes().at(XModel_MSRecord::COLUMN_NUMBER);

            qint64 nOffset = ui->tableViewResult->model()->data(index, Qt::UserRole + XModel_MSRecord::USERROLE_OFFSET).toLongLong();

            emit followLocation(nOffset, XBinary::LT_OFFSET, 0, XOptions::WIDGETTYPE_DISASM);
        }
    }
}

void SearchValuesWidget::viewSelection()
{
    QItemSelectionModel *pSelectionModel = ui->tableViewResult->selectionModel();

    if (pSelectionModel) {
        QModelIndexList listIndexes = pSelectionModel->selectedIndexes();

        if (listIndexes.count()) {
            QModelIndex indexNumber = listIndexes.at(XModel_MSRecord::COLUMN_NUMBER);
            XADDR nVirtualAddress = ui->tableViewResult->model()->data(indexNumber, Qt::UserRole + XModel_MSRecord::USERROLE_ADDRESS).toULongLong();
            qint64 nOffset = ui->tableViewResult->model()->data(indexNumber, Qt::UserRole + XModel_MSRecord::USERROLE_OFFSET).toULongLong();
            qint64 nSize = ui->tableViewResult->model()->data(indexNumber, Qt::UserRole + XModel_MSRecord::USERROLE_SIZE).toLongLong();

            if (nOffset != -1) {
                emit currentLocationChanged(nOffset, XBinary::LT_OFFSET, nSize);
            } else if (nVirtualAddress != (XADDR)-1) {
                emit currentLocationChanged(nVirtualAddress, XBinary::LT_ADDRESS, nSize);
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

void SearchValuesWidget::on_comboBoxType_currentIndexChanged(int nIndex)
{
    Q_UNUSED(nIndex)

    XBinary::FT fileType = (XBinary::FT)(ui->comboBoxType->currentData().toInt());
    XFormats::getMapModesList(fileType, ui->comboBoxMapMode);
}
