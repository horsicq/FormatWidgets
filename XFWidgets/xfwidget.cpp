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

XFWidget::XFWidget(QWidget *pParent) : QWidget(pParent)
{
    m_pXBinary = nullptr;
    m_bIsReadonly = false;

    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setContentsMargins(0, 0, 0, 0);

    m_pSplitter = new QSplitter(Qt::Horizontal, this);

    m_pTreeView = new XFTreeView(m_pSplitter);

    m_pTablePanel = new QWidget(m_pSplitter);
    QVBoxLayout *pTableLayout = new QVBoxLayout(m_pTablePanel);
    pTableLayout->setContentsMargins(0, 0, 0, 0);

    m_pTableView = new XFTableView(m_pTablePanel);
    pTableLayout->addWidget(m_pTableView);

    m_pToolBar = new QWidget(m_pTablePanel);
    QHBoxLayout *pToolBarLayout = new QHBoxLayout(m_pToolBar);
    pToolBarLayout->setContentsMargins(0, 0, 0, 0);

    m_pCheckBoxShowOffsets = new QCheckBox(tr("Show Offsets"), m_pToolBar);
    m_pCheckBoxShowOffsets->setChecked(false);
    m_pCheckBoxShowPresentation = new QCheckBox(tr("Show Presentation"), m_pToolBar);
    m_pCheckBoxShowPresentation->setChecked(true);

    pToolBarLayout->addWidget(m_pCheckBoxShowOffsets);
    pToolBarLayout->addWidget(m_pCheckBoxShowPresentation);
    pToolBarLayout->addStretch(1);

    m_pToolBar->setVisible(false);

    pTableLayout->insertWidget(0, m_pToolBar);

    QHBoxLayout *pBottomLayout = new QHBoxLayout();
    pBottomLayout->setContentsMargins(0, 0, 0, 0);

    m_pLineEditTag = new QLineEdit(m_pTablePanel);
    m_pLineEditTag->setReadOnly(true);

    m_pPushButtonSave = new QPushButton(tr("Save..."), m_pTablePanel);

    pBottomLayout->addWidget(m_pLineEditTag);
    pBottomLayout->addWidget(m_pPushButtonSave);

    pTableLayout->addLayout(pBottomLayout);

    m_pSplitter->addWidget(m_pTreeView);
    m_pSplitter->addWidget(m_pTablePanel);
    m_pSplitter->setStretchFactor(0, 1);
    m_pSplitter->setStretchFactor(1, 2);

    pLayout->addWidget(m_pSplitter);

    connect(m_pTreeView, SIGNAL(headerSelected(XBinary::XFHEADER)), this, SLOT(onHeaderSelected(XBinary::XFHEADER)));
    connect(m_pTableView, SIGNAL(fieldSelected(qint32, quint64, XBinary::XFRECORD)), this, SIGNAL(fieldSelected(qint32, quint64, XBinary::XFRECORD)));
    connect(m_pTableView, SIGNAL(fieldDoubleClicked(qint32, quint64, XBinary::XFRECORD)), this, SIGNAL(fieldDoubleClicked(qint32, quint64, XBinary::XFRECORD)));
    connect(m_pPushButtonSave, SIGNAL(clicked()), this, SLOT(onSaveClicked()));
    connect(m_pCheckBoxShowOffsets, SIGNAL(toggled(bool)), this, SLOT(onShowOffsetsToggled(bool)));
    connect(m_pCheckBoxShowPresentation, SIGNAL(toggled(bool)), this, SLOT(onShowPresentationToggled(bool)));
}

XFWidget::~XFWidget()
{
}

void XFWidget::setData(XBinary *pXBinary, const QList<XBinary::XFHEADER> &listHeaders)
{
    m_pXBinary = pXBinary;
    m_mapHeaders.clear();

    for (qint32 i = 0; i < listHeaders.count(); i++) {
        m_mapHeaders.insert(listHeaders.at(i).sTag, listHeaders.at(i));
    }

    m_pTreeView->setData(pXBinary, listHeaders);

    if (!listHeaders.isEmpty()) {
        m_pTableView->setData(pXBinary, listHeaders.at(0));

        QString sStructName;
        if (m_pXBinary) {
            sStructName = m_pXBinary->structIDToString(listHeaders.at(0).structID);
        }

        m_sCurrentTag = XBinary::xfHeaderToTag(listHeaders.at(0), sStructName, listHeaders.at(0).sParentTag);
        m_pLineEditTag->setText(m_sCurrentTag);

        bool bIsTable = (listHeaders.at(0).xfType == XBinary::XFTYPE_TABLE);
        m_pToolBar->setVisible(bIsTable);

        if (bIsTable) {
            m_pTableView->setShowOffset(m_pCheckBoxShowOffsets->isChecked());
            m_pTableView->setShowPresentation(m_pCheckBoxShowPresentation->isChecked());
        }
    }
}

void XFWidget::clear()
{
    m_pTreeView->clear();
    m_pTableView->clear();
    m_pXBinary = nullptr;
    m_mapHeaders.clear();
}

void XFWidget::setReadonly(bool bIsReadonly)
{
    m_bIsReadonly = bIsReadonly;
}

XFTreeView *XFWidget::getTreeView()
{
    return m_pTreeView;
}

XFTableView *XFWidget::getTableView()
{
    return m_pTableView;
}

void XFWidget::onHeaderSelected(const XBinary::XFHEADER &xfHeader)
{
    if (m_pXBinary) {
        m_pTableView->setData(m_pXBinary, xfHeader);
    }

    QString sStructName;
    if (m_pXBinary) {
        sStructName = m_pXBinary->structIDToString(xfHeader.structID);
    }

    m_sCurrentTag = XBinary::xfHeaderToTag(xfHeader, sStructName, xfHeader.sParentTag);
    m_pLineEditTag->setText(m_sCurrentTag);

    bool bIsTable = (xfHeader.xfType == XBinary::XFTYPE_TABLE);
    m_pToolBar->setVisible(bIsTable);

    if (bIsTable) {
        m_pTableView->setShowOffset(m_pCheckBoxShowOffsets->isChecked());
        m_pTableView->setShowPresentation(m_pCheckBoxShowPresentation->isChecked());
    }

    emit headerSelected(xfHeader);
}

void XFWidget::onSaveClicked()
{
    QAbstractItemModel *pModel = m_pTableView->model();

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
    m_pTableView->setShowOffset(bChecked);
}

void XFWidget::onShowPresentationToggled(bool bChecked)
{
    m_pTableView->setShowPresentation(bChecked);
}
