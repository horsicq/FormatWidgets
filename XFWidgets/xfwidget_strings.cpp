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

#include "xfwidget_strings.h"

#include "ui_xfwidget_strings.h"

#include "xdialogprocess.h"
#include "xsearchprocess.h"

#include <QAbstractItemView>
#include <QAbstractItemModel>
#include <QApplication>
#include <QFile>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QVariant>
#include <QtGlobal>

namespace {
const qint32 N_MAX_RECORDS_IN_MEMORY = 100000;  // If more strings are found, the values are cached to disk
}

XFWidget_Strings::XFWidget_Strings(QWidget *pParent) : QWidget(pParent), ui(new Ui::XFWidget_Strings)
{
    ui->setupUi(this);

    m_pCurrentDevice = nullptr;
    m_bIsReadonly = false;

    ui->tableView->setThreadedFilterSortEnabled(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    applyOptionsToUi(m_options);

    connect(ui->toolButtonSearch, SIGNAL(clicked()), this, SLOT(onSearchClicked()));
    connect(ui->comboBoxFileType, SIGNAL(currentIndexChanged(int)), this, SLOT(onFileTypeIndexChanged(int)));
    connect(ui->checkBoxANSI, SIGNAL(toggled(bool)), ui->comboBoxANSICode, SLOT(setEnabled(bool)));

    updateStatusText();
}

XFWidget_Strings::~XFWidget_Strings()
{
    clear();

    delete ui;
}

void XFWidget_Strings::setData(const XBinary::INDATA &inData)
{
    setData(inData, OPTIONS());
}

void XFWidget_Strings::setData(const XBinary::INDATA &inData, const OPTIONS &options)
{
    clear();

    m_inData = inData;
    m_options = options;

    m_pCurrentDevice = XFormats::createDevice(m_inData);

    if (m_pCurrentDevice) {
        XBinary::FT fileType = XFormats::setFileTypeComboBox(m_inData.fileType, m_pCurrentDevice, ui->comboBoxFileType);
        applyOptionsToUi(m_options);
        updateMapModeComboBox(fileType, m_options.mapMode);
        reload();
    }
}

void XFWidget_Strings::clear()
{
    ui->tableView->clear();
    m_listRecords.clear();
    m_memoryMap = {};

    if (m_pCurrentDevice) {
        XFormats::removeDevice(m_pCurrentDevice, m_inData);
        m_pCurrentDevice = nullptr;
    }

    updateStatusText();
}

void XFWidget_Strings::reload()
{
    if (!m_pCurrentDevice) {
        return;
    }

    m_options = getOptionsFromUi();
    m_listRecords.clear();
    m_memoryMap = {};
    ui->tableView->clear();

    if (!(m_options.bANSI || m_options.bUTF8 || m_options.bUTF16 || m_options.bUTF32)) {
        updateStatusText();
        return;
    }

    ui->tableView->setProgressVisible(true);
    ui->tableView->setProgressRange(0, 100);
    ui->tableView->setProgressValue(0);
    ui->tableView->setProgressFormat(tr("Searching") + QString("... %p%"));

    XBinary::FT fileType = (XBinary::FT)(ui->comboBoxFileType->currentData().toUInt());

    XBinary::XFSS_OPTIONS ssOptions = {};
    ssOptions.nLimit = m_options.nLimit;
    ssOptions.nMinLenght = m_options.nMinLength;
    ssOptions.nMaxLenght = m_options.nMaxLength;
    ssOptions.bANSI = m_options.bANSI;
    ssOptions.bUTF8 = m_options.bUTF8;
    ssOptions.bUTF16 = m_options.bUTF16;
    ssOptions.bUTF32 = m_options.bUTF32;
    ssOptions.nCodepage = m_options.codepage;
    ssOptions.endian = m_options.endian;

    XBinary::INDATA inData = m_inData;
    inData.fileType = fileType;

    if (inData.inDataMode == XBinary::INDATA_MODE_DEVICE) {
        QFile *pFile = qobject_cast<QFile *>(m_pCurrentDevice);

        if (pFile) {
            inData = XFormats::createINDATA(fileType, pFile->fileName(), m_inData.bIsImage, m_inData.nModuleAddress);
        }
    }

    XBinary::XLOC location = XBinary::offsetToLoc(m_options.nOffset);

    XSearchProcess searchProcess;
    XDialogProcess dialogProcess(XOptions::getMainWidget(this), &searchProcess);
    connect(&searchProcess, &XSearchProcess::progressChanged, this, &XFWidget_Strings::onSearchProgressChanged, Qt::QueuedConnection);
    m_memoryMap = XFormats::getMemoryMap(fileType, m_options.mapMode, m_pCurrentDevice, m_inData.bIsImage, m_inData.nModuleAddress, dialogProcess.getPdStruct());
    searchProcess.setData(inData, location, m_options.nSize, ssOptions, &m_memoryMap, &m_listRecords, dialogProcess.getPdStruct());
    dialogProcess.start();
    dialogProcess.showDialogDelay();

    XModel_MSRecord *pModel = new XModel_MSRecord(inData, m_memoryMap, &m_listRecords, XBinary::VT_STRING, this);
    pModel->setValue(m_options.endian, XBinary::VT_STRING, QVariant());

    if (m_listRecords.count() > N_MAX_RECORDS_IN_MEMORY) {
        pModel->spillValuesToDisk();  // Cache the string values to disk; filters and sorts read them back on demand
    }

    ui->tableView->setCustomModel(pModel, true);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    QItemSelectionModel *pSelectionModel = ui->tableView->selectionModel();

    if (pSelectionModel) {
        connect(pSelectionModel, SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(onSelectionChanged(QModelIndex, QModelIndex)), Qt::UniqueConnection);
    }

    updateStatusText();
}

void XFWidget_Strings::setReadonly(bool bIsReadonly)
{
    m_bIsReadonly = bIsReadonly;
}

XFTableView *XFWidget_Strings::getTableView()
{
    return ui->tableView;
}

void XFWidget_Strings::onSearchClicked()
{
    reload();
}

void XFWidget_Strings::onFileTypeIndexChanged(int nIndex)
{
    Q_UNUSED(nIndex)

    XBinary::FT fileType = (XBinary::FT)(ui->comboBoxFileType->currentData().toUInt());
    updateMapModeComboBox(fileType, m_options.mapMode);
}

void XFWidget_Strings::onSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    if (!current.isValid()) {
        return;
    }

    QAbstractItemModel *pModel = ui->tableView->model();

    if (!pModel) {
        return;
    }

    QModelIndex indexNumber = current.sibling(current.row(), XModel_MSRecord::COLUMN_NUMBER);

    if (!indexNumber.isValid()) {
        return;
    }

    QVariant varOffset = pModel->data(indexNumber, Qt::UserRole + XModel_MSRecord::USERROLE_OFFSET);
    QVariant varAddress = pModel->data(indexNumber, Qt::UserRole + XModel_MSRecord::USERROLE_ADDRESS);
    QVariant varSize = pModel->data(indexNumber, Qt::UserRole + XModel_MSRecord::USERROLE_SIZE);
    QVariant varValueType = pModel->data(indexNumber, Qt::UserRole + XModel_MSRecord::USERROLE_STRING1);

    qint64 nOffset = varOffset.isValid() ? varOffset.toLongLong() : -1;
    XADDR nAddress = varAddress.isValid() ? varAddress.toULongLong() : (XADDR)-1;
    qint64 nSize = varSize.toLongLong();
    XBinary::VT valueType = (XBinary::VT)(varValueType.toUInt());

    if (nOffset != -1) {
        emit currentLocationChanged(nOffset, XBinary::LT_OFFSET, nSize);
    } else if (nAddress != (XADDR)-1) {
        emit currentLocationChanged(nAddress, XBinary::LT_ADDRESS, nSize);
    }

    emit stringSelected(nOffset, nAddress, nSize, valueType);
}

void XFWidget_Strings::onSearchProgressChanged(qint32 nValue, qint64 nCurrent, qint64 nTotal, const QString &sStatus)
{
    ui->tableView->setProgressVisible(true);
    ui->tableView->setProgressRange(0, 100);
    ui->tableView->setProgressValue(qBound(0, nValue, 100));

    QString sFormat = tr("Searching") + QString("... %p%");

    if (nTotal > 0) {
        sFormat += QString(" [%1/%2]").arg(QString::number(nCurrent), QString::number(nTotal));
    }

    if (!sStatus.isEmpty()) {
        sFormat += QString(" ") + sStatus;
    }

    ui->tableView->setProgressFormat(sFormat);
}

void XFWidget_Strings::applyOptionsToUi(const OPTIONS &options)
{
    ui->checkBoxANSI->setChecked(options.bANSI);
    XFormats::setCodepageComboBox(ui->comboBoxANSICode, options.codepage);
    ui->comboBoxANSICode->setEnabled(options.bANSI);
    ui->checkBoxUTF8->setChecked(options.bUTF8);
    ui->checkBoxUTF16->setChecked(options.bUTF16);
    ui->checkBoxUTF32->setChecked(options.bUTF32);
    ui->spinBoxMinLength->setValue(qMax(1, options.nMinLength));
    ui->spinBoxMaxLength->setValue(qMax(1, options.nMaxLength));
    XFormats::setEndiannessComboBox(ui->comboBoxEndian, options.endian);
}

XFWidget_Strings::OPTIONS XFWidget_Strings::getOptionsFromUi() const
{
    OPTIONS result = m_options;

    result.bANSI = ui->checkBoxANSI->isChecked();
    result.codepage = (XBinary::CODEPAGE)(ui->comboBoxANSICode->currentData().toUInt());
    result.bUTF8 = ui->checkBoxUTF8->isChecked();
    result.bUTF16 = ui->checkBoxUTF16->isChecked();
    result.bUTF32 = ui->checkBoxUTF32->isChecked();
    result.nMinLength = ui->spinBoxMinLength->value();
    result.nMaxLength = ui->spinBoxMaxLength->value();
    result.nLimit = -1;
    result.endian = (XBinary::ENDIAN)(ui->comboBoxEndian->currentData().toUInt());
    result.mapMode = (XBinary::MAPMODE)(ui->comboBoxMapMode->currentData().toUInt());

    return result;
}

void XFWidget_Strings::updateMapModeComboBox(XBinary::FT fileType, XBinary::MAPMODE mapMode)
{
    XBinary::MAPMODE currentMapMode = XFormats::getMapModesList(fileType, ui->comboBoxMapMode);

    if (mapMode != XBinary::MAPMODE_UNKNOWN) {
        XFormats::setComboBoxCurrent(ui->comboBoxMapMode, mapMode);
    } else if (currentMapMode != XBinary::MAPMODE_UNKNOWN) {
        XFormats::setComboBoxCurrent(ui->comboBoxMapMode, currentMapMode);
    }
}

void XFWidget_Strings::updateStatusText()
{
    ui->tableView->setProgressVisible(true);
    ui->tableView->setProgressRange(0, 100);
    ui->tableView->setProgressValue(m_listRecords.isEmpty() ? 0 : 100);
    ui->tableView->setProgressFormat((QString("%1 ") + tr("strings found")).arg(m_listRecords.count()));
}
