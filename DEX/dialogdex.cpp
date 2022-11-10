/* Copyright (c) 2017-2022 hors<horsicq@gmail.com>
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
#include "dialogdex.h"

#include "ui_dialogdex.h"

DialogDEX::DialogDEX(QWidget *pParent) : XShortcutsDialog(pParent), ui(new Ui::DialogDEX) {
    ui->setupUi(this);

    setWindowFlags(Qt::Window);
}

DialogDEX::~DialogDEX() {
    delete ui;
}

void DialogDEX::setData(QIODevice *pDevice, FW_DEF::OPTIONS options) {
    if (options.sTitle != "") {
        setWindowTitle(options.sTitle);
    }

    ui->widget->setData(pDevice, options, 0, 0, 0);
    ui->widget->reload();
}

void DialogDEX::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions) {
    ui->widget->setGlobal(pShortcuts, pXOptions);
    XShortcutsDialog::setGlobal(pShortcuts, pXOptions);
}
