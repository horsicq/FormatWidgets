// copyright (c) 2019-2021 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "machsectionheaderwidget.h"
#include "ui_machsectionheaderwidget.h"

MACHSectionHeaderWidget::MACHSectionHeaderWidget(QWidget *pParent):
    FormatWidget(pParent),
    ui(new Ui::MACHSectionHeaderWidget)
{
    ui->setupUi(this);
}

MACHSectionHeaderWidget::MACHSectionHeaderWidget(QIODevice *pDevice,FW_DEF::OPTIONS options,quint32 nNumber,qint64 nOffset,qint32 nType,QWidget *pParent):
    MACHSectionHeaderWidget(pParent)
{
     MACHSectionHeaderWidget::setData(pDevice,options,nNumber,nOffset,nType);

     g_ppLinedEdit=0;
     g_nLineEditSize=0;
     g_ppComboBox=0;
     g_nComboBoxSize=0;
     g_ppInvWidget=0;
     g_nInvWidgetSize=0;

     if(nType==SMACH::TYPE_mach_commands)
     {
//         g_nLineEditSize=N_IMAGE_SECTION_HEADER::__data_size+1;
//         g_nComboBoxSize=N_IMAGE_SECTION_HEADER::__CB_size;
//         g_nInvWidgetSize=N_IMAGE_SECTION_HEADER::__INV_size;
     }

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

MACHSectionHeaderWidget::~MACHSectionHeaderWidget()
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

void MACHSectionHeaderWidget::clear()
{
    reset();

    memset(g_ppLinedEdit,0,g_nLineEditSize*sizeof(XLineEditHEX *));
    memset(g_ppComboBox,0,g_nComboBoxSize*sizeof(XComboBoxEx *));
    memset(g_ppInvWidget,0,g_nInvWidgetSize*sizeof(InvWidget *));

    g_pSubDevice=nullptr;

    ui->checkBoxReadonly->setChecked(true);
}

void MACHSectionHeaderWidget::cleanup()
{

}

void MACHSectionHeaderWidget::reset()
{
    g_bInit=false;
}

void MACHSectionHeaderWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    reloadData();
}

FormatWidget::SV MACHSectionHeaderWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    return SV_NONE;
}

void MACHSectionHeaderWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(g_ppLinedEdit,g_nLineEditSize,bState);
    setComboBoxesReadOnly(g_ppComboBox,g_nComboBoxSize,bState);
}

void MACHSectionHeaderWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)g_ppLinedEdit,g_nLineEditSize,bState);
    _blockSignals((QObject **)g_ppComboBox,g_nComboBoxSize,bState);
}

void MACHSectionHeaderWidget::adjustHeaderTable(int nType, QTableWidget *pTableWidget)
{
    int nSymbolWidth=XLineEditHEX::getSymbolWidth(this);

    switch(nType)
    {

    }
}

void MACHSectionHeaderWidget::on_checkBoxReadonly_toggled(bool bChecked)
{
    setReadonly(bChecked);
}

void MACHSectionHeaderWidget::reloadData()
{
    int nType=getType();

    XMACH mach(getDevice(),getOptions().bIsImage,getOptions().nImageBase);

    if(mach.isValid())
    {
        if(!g_bInit)
        {

        }
    }
}

void MACHSectionHeaderWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget=qobject_cast<QWidget *>(sender());
    int nStype=pWidget->property("STYPE").toInt();
    int nNdata=pWidget->property("NDATA").toInt();

    switch(nStype)
    {

    }
}

void MACHSectionHeaderWidget::on_tableWidget_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow)
    Q_UNUSED(nCurrentColumn)
    Q_UNUSED(nPreviousRow)
    Q_UNUSED(nPreviousColumn)

    setHeaderTableSelection(ui->widgetHex,ui->tableWidget);
}
