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
#include "dialogsearchvalues.h"

#include "ui_dialogsearchvalues.h"

DialogSearchValues::DialogSearchValues(QWidget *pParent) : XShortcutsDialog(pParent, true), ui(new Ui::DialogSearchValues)
{
    ui->setupUi(this);

    connect(ui->widgetSearchValues, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SIGNAL(followLocation(quint64, qint32, qint64, qint32)));
    connect(ui->widgetSearchValues, SIGNAL(currentLocationChanged(quint64, qint32, qint64)), this, SIGNAL(currentLocationChanged(quint64, qint32, qint64)));
}

DialogSearchValues::~DialogSearchValues()
{
    delete ui;
}

void DialogSearchValues::setData(QIODevice *pDevice, SearchValuesWidget::OPTIONS options)
{
    ui->widgetSearchValues->setData(pDevice, options);
}

void DialogSearchValues::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    ui->widgetSearchValues->setGlobal(pShortcuts, pXOptions);
    XShortcutsDialog::setGlobal(pShortcuts, pXOptions);
}

void DialogSearchValues::adjustView()
{
    ui->widgetSearchValues->adjustView();
}

void DialogSearchValues::on_pushButtonClose_clicked()
{
    this->close();
}

void DialogSearchValues::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
