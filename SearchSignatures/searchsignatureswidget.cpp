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
#include "searchsignatureswidget.h"

#include "ui_searchsignatureswidget.h"

SearchSignaturesWidget::SearchSignaturesWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::SearchSignaturesWidget)
{
    ui->setupUi(this);

    addShortcut(X_ID_TABLE_FOLLOWIN_HEX, this, SLOT(_hex()));
    addShortcut(X_ID_TABLE_FOLLOWIN_DISASM, this, SLOT(_disasm()));

    XOptions::adjustToolButton(ui->toolButtonSave, XOptions::ICONTYPE_SAVE);
    XOptions::adjustToolButton(ui->toolButtonSearch, XOptions::ICONTYPE_SEARCH);

    ui->toolButtonSave->setToolTip(tr("Save"));
    ui->toolButtonSearch->setToolTip(tr("Search"));
    ui->comboBoxFile->setToolTip(tr("File"));
    ui->comboBoxType->setToolTip(tr("Type"));
    ui->comboBoxEndianness->setToolTip(tr("Endianness"));
    ui->labelInfo->setToolTip(tr("Info"));
    ui->tableViewResult->setToolTip(tr("Result"));

    g_pDevice = nullptr;
    g_bInit = false;

    ui->toolButtonPatch->setEnabled(false);

    // ui->tableViewResult->installEventFilter(this);
}

SearchSignaturesWidget::~SearchSignaturesWidget()
{
    delete ui;
}

void SearchSignaturesWidget::setData(QIODevice *pDevice, OPTIONS options, bool bAuto)
{
    this->g_pDevice = pDevice;
    g_bInit = false;

    XFormats::setFileTypeComboBox(options.fileType, g_pDevice, ui->comboBoxType);
    XFormats::setEndiannessComboBox(ui->comboBoxEndianness, XBinary::ENDIAN_LITTLE);

    // ui->tableViewResult->setModel(nullptr);

    g_options = options;

    reloadFileType();

    if (bAuto) {
        search();
    }
}

SearchSignaturesWidget::OPTIONS SearchSignaturesWidget::getOptions()
{
    return g_options;
}

void SearchSignaturesWidget::updateSignaturesPath()
{
    QString sArch = XBinary::disasmIdToArch((XBinary::DM)(ui->comboBoxMode->currentData().toULongLong()));

    const bool bBlocked1 = ui->comboBoxFile->blockSignals(true);

    ui->comboBoxFile->clear();
    QList<QString> listFiles;

    QString sPath = XBinary::convertPathName(getGlobalOptions()->getSearchSignaturesPath());

    if (sPath != "") {
        listFiles.append(XBinary::getAllFilesFromDirectory(sPath + QDir::separator() + "generic", "*.db"));
        listFiles.append(XBinary::getAllFilesFromDirectory(sPath + QDir::separator() + sArch, "*.db"));
    }

    qint32 nNumberOfFiles = listFiles.count();

    for (qint32 i = 0; i < nNumberOfFiles; i++) {
        QString sFileName = listFiles.at(i);
        ui->comboBoxFile->addItem(XBinary::getBaseFileName(sFileName), listFiles.at(i));
    }

    if (nNumberOfFiles) {
        loadSignatures(ui->comboBoxFile->currentData().toString());
    }

    ui->comboBoxFile->blockSignals(bBlocked1);
}

void SearchSignaturesWidget::reload()
{
    search();
}

bool SearchSignaturesWidget::getInitStatus()
{
    return g_bInit;
}

void SearchSignaturesWidget::adjustView()
{
    updateSignaturesPath();

    getGlobalOptions()->adjustWidget(this, XOptions::ID_VIEW_FONT_CONTROLS);
    getGlobalOptions()->adjustTableView(ui->tableViewResult, XOptions::ID_VIEW_FONT_TABLEVIEWS);
    ui->tableViewResult->adjust();
}

void SearchSignaturesWidget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)
    search();
}

void SearchSignaturesWidget::on_toolButtonSave_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableViewResult->getProxyModel(), XBinary::getResultFileName(g_pDevice, QString("%1.txt").arg(tr("Signatures"))));
}

void SearchSignaturesWidget::on_toolButtonSearch_clicked()
{
    search();
}

void SearchSignaturesWidget::on_tableViewResult_customContextMenuRequested(const QPoint &pos)
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

    QList<QObject *> listObjects = getShortcuts()->adjustContextMenu(&contextMenu, &listMenuItems);

    contextMenu.exec(ui->tableViewResult->viewport()->mapToGlobal(pos));

    XOptions::deleteQObjectList(&listObjects);
}

void SearchSignaturesWidget::_hex()
{
    if (g_options.bMenu_Hex) {
        qint32 nRow = ui->tableViewResult->currentIndex().row();

        if ((nRow != -1) && (g_listRecords.count())) {
            QModelIndex index = ui->tableViewResult->selectionModel()->selectedIndexes().at(0);

            qint64 nOffset = ui->tableViewResult->model()->data(index, Qt::UserRole + XModel_MSRecord::USERROLE_OFFSET).toLongLong();
            qint64 nSize = ui->tableViewResult->model()->data(index, Qt::UserRole + XModel_MSRecord::USERROLE_SIZE).toLongLong();

            XIODevice *pSubDevice = dynamic_cast<XIODevice *>(g_pDevice);

            if (pSubDevice) {
                nOffset += pSubDevice->getInitLocation();
            }

            emit followLocation(nOffset, XBinary::LT_OFFSET, nSize, XOptions::WIDGETTYPE_HEX);
        }
    }
}

void SearchSignaturesWidget::_disasm()
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

void SearchSignaturesWidget::search()
{
    if (g_pDevice) {
        // ui->tableViewResult->setModel(nullptr);

        XBinary::FT fileType = (XBinary::FT)(ui->comboBoxType->currentData().toInt());

        MultiSearch::OPTIONS options = {};

        options.bMenu_Hex = g_options.bMenu_Hex;
        options.memoryMap = XFormats::getMemoryMap(fileType, XBinary::MAPMODE_UNKNOWN, g_pDevice);
        options.endian = (XBinary::ENDIAN)(ui->comboBoxEndianness->currentData().toUInt());
        options.pListSignatureRecords = &g_listSignatureRecords;

        QWidget *pParent = XOptions::getMainWidget(this);

        DialogMultiSearchProcess dsp(pParent);
        dsp.setGlobal(getShortcuts(), getGlobalOptions());
        dsp.processSearch(g_pDevice, &g_listRecords, options, MultiSearch::TYPE_SIGNATURES);
        dsp.showDialogDelay();

        // DialogMultiSearchProcess dmp(pParent);
        // dmp.setGlobal(getShortcuts(), getGlobalOptions());
        // dmp.processModel(&listRecords, &g_pModel, options, MultiSearch::TYPE_SIGNATURES);
        // dmp.showDialogDelay();

        XModel_MSRecord *pModel = new XModel_MSRecord(g_pDevice, options.memoryMap, &g_listRecords, XBinary::VT_SIGNATURE, this);
        pModel->setSignaturesList(&g_listSignatureRecords);

        ui->tableViewResult->setCustomModel(pModel, true);

        connect(ui->tableViewResult->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this,
                SLOT(on_tableViewSelection(QItemSelection, QItemSelection)));

        g_bInit = true;
    }
}

void SearchSignaturesWidget::loadSignatures(const QString &sFileName)
{
    qint32 nNumberOfSignatures = 0;

    g_listSignatureRecords.clear();

    if (sFileName != "") {
        g_listSignatureRecords = MultiSearch::loadSignaturesFromFile(sFileName);
        nNumberOfSignatures = g_listSignatureRecords.count();
    }

    ui->labelInfo->setText(QString("%1: %2").arg(tr("Signatures"), QString::number(nNumberOfSignatures)));
}

void SearchSignaturesWidget::on_comboBoxFile_currentIndexChanged(int nIndex)
{
    Q_UNUSED(nIndex)

    QString sFileName = ui->comboBoxFile->currentData().toString();

    loadSignatures(sFileName);
}

void SearchSignaturesWidget::on_tableViewSelection(const QItemSelection &itemSelected, const QItemSelection &itemDeselected)
{
    Q_UNUSED(itemSelected)
    Q_UNUSED(itemDeselected)

    viewSelection();
}

void SearchSignaturesWidget::on_tableViewResult_clicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    viewSelection();
}

void SearchSignaturesWidget::viewSelection()
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

void SearchSignaturesWidget::reloadFileType()
{
    if (g_pDevice) {
        XBinary::FT fileType = (XBinary::FT)(ui->comboBoxType->currentData().toInt());

        XBinary::_MEMORY_MAP memoryMap = {};

        if (g_options.fileType == XBinary::FT_REGION) {
            memoryMap = XFormats::getMemoryMap(fileType, XBinary::MAPMODE_UNKNOWN, g_pDevice, true, g_options.nStartAddress);
        } else {
            memoryMap = XFormats::getMemoryMap(fileType, XBinary::MAPMODE_UNKNOWN, g_pDevice);
        }

        XBinary::DM disasmMode = XBinary::getDisasmMode(&memoryMap);
        XFormats::setDisasmModeComboBox(disasmMode, ui->comboBoxMode);

        {
            const bool bBlocked1 = ui->comboBoxEndianness->blockSignals(true);

            ui->comboBoxEndianness->clear();

            if (disasmMode == XBinary::DM_UNKNOWN) {
                ui->comboBoxEndianness->addItem(XBinary::endianToString(XBinary::ENDIAN_LITTLE), XBinary::ENDIAN_LITTLE);
                ui->comboBoxEndianness->addItem(XBinary::endianToString(XBinary::ENDIAN_BIG), XBinary::ENDIAN_BIG);
            } else {
                ui->comboBoxEndianness->addItem(XBinary::endianToString(memoryMap.endian), memoryMap.endian);
            }

            ui->comboBoxEndianness->blockSignals(bBlocked1);
        }

        updateSignaturesPath();
    }
}

void SearchSignaturesWidget::on_comboBoxType_currentIndexChanged(int nIndex)
{
    Q_UNUSED(nIndex)

    reloadFileType();
}

void SearchSignaturesWidget::on_toolButtonPatch_clicked()
{
}
