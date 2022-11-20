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
#include "petoolswidget.h"
#include "ui_petoolswidget.h"

PEToolsWidget::PEToolsWidget(QWidget *pParent) :
    XShortcutsWidget(pParent),
    ui(new Ui::PEToolsWidget)
{
    ui->setupUi(this);

    g_pPE = nullptr;
    g_bReadonly = true;
}

PEToolsWidget::~PEToolsWidget()
{
    delete ui;
}

void PEToolsWidget::setData(XPE *pPE)
{
    g_pPE = pPE;
}

void PEToolsWidget::reload()
{
    if (g_pPE) {
        // TODO
    }
}

void PEToolsWidget::setReadonly(bool bState)
{
    g_bReadonly = bState;
}

void PEToolsWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
