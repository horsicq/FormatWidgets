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

#include "xfwidget_header.h"

#include "ui_xfwidget_header.h"

XFWidget_Header::XFWidget_Header(QWidget *pParent) : QWidget(pParent), ui(new Ui::XFWidget_Header)
{
    ui->setupUi(this);

    m_bIsReadonly = false;

    connect(ui->tableView, SIGNAL(fieldSelected(qint32, QVariant, XBinary::XFRECORD)), this, SIGNAL(fieldSelected(qint32, QVariant, XBinary::XFRECORD)));
    connect(ui->tableView, SIGNAL(fieldDoubleClicked(qint32, QVariant, XBinary::XFRECORD)), this, SIGNAL(fieldDoubleClicked(qint32, QVariant, XBinary::XFRECORD)));
}

XFWidget_Header::~XFWidget_Header()
{
    delete ui;
}

void XFWidget_Header::setData(XBinary *pXBinary, const XBinary::XFHEADER &xfHeader)
{
    ui->tableView->setData(pXBinary, xfHeader);
}

void XFWidget_Header::clear()
{
    ui->tableView->clear();
}

void XFWidget_Header::setReadonly(bool bIsReadonly)
{
    m_bIsReadonly = bIsReadonly;
}

XFTableView *XFWidget_Header::getTableView()
{
    return ui->tableView;
}
