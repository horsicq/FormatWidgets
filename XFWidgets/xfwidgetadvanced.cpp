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

#include "xfwidgetadvanced.h"

#include "ui_xfwidgetadvanced.h"

#include <QAbstractItemModel>
#include <QFileDialog>
#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QRegularExpression>
#else
#include <QRegExp>
#endif

#include "xfmodel.h"

XFWidgetAdvanced::XFWidgetAdvanced(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::XFWidgetAdvanced)
{
    ui->setupUi(this);

    // m_pXBinary = nullptr;
    m_bIsReadonly = false;

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 2);

    connect(ui->treeView, SIGNAL(headerSelected(XBinary::XFHEADER)), this, SLOT(onHeaderSelected(XBinary::XFHEADER)));
}

XFWidgetAdvanced::~XFWidgetAdvanced()
{
    delete ui;
}

void XFWidgetAdvanced::setData(QIODevice *pDevice, const OPTIONS &options)
{
    m_pDevice = pDevice;
    m_options = options;

    reload();
    // m_pXBinary = pXBinary;
    // m_mapHeaders.clear();

    // for (qint32 i = 0; i < listHeaders.count(); i++) {
    //     m_mapHeaders.insert(listHeaders.at(i).sTag, listHeaders.at(i));
    // }

    // ui->treeView->setData(pXBinary, listHeaders);

    // if (!listHeaders.isEmpty()) {
    //     ui->tableView->setData(pXBinary, listHeaders.at(0));

    //     QString sStructName;
    //     if (m_pXBinary) {
    //         sStructName = m_pXBinary->structIDToString(listHeaders.at(0).structID);
    //     }

    //     m_sCurrentTag = XBinary::xfHeaderToTag(listHeaders.at(0), sStructName, listHeaders.at(0).sParentTag);
    //     ui->lineEditTag->setText(m_sCurrentTag);

    //     bool bIsTable = (listHeaders.at(0).xfType == XBinary::XFTYPE_TABLE);
    //     ui->toolBar->setVisible(bIsTable);

    //     if (bIsTable) {
    //         ui->tableView->setShowOffset(ui->checkBoxShowOffsets->isChecked());
    //         ui->tableView->setShowPresentation(ui->checkBoxShowPresentation->isChecked());
    //     }
    // }
}

void XFWidgetAdvanced::clear()
{
    ui->treeView->clear();
}

void XFWidgetAdvanced::reload()
{
    // TODO
}

void XFWidgetAdvanced::setReadonly(bool bIsReadonly)
{
    m_bIsReadonly = bIsReadonly;
}

void XFWidgetAdvanced::adjustView()
{
    getGlobalOptions()->adjustTreeView(ui->treeView, XOptions::ID_VIEW_FONT_TREEVIEWS);
}

void XFWidgetAdvanced::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
}

void XFWidgetAdvanced::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)
}

void XFWidgetAdvanced::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}

void XFWidgetAdvanced::onHeaderSelected(const XBinary::XFHEADER &xfHeader)
{
    // if (m_pXBinary) {
    //     ui->tableView->setData(m_pXBinary, xfHeader);
    // }

    // QString sStructName;
    // if (m_pXBinary) {
    //     sStructName = m_pXBinary->structIDToString(xfHeader.structID);
    // }

    // m_sCurrentTag = XBinary::xfHeaderToTag(xfHeader, sStructName, xfHeader.sParentTag);
    // ui->lineEditTag->setText(m_sCurrentTag);

    // bool bIsTable = (xfHeader.xfType == XBinary::XFTYPE_TABLE);
    // ui->toolBar->setVisible(bIsTable);

    // if (bIsTable) {
    //     ui->tableView->setShowOffset(ui->checkBoxShowOffsets->isChecked());
    //     ui->tableView->setShowPresentation(ui->checkBoxShowPresentation->isChecked());
    // }

    emit headerSelected(xfHeader);
}

void XFWidgetAdvanced::on_toolButtonReload_clicked()
{
    reload();
}

