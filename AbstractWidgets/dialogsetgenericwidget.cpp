/* Copyright (c) 2024-2025 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "dialogsetgenericwidget.h"
#include "ui_dialogsetgenericwidget.h"

DialogSetGenericWidget::DialogSetGenericWidget(QWidget *pParent) : XShortcutsDialog(pParent), ui(new Ui::DialogSetGenericWidget)
{
    ui->setupUi(this);

    g_pDevice = nullptr;
    g_nOffset = 0;
    g_nSize = 0;

    XFormatWidget::adjustWidgetModeComboBox(ui->comboBoxMode);
}

DialogSetGenericWidget::~DialogSetGenericWidget()
{
    delete ui;
}

void DialogSetGenericWidget::setData(QIODevice *pDevice, qint64 nOffset, qint64 nSize)
{
    g_pDevice = pDevice;
    g_nOffset = nOffset;
    g_nSize = nSize;

    adjustView();
}

void DialogSetGenericWidget::adjustView()
{
    ui->lineEditOffset->setValue32_64(g_nOffset);
    ui->lineEditSize->setValue32_64(g_nSize);
}

void DialogSetGenericWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}

void DialogSetGenericWidget::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogSetGenericWidget::on_pushButtonOK_clicked()
{
    // TODO
}
