/* Copyright (c) 2019-2024 hors<horsicq@gmail.com>
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
#include "lesectionheaderwidget.h"

#include "ui_lesectionheaderwidget.h"

LESectionHeaderWidget::LESectionHeaderWidget(QWidget *pParent) : FormatWidget(pParent), ui(new Ui::LESectionHeaderWidget)
{
    ui->setupUi(this);
}

LESectionHeaderWidget::LESectionHeaderWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType, QWidget *pParent)
    : LESectionHeaderWidget(pParent)
{
    LESectionHeaderWidget::setData(pDevice, options, nNumber, nOffset, nType);
}

LESectionHeaderWidget::~LESectionHeaderWidget()
{
}

void LESectionHeaderWidget::clear()
{
}

void LESectionHeaderWidget::reload()
{
}

FormatWidget::SV LESectionHeaderWidget::_setValue(QVariant vValue, qint32 nStype, qint32 nNdata, qint32 nVtype, qint32 nPosition, qint64 nOffset)
{
    Q_UNUSED(vValue)
    Q_UNUSED(nStype)
    Q_UNUSED(nNdata)
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)
    Q_UNUSED(nOffset)

    return SV_NONE;
}

void LESectionHeaderWidget::setReadonly(bool bState)
{
    if (ui->checkBoxReadonly->isChecked() != bState) {
        const bool bBlocked1 = ui->checkBoxReadonly->blockSignals(true);
        ui->checkBoxReadonly->setChecked(bState);
        ui->checkBoxReadonly->blockSignals(bBlocked1);
    }
}

void LESectionHeaderWidget::blockSignals(bool bState)
{
    Q_UNUSED(bState)
}

void LESectionHeaderWidget::adjustHeaderTable(qint32 nType, QTableWidget *pTableWidget)
{
    Q_UNUSED(nType)
    Q_UNUSED(pTableWidget)
}

void LESectionHeaderWidget::on_checkBoxReadonly_toggled(bool bChecked)
{
    Q_UNUSED(bChecked)
}

void LESectionHeaderWidget::reloadData()
{
}

void LESectionHeaderWidget::on_tableWidget_Section_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow)
    Q_UNUSED(nCurrentColumn)
    Q_UNUSED(nPreviousRow)
    Q_UNUSED(nPreviousColumn)
}
