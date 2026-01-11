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
#include "dexsectionheaderwidget.h"

#include "ui_dexsectionheaderwidget.h"

DEXSectionHeaderWidget::DEXSectionHeaderWidget(QWidget *pParent) : FormatWidget(pParent), ui(new Ui::DEXSectionHeaderWidget)
{
    ui->setupUi(this);
}

DEXSectionHeaderWidget::DEXSectionHeaderWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType, QWidget *pParent)
    : DEXSectionHeaderWidget(pParent)
{
    DEXSectionHeaderWidget::setData(pDevice, options, nNumber, nOffset, nType);

    m_ppLinedEdit = nullptr;
    m_nLineEditSize = 0;
    m_ppComboBox = nullptr;
    m_nComboBoxSize = 0;
    m_ppInvWidget = nullptr;
    m_nInvWidgetSize = 0;

    if (m_nLineEditSize) {
        m_ppLinedEdit = new PXLineEditHEX[m_nLineEditSize];
    }

    if (m_nComboBoxSize) {
        m_ppComboBox = new PXComboBoxEx[m_nComboBoxSize];
    }

    if (m_nInvWidgetSize) {
        m_ppInvWidget = new PInvWidget[m_nInvWidgetSize];
    }
}

DEXSectionHeaderWidget::~DEXSectionHeaderWidget()
{
    if (m_ppLinedEdit) {
        delete[] m_ppLinedEdit;
    }

    if (m_ppComboBox) {
        delete[] m_ppComboBox;
    }

    if (m_ppInvWidget) {
        delete[] m_ppInvWidget;
    }

    delete ui;
}

void DEXSectionHeaderWidget::clear()
{
    reset();

    memset(m_ppLinedEdit, 0, m_nLineEditSize * sizeof(XLineEditHEX *));
    memset(m_ppComboBox, 0, m_nComboBoxSize * sizeof(XComboBoxEx *));
    memset(m_ppInvWidget, 0, m_nInvWidgetSize * sizeof(InvWidget *));

    m_pSubDevice = nullptr;

    ui->checkBoxReadonly->setChecked(true);
}

void DEXSectionHeaderWidget::cleanup()
{
    DEXSectionHeaderWidget::clear();
}

void DEXSectionHeaderWidget::reload()
{
    DEXSectionHeaderWidget::clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    reloadData(false);
}

void DEXSectionHeaderWidget::setStringTable(qint64 nStringTableOffset, qint64 nStringTableSize)
{
    this->m_nStringTableOffset = nStringTableOffset;
    this->m_nStringTableSize = nStringTableSize;
}

FormatWidget::SV DEXSectionHeaderWidget::_setValue(QVariant vValue, qint32 nStype, qint32 nNdata, qint32 nVtype, qint32 nPosition, qint64 nOffset)
{
    Q_UNUSED(vValue)
    Q_UNUSED(nStype)
    Q_UNUSED(nNdata)
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)
    Q_UNUSED(nOffset)

    SV result = SV_NONE;
    //    quint64 nValue=vValue.toULongLong();

    if (getDevice()->isWritable()) {
        XDEX dex(getDevice());

        if (dex.isValid()) {
            // TODO

            ui->widgetHex->reload();

            result = SV_EDITED;
        }
    }

    return result;
}
void DEXSectionHeaderWidget::setReadonly(bool bState)
{
    if (ui->checkBoxReadonly->isChecked() != bState) {
        const bool bBlocked1 = ui->checkBoxReadonly->blockSignals(true);
        ui->checkBoxReadonly->setChecked(bState);
        ui->checkBoxReadonly->blockSignals(bBlocked1);
    }

    setLineEditsReadOnly(m_ppLinedEdit, m_nLineEditSize, bState);

    setComboBoxesReadOnly(m_ppComboBox, m_nComboBoxSize, bState);
}

void DEXSectionHeaderWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)m_ppLinedEdit, m_nLineEditSize, bState);

    _blockSignals((QObject **)m_ppComboBox, m_nComboBoxSize, bState);
}

void DEXSectionHeaderWidget::adjustHeaderTable(qint32 nType, QTableWidget *pTableWidget)
{
    FormatWidget::adjustHeaderTable(nType, pTableWidget);
}

void DEXSectionHeaderWidget::on_checkBoxReadonly_toggled(bool bChecked)
{
    setReadonly(bChecked);
}

void DEXSectionHeaderWidget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)

    XDEX dex(getDevice());

    if (dex.isValid()) {
        // TODO

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void DEXSectionHeaderWidget::_widgetValueChanged(QVariant vValue)
{
    Q_UNUSED(vValue)

    XDEX dex(getDevice());

    if (dex.isValid()) {
        // TODO
    }
}

void DEXSectionHeaderWidget::on_tableWidget_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow)
    Q_UNUSED(nCurrentColumn)
    Q_UNUSED(nPreviousRow)
    Q_UNUSED(nPreviousColumn)

    setHeaderTableSelection(ui->widgetHex, ui->tableWidget);
}
