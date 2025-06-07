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
#include "searchstringswidget.h"

#include "ui_searchstringswidget.h"

SearchStringsWidget::SearchStringsWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::SearchStringsWidget)
{
    ui->setupUi(this);

    addShortcut(X_ID_TABLE_FOLLOWIN_HEX, this, SLOT(_hex()));
    addShortcut(X_ID_TABLE_FOLLOWIN_DISASM, this, SLOT(_disasm()));
    addShortcut(X_ID_TABLE_DEMANGLE, this, SLOT(_demangle()));
    addShortcut(X_ID_TABLE_EDIT_STRING, this, SLOT(_editString()));

    XOptions::adjustToolButton(ui->toolButtonSave, XOptions::ICONTYPE_SAVE);
    XOptions::adjustToolButton(ui->toolButtonSearch, XOptions::ICONTYPE_SEARCH);

    ui->toolButtonSave->setToolTip(tr("Save"));
    ui->toolButtonSearch->setToolTip(tr("Search"));
    ui->comboBoxType->setToolTip(tr("Type"));
    ui->comboBoxMapMode->setToolTip(tr("Mode"));
    ui->checkBoxAnsi->setToolTip(QString("ANSI"));
    // ui->checkBoxUTF8->setToolTip(tr("UTF-8"));
    ui->checkBoxUnicode->setToolTip(QString("Unicode"));
    ui->checkBoxNullTerminated->setToolTip(tr("Null-terminated"));
    ui->checkBoxLinks->setToolTip(tr("Links"));
    ui->lineEditMask->setToolTip(tr("Mask"));
    ui->checkBoxRegExp->setToolTip(tr("Regular expression"));
    ui->spinBoxMinLength->setToolTip(tr("Min length"));
    ui->tableViewResult->setToolTip(tr("Result"));

    g_pDevice = nullptr;

    g_options.nBaseAddress = 0;
    g_bInit = false;
    g_options = {};

    ui->checkBoxNullTerminated->setEnabled(false);

    ui->lineEditMask->setEnabled(false);

    ui->checkBoxAnsi->setChecked(true);
    // ui->checkBoxUTF8->setChecked(false);
    ui->checkBoxUnicode->setChecked(true);
    ui->checkBoxNullTerminated->setChecked(false);

    ui->spinBoxMinLength->setMinimum(2);
    ui->spinBoxMinLength->setValue(5);

    // #if (QT_VERSION_MAJOR < 6) || defined(QT_CORE5COMPAT_LIB)
    //     ui->comboBoxANSICodec->addItem("");

    //     QList<QString> listCodePages = XOptions::getCodePages(false);

    //     qint32 nNumberOfRecords = listCodePages.count();

    //     for (qint32 i = 0; i < nNumberOfRecords; i++) {
    //         ui->comboBoxANSICodec->addItem(listCodePages.at(i));
    //     }
    // #endif

    // ui->tableViewResult->installEventFilter(this);

    setReadonly(true);
}

SearchStringsWidget::~SearchStringsWidget()
{
    delete ui;
}

void SearchStringsWidget::setData(QIODevice *pDevice, XBinary::FT fileType, OPTIONS options, bool bAuto)
{
    this->g_pDevice = pDevice;

    XBinary::FT _fileType = XFormats::setFileTypeComboBox(fileType, g_pDevice, ui->comboBoxType);
    XFormats::getMapModesList(_fileType, ui->comboBoxMapMode);

    ui->checkBoxAnsi->setChecked(options.bAnsi);
    // ui->checkBoxUTF8->setChecked(options.bUTF8);
    ui->checkBoxUnicode->setChecked(options.bUnicode);
    ui->checkBoxNullTerminated->setChecked(options.bNullTerminated);
    ui->checkBoxLinks->setChecked(options.bLinks);

    g_bInit = false;

    // ui->tableViewResult->setModel(nullptr);

    g_options = options;

    if (g_options.nBaseAddress == -1) {
        g_options.nBaseAddress = 0;
    }

    if (bAuto) {
        search();
    }
}

QIODevice *SearchStringsWidget::getDevice()
{
    return g_pDevice;
}

void SearchStringsWidget::reload()
{
    search();
}

bool SearchStringsWidget::getInitStatus()
{
    return g_bInit;
}

void SearchStringsWidget::adjustView()
{
    getGlobalOptions()->adjustWidget(this, XOptions::ID_VIEW_FONT_CONTROLS);
    getGlobalOptions()->adjustTableView(ui->tableViewResult, XOptions::ID_VIEW_FONT_TABLEVIEWS);
    ui->tableViewResult->adjust();
}

void SearchStringsWidget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)
    search();
}

void SearchStringsWidget::on_toolButtonSave_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableViewResult->getProxyModel(), XBinary::getResultFileName(g_pDevice, QString("%1.txt").arg(tr("Strings"))));
}

void SearchStringsWidget::on_toolButtonSearch_clicked()
{
    search();
}

void SearchStringsWidget::on_tableViewResult_customContextMenuRequested(const QPoint &pos)
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

    if (g_options.bMenu_Demangle) {
        getShortcuts()->_addMenuItem(&listMenuItems, X_ID_TABLE_DEMANGLE, this, SLOT(_demangle()), XShortcuts::GROUPID_NONE);
    }

    if (!isReadonly()) {
        getShortcuts()->_addMenuItem(&listMenuItems, X_ID_TABLE_EDIT_STRING, this, SLOT(_editString()), XShortcuts::GROUPID_EDIT);
    }

    QList<QObject *> listObjects = getShortcuts()->adjustContextMenu(&contextMenu, &listMenuItems);

    contextMenu.exec(ui->tableViewResult->viewport()->mapToGlobal(pos));

    XOptions::deleteQObjectList(&listObjects);
}

void SearchStringsWidget::_hex()
{
    if (g_options.bMenu_Hex) {
        qint32 nRow = ui->tableViewResult->currentIndex().row();

        if ((nRow != -1) && (g_listRecords.count())) {
            QModelIndex index = ui->tableViewResult->selectionModel()->selectedIndexes().at(XModel_MSRecord::COLUMN_NUMBER);

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

void SearchStringsWidget::_disasm()
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

void SearchStringsWidget::_demangle()
{
    if (g_options.bMenu_Disasm) {
        qint32 nRow = ui->tableViewResult->currentIndex().row();

        if ((nRow != -1) && (g_listRecords.count())) {
            QModelIndex index = ui->tableViewResult->selectionModel()->selectedIndexes().at(XModel_MSRecord::COLUMN_VALUE);

            QString sString = ui->tableViewResult->model()->data(index).toString();

            emit showDemangle(sString);
        }
    }
}

void SearchStringsWidget::_editString()
{
    if (!isReadonly()) {
        qint32 nRow = ui->tableViewResult->currentIndex().row();

        if ((nRow != -1) && (g_listRecords.count())) {
            QModelIndex indexNumber = ui->tableViewResult->selectionModel()->selectedIndexes().at(XModel_MSRecord::COLUMN_NUMBER);
            // QModelIndex indexOffset = ui->tableViewResult->selectionModel()->selectedIndexes().at(MultiSearch::COLUMN_STRING_OFFSET);
            QModelIndex indexSize = ui->tableViewResult->selectionModel()->selectedIndexes().at(XModel_MSRecord::COLUMN_SIZE);
            QModelIndex indexType = ui->tableViewResult->selectionModel()->selectedIndexes().at(XModel_MSRecord::COLUMN_INFO);
            QModelIndex indexValue = ui->tableViewResult->selectionModel()->selectedIndexes().at(XModel_MSRecord::COLUMN_VALUE);

            DialogEditString::DATA_STRUCT dataStruct = {};

            dataStruct.nOffset = ui->tableViewResult->model()->data(indexNumber, Qt::UserRole + XModel_MSRecord::USERROLE_OFFSET).toLongLong();
            dataStruct.nSize = ui->tableViewResult->model()->data(indexNumber, Qt::UserRole + XModel_MSRecord::USERROLE_SIZE).toLongLong();
            dataStruct.valueType = (XBinary::VT)(ui->tableViewResult->model()->data(indexNumber, Qt::UserRole + XModel_MSRecord::USERROLE_TYPE).toLongLong());
            dataStruct.bIsNullTerminated = false;

            dataStruct.sString = ui->tableViewResult->model()->data(indexValue).toString();

            DialogEditString dialogEditString(this, g_pDevice, &dataStruct);
            dialogEditString.setGlobal(getShortcuts(), getGlobalOptions());

            if (dialogEditString.exec() == QDialog::Accepted)  // TODO use status
            {
                bool bSuccess = false;

                if (XBinary::saveBackup(XBinary::getBackupDevice(getDevice()))) {
                    if (XBinary::write_array(g_pDevice, dataStruct.nOffset,
                                             XBinary::getStringData(dataStruct.valueType, dataStruct.sString, dataStruct.bIsNullTerminated))) {
                        ui->tableViewResult->model()->setData(indexNumber, dataStruct.nSize, Qt::UserRole + XModel_MSRecord::USERROLE_SIZE);
                        ui->tableViewResult->model()->setData(indexNumber, dataStruct.valueType, Qt::UserRole + XModel_MSRecord::USERROLE_TYPE);

                        ui->tableViewResult->model()->setData(indexSize, XBinary::valueToHexEx(dataStruct.sString.size()), Qt::DisplayRole);
                        ui->tableViewResult->model()->setData(indexType, XBinary::valueTypeToString(dataStruct.valueType, 0), Qt::DisplayRole);
                        ui->tableViewResult->model()->setData(indexValue, dataStruct.sString, Qt::DisplayRole);

                        bSuccess = true;
                    }
                }

                if (bSuccess) {
                    emit dataChanged(dataStruct.nOffset, dataStruct.nSize);
                } else {
                    QMessageBox::critical(XOptions::getMainWidget(this), tr("Error"),
                                          tr("Cannot save file") + QString(": %1").arg(XBinary::getBackupFileName(XBinary::getBackupDevice(getDevice()))));
                }
            }
        }
    }
}

void SearchStringsWidget::search()
{
    if (g_pDevice) {
        g_options.bAnsi = ui->checkBoxAnsi->isChecked();
        // g_options.bUTF8 = ui->checkBoxUTF8->isChecked();
        g_options.bUnicode = ui->checkBoxUnicode->isChecked();
        g_options.bNullTerminated = ui->checkBoxNullTerminated->isChecked();
        // g_options.sANSICodec = ui->comboBoxANSICodec->currentText();
        g_options.nMinLenght = ui->spinBoxMinLength->value();
        g_options.bLinks = ui->checkBoxLinks->isChecked();
        g_options.sMask = ui->lineEditMask->text().trimmed();

        if (g_options.bAnsi || g_options.bUnicode) {
            XBinary::FT fileType = (XBinary::FT)(ui->comboBoxType->currentData().toInt());
            XBinary::MAPMODE mapMode = (XBinary::MAPMODE)(ui->comboBoxMapMode->currentData().toInt());

            MultiSearch::OPTIONS options = {};

            options.bAnsi = g_options.bAnsi;
            // options.bUTF8 = g_options.bUTF8;
            options.bUnicode = g_options.bUnicode;
            options.bNullTerminated = g_options.bNullTerminated;
            // options.sANSICodec = g_options.sANSICodec;
            options.bLinks = g_options.bLinks;
            options.bMenu_Hex = g_options.bMenu_Hex;
            options.nMinLenght = g_options.nMinLenght;
            options.sMask = g_options.sMask;

            if (fileType == XBinary::FT_REGION) {
                options.memoryMap = XBinary(g_pDevice, true, g_options.nBaseAddress).getMemoryMap();
            } else {
                options.memoryMap = XFormats::getMemoryMap(fileType, mapMode, g_pDevice);
            }

            QWidget *pParent = XOptions::getMainWidget(this);

            ui->tableViewResult->clear();

            DialogMultiSearchProcess dsp(pParent);
            dsp.setGlobal(getShortcuts(), getGlobalOptions());
            dsp.processSearch(g_pDevice, &g_listRecords, options, MultiSearch::TYPE_STRINGS);
            dsp.showDialogDelay();

            // DialogMultiSearchProcess dmp(pParent);
            // dmp.setGlobal(getShortcuts(), getGlobalOptions());
            // dmp.processModel(&listRecords, &g_pModel, options, MultiSearch::TYPE_STRINGS);
            // dmp.showDialogDelay();

            XModel_MSRecord *pModel = new XModel_MSRecord(g_pDevice, options.memoryMap, &g_listRecords, XBinary::VT_STRING, this);

            ui->tableViewResult->setCustomModel(pModel, true);

            connect(ui->tableViewResult->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this,
                    SLOT(on_tableViewSelection(QItemSelection, QItemSelection)));
        }

        g_bInit = true;
    }
}

void SearchStringsWidget::on_checkBoxAnsi_stateChanged(int nArg)
{
    Q_UNUSED(nArg)

    adjust();
}

// void SearchStringsWidget::on_checkBoxUTF8_stateChanged(int nArg)
// {
//     Q_UNUSED(nArg)

//     adjust();
// }

void SearchStringsWidget::on_checkBoxUnicode_stateChanged(int nArg)
{
    Q_UNUSED(nArg)

    adjust();
}

void SearchStringsWidget::adjust()
{
    bool bIsANSI = ui->checkBoxAnsi->isChecked();
    // bool bIsUTF8 = ui->checkBoxUTF8->isChecked();
    bool bIsUnicode = ui->checkBoxUnicode->isChecked();

    // ui->comboBoxANSICodec->setEnabled(bIsANSI);

    ui->checkBoxNullTerminated->setEnabled(bIsANSI | bIsUnicode);
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

void SearchStringsWidget::on_comboBoxType_currentIndexChanged(int nIndex)
{
    Q_UNUSED(nIndex)

    XBinary::FT fileType = (XBinary::FT)(ui->comboBoxType->currentData().toInt());
    XFormats::getMapModesList(fileType, ui->comboBoxMapMode);
}

void SearchStringsWidget::on_lineEditMask_textChanged(const QString &sText)
{
    ui->toolButtonSearch->setEnabled(XBinary::isRegExpValid(sText.trimmed()));
}

void SearchStringsWidget::on_checkBoxRegExp_stateChanged(int nArg)
{
    Q_UNUSED(nArg)

    ui->lineEditMask->setEnabled(ui->checkBoxRegExp->isChecked());
}
