/* Copyright (c) 2017-2022 hors<horsicq@gmail.com>
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
#include "dexsectionheaderwidget.h"
#include "ui_dexsectionheaderwidget.h"

DEXSectionHeaderWidget::DEXSectionHeaderWidget(QWidget *pParent):
    FormatWidget(pParent),
    ui(new Ui::DEXSectionHeaderWidget)
{
    ui->setupUi(this);
}

DEXSectionHeaderWidget::DEXSectionHeaderWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType, QWidget *pParent):
    DEXSectionHeaderWidget(pParent)
{
    DEXSectionHeaderWidget::setData(pDevice,options,nNumber,nOffset,nType);

    g_ppLinedEdit=nullptr;
    g_nLineEditSize=0;
    g_ppComboBox=nullptr;
    g_nComboBoxSize=0;
    g_ppInvWidget=nullptr;
    g_nInvWidgetSize=0;

    if(g_nLineEditSize)
    {
        g_ppLinedEdit=new PXLineEditHEX[g_nLineEditSize];
    }

    if(g_nComboBoxSize)
    {
        g_ppComboBox=new PXComboBoxEx[g_nComboBoxSize];
    }

    if(g_nInvWidgetSize)
    {
        g_ppInvWidget=new PInvWidget[g_nInvWidgetSize];
    }
}

DEXSectionHeaderWidget::~DEXSectionHeaderWidget()
{
    if(g_ppLinedEdit)
    {
        delete[] g_ppLinedEdit;
    }

    if(g_ppComboBox)
    {
        delete[] g_ppComboBox;
    }

    if(g_ppInvWidget)
    {
        delete[] g_ppInvWidget;
    }

    delete ui;
}

void DEXSectionHeaderWidget::clear()
{
    reset();

    memset(g_ppLinedEdit,0,g_nLineEditSize*sizeof(XLineEditHEX *));
    memset(g_ppComboBox,0,g_nComboBoxSize*sizeof(XComboBoxEx *));
    memset(g_ppInvWidget,0,g_nInvWidgetSize*sizeof(InvWidget *));

    g_pSubDevice=nullptr;

    ui->checkBoxReadonly->setChecked(true);
}

void DEXSectionHeaderWidget::cleanup()
{

}

void DEXSectionHeaderWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    reloadData();
}

void DEXSectionHeaderWidget::setStringTable(qint64 nStringTableOffset, qint64 nStringTableSize)
{
    this->g_nStringTableOffset=nStringTableOffset;
    this->g_nStringTableSize=nStringTableSize;
}

FormatWidget::SV DEXSectionHeaderWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    Q_UNUSED(vValue)
    Q_UNUSED(nStype)
    Q_UNUSED(nNdata)
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)
    Q_UNUSED(nOffset)

    SV result=SV_NONE;

//    quint64 nValue=vValue.toULongLong();

    if(getDevice()->isWritable())
    {
        XDEX dex(getDevice());

        if(dex.isValid())
        {
            // TODO

            ui->widgetHex->reload();

            result=SV_EDITED;
        }
    }

    return result;
}
void DEXSectionHeaderWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(g_ppLinedEdit,g_nLineEditSize,bState);

    setComboBoxesReadOnly(g_ppComboBox,g_nComboBoxSize,bState);
}

void DEXSectionHeaderWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)g_ppLinedEdit,g_nLineEditSize,bState);

    _blockSignals((QObject **)g_ppComboBox,g_nComboBoxSize,bState);
}

void DEXSectionHeaderWidget::adjustHeaderTable(int nType, QTableWidget *pTableWidget)
{
    Q_UNUSED(nType)
    Q_UNUSED(pTableWidget)
}

void DEXSectionHeaderWidget::on_checkBoxReadonly_toggled(bool bChecked)
{
    setReadonly(bChecked);
}

void DEXSectionHeaderWidget::reloadData()
{
//    int nType=getType();

    XDEX dex(getDevice());

    if(dex.isValid())
    {       
        // TODO

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void DEXSectionHeaderWidget::widgetValueChanged(quint64 nValue)
{
    Q_UNUSED(nValue)

//    QWidget *pWidget=qobject_cast<QWidget *>(sender());
//    int nStype=pWidget->property("STYPE").toInt();
//    int nNdata=pWidget->property("NDATA").toInt();

    XDEX dex(getDevice());

    if(dex.isValid())
    {
        // TODO
    }
}

void DEXSectionHeaderWidget::on_tableWidget_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow)
    Q_UNUSED(nCurrentColumn)
    Q_UNUSED(nPreviousRow)
    Q_UNUSED(nPreviousColumn)

    setHeaderTableSelection(ui->widgetHex,ui->tableWidget);
}
