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

#include "xfwidget.h"

#include "ui_xfwidget.h"

#include <QAbstractItemModel>
#include <QFileDialog>
#include <QRegExp>

#include "xfmodel.h"

XFWidget::XFWidget(QWidget *pParent) : QWidget(pParent), ui(new Ui::XFWidget)
{
    ui->setupUi(this);

    m_pXBinary = nullptr;
    m_bIsReadonly = false;

    ui->toolBar->setVisible(false);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 2);

    connect(ui->treeView, SIGNAL(headerSelected(XBinary::XFHEADER)), this, SLOT(onHeaderSelected(XBinary::XFHEADER)));
    connect(ui->tableView, SIGNAL(fieldSelected(qint32, quint64, XBinary::XFRECORD)), this, SIGNAL(fieldSelected(qint32, quint64, XBinary::XFRECORD)));
    connect(ui->tableView, SIGNAL(fieldDoubleClicked(qint32, quint64, XBinary::XFRECORD)), this, SIGNAL(fieldDoubleClicked(qint32, quint64, XBinary::XFRECORD)));
    connect(ui->pushButtonSave, SIGNAL(clicked()), this, SLOT(onSaveClicked()));
    connect(ui->checkBoxShowOffsets, SIGNAL(toggled(bool)), this, SLOT(onShowOffsetsToggled(bool)));
    connect(ui->checkBoxShowPresentation, SIGNAL(toggled(bool)), this, SLOT(onShowPresentationToggled(bool)));
}

XFWidget::~XFWidget()
{
    delete ui;
}

void XFWidget::setData(XBinary *pXBinary, const QList<XBinary::XFHEADER> &listHeaders)
{
    m_pXBinary = pXBinary;
    m_mapHeaders.clear();

    for (qint32 i = 0; i < listHeaders.count(); i++) {
        m_mapHeaders.insert(listHeaders.at(i).sTag, listHeaders.at(i));
    }

    ui->treeView->setData(pXBinary, listHeaders);

    if (!listHeaders.isEmpty()) {
        ui->tableView->setData(pXBinary, listHeaders.at(0));

        QString sStructName;
        if (m_pXBinary) {
            sStructName = m_pXBinary->structIDToString(listHeaders.at(0).structID);
        }

        m_sCurrentTag = XBinary::xfHeaderToTag(listHeaders.at(0), sStructName, listHeaders.at(0).sParentTag);
        ui->lineEditTag->setText(m_sCurrentTag);

        bool bIsTable = (listHeaders.at(0).xfType == XBinary::XFTYPE_TABLE);
        ui->toolBar->setVisible(bIsTable);

        if (bIsTable) {
            ui->tableView->setShowOffset(ui->checkBoxShowOffsets->isChecked());
            ui->tableView->setShowPresentation(ui->checkBoxShowPresentation->isChecked());
        }
    }
}

void XFWidget::clear()
{
    ui->treeView->clear();
    ui->tableView->clear();
    m_pXBinary = nullptr;
    m_mapHeaders.clear();
}

void XFWidget::setReadonly(bool bIsReadonly)
{
    m_bIsReadonly = bIsReadonly;
}

XFTreeView *XFWidget::getTreeView()
{
    return ui->treeView;
}

XFTableView *XFWidget::getTableView()
{
    return ui->tableView;
}

void XFWidget::onHeaderSelected(const XBinary::XFHEADER &xfHeader)
{
    if (m_pXBinary) {
        ui->tableView->setData(m_pXBinary, xfHeader);
    }

    QString sStructName;
    if (m_pXBinary) {
        sStructName = m_pXBinary->structIDToString(xfHeader.structID);
    }

    m_sCurrentTag = XBinary::xfHeaderToTag(xfHeader, sStructName, xfHeader.sParentTag);
    ui->lineEditTag->setText(m_sCurrentTag);

    bool bIsTable = (xfHeader.xfType == XBinary::XFTYPE_TABLE);
    ui->toolBar->setVisible(bIsTable);

    if (bIsTable) {
        ui->tableView->setShowOffset(ui->checkBoxShowOffsets->isChecked());
        ui->tableView->setShowPresentation(ui->checkBoxShowPresentation->isChecked());
    }

    emit headerSelected(xfHeader);
}

void XFWidget::onSaveClicked()
{
    QAbstractItemModel *pModel = ui->tableView->model();

    if (pModel) {
        QString sDefaultName = m_sCurrentTag;
        sDefaultName.replace(QRegExp("[\\\\/:*?\"<>|]"), "_");

        QString sSelectedFilter;
        QString sFileName = QFileDialog::getSaveFileName(this, tr("Save"), sDefaultName, XFModel::exportAllFilters(), &sSelectedFilter);

        if (!sFileName.isEmpty()) {
            XFModel::EXPORT_FORMAT exportFormat = XFModel::filterToExportFormat(sSelectedFilter);
            XFModel::exportToFile(pModel, exportFormat, sFileName);
        }
    }
}

void XFWidget::onShowOffsetsToggled(bool bChecked)
{
    ui->tableView->setShowOffset(bChecked);
}

void XFWidget::onShowPresentationToggled(bool bChecked)
{
    ui->tableView->setShowPresentation(bChecked);
}
