// copyright (c) 2017-2019 hors<horsicq@gmail.com>
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
#include "sectionheaderwidget.h"
#include "ui_sectionheaderwidget.h"

SectionHeaderWidget::SectionHeaderWidget(QWidget *parent):
    FormatWidget(parent),
    ui(new Ui::SectionHeaderWidget)
{
    ui->setupUi(this);
}

SectionHeaderWidget::SectionHeaderWidget(QIODevice *pDevice, FormatWidget::OPTIONS *pOptions, quint32 nNumber, QWidget *parent):
    FormatWidget(pDevice,pOptions,parent),
    ui(new Ui::SectionHeaderWidget)
{
    ui->setupUi(this);

    setData(pDevice,pOptions,nNumber);
}

SectionHeaderWidget::~SectionHeaderWidget()
{
    delete ui;
}

void SectionHeaderWidget::clear()
{
    bInit=false;

    memset(lineEdit_IMAGE_SECTION_HEADER,0,sizeof lineEdit_IMAGE_SECTION_HEADER);
    memset(comboBox,0,sizeof comboBox);

    ui->checkBoxReadonly->setChecked(true);
}

void SectionHeaderWidget::setData(QIODevice *pDevice, FormatWidget::OPTIONS *pOptions, quint32 nNumber)
{
    this->nNumber=nNumber;
    clear();
    FormatWidget::setData(pDevice,pOptions);
    ui->checkBoxReadonly->setEnabled(!isReadonly());

    reloadData();
}

bool SectionHeaderWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype,int nPosition)
{
    bool bResult=false;

    quint64 nValue=vValue.toULongLong();
    QString sValue=vValue.toString();

    if(getDevice()->isWritable())
    {
        XPE pe(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

        if(pe.isValid())
        {
            switch(nStype)
            {
                case SPE::TYPE_IMAGE_SECTION_HEADER:
                    switch(nNdata)
                    {
                        case N_IMAGE_SECTION_HEADER::Name:
                            pe.setSection_NameAsString((quint32)nPosition,sValue);
                            break;

                        case N_IMAGE_SECTION_HEADER::VirtualSize:
                            pe.setSection_VirtualSize((quint32)nPosition,(quint32)nValue);
                            break;

                        case N_IMAGE_SECTION_HEADER::VirtualAddress:
                            pe.setSection_VirtualAddress((quint32)nPosition,(quint32)nValue);
                            break;

                        case N_IMAGE_SECTION_HEADER::SizeOfRawData:
                            pe.setSection_SizeOfRawData((quint32)nPosition,(quint32)nValue);
                            break;

                        case N_IMAGE_SECTION_HEADER::PointerToRawData:
                            pe.setSection_PointerToRawData((quint32)nPosition,(quint32)nValue);
                            break;

                        case N_IMAGE_SECTION_HEADER::PointerToRelocations:
                            pe.setSection_PointerToRelocations((quint32)nPosition,(quint32)nValue);
                            break;

                        case N_IMAGE_SECTION_HEADER::PointerToLinenumbers:
                            pe.setSection_PointerToLinenumbers((quint32)nPosition,(quint32)nValue);
                            break;

                        case N_IMAGE_SECTION_HEADER::NumberOfRelocations:
                            pe.setSection_NumberOfRelocations((quint32)nPosition,(quint16)nValue);
                            break;

                        case N_IMAGE_SECTION_HEADER::NumberOfLinenumbers:
                            pe.setSection_NumberOfLinenumbers((quint32)nPosition,(quint16)nValue);
                            break;

                        case N_IMAGE_SECTION_HEADER::Characteristics:
                            pe.setSection_Characteristics((quint32)nPosition,(quint32)nValue);
                            break;
                    }
            }

            bResult=true;
        }
    }

    return bResult;
}
void SectionHeaderWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(lineEdit_IMAGE_SECTION_HEADER,N_IMAGE_SECTION_HEADER::__data_size,bState);

    setComboBoxesReadOnly(comboBox,__CB_size,bState);
}

void SectionHeaderWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)lineEdit_IMAGE_SECTION_HEADER,N_IMAGE_SECTION_HEADER::__data_size,bState);

    _blockSignals((QObject **)comboBox,__CB_size,bState);
}

void SectionHeaderWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    int nSymbolWidth=getSymbolWidth();

    switch(type)
    {
        case SPE::TYPE_IMAGE_SECTION_HEADER:
            pTableWidget->setColumnWidth(0,nSymbolWidth*18);
            pTableWidget->setColumnWidth(1,nSymbolWidth*6);
            pTableWidget->setColumnWidth(2,nSymbolWidth*14);
            pTableWidget->setColumnWidth(3,nSymbolWidth*22);
            break;
    }
}

void SectionHeaderWidget::on_checkBoxReadonly_toggled(bool checked)
{
    setReadonly(checked);
}

void SectionHeaderWidget::reloadData()
{
    XPE pe(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(pe.isValid())
    {
        if(!bInit)
        {
            bInit=createHeaderTable(SPE::TYPE_IMAGE_SECTION_HEADER,ui->tableWidget_IMAGE_SECTION_HEADER,N_IMAGE_SECTION_HEADER::records,lineEdit_IMAGE_SECTION_HEADER,N_IMAGE_SECTION_HEADER::__data_size+1,nNumber);
            comboBox[CB_CHARACTERISTICS]=createComboBox(ui->tableWidget_IMAGE_SECTION_HEADER,XPE::getImageSectionHeaderFlagsS(),SPE::TYPE_IMAGE_SECTION_HEADER,N_IMAGE_SECTION_HEADER::Characteristics,XComboBoxEx::CBTYPE_FLAGS);
            comboBox[CB_ALIGH]=createComboBox(ui->tableWidget_IMAGE_SECTION_HEADER,XPE::getImageSectionHeaderAlignsS(),SPE::TYPE_IMAGE_SECTION_HEADER,N_IMAGE_SECTION_HEADER::Characteristics+1,XComboBoxEx::CBTYPE_EFLAGS,S_IMAGE_SCN_ALIGN_MASK);
        }

        blockSignals(true);

        S_IMAGE_SECTION_HEADER ish=pe.getSectionHeader(nNumber);

        //        lineEdit_IMAGE_SECTION_HEADER[N_IMAGE_SECTION_HEADER::e_magic]->setValue(ish.);

        QString sName=QString((char *)ish.Name);
        sName.resize(qMin(sName.length(),S_IMAGE_SIZEOF_SHORT_NAME));

        lineEdit_IMAGE_SECTION_HEADER[N_IMAGE_SECTION_HEADER::Name]->setStringValue(sName,S_IMAGE_SIZEOF_SHORT_NAME); // TODO Check
        lineEdit_IMAGE_SECTION_HEADER[N_IMAGE_SECTION_HEADER::VirtualSize]->setValue(ish.Misc.VirtualSize);
        lineEdit_IMAGE_SECTION_HEADER[N_IMAGE_SECTION_HEADER::VirtualAddress]->setValue(ish.VirtualAddress);
        lineEdit_IMAGE_SECTION_HEADER[N_IMAGE_SECTION_HEADER::SizeOfRawData]->setValue(ish.SizeOfRawData);
        lineEdit_IMAGE_SECTION_HEADER[N_IMAGE_SECTION_HEADER::PointerToRawData]->setValue(ish.PointerToRawData);
        lineEdit_IMAGE_SECTION_HEADER[N_IMAGE_SECTION_HEADER::PointerToRelocations]->setValue(ish.PointerToRelocations);
        lineEdit_IMAGE_SECTION_HEADER[N_IMAGE_SECTION_HEADER::PointerToLinenumbers]->setValue(ish.PointerToLinenumbers);
        lineEdit_IMAGE_SECTION_HEADER[N_IMAGE_SECTION_HEADER::NumberOfRelocations]->setValue(ish.NumberOfRelocations);
        lineEdit_IMAGE_SECTION_HEADER[N_IMAGE_SECTION_HEADER::NumberOfLinenumbers]->setValue(ish.NumberOfLinenumbers);
        lineEdit_IMAGE_SECTION_HEADER[N_IMAGE_SECTION_HEADER::Characteristics]->setValue(ish.Characteristics);

        comboBox[CB_CHARACTERISTICS]->setValue(ish.Characteristics);
        comboBox[CB_ALIGH]->setValue(ish.Characteristics);

        blockSignals(false);

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void SectionHeaderWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget=qobject_cast<QWidget *>(sender());
    int nStype=pWidget->property("STYPE").toInt();
    int nNdata=pWidget->property("NDATA").toInt();

    switch(nStype)
    {
        case SPE::TYPE_IMAGE_SECTION_HEADER:
            switch(nNdata)
            {
                case N_IMAGE_SECTION_HEADER::Characteristics:
                    lineEdit_IMAGE_SECTION_HEADER[N_IMAGE_SECTION_HEADER::Characteristics]->setValue((quint32)nValue);
                    this->comboBox[CB_CHARACTERISTICS]->setValue(nValue);
                    break;

                case N_IMAGE_SECTION_HEADER::Characteristics+1:
                    lineEdit_IMAGE_SECTION_HEADER[N_IMAGE_SECTION_HEADER::Characteristics]->setValue((quint32)nValue);
                    this->comboBox[CB_ALIGH]->setValue(nValue);
                    break;
            }

            break;
    }
}
