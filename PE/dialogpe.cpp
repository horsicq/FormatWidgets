/* Copyright (c) 2017-2024 hors<horsicq@gmail.com>
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
#include "dialogpe.h"

#include "ui_dialogpe.h"

DialogPE::DialogPE(QWidget *pParent) : XShortcutsDialog(pParent, true), ui(new Ui::DialogPE)
{
    ui->setupUi(this);
}

DialogPE::~DialogPE()
{
    delete ui;
}

void DialogPE::adjustView()
{
}

void DialogPE::setData(QIODevice *pDevice, FW_DEF::OPTIONS options)
{
    if (options.sTitle != "") {
        setWindowTitle(options.sTitle);
    }

    ui->widget->setData(pDevice, options, 0, 0, 0);
    ui->widget->reload();
}

void DialogPE::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    ui->widget->setGlobal(pShortcuts, pXOptions);
    XShortcutsDialog::setGlobal(pShortcuts, pXOptions);
}

void DialogPE::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
