/* Copyright (c) 2017-2025 hors<horsicq@gmail.com>
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
#include "dialogsearchsignatures.h"

#include "ui_dialogsearchsignatures.h"

DialogSearchSignatures::DialogSearchSignatures(QWidget *pParent) : XShortcutsDialog(pParent, true), ui(new Ui::DialogSearchSignatures)
{
    ui->setupUi(this);

    connect(ui->searchSignaturesWidget, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SIGNAL(followLocation(quint64, qint32, qint64, qint32)));
}

DialogSearchSignatures::~DialogSearchSignatures()
{
    delete ui;
}

void DialogSearchSignatures::setData(QIODevice *pDevice, SearchSignaturesWidget::OPTIONS options, bool bAuto)
{
    ui->searchSignaturesWidget->setData(pDevice, options, bAuto);
}

void DialogSearchSignatures::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    ui->searchSignaturesWidget->setGlobal(pShortcuts, pXOptions);
    XShortcutsDialog::setGlobal(pShortcuts, pXOptions);
}

void DialogSearchSignatures::adjustView()
{
    ui->searchSignaturesWidget->adjustView();
}

void DialogSearchSignatures::on_pushButtonClose_clicked()
{
    this->close();
}

void DialogSearchSignatures::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
