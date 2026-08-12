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

#include "xfwidget_table.h"

#include "ui_xfwidget_table.h"

#include <QAbstractItemModel>
#include <QFileDialog>
#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QRegularExpression>
#else
#include <QRegExp>
#endif

#include "xfmodel.h"

XFWidget_Table::XFWidget_Table(QWidget *pParent) : QWidget(pParent), ui(new Ui::XFWidget_Table)
{
    ui->setupUi(this);

    // m_pXBinary = nullptr;
    m_bIsReadonly = false;
    m_listType = LISTTYPE_NONE;

    // Demangle picker: index 0 is Raw (no demangling, the default), then Auto
    // and every syntax XDemangle supports. Item data carries the MODE (-1 = raw).
    ui->comboBoxDemangle->addItem(tr("Raw"), (int)-1);
    QList<XDemangle::MODE> listModes = XDemangle::getSupportedModes();
    for (qint32 i = 0; i < listModes.count(); i++) {
        ui->comboBoxDemangle->addItem(XDemangle::modeIdToString(listModes.at(i)), (int)listModes.at(i));
    }

    connect(ui->tableView, SIGNAL(fieldSelected(qint32, QVariant, XBinary::XFRECORD)), this, SIGNAL(fieldSelected(qint32, QVariant, XBinary::XFRECORD)));
    connect(ui->tableView, SIGNAL(fieldDoubleClicked(qint32, QVariant, XBinary::XFRECORD)), this, SIGNAL(fieldDoubleClicked(qint32, QVariant, XBinary::XFRECORD)));
    connect(ui->checkBoxShowOffsets, SIGNAL(toggled(bool)), this, SLOT(onShowOffsetsToggled(bool)));
    connect(ui->checkBoxShowPresentation, SIGNAL(toggled(bool)), this, SLOT(onShowPresentationToggled(bool)));
    connect(ui->comboBoxDemangle, SIGNAL(currentIndexChanged(int)), this, SLOT(onDemangleModeChanged(int)));
}

XFWidget_Table::~XFWidget_Table()
{
    delete ui;
}

void XFWidget_Table::setData(const XBinary::INDATA &inData, const XBinary::XFHEADER &xfHeader)
{
    m_inData = inData;
    m_listType = LISTTYPE_NONE;

    // Struct table: show the offset/presentation checkboxes, hide the demangle picker.
    ui->toolBar->setVisible(true);
    ui->checkBoxShowOffsets->setVisible(true);
    ui->checkBoxShowPresentation->setVisible(true);
    ui->labelDemangle->setVisible(false);
    ui->comboBoxDemangle->setVisible(false);

    ui->tableView->setData(inData, xfHeader);

    QString sStructName;
    QIODevice *pDevice = XFormats::createDevice(m_inData);
    XBinary *pBinary = XFormats::createClass(m_inData.fileType, pDevice, m_inData.bIsImage, m_inData.nModuleAddress);
    if (pBinary) {
        sStructName = pBinary->structIDToString(xfHeader.structID);
        delete pBinary;
    }
    XFormats::removeDevice(pDevice, m_inData);

    m_sCurrentTag = XBinary::xfHeaderToTag(xfHeader, sStructName, xfHeader.sParentTag);
    ui->tableView->setStatusBarText(m_sCurrentTag);

    ui->tableView->setShowOffset(ui->checkBoxShowOffsets->isChecked());
    ui->tableView->setShowPresentation(ui->checkBoxShowPresentation->isChecked());
    ui->tableView->resizeColumnsToContents();
}

void XFWidget_Table::setListMode(const QString &sTag, bool bDemanglable)
{
    m_sCurrentTag = sTag;
    ui->tableView->setStatusBarText(m_sCurrentTag);

    // The struct-table checkboxes are meaningless for a flat list; the toolbar is
    // shown only when it has to host the demangle picker.
    ui->checkBoxShowOffsets->setVisible(false);
    ui->checkBoxShowPresentation->setVisible(false);
    ui->labelDemangle->setVisible(bDemanglable);
    ui->comboBoxDemangle->setVisible(bDemanglable);
    ui->toolBar->setVisible(bDemanglable);

    {
        // Reset to Raw without re-running the (already-raw) demangle pass.
        QSignalBlocker blocker(ui->comboBoxDemangle);
        ui->comboBoxDemangle->setCurrentIndex(0);
    }

    ui->tableView->resizeColumnsToContents();
}

void XFWidget_Table::onDemangleModeChanged(int nIndex)
{
    Q_UNUSED(nIndex)

    _applyDemangle();
}

void XFWidget_Table::_applyDemangle()
{
    int nMode = ui->comboBoxDemangle->currentData().toInt();
    bool bRaw = (nMode < 0);

    // Demangling is synchronous on the GUI thread; bound the number of names we
    // decode per change so a huge symbol table can't freeze the UI (rows past the
    // cap stay raw). Real import/export lists are far below this.
    const qint32 nMaxDemangle = 0x10000;

    if (m_listType == LISTTYPE_IMPORT) {
        qint32 nCount = m_listImportsRaw.count();
        for (qint32 i = 0; i < nCount; i++) {
            m_listImports[i] = m_listImportsRaw[i];
            if (!bRaw && (i < nMaxDemangle)) {
                QString sDemangled = m_demangle.demangle(m_listImportsRaw[i].sFunction, (XDemangle::MODE)nMode);
                if (!sDemangled.isEmpty()) {
                    m_listImports[i].sFunction = sDemangled;
                }
            }
        }
        XModel_XImport *pModel = new XModel_XImport(&m_listImports, this);
        ui->tableView->setCustomModel(pModel, true);
    } else if (m_listType == LISTTYPE_EXPORT) {
        qint32 nCount = m_listExportsRaw.count();
        for (qint32 i = 0; i < nCount; i++) {
            m_listExports[i] = m_listExportsRaw[i];
            if (!bRaw && (i < nMaxDemangle)) {
                QString sDemangled = m_demangle.demangle(m_listExportsRaw[i].sFunction, (XDemangle::MODE)nMode);
                if (!sDemangled.isEmpty()) {
                    m_listExports[i].sFunction = sDemangled;
                }
            }
        }
        XModel_XExport *pModel = new XModel_XExport(&m_listExports, this);
        ui->tableView->setCustomModel(pModel, true);
    } else if (m_listType == LISTTYPE_SYMBOL) {
        qint32 nCount = m_listSymbolsRaw.count();
        for (qint32 i = 0; i < nCount; i++) {
            m_listSymbols[i] = m_listSymbolsRaw[i];
            if (!bRaw && (i < nMaxDemangle)) {
                QString sDemangled = m_demangle.demangle(m_listSymbolsRaw[i].sName, (XDemangle::MODE)nMode);
                if (!sDemangled.isEmpty()) {
                    m_listSymbols[i].sName = sDemangled;
                }
            }
        }
        XModel_XSymbol *pModel = new XModel_XSymbol(&m_listSymbols, this);
        ui->tableView->setCustomModel(pModel, true);
    }

    ui->tableView->resizeColumnsToContents();
}

void XFWidget_Table::setData(const XBinary::INDATA &inData, const QVector<XBinary::XIMPORT_STRUCT> &listImports)
{
    m_inData = inData;
    m_listType = LISTTYPE_IMPORT;
    m_listImportsRaw = listImports;
    m_listImports = listImports;

    XModel_XImport *pModel = new XModel_XImport(&m_listImports, this);
    ui->tableView->setCustomModel(pModel, true);

    setListMode(tr("Import"), true);
}

void XFWidget_Table::setData(const XBinary::INDATA &inData, const QVector<XBinary::XEXPORT_STRUCT> &listExports)
{
    m_inData = inData;
    m_listType = LISTTYPE_EXPORT;
    m_listExportsRaw = listExports;
    m_listExports = listExports;

    XModel_XExport *pModel = new XModel_XExport(&m_listExports, this);
    ui->tableView->setCustomModel(pModel, true);

    setListMode(tr("Export"), true);
}

void XFWidget_Table::setData(const XBinary::INDATA &inData, const QVector<XBinary::XSYMBOL_STRUCT> &listSymbols)
{
    m_inData = inData;
    m_listType = LISTTYPE_SYMBOL;
    m_listSymbolsRaw = listSymbols;
    m_listSymbols = listSymbols;

    XModel_XSymbol *pModel = new XModel_XSymbol(&m_listSymbols, this);
    ui->tableView->setCustomModel(pModel, true);

    setListMode(tr("Symbols"), true);
}

void XFWidget_Table::setData(const XBinary::INDATA &inData, const QVector<XBinary::XRESOURCE_STRUCT> &listResources)
{
    m_inData = inData;
    m_listType = LISTTYPE_RESOURCE;
    m_listResources = listResources;

    XModel_XResource *pModel = new XModel_XResource(&m_listResources, this);
    ui->tableView->setCustomModel(pModel, true);

    setListMode(tr("Resources"), false);
}

void XFWidget_Table::clear()
{
    ui->tableView->clear();
    m_sCurrentTag.clear();
    m_listType = LISTTYPE_NONE;
    m_listImports.clear();
    m_listImportsRaw.clear();
    m_listExports.clear();
    m_listExportsRaw.clear();
    m_listSymbols.clear();
    m_listSymbolsRaw.clear();
    m_listResources.clear();
}

void XFWidget_Table::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    Q_UNUSED(pShortcuts)

    ui->tableView->adjustView(pXOptions);
}

void XFWidget_Table::setReadonly(bool bIsReadonly)
{
    m_bIsReadonly = bIsReadonly;
}

XFTableView *XFWidget_Table::getTableView()
{
    return ui->tableView;
}

void XFWidget_Table::onSaveClicked()
{
    QAbstractItemModel *pModel = ui->tableView->model();

    if (pModel) {
        QString sDefaultName = m_sCurrentTag;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        sDefaultName.replace(QRegularExpression(QStringLiteral(R"([\\/:*?"<>|])")), QStringLiteral("_"));
#else
        sDefaultName.replace(QRegExp(QStringLiteral("[\\\\/:*?\"<>|]")), QStringLiteral("_"));
#endif

        QString sSelectedFilter;
        QString sFileName = QFileDialog::getSaveFileName(this, tr("Save"), sDefaultName, XFModel::exportAllFilters(), &sSelectedFilter);

        if (!sFileName.isEmpty()) {
            XFModel::EXPORT_FORMAT exportFormat = XFModel::filterToExportFormat(sSelectedFilter);
            XFModel::exportToFile(pModel, exportFormat, sFileName);
        }
    }
}

void XFWidget_Table::onShowOffsetsToggled(bool bChecked)
{
    ui->tableView->setShowOffset(bChecked);
}

void XFWidget_Table::onShowPresentationToggled(bool bChecked)
{
    ui->tableView->setShowPresentation(bChecked);
}
