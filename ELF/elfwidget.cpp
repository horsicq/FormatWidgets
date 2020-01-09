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

ELFWidget::ELFWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, QWidget *parent) :
    FormatWidget(pDevice,pOptions,0,parent),
    ui(new Ui::ELFWidget)
{
    ui->setupUi(this);

    setData(pDevice,pOptions,0);
    reload();
}

ELFWidget::~ELFWidget()
{
    delete ui;
}

void ELFWidget::clear()
{
    memset(bInit,0,sizeof bInit);
    memset(lineEdit_Elf_Ehdr,0,sizeof lineEdit_Elf_Ehdr);
    memset(comboBox,0,sizeof comboBox);
    memset(invWidget,0,sizeof invWidget);
    memset(subDevice,0,sizeof subDevice);

    ui->checkBoxReadonly->setChecked(true);

    ui->treeWidgetNavi->clear();
}

void ELFWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XELF elf(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(elf.isValid())
    {
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_TOOLS,tr("Tools")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_Elf_Ehdr,"Elf_Ehdr"));

        QList<XELF_DEF::Elf_Shdr> listSections=elf.getElf_ShdrList();

        if(listSections.count())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_Elf_Shdr,"Sections"));
        }

        QList<XELF_DEF::Elf_Phdr> listPrograms=elf.getElf_PhdrList();

        if(listPrograms.count())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_Elf_Phdr,"Programs"));
        }

        QList<XELF::TAG_STRUCT> listTags=elf.getTagStructs();

        if(listTags.count())
        {
            QTreeWidgetItem *pItemDynamicArrayTags=createNewItem(SELF::TYPE_Elf_DynamicArrayTags,"Dynamic Array Tags");

            ui->treeWidgetNavi->addTopLevelItem(pItemDynamicArrayTags);
        }

        ui->treeWidgetNavi->expandAll();

        ui->treeWidgetNavi->setCurrentItem(ui->treeWidgetNavi->topLevelItem(1));
    }
}

bool ELFWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition)
{
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)

    bool bResult=false;

    blockSignals(true);

    quint64 nValue=vValue.toULongLong();

    if(getDevice()->isWritable())
    {
        XELF elf(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

        if(elf.isValid())
        {
            switch(nStype)
            {
                case SELF::TYPE_Elf_Ehdr:
                    switch(nNdata)
                    {
                        case N_Elf_Ehdr::ei_class:      comboBox[CB_Elf_Ehdr_iclass]->setValue(nValue);                                                             break;
                        case N_Elf_Ehdr::ei_data:       comboBox[CB_Elf_Ehdr_idata]->setValue(nValue);                                                              break;
                        case N_Elf_Ehdr::ei_version:    comboBox[CB_Elf_Ehdr_iversion]->setValue(nValue);                                                           break;
                        case N_Elf_Ehdr::ei_osabi:      comboBox[CB_Elf_Ehdr_iosabi]->setValue(nValue);                                                             break;
                        case N_Elf_Ehdr::e_type:        comboBox[CB_Elf_Ehdr_type]->setValue(nValue);                                                               break;
                        case N_Elf_Ehdr::e_machine:     comboBox[CB_Elf_Ehdr_machine]->setValue(nValue);                                                            break;
                        case N_Elf_Ehdr::e_entry:       invWidget[INV_CB_Elf_e_entry]->setAddressAndSize(&elf,elf.is64()?((quint64)nValue):((quint32)nValue),0);    break;
                    }
                    break;
            }

            switch(nStype)
            {
                case SELF::TYPE_Elf_Ehdr:
                    switch(nNdata)
                    {
                        case N_Elf_Ehdr::ei_mag_0:      elf.setIdent_mag((quint8)nValue,0);                                                                 break;
                        case N_Elf_Ehdr::ei_mag_1:      elf.setIdent_mag((quint8)nValue,1);                                                                 break;
                        case N_Elf_Ehdr::ei_mag_2:      elf.setIdent_mag((quint8)nValue,2);                                                                 break;
                        case N_Elf_Ehdr::ei_mag_3:      elf.setIdent_mag((quint8)nValue,3);                                                                 break;
                        case N_Elf_Ehdr::ei_class:      elf.setIdent_class((quint8)nValue);                                                                 break;
                        case N_Elf_Ehdr::ei_data:       elf.setIdent_data((quint8)nValue);                                                                  break;
                        case N_Elf_Ehdr::ei_version:    elf.setIdent_version((quint8)nValue);                                                               break;
                        case N_Elf_Ehdr::ei_osabi:      elf.setIdent_osabi((quint8)nValue);                                                                 break;
                        case N_Elf_Ehdr::ei_abiversion: elf.setIdent_abiversion((quint8)nValue);                                                            break;
                        case N_Elf_Ehdr::ei_pad_0:      elf.setIdent_pad((quint8)nValue,0);                                                                 break;
                        case N_Elf_Ehdr::ei_pad_1:      elf.setIdent_pad((quint8)nValue,1);                                                                 break;
                        case N_Elf_Ehdr::ei_pad_2:      elf.setIdent_pad((quint8)nValue,2);                                                                 break;
                        case N_Elf_Ehdr::ei_pad_3:      elf.setIdent_pad((quint8)nValue,3);                                                                 break;
                        case N_Elf_Ehdr::ei_pad_4:      elf.setIdent_pad((quint8)nValue,4);                                                                 break;
                        case N_Elf_Ehdr::ei_pad_5:      elf.setIdent_pad((quint8)nValue,5);                                                                 break;
                        case N_Elf_Ehdr::ei_pad_6:      elf.setIdent_pad((quint8)nValue,6);                                                                 break;
                        case N_Elf_Ehdr::e_type:        (elf.is64()?(elf.setHdr64_type((quint16)nValue)):(elf.setHdr32_type((quint16)nValue)));             break;
                        case N_Elf_Ehdr::e_machine:     (elf.is64()?(elf.setHdr64_machine((quint16)nValue)):(elf.setHdr32_machine((quint16)nValue)));       break;
                        case N_Elf_Ehdr::e_version:     (elf.is64()?(elf.setHdr64_version((quint32)nValue)):(elf.setHdr32_version((quint32)nValue)));       break;
                        case N_Elf_Ehdr::e_entry:       (elf.is64()?(elf.setHdr64_entry((quint64)nValue)):(elf.setHdr32_entry((quint32)nValue)));           break;
                        case N_Elf_Ehdr::e_phoff:       (elf.is64()?(elf.setHdr64_phoff((quint64)nValue)):(elf.setHdr32_phoff((quint32)nValue)));           break;
                        case N_Elf_Ehdr::e_shoff:       (elf.is64()?(elf.setHdr64_shoff((quint64)nValue)):(elf.setHdr32_shoff((quint32)nValue)));           break;
                        case N_Elf_Ehdr::e_flags:       (elf.is64()?(elf.setHdr64_flags((quint32)nValue)):(elf.setHdr32_flags((quint32)nValue)));           break;
                        case N_Elf_Ehdr::e_ehsize:      (elf.is64()?(elf.setHdr64_ehsize((quint16)nValue)):(elf.setHdr32_ehsize((quint16)nValue)));         break;
                        case N_Elf_Ehdr::e_phentsize:   (elf.is64()?(elf.setHdr64_phentsize((quint16)nValue)):(elf.setHdr32_phentsize((quint16)nValue)));   break;
                        case N_Elf_Ehdr::e_phnum:       (elf.is64()?(elf.setHdr64_phnum((quint16)nValue)):(elf.setHdr32_phnum((quint16)nValue)));           break;
                        case N_Elf_Ehdr::e_shentsize:   (elf.is64()?(elf.setHdr64_shentsize((quint16)nValue)):(elf.setHdr32_shentsize((quint16)nValue)));   break;
                        case N_Elf_Ehdr::e_shnum:       (elf.is64()?(elf.setHdr64_shnum((quint16)nValue)):(elf.setHdr32_shnum((quint16)nValue)));           break;
                        case N_Elf_Ehdr::e_shstrndx:    (elf.is64()?(elf.setHdr64_shstrndx((quint16)nValue)):(elf.setHdr32_shstrndx((quint16)nValue)));     break;
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

    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_NAME,QHeaderView::ResizeToContents);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_OFFSET,QHeaderView::ResizeToContents);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_TYPE,QHeaderView::ResizeToContents);

    switch(type)
    {
        case SELF::TYPE_Elf_Ehdr:
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*12);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*15);
            break;
    }
}

void ELFWidget::reloadData()
{
    int nData=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole).toInt();
    ui->stackedWidgetInfo->setCurrentIndex(nData);

    XELF elf(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(elf.isValid())
    {
        if(nData==SELF::TYPE_TOOLS)
        {
            if(!bInit[nData])
            {
                ui->widgetHex->setData(getDevice(),getOptions()); // TODO rename widget
                ui->widgetHex->setEdited(isEdited());
                connect(ui->widgetHex,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));

                bInit[nData]=true;
            }
            ui->widgetHex->reload();
        }
        else if(nData==SELF::TYPE_Elf_Ehdr)
        {
            if(!bInit[nData])
            {
                if(elf.is64())
                {
                    bInit[nData]=createHeaderTable(SELF::TYPE_Elf_Ehdr,ui->tableWidget_Elf_Ehdr,N_Elf_Ehdr::records64,lineEdit_Elf_Ehdr,N_Elf_Ehdr::__data_size,0);
                }
                else
                {
                    bInit[nData]=createHeaderTable(SELF::TYPE_Elf_Ehdr,ui->tableWidget_Elf_Ehdr,N_Elf_Ehdr::records32,lineEdit_Elf_Ehdr,N_Elf_Ehdr::__data_size,0);
                }

                comboBox[CB_Elf_Ehdr_iclass]=createComboBox(ui->tableWidget_Elf_Ehdr,XELF::getIndentClassesS(),SELF::TYPE_Elf_Ehdr,N_Elf_Ehdr::ei_class,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_Elf_Ehdr_idata]=createComboBox(ui->tableWidget_Elf_Ehdr,XELF::getIndentDatasS(),SELF::TYPE_Elf_Ehdr,N_Elf_Ehdr::ei_data,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_Elf_Ehdr_iversion]=createComboBox(ui->tableWidget_Elf_Ehdr,XELF::getIndentVersionsS(),SELF::TYPE_Elf_Ehdr,N_Elf_Ehdr::ei_version,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_Elf_Ehdr_iosabi]=createComboBox(ui->tableWidget_Elf_Ehdr,XELF::getIndentOsabisS(),SELF::TYPE_Elf_Ehdr,N_Elf_Ehdr::ei_osabi,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_Elf_Ehdr_type]=createComboBox(ui->tableWidget_Elf_Ehdr,XELF::getTypesS(),SELF::TYPE_Elf_Ehdr,N_Elf_Ehdr::e_type,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_Elf_Ehdr_machine]=createComboBox(ui->tableWidget_Elf_Ehdr,XELF::getMachinesS(),SELF::TYPE_Elf_Ehdr,N_Elf_Ehdr::e_machine,XComboBoxEx::CBTYPE_NORMAL);

                invWidget[INV_CB_Elf_e_entry]=createInvWidget(ui->tableWidget_Elf_Ehdr,SELF::TYPE_Elf_Ehdr,N_Elf_Ehdr::e_entry,InvWidget::TYPE_HEX);
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

                invWidget[INV_CB_Elf_e_entry]->setAddressAndSize(&elf,elf.getHdr64_entry(),0);
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

                invWidget[INV_CB_Elf_e_entry]->setAddressAndSize(&elf,elf.getHdr32_entry(),0);
            }

            comboBox[CB_Elf_Ehdr_iclass]->setValue(elf.getIdent_class());
            comboBox[CB_Elf_Ehdr_idata]->setValue(elf.getIdent_data());
            comboBox[CB_Elf_Ehdr_iversion]->setValue(elf.getIdent_version());
            comboBox[CB_Elf_Ehdr_iosabi]->setValue(elf.getIdent_osabi());

            qint64 nOffset=elf.getEhdrOffset();
            qint64 nSize=0;

            if(elf.is64())
            {
                nSize=elf.getEhdr64Size();
            }
            else
            {
                nSize=elf.getEhdr32Size();
            }

            loadHexSubdevice(nOffset,nSize,nOffset,&subDevice[SELF::TYPE_Elf_Ehdr],ui->widgetHex_Elf_Ehdr);

            blockSignals(false);
        }
        else if(nData==SELF::TYPE_Elf_Shdr)
        {
            bool bIs64=elf.is64();

            if(!bInit[nData])
            {
                bInit[nData]=createSectionTable(SELF::TYPE_Elf_Shdr,ui->tableWidget_Elf_Shdr,bIs64?(N_Elf_Shdr::records64):(N_Elf_Shdr::records32),N_Elf_Shdr::__data_size);
            }

            blockSignals(true);

            QMap<quint64,QString> mapTypes=XELF::getSectionTypesS();

            QList<XELF_DEF::Elf64_Shdr> listSections64;
            QList<XELF_DEF::Elf32_Shdr> listSections32;

            int nCount=0;

            if(bIs64)
            {
                listSections64=elf.getElf64_ShdrList();
                nCount=listSections64.count();
            }
            else
            {
                listSections32=elf.getElf32_ShdrList();
                nCount=listSections32.count();
            }

            ui->tableWidget_Elf_Shdr->setRowCount(nCount);

            for(int i=0; i<nCount; i++)
            {
                if(bIs64)
                {
                    QTableWidgetItem *pItem=new QTableWidgetItem(QString::number(i));

                    if(getOptions()->bIsImage)
                    {
                        pItem->setData(Qt::UserRole+SECTION_DATA_OFFSET,listSections64.at(i).sh_addr);
                    }
                    else
                    {
                        pItem->setData(Qt::UserRole+SECTION_DATA_OFFSET,listSections64.at(i).sh_offset);
                    }

                    pItem->setData(Qt::UserRole+SECTION_DATA_SIZE,listSections64.at(i).sh_size);
                    pItem->setData(Qt::UserRole+SECTION_DATA_ADDRESS,listSections64.at(i).sh_addr);

                    ui->tableWidget_Elf_Shdr->setItem(i,0,                              pItem);
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_name+1,          new QTableWidgetItem(XBinary::valueToHex(listSections64.at(i).sh_name)));
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_type+1,          new QTableWidgetItem(XBinary::valueToHex(listSections64.at(i).sh_type)));
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_flags+1,         new QTableWidgetItem(XBinary::valueToHex(listSections64.at(i).sh_flags)));
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_addr+1,          new QTableWidgetItem(XBinary::valueToHex(listSections64.at(i).sh_addr)));
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_offset+1,        new QTableWidgetItem(XBinary::valueToHex(listSections64.at(i).sh_offset)));
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_size+1,          new QTableWidgetItem(XBinary::valueToHex(listSections64.at(i).sh_size)));
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_link+1,          new QTableWidgetItem(XBinary::valueToHex(listSections64.at(i).sh_link)));
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_info+1,          new QTableWidgetItem(XBinary::valueToHex(listSections64.at(i).sh_info)));
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_addralign+1,     new QTableWidgetItem(XBinary::valueToHex(listSections64.at(i).sh_addralign)));
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_entsize+1,       new QTableWidgetItem(XBinary::valueToHex(listSections64.at(i).sh_entsize)));
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_entsize+2,       new QTableWidgetItem(elf.getStringFromMainSection(listSections64.at(i).sh_name)));
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_entsize+3,       new QTableWidgetItem(mapTypes.value(listSections64.at(i).sh_type)));
                }
                else
                {
                    QTableWidgetItem *pItem=new QTableWidgetItem(QString::number(i));

                    if(getOptions()->bIsImage)
                    {
                        pItem->setData(Qt::UserRole+SECTION_DATA_OFFSET,listSections32.at(i).sh_addr);
                    }
                    else
                    {
                        pItem->setData(Qt::UserRole+SECTION_DATA_OFFSET,listSections32.at(i).sh_offset);
                    }

                    pItem->setData(Qt::UserRole+SECTION_DATA_SIZE,listSections32.at(i).sh_size);
                    pItem->setData(Qt::UserRole+SECTION_DATA_ADDRESS,listSections32.at(i).sh_addr);

                    ui->tableWidget_Elf_Shdr->setItem(i,0,                              pItem);
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_name+1,          new QTableWidgetItem(XBinary::valueToHex(listSections32.at(i).sh_name)));
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_type+1,          new QTableWidgetItem(XBinary::valueToHex(listSections32.at(i).sh_type)));
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_flags+1,         new QTableWidgetItem(XBinary::valueToHex(listSections32.at(i).sh_flags)));
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_addr+1,          new QTableWidgetItem(XBinary::valueToHex(listSections32.at(i).sh_addr)));
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_offset+1,        new QTableWidgetItem(XBinary::valueToHex(listSections32.at(i).sh_offset)));
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_size+1,          new QTableWidgetItem(XBinary::valueToHex(listSections32.at(i).sh_size)));
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_link+1,          new QTableWidgetItem(XBinary::valueToHex(listSections32.at(i).sh_link)));
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_info+1,          new QTableWidgetItem(XBinary::valueToHex(listSections32.at(i).sh_info)));
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_addralign+1,     new QTableWidgetItem(XBinary::valueToHex(listSections32.at(i).sh_addralign)));
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_entsize+1,       new QTableWidgetItem(XBinary::valueToHex(listSections32.at(i).sh_entsize)));
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_entsize+2,       new QTableWidgetItem(elf.getStringFromMainSection(listSections32.at(i).sh_name)));
                    ui->tableWidget_Elf_Shdr->setItem(i,N_Elf_Shdr::sh_entsize+3,       new QTableWidgetItem(mapTypes.value(listSections32.at(i).sh_type)));
                }
            }

            if(nCount)
            {
                if(ui->tableWidget_Elf_Shdr->currentRow()==0)
                {
                    loadShdr(0);
                }
                else
                {
                    ui->tableWidget_Elf_Shdr->setCurrentCell(0,0);
                }
            }

            blockSignals(false);
        }
        else if(nData==SELF::TYPE_Elf_Phdr)
        {
            bool bIs64=elf.is64();

            if(!bInit[nData])
            {
                bInit[nData]=createSectionTable(SELF::TYPE_Elf_Phdr,ui->tableWidget_Elf_Phdr,bIs64?(N_Elf_Phdr64::records):(N_Elf_Phdr32::records),bIs64?(N_Elf_Phdr64::__data_size):(N_Elf_Phdr32::__data_size));
            }

            blockSignals(true);

            QMap<quint64,QString> mapTypes=XELF::getProgramTypesS();

            QList<XELF_DEF::Elf64_Phdr> listPrograms64;
            QList<XELF_DEF::Elf32_Phdr> listPrograms32;

            int nCount=0;

            if(bIs64)
            {
                listPrograms64=elf.getElf64_PhdrList();
                nCount=listPrograms64.count();
            }
            else
            {
                listPrograms32=elf.getElf32_PhdrList();
                nCount=listPrograms32.count();
            }

            ui->tableWidget_Elf_Phdr->setRowCount(nCount);

            for(int i=0; i<nCount; i++)
            {
                if(bIs64)
                {
                    QTableWidgetItem *pItem=new QTableWidgetItem(QString::number(i));

                    if(getOptions()->bIsImage)
                    {
                        pItem->setData(Qt::UserRole+SECTION_DATA_SIZE,listPrograms64.at(i).p_memsz);
                        pItem->setData(Qt::UserRole+SECTION_DATA_OFFSET,listPrograms64.at(i).p_vaddr);
                    }
                    else
                    {
                        pItem->setData(Qt::UserRole+SECTION_DATA_SIZE,listPrograms64.at(i).p_filesz);
                        pItem->setData(Qt::UserRole+SECTION_DATA_OFFSET,listPrograms64.at(i).p_offset);
                    }

                    pItem->setData(Qt::UserRole+SECTION_DATA_ADDRESS,listPrograms64.at(i).p_vaddr);

                    ui->tableWidget_Elf_Phdr->setItem(i,0,                              pItem);
                    ui->tableWidget_Elf_Phdr->setItem(i,N_Elf_Phdr64::p_type+1,         new QTableWidgetItem(XBinary::valueToHex(listPrograms64.at(i).p_type)));
                    ui->tableWidget_Elf_Phdr->setItem(i,N_Elf_Phdr64::p_flags+1,        new QTableWidgetItem(XBinary::valueToHex(listPrograms64.at(i).p_flags)));
                    ui->tableWidget_Elf_Phdr->setItem(i,N_Elf_Phdr64::p_offset+1,       new QTableWidgetItem(XBinary::valueToHex(listPrograms64.at(i).p_offset)));
                    ui->tableWidget_Elf_Phdr->setItem(i,N_Elf_Phdr64::p_vaddr+1,        new QTableWidgetItem(XBinary::valueToHex(listPrograms64.at(i).p_vaddr)));
                    ui->tableWidget_Elf_Phdr->setItem(i,N_Elf_Phdr64::p_paddr+1,        new QTableWidgetItem(XBinary::valueToHex(listPrograms64.at(i).p_paddr)));
                    ui->tableWidget_Elf_Phdr->setItem(i,N_Elf_Phdr64::p_filesz+1,       new QTableWidgetItem(XBinary::valueToHex(listPrograms64.at(i).p_filesz)));
                    ui->tableWidget_Elf_Phdr->setItem(i,N_Elf_Phdr64::p_memsz+1,        new QTableWidgetItem(XBinary::valueToHex(listPrograms64.at(i).p_memsz)));
                    ui->tableWidget_Elf_Phdr->setItem(i,N_Elf_Phdr64::p_align+1,        new QTableWidgetItem(XBinary::valueToHex(listPrograms64.at(i).p_align)));
                    ui->tableWidget_Elf_Phdr->setItem(i,N_Elf_Phdr64::p_align+2,        new QTableWidgetItem(mapTypes.value(listPrograms64.at(i).p_type)));
                }
                else
                {
                    QTableWidgetItem *pItem=new QTableWidgetItem(QString::number(i));

                    if(getOptions()->bIsImage)
                    {
                        pItem->setData(Qt::UserRole+SECTION_DATA_SIZE,listPrograms32.at(i).p_memsz);
                        pItem->setData(Qt::UserRole+SECTION_DATA_OFFSET,listPrograms32.at(i).p_vaddr);
                    }
                    else
                    {
                        pItem->setData(Qt::UserRole+SECTION_DATA_SIZE,listPrograms32.at(i).p_filesz);
                        pItem->setData(Qt::UserRole+SECTION_DATA_OFFSET,listPrograms32.at(i).p_offset);
                    }

                    pItem->setData(Qt::UserRole+SECTION_DATA_ADDRESS,listPrograms32.at(i).p_vaddr);

                    ui->tableWidget_Elf_Phdr->setItem(i,0,                              pItem);
                    ui->tableWidget_Elf_Phdr->setItem(i,N_Elf_Phdr32::p_type+1,         new QTableWidgetItem(XBinary::valueToHex(listPrograms32.at(i).p_type)));
                    ui->tableWidget_Elf_Phdr->setItem(i,N_Elf_Phdr32::p_offset+1,       new QTableWidgetItem(XBinary::valueToHex(listPrograms32.at(i).p_offset)));
                    ui->tableWidget_Elf_Phdr->setItem(i,N_Elf_Phdr32::p_vaddr+1,        new QTableWidgetItem(XBinary::valueToHex(listPrograms32.at(i).p_vaddr)));
                    ui->tableWidget_Elf_Phdr->setItem(i,N_Elf_Phdr32::p_paddr+1,        new QTableWidgetItem(XBinary::valueToHex(listPrograms32.at(i).p_paddr)));
                    ui->tableWidget_Elf_Phdr->setItem(i,N_Elf_Phdr32::p_filesz+1,       new QTableWidgetItem(XBinary::valueToHex(listPrograms32.at(i).p_filesz)));
                    ui->tableWidget_Elf_Phdr->setItem(i,N_Elf_Phdr32::p_memsz+1,        new QTableWidgetItem(XBinary::valueToHex(listPrograms32.at(i).p_memsz)));
                    ui->tableWidget_Elf_Phdr->setItem(i,N_Elf_Phdr32::p_flags+1,        new QTableWidgetItem(XBinary::valueToHex(listPrograms32.at(i).p_flags)));
                    ui->tableWidget_Elf_Phdr->setItem(i,N_Elf_Phdr32::p_align+1,        new QTableWidgetItem(XBinary::valueToHex(listPrograms32.at(i).p_align)));
                    ui->tableWidget_Elf_Phdr->setItem(i,N_Elf_Phdr32::p_align+2,        new QTableWidgetItem(mapTypes.value(listPrograms32.at(i).p_type)));
                }
            }

            if(nCount)
            {
                if(ui->tableWidget_Elf_Phdr->currentRow()==0)
                {
                    loadPhdr(0);
                }
                else
                {
                    ui->tableWidget_Elf_Phdr->setCurrentCell(0,0);
                }
            }

            blockSignals(false);
        }
        else if(nData==SELF::TYPE_Elf_DynamicArrayTags)
        {
            bool bIs64=elf.is64();
            if(!bInit[nData])
            {
                bInit[nData]=createSectionTable(SELF::TYPE_Elf_DynamicArrayTags,ui->tableWidget_DynamicArrayTags,bIs64?(N_Elf_DynamicArrayTags::records64):(N_Elf_DynamicArrayTags::records32),N_Elf_DynamicArrayTags::__data_size);
            }

            blockSignals(true);

            QList<XELF::TAG_STRUCT> listTagStructs=elf.getTagStructs();

            int nCount=listTagStructs.count();

            ui->tableWidget_DynamicArrayTags->setRowCount(nCount);

            QMap<quint64,QString> mapTags=elf.getDynamicTagsS();

            for(int i=0; i<nCount; i++)
            {
                QTableWidgetItem *pItem=new QTableWidgetItem(QString::number(i));

//                pItem->setData(Qt::UserRole+SECTION_DATA_OFFSET,listCommandRecords.at(i).nOffset);
//                pItem->setData(Qt::UserRole+SECTION_DATA_SIZE,listCommandRecords.at(i).nSize);
//                pItem->setData(Qt::UserRole+SECTION_DATA_ADDRESS,listCommandRecords.at(i).nOffset);

                ui->tableWidget_DynamicArrayTags->setItem(i,0,                                      pItem);
                ui->tableWidget_DynamicArrayTags->setItem(i,N_Elf_DynamicArrayTags::d_tag+1,        new QTableWidgetItem(XBinary::valueToHex(bIs64?((quint64)listTagStructs.at(i).nTag):((quint32)listTagStructs.at(i).nTag))));
                ui->tableWidget_DynamicArrayTags->setItem(i,N_Elf_DynamicArrayTags::d_value+1,      new QTableWidgetItem(XBinary::valueToHex(bIs64?((quint64)listTagStructs.at(i).nValue):((quint32)listTagStructs.at(i).nValue))));
                ui->tableWidget_DynamicArrayTags->setItem(i,N_Elf_DynamicArrayTags::d_value+2,      new QTableWidgetItem(mapTags.value(listTagStructs.at(i).nTag)));
            }

            if(nCount)
            {
                // TODO
                ui->tableWidget_DynamicArrayTags->setCurrentCell(0,0);
            }

            blockSignals(false);
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

bool ELFWidget::createSectionTable(int type, QTableWidget *pTableWidget, const FormatWidget::HEADER_RECORD *pRecords, int nRecordCount)
{
    int nSymbolWidth=getSymbolWidth();
    QStringList slHeader;

    switch(type)
    {
        case SELF::TYPE_Elf_Shdr:
            slHeader.append(tr(""));
            pTableWidget->setColumnCount(nRecordCount+3);
            pTableWidget->setColumnWidth(0,nSymbolWidth*4);
            pTableWidget->setColumnWidth(1,nSymbolWidth*8);
            pTableWidget->setColumnWidth(2,nSymbolWidth*8);
            pTableWidget->setColumnWidth(3,nSymbolWidth*12);
            pTableWidget->setColumnWidth(4,nSymbolWidth*12);
            pTableWidget->setColumnWidth(5,nSymbolWidth*12);
            pTableWidget->setColumnWidth(6,nSymbolWidth*12);
            pTableWidget->setColumnWidth(7,nSymbolWidth*8);
            pTableWidget->setColumnWidth(8,nSymbolWidth*8);
            pTableWidget->setColumnWidth(9,nSymbolWidth*12);
            pTableWidget->setColumnWidth(10,nSymbolWidth*12);
            break;

        case SELF::TYPE_Elf_Phdr:
            slHeader.append(tr(""));
            pTableWidget->setColumnCount(nRecordCount+2);
            pTableWidget->setColumnWidth(0,nSymbolWidth*4);
            pTableWidget->setColumnWidth(1,nSymbolWidth*8);
            pTableWidget->setColumnWidth(2,nSymbolWidth*8);
            pTableWidget->setColumnWidth(3,nSymbolWidth*12);
            pTableWidget->setColumnWidth(4,nSymbolWidth*12);
            pTableWidget->setColumnWidth(5,nSymbolWidth*12);
            pTableWidget->setColumnWidth(6,nSymbolWidth*12);
            pTableWidget->setColumnWidth(7,nSymbolWidth*12);
            pTableWidget->setColumnWidth(8,nSymbolWidth*12);
            break;

        case SELF::TYPE_Elf_DynamicArrayTags:
            slHeader.append(tr(""));
            pTableWidget->setColumnCount(nRecordCount+2);
            pTableWidget->setColumnWidth(0,nSymbolWidth*4);
            pTableWidget->setColumnWidth(1,nSymbolWidth*12);
            pTableWidget->setColumnWidth(2,nSymbolWidth*12);
            pTableWidget->setColumnWidth(3,nSymbolWidth*20);
            break;

        default:
            pTableWidget->setColumnCount(nRecordCount);
    }

    pTableWidget->setRowCount(0);

    for(int i=0; i<nRecordCount; i++)
    {
        slHeader.append(pRecords[i].pszName);
    }

    switch(type)
    {
        case SELF::TYPE_Elf_Shdr:
            slHeader.append(tr("Name"));
            slHeader.append(tr("Type"));
            break;
        case SELF::TYPE_Elf_Phdr:
            slHeader.append(tr("Type"));
            break;
        case SELF::TYPE_Elf_DynamicArrayTags:
            slHeader.append(tr("Type"));
            break;
    }

    pTableWidget->setHorizontalHeaderLabels(slHeader);
    pTableWidget->horizontalHeader()->setVisible(true);

    return true;
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
                case N_Elf_Ehdr::ei_class:      lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_class]->setValue((quint8)nValue);      break;
                case N_Elf_Ehdr::ei_data:       lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_data]->setValue((quint8)nValue);       break;
                case N_Elf_Ehdr::ei_version:    lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_version]->setValue((quint8)nValue);    break;
                case N_Elf_Ehdr::ei_osabi:      lineEdit_Elf_Ehdr[N_Elf_Ehdr::ei_osabi]->setValue((quint8)nValue);      break;
                case N_Elf_Ehdr::e_type:        lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_type]->setValue((quint16)nValue);       break;
                case N_Elf_Ehdr::e_machine:     lineEdit_Elf_Ehdr[N_Elf_Ehdr::e_machine]->setValue((quint16)nValue);    break;
            }

            break;
    }
}

void ELFWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous)

    if(current)
    {
        reloadData();
    }
}

void ELFWidget::on_checkBoxReadonly_toggled(bool checked)
{
    setReadonly(checked);
}

void ELFWidget::on_tableWidget_Elf_Shdr_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    if(currentRow!=-1)
    {
        loadShdr(currentRow);
    }
}

void ELFWidget::on_tableWidget_Elf_Phdr_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    if(currentRow!=-1)
    {
        loadPhdr(currentRow);
    }
}

void ELFWidget::on_pushButtonReload_clicked()
{
    reload();
}

void ELFWidget::loadShdr(int nNumber)
{
    qint64 nOffset=ui->tableWidget_Elf_Shdr->item(nNumber,0)->data(Qt::UserRole+SECTION_DATA_OFFSET).toLongLong();
    qint64 nSize=ui->tableWidget_Elf_Shdr->item(nNumber,0)->data(Qt::UserRole+SECTION_DATA_SIZE).toLongLong();
    qint64 nAddress=ui->tableWidget_Elf_Shdr->item(nNumber,0)->data(Qt::UserRole+SECTION_DATA_ADDRESS).toLongLong();

    loadHexSubdevice(nOffset,nSize,nAddress,&subDevice[SD_SECTION],ui->widgetHex_Elf_Shdr);
}

void ELFWidget::loadPhdr(int nNumber)
{
    qint64 nOffset=ui->tableWidget_Elf_Phdr->item(nNumber,0)->data(Qt::UserRole+SECTION_DATA_OFFSET).toLongLong();
    qint64 nSize=ui->tableWidget_Elf_Phdr->item(nNumber,0)->data(Qt::UserRole+SECTION_DATA_SIZE).toLongLong();
    qint64 nAddress=ui->tableWidget_Elf_Phdr->item(nNumber,0)->data(Qt::UserRole+SECTION_DATA_ADDRESS).toLongLong();

    loadHexSubdevice(nOffset,nSize,nAddress,&subDevice[SD_PROGRAM],ui->widgetHex_Elf_Phdr);
}

void ELFWidget::on_tableWidget_Elf_Ehdr_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow);
    Q_UNUSED(currentColumn);
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);

    setHeaderTableSelection(ui->widgetHex_Elf_Ehdr,ui->tableWidget_Elf_Ehdr);
}
