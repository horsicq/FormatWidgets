/* Copyright (c) 2017-2026 hors<horsicq@gmail.com>
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
#include "dialogsearchstrings.h"

#include "ui_dialogsearchstrings.h"

DialogSearchStrings::DialogSearchStrings(QWidget *pParent) : XShortcutsDialog(pParent, true), ui(new Ui::DialogSearchStrings)
{
    ui->setupUi(this);

    connect(ui->widgetSearchStrings, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SIGNAL(followLocation(quint64, qint32, qint64, qint32)));
    connect(ui->widgetSearchStrings, SIGNAL(currentLocationChanged(quint64, qint32, qint64)), this, SIGNAL(currentLocationChanged(quint64, qint32, qint64)));

    ui->widgetSearchStrings->setReadonly(true);
}

DialogSearchStrings::~DialogSearchStrings()
{
    delete ui;
}

void DialogSearchStrings::setData(QIODevice *pDevice, XBinary::FT fileType, SearchStringsWidget::OPTIONS options, bool bAuto)
{
    ui->widgetSearchStrings->setData(pDevice, fileType, options, bAuto);

    if (options.sTitle != "") {
        setWindowTitle(options.sTitle);
    }
}

void DialogSearchStrings::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    ui->widgetSearchStrings->setGlobal(pShortcuts, pXOptions);
    XShortcutsDialog::setGlobal(pShortcuts, pXOptions);
}

void DialogSearchStrings::adjustView()
{
}

void DialogSearchStrings::on_pushButtonClose_clicked()
{
    this->close();
}

void DialogSearchStrings::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
