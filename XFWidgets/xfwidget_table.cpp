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

    connect(ui->tableView, SIGNAL(fieldSelected(qint32, QVariant, XBinary::XFRECORD)), this, SIGNAL(fieldSelected(qint32, QVariant, XBinary::XFRECORD)));
    connect(ui->tableView, SIGNAL(fieldDoubleClicked(qint32, QVariant, XBinary::XFRECORD)), this, SIGNAL(fieldDoubleClicked(qint32, QVariant, XBinary::XFRECORD)));
    connect(ui->checkBoxShowOffsets, SIGNAL(toggled(bool)), this, SLOT(onShowOffsetsToggled(bool)));
    connect(ui->checkBoxShowPresentation, SIGNAL(toggled(bool)), this, SLOT(onShowPresentationToggled(bool)));
}

XFWidget_Table::~XFWidget_Table()
{
    delete ui;
}

void XFWidget_Table::setData(const XBinary::INDATA &inData, const XBinary::XFHEADER &xfHeader)
{
    m_inData = inData;
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

void XFWidget_Table::setListMode(const QString &sTag)
{
    m_sCurrentTag = sTag;
    ui->tableView->setStatusBarText(m_sCurrentTag);
    ui->toolBar->setVisible(false);
    ui->tableView->resizeColumnsToContents();
}

void XFWidget_Table::setData(const XBinary::INDATA &inData, const QVector<XBinary::XIMPORT_STRUCT> &listImports)
{
    m_inData = inData;
    m_listImports = listImports;

    XModel_XImport *pModel = new XModel_XImport(&m_listImports, this);
    ui->tableView->setCustomModel(pModel, true);

    setListMode(tr("Import"));
}

void XFWidget_Table::setData(const XBinary::INDATA &inData, const QVector<XBinary::XEXPORT_STRUCT> &listExports)
{
    m_inData = inData;
    m_listExports = listExports;

    XModel_XExport *pModel = new XModel_XExport(&m_listExports, this);
    ui->tableView->setCustomModel(pModel, true);

    setListMode(tr("Export"));
}

void XFWidget_Table::setData(const XBinary::INDATA &inData, const QVector<XBinary::XSYMBOL_STRUCT> &listSymbols)
{
    m_inData = inData;
    m_listSymbols = listSymbols;

    XModel_XSymbol *pModel = new XModel_XSymbol(&m_listSymbols, this);
    ui->tableView->setCustomModel(pModel, true);

    setListMode(tr("Symbols"));
}

void XFWidget_Table::setData(const XBinary::INDATA &inData, const QVector<XBinary::XRESOURCE_STRUCT> &listResources)
{
    m_inData = inData;
    m_listResources = listResources;

    XModel_XResource *pModel = new XModel_XResource(&m_listResources, this);
    ui->tableView->setCustomModel(pModel, true);

    setListMode(tr("Resources"));
}

void XFWidget_Table::clear()
{
    ui->tableView->clear();
    m_sCurrentTag.clear();
    m_listImports.clear();
    m_listExports.clear();
    m_listSymbols.clear();
    m_listResources.clear();
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
