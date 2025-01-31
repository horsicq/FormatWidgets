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
#include "searchsignatureswidget.h"

#include "ui_searchsignatureswidget.h"

SearchSignaturesWidget::SearchSignaturesWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::SearchSignaturesWidget)
{
    ui->setupUi(this);

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
    g_pModel = nullptr;
    g_bInit = false;

    memset(shortCuts, 0, sizeof shortCuts);

    // ui->tableViewResult->installEventFilter(this);
}

SearchSignaturesWidget::~SearchSignaturesWidget()
{
    delete ui;
}

void SearchSignaturesWidget::setData(QIODevice *pDevice, XBinary::FT fileType, OPTIONS options, bool bAuto)
{
    this->g_pDevice = pDevice;
    g_bInit = false;

    XFormats::setFileTypeComboBox(fileType, g_pDevice, ui->comboBoxType);
    XFormats::setEndiannessComboBox(ui->comboBoxEndianness, XBinary::ENDIAN_LITTLE);

    // ui->tableViewResult->setModel(nullptr);

    setOptions(options);

    if (bAuto) {
        search();
    }
}

void SearchSignaturesWidget::setOptions(SearchSignaturesWidget::OPTIONS options)
{
    g_options = options;
    adjustView();
}

SearchSignaturesWidget::OPTIONS SearchSignaturesWidget::getOptions()
{
    return g_options;
}

void SearchSignaturesWidget::updateSignaturesPath()
{
    const bool bBlocked1 = ui->comboBoxFile->blockSignals(true);

    ui->comboBoxFile->clear();

    if (g_options.sUserSignature != "") {
        ui->comboBoxFile->addItem("", g_options.sUserSignature);
    }

    QList<QString> listFiles = XBinary::getAllFilesFromDirectory(XBinary::convertPathName(getGlobalOptions()->getSearchSignaturesPath()), "*.db");

    qint32 nNumberOfFiles = listFiles.count();

    for (qint32 i = 0; i < nNumberOfFiles; i++) {
        QString sFileName = listFiles.at(i);
        ui->comboBoxFile->addItem(XBinary::getBaseFileName(sFileName),
                                  XBinary::convertPathName(getGlobalOptions()->getSearchSignaturesPath()) + QDir::separator() + sFileName);
    }

    if (g_options.sUserSignature != "") {
        loadSignatures("");
    } else if (nNumberOfFiles) {
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
    getGlobalOptions()->adjustWidget(ui->tableViewResult, XOptions::ID_VIEW_FONT_TABLEVIEWS);
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
    QMenu contextMenu(this);  // TODO

    QMenu menuCopy(tr("Copy"), this);  // TODO obsolete
    QMenu menuFollowIn(tr("Follow in"), this);

    QAction actionCopyName(tr("Name"), this);
    actionCopyName.setShortcut(getShortcuts()->getShortcut(X_ID_SIGNATURES_COPY_NAME));
    connect(&actionCopyName, SIGNAL(triggered()), this, SLOT(_copyName()));
    menuCopy.addAction(&actionCopyName);

    QAction actionCopySignature(tr("Signature"), this);
    actionCopySignature.setShortcut(getShortcuts()->getShortcut(X_ID_SIGNATURES_COPY_SIGNATURE));
    connect(&actionCopySignature, SIGNAL(triggered()), this, SLOT(_copySignature()));
    menuCopy.addAction(&actionCopySignature);

    QAction actionCopyAddress(tr("Address"), this);
    actionCopyAddress.setShortcut(getShortcuts()->getShortcut(X_ID_SIGNATURES_COPY_ADDRESS));
    connect(&actionCopyAddress, SIGNAL(triggered()), this, SLOT(_copyAddress()));
    menuCopy.addAction(&actionCopyAddress);

    QAction actionCopyOffset(tr("Offset"), this);
    actionCopyOffset.setShortcut(getShortcuts()->getShortcut(X_ID_SIGNATURES_COPY_OFFSET));
    connect(&actionCopyOffset, SIGNAL(triggered()), this, SLOT(_copyOffset()));
    menuCopy.addAction(&actionCopyOffset);

    contextMenu.addMenu(&menuCopy);

    QAction actionHex(tr("Hex"), this);

    if (g_options.bMenu_Hex) {
        actionHex.setShortcut(getShortcuts()->getShortcut(X_ID_SIGNATURES_FOLLOWIN_HEX));
        connect(&actionHex, SIGNAL(triggered()), this, SLOT(_hex()));
        menuFollowIn.addAction(&actionHex);

        contextMenu.addMenu(&menuFollowIn);
    }

    contextMenu.exec(ui->tableViewResult->viewport()->mapToGlobal(pos));
}

void SearchSignaturesWidget::_copyName()
{
    qint32 nRow = ui->tableViewResult->currentIndex().row();

    if ((nRow != -1) && (g_pModel)) {
        QModelIndex index = ui->tableViewResult->selectionModel()->selectedIndexes().at(2);

        QString sString = ui->tableViewResult->model()->data(index).toString();

        QApplication::clipboard()->setText(sString);
    }
}

void SearchSignaturesWidget::_copySignature()
{
    qint32 nRow = ui->tableViewResult->currentIndex().row();

    if ((nRow != -1) && (g_pModel)) {
        QModelIndex index = ui->tableViewResult->selectionModel()->selectedIndexes().at(0);

        // QString sString = ui->tableViewResult->model()->data(index, Qt::UserRole + XModel_MSRecord::USERROLE_STRING).toString();

        // QApplication::clipboard()->setText(sString);
    }
}

void SearchSignaturesWidget::_copyAddress()
{
    qint32 nRow = ui->tableViewResult->currentIndex().row();

    if ((nRow != -1) && (g_pModel)) {
        QModelIndex index = ui->tableViewResult->selectionModel()->selectedIndexes().at(0);

        QString sString = ui->tableViewResult->model()->data(index).toString();

        QApplication::clipboard()->setText(sString);
    }
}

void SearchSignaturesWidget::_copyOffset()
{
    qint32 nRow = ui->tableViewResult->currentIndex().row();

    if ((nRow != -1) && (g_pModel)) {
        QModelIndex index = ui->tableViewResult->selectionModel()->selectedIndexes().at(1);

        QString sString = ui->tableViewResult->model()->data(index).toString();

        QApplication::clipboard()->setText(sString);
    }
}

void SearchSignaturesWidget::_hex()
{
    qint32 nRow = ui->tableViewResult->currentIndex().row();

    if ((nRow != -1) && (g_pModel)) {
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

        QVector<XBinary::MS_RECORD> listRecords;

        QWidget *pParent = XOptions::getMainWidget(this);

        DialogMultiSearchProcess dsp(pParent);
        dsp.setGlobal(getShortcuts(), getGlobalOptions());
        dsp.processSearch(g_pDevice, &listRecords, options, MultiSearch::TYPE_SIGNATURES);
        dsp.showDialogDelay();

        // DialogMultiSearchProcess dmp(pParent);
        // dmp.setGlobal(getShortcuts(), getGlobalOptions());
        // dmp.processModel(&listRecords, &g_pModel, options, MultiSearch::TYPE_SIGNATURES);
        // dmp.showDialogDelay();

        ui->tableViewResult->setCustomModel(g_pModel, true);

        ui->tableViewResult->setColumnWidth(0, 120);  // TODO
        ui->tableViewResult->setColumnWidth(1, 120);  // TODO
        ui->tableViewResult->setColumnWidth(2, 120);  // TODO

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
    } else {
        // User signature
        g_listSignatureRecords.append(MultiSearch::createSignature(g_options.sUserSignature, g_options.sUserSignature));
        nNumberOfSignatures = 1;
    }

    ui->labelInfo->setText(QString("%1: %2").arg(tr("Signatures"), QString::number(nNumberOfSignatures)));
}

void SearchSignaturesWidget::on_comboBoxFile_currentIndexChanged(int nIndex)
{
    Q_UNUSED(nIndex)

    QString sFileName = ui->comboBoxFile->currentData().toString();

    loadSignatures(sFileName);
}

void SearchSignaturesWidget::registerShortcuts(bool bState)
{
    if (bState) {
        if (!shortCuts[SC_COPYNAME]) shortCuts[SC_COPYNAME] = new QShortcut(getShortcuts()->getShortcut(X_ID_SIGNATURES_COPY_NAME), this, SLOT(_copyName()));
        if (!shortCuts[SC_COPYSIGNATURE])
            shortCuts[SC_COPYSIGNATURE] = new QShortcut(getShortcuts()->getShortcut(X_ID_SIGNATURES_COPY_SIGNATURE), this, SLOT(_copySignature()));
        if (!shortCuts[SC_COPYADDRESS]) shortCuts[SC_COPYADDRESS] = new QShortcut(getShortcuts()->getShortcut(X_ID_SIGNATURES_COPY_ADDRESS), this, SLOT(_copyAddress()));
        if (!shortCuts[SC_COPYOFFSET]) shortCuts[SC_COPYOFFSET] = new QShortcut(getShortcuts()->getShortcut(X_ID_SIGNATURES_COPY_OFFSET), this, SLOT(_copyOffset()));
        if (!shortCuts[SC_HEX]) shortCuts[SC_HEX] = new QShortcut(getShortcuts()->getShortcut(X_ID_SIGNATURES_FOLLOWIN_HEX), this, SLOT(_hex()));
    } else {
        for (qint32 i = 0; i < __SC_SIZE; i++) {
            if (shortCuts[i]) {
                delete shortCuts[i];
                shortCuts[i] = nullptr;
            }
        }
    }
}
