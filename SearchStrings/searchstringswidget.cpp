/* Copyright (c) 2019-2023 hors<horsicq@gmail.com>
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
#include "searchstringswidget.h"

#include "ui_searchstringswidget.h"

SearchStringsWidget::SearchStringsWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::SearchStringsWidget)
{
    ui->setupUi(this);
    g_pDevice = nullptr;
    g_pBackupDevice = nullptr;
    g_pFilter = new QSortFilterProxyModel(this);

    g_options.nBaseAddress = 0;
    g_pModel = nullptr;
    g_bInit = false;
    g_bIsReadonly = true;

    g_options = {};

    ui->checkBoxCStrings->setEnabled(false);

    ui->checkBoxAnsi->setChecked(true);
    ui->checkBoxUTF8->setChecked(false);
    ui->checkBoxUnicode->setChecked(true);
    ui->checkBoxCStrings->setChecked(false);

    ui->spinBoxMinLength->setMinimum(2);
    ui->spinBoxMinLength->setValue(5);

    memset(g_shortCuts, 0, sizeof g_shortCuts);

#if (QT_VERSION_MAJOR < 6) || defined(QT_CORE5COMPAT_LIB)
    ui->comboBoxANSICodec->addItem("");

    QList<QString> listCodePages = XOptions::getCodePages(false);

    qint32 nNumberOfRecords = listCodePages.count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        ui->comboBoxANSICodec->addItem(listCodePages.at(i));
    }
#endif

    ui->tableViewResult->installEventFilter(this);

    setReadonly(true);
    setReadonlyVisible(false);
}

SearchStringsWidget::~SearchStringsWidget()
{
    g_watcher.waitForFinished();

    delete ui;
}

void SearchStringsWidget::setData(QIODevice *pDevice, XBinary::FT fileType, OPTIONS options, bool bAuto)
{
    this->g_pDevice = pDevice;

    ui->checkBoxReadonly->setEnabled(pDevice->isWritable());

    XFormats::setFileTypeComboBox(fileType, g_pDevice, ui->comboBoxType);
    XFormats::setMapModeComboBox(fileType, pDevice, false, -1, ui->comboBoxMapMode);

    ui->checkBoxAnsi->setChecked(options.bAnsi);
    ui->checkBoxUTF8->setChecked(options.bUTF8);
    ui->checkBoxUnicode->setChecked(options.bUnicode);
    ui->checkBoxCStrings->setChecked(options.bCStrings);
    ui->checkBoxLinks->setChecked(options.bLinks);

    g_bInit = false;

    ui->tableViewResult->setModel(nullptr);

    g_options = options;

    if (g_options.nBaseAddress == -1) {
        g_options.nBaseAddress = 0;
    }

    if (bAuto) {
        search();
    }
}

void SearchStringsWidget::setBackupDevice(QIODevice *pDevice)
{
    g_pBackupDevice = pDevice;
}

QIODevice *SearchStringsWidget::getDevice()
{
    return g_pDevice;
}

QIODevice *SearchStringsWidget::getBackupDevice()
{
    QIODevice *pResult = nullptr;

    if (g_pBackupDevice) {
        pResult = g_pBackupDevice;
    } else {
        pResult = g_pDevice;
    }

    return pResult;
}

void SearchStringsWidget::reload()
{
    search();
}

bool SearchStringsWidget::getInitStatus()
{
    return g_bInit;
}

bool SearchStringsWidget::isEdited()
{
    bool bResult = XBinary::isBackupPresent(getBackupDevice());

    return bResult;
}

bool SearchStringsWidget::saveBackup()
{
    bool bResult = true;

    if ((getGlobalOptions()->isSaveBackup()) && (!isEdited())) {
        // Save backup
        bResult = XBinary::saveBackup(getBackupDevice());
    }

    return bResult;
}

void SearchStringsWidget::setReadonly(bool bState)
{
    g_bIsReadonly = bState;

    ui->checkBoxReadonly->setChecked(bState);
}

bool SearchStringsWidget::isReadonly()
{
    return g_bIsReadonly;
}

void SearchStringsWidget::setReadonlyVisible(bool bState)
{
    if (bState) {
        ui->checkBoxReadonly->show();
    } else {
        ui->checkBoxReadonly->hide();
    }
}

void SearchStringsWidget::on_pushButtonSave_clicked()
{
    if (g_pFilter) {
        XShortcutsWidget::saveTableModel(g_pFilter, XBinary::getResultFileName(g_pDevice, QString("%1.txt").arg(tr("Strings"))));
    }
}

void SearchStringsWidget::on_pushButtonSearch_clicked()
{
    search();
}

void SearchStringsWidget::on_lineEditFilter_textChanged(const QString &sText)
{
    filter(sText);
}

void SearchStringsWidget::filter(const QString &sString)
{
    g_pFilter->setFilterFixedString(sString);
    g_pFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
    g_pFilter->setFilterKeyColumn(MultiSearch::COLUMN_STRING_VALUE);
}

void SearchStringsWidget::on_tableViewResult_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextMenu(this);

    QMenu menuFollowIn(tr("Follow in"), this);
    QMenu menuEdit(tr("Edit"), this);

    contextMenu.addMenu(getShortcuts()->getRowCopyMenu(this, ui->tableViewResult));

    QAction actionHex(tr("Hex"), this);
    QAction actionDemangle(tr("Demangle"), this);

    if (g_options.bMenu_Hex) {
        actionHex.setShortcut(getShortcuts()->getShortcut(X_ID_STRINGS_FOLLOWIN_HEX));
        connect(&actionHex, SIGNAL(triggered()), this, SLOT(_hex()));

        menuFollowIn.addAction(&actionHex);
    }

    if (g_options.bMenu_Demangle) {
        actionDemangle.setShortcut(getShortcuts()->getShortcut(X_ID_STRINGS_DEMANGLE));
        connect(&actionDemangle, SIGNAL(triggered()), this, SLOT(_demangle()));
        contextMenu.addAction(&actionDemangle);
    }

    if (g_options.bMenu_Hex) {
        contextMenu.addMenu(&menuFollowIn);
    }

    QAction actionEditString(tr("String"), this);
    actionEditString.setShortcut(getShortcuts()->getShortcut(X_ID_STRINGS_EDIT_STRING));
    connect(&actionEditString, SIGNAL(triggered()), this, SLOT(_editString()));

    menuEdit.addAction(&actionEditString);
    menuEdit.setEnabled(!isReadonly());

    contextMenu.addMenu(&menuEdit);

    contextMenu.exec(ui->tableViewResult->viewport()->mapToGlobal(pos));
}

void SearchStringsWidget::_hex()
{
    int nRow = ui->tableViewResult->currentIndex().row();

    if ((nRow != -1) && (g_pModel)) {
        QModelIndex index = ui->tableViewResult->selectionModel()->selectedIndexes().at(MultiSearch::COLUMN_STRING_NUMBER);

        qint64 nOffset = ui->tableViewResult->model()->data(index, Qt::UserRole + MultiSearch::USERROLE_OFFSET).toLongLong();
        qint64 nSize = ui->tableViewResult->model()->data(index, Qt::UserRole + MultiSearch::USERROLE_SIZE).toLongLong();

        XIODevice *pSubDevice = dynamic_cast<XIODevice *>(g_pDevice);

        if (pSubDevice) {
            nOffset += pSubDevice->getInitLocation();
        }

        emit showHex(nOffset, nSize);
    }
}

void SearchStringsWidget::_demangle()
{
    int nRow = ui->tableViewResult->currentIndex().row();

    if ((nRow != -1) && (g_pModel)) {
        QModelIndex index = ui->tableViewResult->selectionModel()->selectedIndexes().at(MultiSearch::COLUMN_STRING_VALUE);

        QString sString = ui->tableViewResult->model()->data(index).toString();

        emit showDemangle(sString);
    }
}

void SearchStringsWidget::_editString()
{
    if (!isReadonly()) {
        QModelIndex indexNumber = ui->tableViewResult->selectionModel()->selectedIndexes().at(MultiSearch::COLUMN_STRING_NUMBER);
        // QModelIndex indexOffset = ui->tableViewResult->selectionModel()->selectedIndexes().at(MultiSearch::COLUMN_STRING_OFFSET);
        QModelIndex indexSize = ui->tableViewResult->selectionModel()->selectedIndexes().at(MultiSearch::COLUMN_STRING_SIZE);
        QModelIndex indexType = ui->tableViewResult->selectionModel()->selectedIndexes().at(MultiSearch::COLUMN_STRING_TYPE);
        QModelIndex indexValue = ui->tableViewResult->selectionModel()->selectedIndexes().at(MultiSearch::COLUMN_STRING_VALUE);

        DialogEditString::DATA_STRUCT dataStruct = {};

        dataStruct.nOffset = ui->tableViewResult->model()->data(indexNumber, Qt::UserRole + MultiSearch::USERROLE_OFFSET).toLongLong();
        dataStruct.nSize = ui->tableViewResult->model()->data(indexNumber, Qt::UserRole + MultiSearch::USERROLE_SIZE).toLongLong();
        dataStruct.recordType = (XBinary::MS_RECORD_TYPE)(ui->tableViewResult->model()->data(indexNumber, Qt::UserRole + MultiSearch::USERROLE_TYPE).toLongLong());
        dataStruct.bIsCStrings = false;

        dataStruct.sString = ui->tableViewResult->model()->data(indexValue).toString();

        DialogEditString dialogEditString(this, g_pDevice, &dataStruct);

        if (dialogEditString.exec() == QDialog::Accepted)  // TODO use status
        {
            bool bSuccess = false;

            if (saveBackup()) {
                if (XBinary::write_array(g_pDevice, dataStruct.nOffset, XBinary::getStringData(dataStruct.recordType, dataStruct.sString, dataStruct.bIsCStrings))) {
                    ui->tableViewResult->model()->setData(indexNumber, dataStruct.nSize, Qt::UserRole + MultiSearch::USERROLE_SIZE);
                    ui->tableViewResult->model()->setData(indexNumber, dataStruct.recordType, Qt::UserRole + MultiSearch::USERROLE_TYPE);

                    ui->tableViewResult->model()->setData(indexSize, XBinary::valueToHexEx(dataStruct.sString.size()), Qt::DisplayRole);
                    ui->tableViewResult->model()->setData(indexType, XBinary::msRecordTypeIdToString(dataStruct.recordType), Qt::DisplayRole);
                    ui->tableViewResult->model()->setData(indexValue, dataStruct.sString, Qt::DisplayRole);

                    bSuccess = true;
                }
            }

            if (bSuccess) {
                emit dataChanged(dataStruct.nOffset, dataStruct.nSize);
            } else {
                QMessageBox::critical(XOptions::getMainWidget(this), tr("Error"),
                                      tr("Cannot save file") + QString(": %1").arg(XBinary::getBackupFileName(getBackupDevice())));
            }
        }
    }
}

void SearchStringsWidget::search()
{
    if (g_pDevice) {
        ui->lineEditFilter->clear();

        g_options.bAnsi = ui->checkBoxAnsi->isChecked();
        g_options.bUTF8 = ui->checkBoxUTF8->isChecked();
        g_options.bUnicode = ui->checkBoxUnicode->isChecked();
        g_options.bCStrings = ui->checkBoxCStrings->isChecked();
        g_options.sANSICodec = ui->comboBoxANSICodec->currentText();
        g_options.nMinLenght = ui->spinBoxMinLength->value();
        g_options.bLinks = ui->checkBoxLinks->isChecked();

        if (g_options.bAnsi || g_options.bUnicode || g_options.bUTF8) {
            XBinary::FT fileType = (XBinary::FT)(ui->comboBoxType->currentData().toInt());
            XBinary::MAPMODE mapMode = (XBinary::MAPMODE)(ui->comboBoxMapMode->currentData().toInt());

            MultiSearch::OPTIONS options = {};

            options.bAnsi = g_options.bAnsi;
            options.bUTF8 = g_options.bUTF8;
            options.bUnicode = g_options.bUnicode;
            options.bCStrings = g_options.bCStrings;
            options.sANSICodec = g_options.sANSICodec;
            options.bLinks = g_options.bLinks;
            options.bMenu_Hex = g_options.bMenu_Hex;
            options.nMinLenght = g_options.nMinLenght;

            if (fileType == XBinary::FT_REGION) {
                options.memoryMap = XBinary(g_pDevice, true, g_options.nBaseAddress).getMemoryMap();
            } else {
                options.memoryMap = XFormats::getMemoryMap(fileType, mapMode, g_pDevice);
            }

            g_pOldModel = g_pModel;

            g_pFilter->setSourceModel(nullptr);
            ui->tableViewResult->setModel(nullptr);

            QList<XBinary::MS_RECORD> listRecords;

            QWidget *pParent = XOptions::getMainWidget(this);

            DialogMultiSearchProcess dsp(pParent);
            dsp.processSearch(g_pDevice, &listRecords, options, MultiSearch::TYPE_STRINGS);
            dsp.showDialogDelay();

            DialogMultiSearchProcess dmp(pParent);
            dmp.processModel(&listRecords, &g_pModel, options, MultiSearch::TYPE_STRINGS);
            dmp.showDialogDelay();

            g_pFilter->setSourceModel(g_pModel);
            ui->tableViewResult->setModel(g_pFilter);

            ui->tableViewResult->setColumnWidth(MultiSearch::COLUMN_STRING_NUMBER, 80);    // TODO
            ui->tableViewResult->setColumnWidth(MultiSearch::COLUMN_STRING_OFFSET, 120);   // TODO
            ui->tableViewResult->setColumnWidth(MultiSearch::COLUMN_STRING_ADDRESS, 120);  // TODO
            ui->tableViewResult->setColumnWidth(MultiSearch::COLUMN_STRING_REGION, 120);   // TODO
            ui->tableViewResult->setColumnWidth(MultiSearch::COLUMN_STRING_SIZE, 80);      // TODO
            ui->tableViewResult->setColumnWidth(MultiSearch::COLUMN_STRING_TYPE, 30);      // TODO

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            QFuture<void> future = QtConcurrent::run(&SearchStringsWidget::deleteOldModel, this);
#else
            QFuture<void> future = QtConcurrent::run(this, &SearchStringsWidget::deleteOldModel);
#endif

            g_watcher.setFuture(future);

            //            watcher.waitForFinished();
            connect(ui->tableViewResult->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this,
                    SLOT(on_tableViewSelection(QItemSelection, QItemSelection)));
        }

        g_bInit = true;
    }
}

void SearchStringsWidget::deleteOldModel()
{
    delete g_pOldModel;
}

void SearchStringsWidget::registerShortcuts(bool bState)
{
    if (bState) {
        if (!g_shortCuts[SC_HEX]) g_shortCuts[SC_HEX] = new QShortcut(getShortcuts()->getShortcut(X_ID_STRINGS_FOLLOWIN_HEX), this, SLOT(_hex()));
        if (!g_shortCuts[SC_DEMANGLE]) g_shortCuts[SC_DEMANGLE] = new QShortcut(getShortcuts()->getShortcut(X_ID_STRINGS_DEMANGLE), this, SLOT(_demangle()));
        if (!g_shortCuts[SC_EDITSTRING]) g_shortCuts[SC_EDITSTRING] = new QShortcut(getShortcuts()->getShortcut(X_ID_STRINGS_EDIT_STRING), this, SLOT(_editString()));
    } else {
        for (qint32 i = 0; i < __SC_SIZE; i++) {
            if (g_shortCuts[i]) {
                delete g_shortCuts[i];
                g_shortCuts[i] = nullptr;
            }
        }
    }
}

void SearchStringsWidget::on_checkBoxAnsi_stateChanged(int nArg)
{
    Q_UNUSED(nArg)

    adjust();
}

void SearchStringsWidget::on_checkBoxUTF8_stateChanged(int nArg)
{
    Q_UNUSED(nArg)

    adjust();
}

void SearchStringsWidget::on_checkBoxUnicode_stateChanged(int nArg)
{
    Q_UNUSED(nArg)

    adjust();
}

void SearchStringsWidget::adjust()
{
    bool bIsANSI = ui->checkBoxAnsi->isChecked();
    bool bIsUTF8 = ui->checkBoxUTF8->isChecked();
    bool bIsUnicode = ui->checkBoxUnicode->isChecked();

    ui->comboBoxANSICodec->setEnabled(bIsANSI);

    ui->checkBoxCStrings->setEnabled(bIsANSI | bIsUTF8 | bIsUnicode);
}

void SearchStringsWidget::on_tableViewSelection(const QItemSelection &itemSelected, const QItemSelection &itemDeselected)
{
    Q_UNUSED(itemSelected)
    Q_UNUSED(itemDeselected)

    viewSelection();
}

void SearchStringsWidget::on_tableViewResult_clicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    viewSelection();
}

void SearchStringsWidget::viewSelection()
{
    QItemSelectionModel *pSelectionModel = ui->tableViewResult->selectionModel();

    if (pSelectionModel) {
        QModelIndexList listIndexes = pSelectionModel->selectedIndexes();

        if (listIndexes.count()) {
            QModelIndex indexNumber = listIndexes.at(MultiSearch::COLUMN_STRING_NUMBER);
            XADDR nVirtualAddress = ui->tableViewResult->model()->data(indexNumber, Qt::UserRole + MultiSearch::USERROLE_ADDRESS).toULongLong();
            qint64 nSize = ui->tableViewResult->model()->data(indexNumber, Qt::UserRole + MultiSearch::USERROLE_SIZE).toLongLong();

            emit currentAddressChanged(nVirtualAddress, nSize);
        }
    }
}

void SearchStringsWidget::on_checkBoxReadonly_toggled(bool bChecked)
{
    setReadonly(bChecked);
}
