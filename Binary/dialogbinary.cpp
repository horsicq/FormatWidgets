/* Copyright (c) 2020-2025 hors<horsicq@gmail.com>
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
#include "dialogbinary.h"

#include "ui_dialogbinary.h"

DialogBinary::DialogBinary(QWidget *pParent) : XShortcutsDialog(pParent, true), ui(new Ui::DialogBinary)
{
    ui->setupUi(this);
}

DialogBinary::~DialogBinary()
{
    delete ui;
}

void DialogBinary::adjustView()
{
}

void DialogBinary::setData(QIODevice *pDevice, FW_DEF::OPTIONS options)
{
    ui->widget->setData(pDevice, options, 0, 0, 0);
    ui->widget->reload();
}

void DialogBinary::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    ui->widget->setGlobal(pShortcuts, pXOptions);

    XShortcutsDialog::setGlobal(pShortcuts, pXOptions);
}

void DialogBinary::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
