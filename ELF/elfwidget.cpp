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
#include "elfwidget.h"
#include "ui_elfwidget.h"

ELFWidget::ELFWidget(QWidget *parent) :
    FormatWidget(parent),
    ui(new Ui::ELFWidget)
{
    ui->setupUi(this);
}

ELFWidget::ELFWidget(QIODevice *pDevice, OPTIONS *pOptions, QWidget *parent) :
    FormatWidget(pDevice,pOptions,parent),
    ui(new Ui::ELFWidget)
{
    ui->setupUi(this);

    setData(pDevice,pOptions);
}

ELFWidget::~ELFWidget()
{
    delete ui;
}

void ELFWidget::setData(QIODevice *pDevice, FormatWidget::OPTIONS *pOptions)
{
    FormatWidget::setData(pDevice,pOptions);

    memset(bInit,0,sizeof bInit);
    memset(lineEdit_Elf_Ehdr,0,sizeof lineEdit_Elf_Ehdr);
    memset(comboBox,0,sizeof comboBox);

    ui->checkBoxReadonly->setChecked(true);

    ui->widgetHex->enableHeader(true);
    ui->widgetHex->enableReadOnly(false);

    XELF elf(pDevice,getOptions()->bIsImage,getOptions()->nImageAddress);

    if(elf.isValid())
    {
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_HEX,"HEX"));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_Elf_Ehdr,"Elf_Ehdr"));

        // TODO Sections
        // TODO Programs

        ui->treeWidgetNavi->expandAll();

        ui->treeWidgetNavi->setCurrentItem(ui->treeWidgetNavi->topLevelItem(1));
    }
}

bool ELFWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition)
{
    bool bResult=false;

    blockSignals(true);

    quint64 nValue=vValue.toULongLong();

    if(getDevice()->isWritable())
    {
        XELF elf(getDevice(),getOptions()->bIsImage,getOptions()->nImageAddress);
        if(elf.isValid())
        {
            switch(nStype)
            {
            case SELF::TYPE_Elf_Ehdr:
                switch(nNdata)
                {
                case N_Elf_Ehdr::ei_mag_0:
                    elf.setIdent_mag((quint8)nValue,0);
                    break;
                case N_Elf_Ehdr::ei_mag_1:
                    elf.setIdent_mag((quint8)nValue,1);
                    break;
                case N_Elf_Ehdr::ei_mag_2:
                    elf.setIdent_mag((quint8)nValue,2);
                    break;
                case N_Elf_Ehdr::ei_mag_3:
                    elf.setIdent_mag((quint8)nValue,3);
                    break;
                case N_Elf_Ehdr::ei_class:
                    comboBox[CB_Elf_Ehdr_iclass]->setValue(nValue);
                    elf.setIdent_class((quint8)nValue);
                    break;
                case N_Elf_Ehdr::ei_data:
                    comboBox[CB_Elf_Ehdr_idata]->setValue(nValue);
                    elf.setIdent_data((quint8)nValue);
                    break;
                case N_Elf_Ehdr::ei_version:
                    comboBox[CB_Elf_Ehdr_iversion]->setValue(nValue);
                    elf.setIdent_version((quint8)nValue);
                    break;
                case N_Elf_Ehdr::ei_osabi:
                    comboBox[CB_Elf_Ehdr_iosabi]->setValue(nValue);
                    elf.setIdent_osabi((quint8)nValue);
                    break;
                case N_Elf_Ehdr::ei_abiversion:
                    elf.setIdent_abiversion((quint8)nValue);
                    break;
                case N_Elf_Ehdr::ei_pad_0:
                    elf.setIdent_pad((quint8)nValue,0);
                    break;
                case N_Elf_Ehdr::ei_pad_1:
                    elf.setIdent_pad((quint8)nValue,1);
                    break;
                case N_Elf_Ehdr::ei_pad_2:
                    elf.setIdent_pad((quint8)nValue,2);
                    break;
                case N_Elf_Ehdr::ei_pad_3:
                    elf.setIdent_pad((quint8)nValue,3);
                    break;
                case N_Elf_Ehdr::ei_pad_4:
                    elf.setIdent_pad((quint8)nValue,4);
                    break;
                case N_Elf_Ehdr::ei_pad_5:
                    elf.setIdent_pad((quint8)nValue,5);
                    break;
                case N_Elf_Ehdr::ei_pad_6:
                    elf.setIdent_pad((quint8)nValue,6);
                    break;
                case N_Elf_Ehdr::ei_pad_7:
                    elf.setIdent_pad((quint8)nValue,7);
                    break;
                case N_Elf_Ehdr::ei_pad_8:
                    elf.setIdent_pad((quint8)nValue,8);
                    break;
                case N_Elf_Ehdr::e_type:
                    comboBox[CB_Elf_Ehdr_type]->setValue(nValue);
                    (elf.is64()?(elf.setHdr64_type((quint16)nValue)):(elf.setHdr32_type((quint16)nValue)));
                    break;
                case N_Elf_Ehdr::e_machine:
                    comboBox[CB_Elf_Ehdr_machine]->setValue(nValue);
                    (elf.is64()?(elf.setHdr64_machine((quint16)nValue)):(elf.setHdr32_machine((quint16)nValue)));
                    break;
                case N_Elf_Ehdr::e_version:
                    (elf.is64()?(elf.setHdr64_version((quint32)nValue)):(elf.setHdr32_version((quint32)nValue)));
                    break;
                case N_Elf_Ehdr::e_entry:
                    (elf.is64()?(elf.setHdr64_entry((quint64)nValue)):(elf.setHdr32_entry((quint32)nValue)));
                    break;
                case N_Elf_Ehdr::e_phoff:
                    (elf.is64()?(elf.setHdr64_phoff((quint64)nValue)):(elf.setHdr32_phoff((quint32)nValue)));
                    break;
                case N_Elf_Ehdr::e_shoff:
                    (elf.is64()?(elf.setHdr64_shoff((quint64)nValue)):(elf.setHdr32_shoff((quint32)nValue)));
                    break;
                case N_Elf_Ehdr::e_flags:
                    (elf.is64()?(elf.setHdr64_flags((quint32)nValue)):(elf.setHdr32_flags((quint32)nValue)));
                    break;
                case N_Elf_Ehdr::e_ehsize:
                    (elf.is64()?(elf.setHdr64_ehsize((quint16)nValue)):(elf.setHdr32_ehsize((quint16)nValue)));
                    break;
                case N_Elf_Ehdr::e_phentsize:
                    (elf.is64()?(elf.setHdr64_phentsize((quint16)nValue)):(elf.setHdr32_phentsize((quint16)nValue)));
                    break;
                case N_Elf_Ehdr::e_phnum:
                    (elf.is64()?(elf.setHdr64_phnum((quint16)nValue)):(elf.setHdr32_phnum((quint16)nValue)));
                    break;
                case N_Elf_Ehdr::e_shentsize:
                    (elf.is64()?(elf.setHdr64_shentsize((quint16)nValue)):(elf.setHdr32_shentsize((quint16)nValue)));
                    break;
                case N_Elf_Ehdr::e_shnum:
                    (elf.is64()?(elf.setHdr64_shnum((quint16)nValue)):(elf.setHdr32_shnum((quint16)nValue)));
                    break;
                case N_Elf_Ehdr::e_shstrndx:
                    (elf.is64()?(elf.setHdr64_shstrndx((quint16)nValue)):(elf.setHdr32_shstrndx((quint16)nValue)));
                    break;
                }
                break;
            }

            bResult=true;
        }
    }

    blockSignals(false);

    return bResult;
}

void ELFWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(lineEdit_Elf_Ehdr,N_Elf_Ehdr::__data_size,bState);

    setComboBoxesReadOnly(comboBox,__CB_size,bState);

    ui->widgetHex->setReadonly(bState);
}

void ELFWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)lineEdit_Elf_Ehdr,N_Elf_Ehdr::__data_size,bState);

    _blockSignals((QObject **)comboBox,__CB_size,bState);
}

void ELFWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    int nSymbolWidth=getSymbolWidth();

    switch(type)
    {
    case SELF::TYPE_Elf_Ehdr:
        pTableWidget->setColumnWidth(0,nSymbolWidth*10);
        pTableWidget->setColumnWidth(1,nSymbolWidth*13);
        pTableWidget->setColumnWidth(2,nSymbolWidth*13);
        pTableWidget->setColumnWidth(3,nSymbolWidth*13);
        break;
    }
}

void ELFWidget::reloadData()
{
    int nData=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole).toInt();
    ui->stackedWidgetInfo->setCurrentIndex(nData);

    XELF elf(getDevice(),getOptions()->bIsImage,getOptions()->nImageAddress);
    if(elf.isValid())
    {
        if(nData==SELF::TYPE_HEX)
        {
            if(!bInit[nData])
            {
                QHexView::OPTIONS options={0};

                options.nBaseAddress=getOptions()->nImageAddress;
                options.sBackupFileName=getOptions()->sBackupFileName;

                ui->widgetHex->setData(getDevice(),&options);

                bInit[nData]=true;
            }
        }
        else if(nData==SELF::TYPE_Elf_Ehdr)
        {
            if(!bInit[nData])
            {
                bInit[nData]=createHeaderTable(SELF::TYPE_Elf_Ehdr,ui->tableWidget_Elf_Ehdr,N_Elf_Ehdr::records32,lineEdit_Elf_Ehdr,N_Elf_Ehdr::__data_size,0);
                comboBox[CB_Elf_Ehdr_iclass]=createComboBox(ui->tableWidget_Elf_Ehdr,XELF::getIndentClassesS(),SELF::TYPE_Elf_Ehdr,N_Elf_Ehdr::ei_class,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_Elf_Ehdr_idata]=createComboBox(ui->tableWidget_Elf_Ehdr,XELF::getIndentDatasS(),SELF::TYPE_Elf_Ehdr,N_Elf_Ehdr::ei_data,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_Elf_Ehdr_iversion]=createComboBox(ui->tableWidget_Elf_Ehdr,XELF::getIndentVersionsS(),SELF::TYPE_Elf_Ehdr,N_Elf_Ehdr::ei_version,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_Elf_Ehdr_iosabi]=createComboBox(ui->tableWidget_Elf_Ehdr,XELF::getIndentOsabisS(),SELF::TYPE_Elf_Ehdr,N_Elf_Ehdr::ei_osabi,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_Elf_Ehdr_type]=createComboBox(ui->tableWidget_Elf_Ehdr,XELF::getTypesS(),SELF::TYPE_Elf_Ehdr,N_Elf_Ehdr::e_type,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_Elf_Ehdr_machine]=createComboBox(ui->tableWidget_Elf_Ehdr,XELF::getMachinesS(),SELF::TYPE_Elf_Ehdr,N_Elf_Ehdr::e_machine,XComboBoxEx::CBTYPE_NORMAL);
            }

            blockSignals(true);

            lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_mag_0]->setValue(elf.getIdent_mag(0));
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_mag_1]->setValue(elf.getIdent_mag(1));
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_mag_2]->setValue(elf.getIdent_mag(2));
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_mag_3]->setValue(elf.getIdent_mag(3));
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_class]->setValue(elf.getIdent_class());
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_data]->setValue(elf.getIdent_data());
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_version]->setValue(elf.getIdent_version());
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_osabi]->setValue(elf.getIdent_osabi());
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_abiversion]->setValue(elf.getIdent_abiversion());
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_0]->setValue(elf.getIdent_pad(0));
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_1]->setValue(elf.getIdent_pad(1));
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_2]->setValue(elf.getIdent_pad(2));
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_3]->setValue(elf.getIdent_pad(3));
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_4]->setValue(elf.getIdent_pad(4));
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_5]->setValue(elf.getIdent_pad(5));
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_6]->setValue(elf.getIdent_pad(6));
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_7]->setValue(elf.getIdent_pad(7));
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_pad_8]->setValue(elf.getIdent_pad(8));

            if(elf.is64())
            {
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_type]->setValue(elf.getHdr64_type());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_machine]->setValue(elf.getHdr64_machine());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_version]->setValue(elf.getHdr64_version());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_entry]->setValue(elf.getHdr64_entry());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_phoff]->setValue(elf.getHdr64_phoff());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shoff]->setValue(elf.getHdr64_shoff());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_flags]->setValue(elf.getHdr64_flags());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_ehsize]->setValue(elf.getHdr64_ehsize());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_phentsize]->setValue(elf.getHdr64_phentsize());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_phnum]->setValue(elf.getHdr64_phnum());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shentsize]->setValue(elf.getHdr64_shentsize());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shnum]->setValue(elf.getHdr64_shnum());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shstrndx]->setValue(elf.getHdr64_shstrndx());

                comboBox[CB_Elf_Ehdr_type]->setValue(elf.getHdr64_type());
                comboBox[CB_Elf_Ehdr_machine]->setValue(elf.getHdr64_machine());
            }
            else
            {
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_type]->setValue(elf.getHdr32_type());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_machine]->setValue(elf.getHdr32_machine());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_version]->setValue(elf.getHdr32_version());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_entry]->setValue(elf.getHdr32_entry());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_phoff]->setValue(elf.getHdr32_phoff());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shoff]->setValue(elf.getHdr32_shoff());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_flags]->setValue(elf.getHdr32_flags());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_ehsize]->setValue(elf.getHdr32_ehsize());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_phentsize]->setValue(elf.getHdr32_phentsize());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_phnum]->setValue(elf.getHdr32_phnum());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shentsize]->setValue(elf.getHdr32_shentsize());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shnum]->setValue(elf.getHdr32_shnum());
                lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_shstrndx]->setValue(elf.getHdr32_shstrndx());

                comboBox[CB_Elf_Ehdr_type]->setValue(elf.getHdr32_type());
                comboBox[CB_Elf_Ehdr_machine]->setValue(elf.getHdr32_machine());
            }

            comboBox[CB_Elf_Ehdr_iclass]->setValue(elf.getIdent_class());
            comboBox[CB_Elf_Ehdr_idata]->setValue(elf.getIdent_data());
            comboBox[CB_Elf_Ehdr_iversion]->setValue(elf.getIdent_version());
            comboBox[CB_Elf_Ehdr_iosabi]->setValue(elf.getIdent_osabi());

            blockSignals(false);
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void ELFWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget=qobject_cast<QWidget *>(sender());
    int nStype=pWidget->property("STYPE").toInt();
    int nNdata=pWidget->property("NDATA").toInt();

    switch(nStype)
    {
    case SELF::TYPE_Elf_Ehdr:
        switch(nNdata)
        {
        case N_Elf_Ehdr::ei_class:
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_class]->setValue((quint8)nValue);
            break;
        case N_Elf_Ehdr::ei_data:
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_data]->setValue((quint8)nValue);
            break;
        case N_Elf_Ehdr::ei_version:
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_version]->setValue((quint8)nValue);
            break;
        case N_Elf_Ehdr::ei_osabi:
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_osabi]->setValue((quint8)nValue);
            break;
        case N_Elf_Ehdr::e_type:
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_type]->setValue((quint16)nValue);
            break;
        case N_Elf_Ehdr::e_machine:
            lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_machine]->setValue((quint16)nValue);
            break;
        }
        break;
    }
}

void ELFWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    reloadData();
}

void ELFWidget::on_checkBoxReadonly_toggled(bool checked)
{
    setReadonly(checked);
}
