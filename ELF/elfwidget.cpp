// copyright (c) 2017-2020 hors<horsicq@gmail.com>
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

ELFWidget::ELFWidget(QWidget *pParent) :
    FormatWidget(pParent),
    ui(new Ui::ELFWidget)
{
    ui->setupUi(this);
}

ELFWidget::ELFWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, QWidget *pParent) :
    FormatWidget(pDevice,pOptions,0,0,0,pParent),
    ui(new Ui::ELFWidget)
{
    ui->setupUi(this);

    nLastType=-1;

    setData(pDevice,pOptions,0,0,0);
    reload();
}

ELFWidget::~ELFWidget()
{
    delete ui;
}

void ELFWidget::clear()
{
    reset();

    memset(lineEdit_Elf_Ehdr,0,sizeof lineEdit_Elf_Ehdr);
    memset(lineEdit_Elf_Interpreter,0,sizeof lineEdit_Elf_Interpreter);
    memset(lineEdit_Elf_RunPath,0,sizeof lineEdit_Elf_RunPath);
    memset(comboBox,0,sizeof comboBox);
    memset(invWidget,0,sizeof invWidget);
    memset(subDevice,0,sizeof subDevice);
    memset(tvModel,0,sizeof tvModel);

    ui->checkBoxReadonly->setChecked(true);

    ui->treeWidgetNavi->clear();
}

void ELFWidget::cleanup()
{

}

void ELFWidget::reset()
{
    stInit.clear();
}

void ELFWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XELF elf(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(elf.isValid())
    {
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_HEX,QString("Hex")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_STRINGS,tr("Strings")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_MEMORYMAP,tr("Memory map")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_ENTROPY,tr("Entropy")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_HEURISTICSCAN,tr("Heuristic scan")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SELF::TYPE_Elf_Ehdr,"Elf_Ehdr"));

        QList<XELF_DEF::Elf_Shdr> listSectionHeaders=elf.getElf_ShdrList();

        if(listSectionHeaders.count())
        {
            QTreeWidgetItem *pItemSections=createNewItem(SELF::TYPE_Elf_Shdr,"Sections");

            ui->treeWidgetNavi->addTopLevelItem(pItemSections);

            QList<XBinary::DATASET> listDS=elf.getDatasetsFromSections(&listSectionHeaders);

            addDatasets(&elf,pItemSections,&listDS);
        }

        QList<XELF_DEF::Elf_Phdr> listProgramHeaders=elf.getElf_PhdrList();

        if(listProgramHeaders.count())
        {
            QTreeWidgetItem *pItemPrograms=createNewItem(SELF::TYPE_Elf_Phdr,"Programs");

            ui->treeWidgetNavi->addTopLevelItem(pItemPrograms);

            QList<XBinary::DATASET> listDS=elf.getDatasetsFromPrograms(&listProgramHeaders);

            addDatasets(&elf,pItemPrograms,&listDS);
        }

        ui->treeWidgetNavi->expandAll();

        setTreeItem(ui->treeWidgetNavi,getOptions()->nStartType);
    }
}

bool ELFWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)

    bool bResult=false;

    blockSignals(true);

    quint64 nValue=vValue.toULongLong();
    QString sValue=vValue.toString();

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
                        case N_Elf_Ehdr::e_phoff:       invWidget[INV_CB_Elf_e_phoff]->setOffsetAndSize(&elf,elf.is64()?((quint64)nValue):((quint32)nValue),0);     break;
                        case N_Elf_Ehdr::e_shoff:       invWidget[INV_CB_Elf_e_shoff]->setOffsetAndSize(&elf,elf.is64()?((quint64)nValue):((quint32)nValue),0);     break;
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

                    ui->widgetHex_Elf_Ehdr->reload();

                    break;

                case SELF::TYPE_INTERPRETER:
                    switch(nNdata)
                    {
                        case N_ELF_INTERPRETER::interpreter:        elf.write_ansiString(nOffset,sValue);                                                     break;
                    }

                    break;

                case SELF::TYPE_RUNPATH:
                    switch(nNdata)
                    {
                        case N_ELF_RUNPATH::runpath:                elf.write_ansiString(nOffset,sValue);                                                     break;
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
    setLineEditsReadOnly(lineEdit_Elf_Interpreter,N_ELF_INTERPRETER::__data_size,bState);
    setLineEditsReadOnly(lineEdit_Elf_RunPath,N_ELF_RUNPATH::__data_size,bState);

    setComboBoxesReadOnly(comboBox,__CB_size,bState);

    ui->widgetHex->setReadonly(bState);
}

void ELFWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)lineEdit_Elf_Ehdr,N_Elf_Ehdr::__data_size,bState);
    _blockSignals((QObject **)lineEdit_Elf_Interpreter,N_ELF_INTERPRETER::__data_size,bState);
    _blockSignals((QObject **)lineEdit_Elf_RunPath,N_ELF_RUNPATH::__data_size,bState);

    _blockSignals((QObject **)comboBox,__CB_size,bState);
}

void ELFWidget::adjustHeaderTable(int nType, QTableWidget *pTableWidget)
{
    int nSymbolWidth=XLineEditHEX::getSymbolWidth(this);

    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_NAME,QHeaderView::ResizeToContents);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_OFFSET,QHeaderView::ResizeToContents);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_TYPE,QHeaderView::ResizeToContents);

    switch(nType)
    {
        case SELF::TYPE_Elf_Ehdr:
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*12);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*15);
            break;
    }
}

void ELFWidget::reloadData()
{
    int nType=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_TYPE).toInt();
    qint64 nDataOffset=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
    qint64 nDataSize=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();
    qint64 nDataExtraOffset=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_EXTRAOFFSET).toLongLong();
    qint64 nDataExtraSize=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_EXTRASIZE).toLongLong();

    QString sInit=QString("%1-%2-%3").arg(nType).arg(nDataOffset).arg(nDataSize);

    if((nLastType==nType)&&(sInit!=sLastInit))
    {
        stInit.remove(sInit);
    }

    nLastType=nType;
    sLastInit=sInit;

    ui->stackedWidgetInfo->setCurrentIndex(nType);

    XELF elf(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(elf.isValid())
    {
        if(nType==SELF::TYPE_HEX)
        {
            if(!stInit.contains(sInit))
            {
                ui->widgetHex->setData(getDevice());
                ui->widgetHex->setBackupFileName(getOptions()->sBackupFileName);
                ui->widgetHex->enableReadOnly(false);
                connect(ui->widgetHex,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));

                ui->widgetHex->reload();
            }
        }
        else if(nType==SELF::TYPE_STRINGS)
        {
            if(!stInit.contains(sInit))
            {
                ui->widgetStrings->setData(getDevice(),0,true);
            }
        }
        else if(nType==SELF::TYPE_MEMORYMAP)
        {
            if(!stInit.contains(sInit))
            {
                ui->widgetMemoryMap->setData(getDevice());
            }
        }
        else if(nType==SELF::TYPE_ENTROPY)
        {
            if(!stInit.contains(sInit))
            {
                ui->widgetEntropy->setData(getDevice(),0,getDevice()->size(),true);
            }
        }
        else if(nType==SELF::TYPE_HEURISTICSCAN)
        {
            if(!stInit.contains(sInit))
            {
                ui->widgetHeuristicScan->setData(getDevice(),true,XBinary::FT_ELF);
            }
        }
        else if(nType==SELF::TYPE_Elf_Ehdr)
        {
            if(!stInit.contains(sInit))
            {
                if(elf.is64())
                {
                    createHeaderTable(SELF::TYPE_Elf_Ehdr,ui->tableWidget_Elf_Ehdr,N_Elf_Ehdr::records64,lineEdit_Elf_Ehdr,N_Elf_Ehdr::__data_size,0);
                }
                else
                {
                    createHeaderTable(SELF::TYPE_Elf_Ehdr,ui->tableWidget_Elf_Ehdr,N_Elf_Ehdr::records32,lineEdit_Elf_Ehdr,N_Elf_Ehdr::__data_size,0);
                }

                comboBox[CB_Elf_Ehdr_iclass]=createComboBox(ui->tableWidget_Elf_Ehdr,XELF::getIndentClassesS(),SELF::TYPE_Elf_Ehdr,N_Elf_Ehdr::ei_class,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_Elf_Ehdr_idata]=createComboBox(ui->tableWidget_Elf_Ehdr,XELF::getIndentDatasS(),SELF::TYPE_Elf_Ehdr,N_Elf_Ehdr::ei_data,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_Elf_Ehdr_iversion]=createComboBox(ui->tableWidget_Elf_Ehdr,XELF::getIndentVersionsS(),SELF::TYPE_Elf_Ehdr,N_Elf_Ehdr::ei_version,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_Elf_Ehdr_iosabi]=createComboBox(ui->tableWidget_Elf_Ehdr,XELF::getIndentOsabisS(),SELF::TYPE_Elf_Ehdr,N_Elf_Ehdr::ei_osabi,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_Elf_Ehdr_type]=createComboBox(ui->tableWidget_Elf_Ehdr,XELF::getTypesS(),SELF::TYPE_Elf_Ehdr,N_Elf_Ehdr::e_type,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_Elf_Ehdr_machine]=createComboBox(ui->tableWidget_Elf_Ehdr,XELF::getMachinesS(),SELF::TYPE_Elf_Ehdr,N_Elf_Ehdr::e_machine,XComboBoxEx::CBTYPE_NORMAL);

                invWidget[INV_CB_Elf_e_entry]=createInvWidget(ui->tableWidget_Elf_Ehdr,SELF::TYPE_Elf_Ehdr,N_Elf_Ehdr::e_entry,InvWidget::TYPE_HEX);
                invWidget[INV_CB_Elf_e_phoff]=createInvWidget(ui->tableWidget_Elf_Ehdr,SELF::TYPE_Elf_Ehdr,N_Elf_Ehdr::e_phoff,InvWidget::TYPE_HEX);
                invWidget[INV_CB_Elf_e_shoff]=createInvWidget(ui->tableWidget_Elf_Ehdr,SELF::TYPE_Elf_Ehdr,N_Elf_Ehdr::e_shoff,InvWidget::TYPE_HEX);

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
                    invWidget[INV_CB_Elf_e_phoff]->setOffsetAndSize(&elf,elf.getHdr64_phoff(),0);
                    invWidget[INV_CB_Elf_e_shoff]->setOffsetAndSize(&elf,elf.getHdr64_shoff(),0); // TODO offset
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
                    invWidget[INV_CB_Elf_e_phoff]->setOffsetAndSize(&elf,elf.getHdr32_phoff(),0);
                    invWidget[INV_CB_Elf_e_shoff]->setOffsetAndSize(&elf,elf.getHdr32_shoff(),0); // TODO offset
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
        }
        else if(nType==SELF::TYPE_Elf_Shdr)
        {
            if(!stInit.contains(sInit))
            {
                ELFProcessData elfProcessData(SELF::TYPE_Elf_Shdr,&tvModel[SELF::TYPE_Elf_Shdr],&elf,nDataOffset,nDataSize,nDataExtraOffset,nDataExtraSize);

                ajustTableView(&elfProcessData,&tvModel[SELF::TYPE_Elf_Shdr],ui->tableView_Elf_Shdr);

                connect(ui->tableView_Elf_Shdr->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(onTableView_Elf_Shdr_currentRowChanged(QModelIndex,QModelIndex)));

                if(tvModel[SELF::TYPE_Elf_Shdr]->rowCount())
                {
                    ui->tableView_Elf_Shdr->setCurrentIndex(ui->tableView_Elf_Shdr->model()->index(0,0));
                }
            }
        }
        else if(nType==SELF::TYPE_Elf_Phdr)
        {
            if(!stInit.contains(sInit))
            {
                ELFProcessData elfProcessData(SELF::TYPE_Elf_Phdr,&tvModel[SELF::TYPE_Elf_Phdr],&elf,nDataOffset,nDataSize,nDataExtraOffset,nDataExtraSize);

                ajustTableView(&elfProcessData,&tvModel[SELF::TYPE_Elf_Phdr],ui->tableView_Elf_Phdr);

                connect(ui->tableView_Elf_Phdr->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(onTableView_Elf_Phdr_currentRowChanged(QModelIndex,QModelIndex)));

                if(tvModel[SELF::TYPE_Elf_Phdr]->rowCount())
                {
                    ui->tableView_Elf_Phdr->setCurrentIndex(ui->tableView_Elf_Phdr->model()->index(0,0));
                }
            }
        }
        else if(nType==SELF::TYPE_Elf_DynamicArrayTags)
        {
            if(!stInit.contains(sInit))
            {
                ELFProcessData elfProcessData(SELF::TYPE_Elf_DynamicArrayTags,&tvModel[SELF::TYPE_Elf_DynamicArrayTags],&elf,nDataOffset,nDataSize,nDataExtraOffset,nDataExtraSize);

                ajustTableView(&elfProcessData,&tvModel[SELF::TYPE_Elf_DynamicArrayTags],ui->tableView_DynamicArrayTags);

                connect(ui->tableView_DynamicArrayTags->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(onTableView_DynamicArrayTags_currentRowChanged(QModelIndex,QModelIndex)));

                if(tvModel[SELF::TYPE_Elf_DynamicArrayTags]->rowCount())
                {
                    ui->tableView_DynamicArrayTags->setCurrentIndex(ui->tableView_DynamicArrayTags->model()->index(0,0));
                }
            }
        }
        else if(nType==SELF::TYPE_LIBRARIES)
        {
            if(!stInit.contains(sInit))
            {
                ELFProcessData elfProcessData(SELF::TYPE_LIBRARIES,&tvModel[SELF::TYPE_LIBRARIES],&elf,nDataOffset,nDataSize,nDataExtraOffset,nDataExtraSize);

                ajustTableView(&elfProcessData,&tvModel[SELF::TYPE_LIBRARIES],ui->tableView_Libraries);

                connect(ui->tableView_Libraries->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(onTableView_Libraries_currentRowChanged(QModelIndex,QModelIndex)));

                if(tvModel[SELF::TYPE_LIBRARIES]->rowCount())
                {
                    ui->tableView_Libraries->setCurrentIndex(ui->tableView_Libraries->model()->index(0,0));
                }
            }
        }
        else if(nType==SELF::TYPE_INTERPRETER)
        {
            if(!stInit.contains(sInit))
            {
                createListTable(SELF::TYPE_INTERPRETER,ui->tableWidget_Interpreter,N_ELF_INTERPRETER::records,lineEdit_Elf_Interpreter,N_ELF_INTERPRETER::__data_size);

                blockSignals(true);

                XBinary::OS_ANSISTRING osAnsiString=elf.getOsAnsiString(nDataOffset,nDataSize);

                setLineEdit(lineEdit_Elf_Interpreter[N_ELF_INTERPRETER::interpreter],osAnsiString.nSize,osAnsiString.sAnsiString,osAnsiString.nOffset);

                blockSignals(false);
            }
        }
        else if(nType==SELF::TYPE_NOTES)
        {
            if(!stInit.contains(sInit))
            {
                ELFProcessData elfProcessData(SELF::TYPE_NOTES,&tvModel[SELF::TYPE_NOTES],&elf,nDataOffset,nDataSize,nDataExtraOffset,nDataExtraSize);

                ajustTableView(&elfProcessData,&tvModel[SELF::TYPE_NOTES],ui->tableView_Notes);

                connect(ui->tableView_Notes->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(onTableView_Notes_currentRowChanged(QModelIndex,QModelIndex)));

                if(tvModel[SELF::TYPE_NOTES]->rowCount())
                {
                    ui->tableView_Notes->setCurrentIndex(ui->tableView_Notes->model()->index(0,0));
                }
            }
        }
        else if(nType==SELF::TYPE_RUNPATH)
        {
            if(!stInit.contains(sInit))
            {
                createListTable(SELF::TYPE_RUNPATH,ui->tableWidget_RunPath,N_ELF_RUNPATH::records,lineEdit_Elf_RunPath,N_ELF_RUNPATH::__data_size);

                blockSignals(true);

                XBinary::OS_ANSISTRING osAnsiString=elf.getOsAnsiString(nDataOffset,nDataSize);

                setLineEdit(lineEdit_Elf_RunPath[N_ELF_RUNPATH::runpath],osAnsiString.nSize,osAnsiString.sAnsiString,osAnsiString.nOffset);

                blockSignals(false);
            }
        }
        else if(nType==SELF::TYPE_STRINGTABLE)
        {
            if(!stInit.contains(sInit))
            {
                loadHexSubdevice(nDataOffset,nDataSize,0,&subDevice[SELF::TYPE_STRINGTABLE],ui->widgetHex_StringTable);
            }
        }
        else if(nType==SELF::TYPE_SYMBOLTABLE)
        {
            if(!stInit.contains(sInit))
            {
                ELFProcessData elfProcessData(SELF::TYPE_SYMBOLTABLE,&tvModel[SELF::TYPE_SYMBOLTABLE],&elf,nDataOffset,nDataSize,nDataExtraOffset,nDataExtraSize);

                ajustTableView(&elfProcessData,&tvModel[SELF::TYPE_SYMBOLTABLE],ui->tableView_SymbolTable);
            }
        }
        else if(nType==SELF::TYPE_Elf_Rela)
        {
            if(!stInit.contains(sInit))
            {
                ELFProcessData elfProcessData(SELF::TYPE_Elf_Rela,&tvModel[SELF::TYPE_Elf_Rela],&elf,nDataOffset,nDataSize,nDataExtraOffset,nDataExtraSize);

                ajustTableView(&elfProcessData,&tvModel[SELF::TYPE_Elf_Rela],ui->tableView_Rela);
            }
        }
        else if(nType==SELF::TYPE_Elf_Rel)
        {
            if(!stInit.contains(sInit))
            {
                ELFProcessData elfProcessData(SELF::TYPE_Elf_Rel,&tvModel[SELF::TYPE_Elf_Rel],&elf,nDataOffset,nDataSize,nDataExtraOffset,nDataExtraSize);

                ajustTableView(&elfProcessData,&tvModel[SELF::TYPE_Elf_Rel],ui->tableView_Rel);
            }
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }

    stInit.insert(sInit);
}

void ELFWidget::addDatasets(XELF *pElf, QTreeWidgetItem *pParent, QList<XBinary::DATASET> *pList)
{
    int nNumberOfRecords=pList->count();

    for(int i=0;i<nNumberOfRecords;i++)
    {
        if(pList->at(i).nType==XELF::DS_INTERPRETER)
        {
            pParent->addChild(createNewItem(SELF::TYPE_INTERPRETER,pList->at(i).sName,pList->at(i).nOffset,pList->at(i).nSize,pList->at(i).nStringTableOffset,pList->at(i).nStringTableSize));
        }
        else if(pList->at(i).nType==XELF::DS_LIBRARIES)
        {
            pParent->addChild(createNewItem(SELF::TYPE_LIBRARIES,pList->at(i).sName,pList->at(i).nOffset,pList->at(i).nSize,pList->at(i).nStringTableOffset,pList->at(i).nStringTableSize));
        }
        else if(pList->at(i).nType==XELF::DS_STRINGTABLE)
        {
            pParent->addChild(createNewItem(SELF::TYPE_STRINGTABLE,pList->at(i).sName,pList->at(i).nOffset,pList->at(i).nSize,pList->at(i).nStringTableOffset,pList->at(i).nStringTableSize));
        }
        else if(pList->at(i).nType==XELF::DS_SYMBOLTABLE)
        {
            pParent->addChild(createNewItem(SELF::TYPE_SYMBOLTABLE,pList->at(i).sName,pList->at(i).nOffset,pList->at(i).nSize,pList->at(i).nStringTableOffset,pList->at(i).nStringTableSize));
        }
        else if(pList->at(i).nType==XELF::DS_RUNPATH)
        {
            pParent->addChild(createNewItem(SELF::TYPE_RUNPATH,pList->at(i).sName,pList->at(i).nOffset,pList->at(i).nSize,pList->at(i).nStringTableOffset,pList->at(i).nStringTableSize));
        }
        else if(pList->at(i).nType==XELF::DS_NOTES)
        {
            pParent->addChild(createNewItem(SELF::TYPE_NOTES,pList->at(i).sName,pList->at(i).nOffset,pList->at(i).nSize,pList->at(i).nStringTableOffset,pList->at(i).nStringTableSize));
        }
        else if(pList->at(i).nType==XELF::DS_RELA)
        {
            pParent->addChild(createNewItem(SELF::TYPE_Elf_Rela,pList->at(i).sName,pList->at(i).nOffset,pList->at(i).nSize,pList->at(i).nStringTableOffset,pList->at(i).nStringTableSize));
        }
        else if(pList->at(i).nType==XELF::DS_REL)
        {
            pParent->addChild(createNewItem(SELF::TYPE_Elf_Rel,pList->at(i).sName,pList->at(i).nOffset,pList->at(i).nSize,pList->at(i).nStringTableOffset,pList->at(i).nStringTableSize));
        }
        else if(pList->at(i).nType==XELF::DS_DYNAMICTAGS)
        {
            QTreeWidgetItem *pDynamicTags=createNewItem(SELF::TYPE_Elf_DynamicArrayTags,pList->at(i).sName,pList->at(i).nOffset,pList->at(i).nSize,pList->at(i).nStringTableOffset,pList->at(i).nStringTableSize);

            pParent->addChild(pDynamicTags);

            QList<XELF::TAG_STRUCT> listTagStructs=pElf->_getTagStructs(pList->at(i).nOffset,pList->at(i).nSize,pElf->is64(),pElf->isBigEndian());

            XBinary::_MEMORY_MAP memoryMap=pElf->getMemoryMap();

            QList<XBinary::DATASET> listDS=pElf->getDatasetsFromTagStructs(&memoryMap,&listTagStructs);

            addDatasets(pElf,pDynamicTags,&listDS);
        }
    }
}

bool ELFWidget::createSectionTable(int nType, QTableWidget *pTableWidget, const FW_DEF::HEADER_RECORD *pRecords, int nNumberOfRecords)
{
    Q_UNUSED(nType)

    QStringList slHeader;

    pTableWidget->setRowCount(0);

    for(int i=0; i<nNumberOfRecords; i++)
    {
        slHeader.append(pRecords[i].sName);
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

void ELFWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pCurrent, QTreeWidgetItem *pPrevious)
{
    Q_UNUSED(pPrevious)

    if(pCurrent)
    {
        reloadData();
    }
}

void ELFWidget::on_checkBoxReadonly_toggled(bool bChecked)
{
    setReadonly(bChecked);
}

void ELFWidget::on_pushButtonReload_clicked()
{
    ui->pushButtonReload->setEnabled(false);
    reload();

    QTimer::singleShot(1000,this,SLOT(enableButton()));
}

void ELFWidget::enableButton()
{
    ui->pushButtonReload->setEnabled(true);
}

void ELFWidget::loadShdr(int nRow)
{
    if(nRow!=-1)
    {
        QModelIndex index=ui->tableView_Elf_Shdr->model()->index(nRow,0);

        qint64 nOffset=ui->tableView_Elf_Shdr->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize=ui->tableView_Elf_Shdr->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();
        qint64 nAddress=ui->tableView_Elf_Shdr->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

        loadHexSubdevice(nOffset,nSize,nAddress,&subDevice[SELF::TYPE_Elf_Shdr],ui->widgetHex_Elf_Shdr);
    }
}

void ELFWidget::loadPhdr(int nRow)
{
    if(nRow!=-1)
    {
        QModelIndex index=ui->tableView_Elf_Phdr->model()->index(nRow,0);

        qint64 nOffset=ui->tableView_Elf_Phdr->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize=ui->tableView_Elf_Phdr->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();
        qint64 nAddress=ui->tableView_Elf_Phdr->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

        loadHexSubdevice(nOffset,nSize,nAddress,&subDevice[SELF::TYPE_Elf_Phdr],ui->widgetHex_Elf_Phdr);
    }
}

void ELFWidget::loadNote(int nRow)
{
    if(nRow!=-1)
    {
        QModelIndex index=ui->tableView_Notes->model()->index(nRow,0);

        qint64 nOffset=ui->tableView_Notes->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize=ui->tableView_Notes->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();
        qint64 nAddress=ui->tableView_Notes->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

        loadHexSubdevice(nOffset,nSize,nAddress,&subDevice[SELF::TYPE_NOTES],ui->widgetHex_Notes);
    }
}

void ELFWidget::on_tableWidget_Elf_Ehdr_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_Elf_Ehdr,ui->tableWidget_Elf_Ehdr);
}

void ELFWidget::editSectionHeader()
{
    showSectionHeader(SELF::TYPE_Elf_Shdr,ui->tableView_Elf_Shdr);
}

void ELFWidget::sectionHex()
{
    showSectionHex(ui->tableView_Elf_Shdr);
}

void ELFWidget::sectionEntropy()
{
    showSectionEntropy(ui->tableView_Elf_Shdr);
}

void ELFWidget::editProgramHeader()
{
    showSectionHeader(SELF::TYPE_Elf_Phdr,ui->tableView_Elf_Phdr);
}

void ELFWidget::programHex()
{
    showSectionHex(ui->tableView_Elf_Phdr);
}

void ELFWidget::programEntropy()
{
    showSectionEntropy(ui->tableView_Elf_Phdr);
}

void ELFWidget::editDynamicArrayTag()
{
    showSectionHeader(SELF::TYPE_Elf_DynamicArrayTags,ui->tableView_DynamicArrayTags);
}

void ELFWidget::on_tableView_SymbolTable_customContextMenuRequested(const QPoint &pos)
{
    int nRow=ui->tableView_SymbolTable->currentIndex().row();

    if(nRow!=-1)
    {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"),this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editSymbolHeader()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_SymbolTable->viewport()->mapToGlobal(pos));
    }
}

void ELFWidget::editSymbolHeader()
{
    showSectionHeader(SELF::TYPE_SYMBOLTABLE,ui->tableView_SymbolTable);
}

void ELFWidget::showSectionHeader(int nType, QTableView *pTableView)
{
    int nRow=pTableView->currentIndex().row();

    if(nRow!=-1)
    {
        QModelIndex index=pTableView->selectionModel()->selectedIndexes().at(0);

        qint64 nOffset=pTableView->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();

        qint64 nStringTableOffset=pTableView->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_STRINGTABLEOFFSET).toLongLong();
        qint64 nStringTableSize=pTableView->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_STRINGTABLESIZE).toLongLong();

        ELFSectionHeaderWidget *pSectionHeaderWidget=new ELFSectionHeaderWidget(getDevice(),getOptions(),(quint32)nRow,nOffset,nType,this);

        pSectionHeaderWidget->setStringTable(nStringTableOffset,nStringTableSize);

        DialogSectionHeader dsh(this);
        dsh.setWidget(pSectionHeaderWidget);
        dsh.setData(typeIdToString(nType));
        dsh.setEdited(isEdited());

        connect(&dsh,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));

        dsh.exec();

        reloadData();

        pTableView->setCurrentIndex(pTableView->model()->index(nRow,0));
    }
}

QString ELFWidget::typeIdToString(int nType)
{
    QString sResult="Unknown";

    switch(nType)
    {
        case SELF::TYPE_Elf_Shdr:               sResult=QString("Section header");      break;
        case SELF::TYPE_Elf_Phdr:               sResult=QString("Program header");      break;
        case SELF::TYPE_SYMBOLTABLE:            sResult=QString("Symbol header");       break;
        case SELF::TYPE_Elf_DynamicArrayTags:   sResult=QString("Tag");                 break;
        case SELF::TYPE_Elf_Rela:               sResult=QString("Relocation");          break;
        case SELF::TYPE_Elf_Rel:                sResult=QString("Relocation");          break;
    }

    return sResult;
}

void ELFWidget::on_tableView_Elf_Shdr_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editSectionHeader();
}

void ELFWidget::on_tableView_Elf_Shdr_customContextMenuRequested(const QPoint &pos)
{
    int nRow=ui->tableView_Elf_Shdr->currentIndex().row();

    if(nRow!=-1)
    {
        bool bIsEnable=getTableViewItemSize(ui->tableView_Elf_Shdr,nRow);

        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"),this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editSectionHeader()));
        contextMenu.addAction(&actionEdit);

        QAction actionHex(QString("Hex"),this);
        connect(&actionHex, SIGNAL(triggered()), this, SLOT(sectionHex()));
        actionHex.setEnabled(bIsEnable);
        contextMenu.addAction(&actionHex);

        QAction actionEntropy(tr("Entropy"),this);
        connect(&actionEntropy, SIGNAL(triggered()), this, SLOT(programEntropy()));
        actionHex.setEnabled(bIsEnable);
        contextMenu.addAction(&actionEntropy);

        contextMenu.exec(ui->tableView_Elf_Shdr->viewport()->mapToGlobal(pos));
    }
}

void ELFWidget::on_tableView_SymbolTable_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editSymbolHeader();
}

void ELFWidget::onTableView_Elf_Shdr_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(),SELF::TYPE_Elf_Shdr,ui->widgetHex_Elf_Shdr,ui->tableView_Elf_Shdr,&subDevice[SELF::TYPE_Elf_Shdr]);
}

void ELFWidget::onTableView_Elf_Phdr_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(),SELF::TYPE_Elf_Phdr,ui->widgetHex_Elf_Phdr,ui->tableView_Elf_Phdr,&subDevice[SELF::TYPE_Elf_Phdr]);
}

void ELFWidget::onTableView_DynamicArrayTags_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    // TODO
}

void ELFWidget::onTableView_Notes_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(),SELF::TYPE_NOTES,ui->widgetHex_Notes,ui->tableView_Notes,&subDevice[SELF::TYPE_NOTES]);
}

void ELFWidget::onTableView_Libraries_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    // TODO
}

void ELFWidget::on_tableView_Elf_Phdr_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editProgramHeader();
}

void ELFWidget::on_tableView_Elf_Phdr_customContextMenuRequested(const QPoint &pos)
{
    int nRow=ui->tableView_Elf_Phdr->currentIndex().row();

    if(nRow!=-1)
    {
        bool bIsEnable=getTableViewItemSize(ui->tableView_Elf_Phdr,nRow);

        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"),this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editProgramHeader()));
        contextMenu.addAction(&actionEdit);

        QAction actionHex(QString("Hex"),this);
        connect(&actionHex, SIGNAL(triggered()), this, SLOT(programHex()));
        actionHex.setEnabled(bIsEnable);
        contextMenu.addAction(&actionHex);

        QAction actionEntropy(tr("Entropy"),this);
        connect(&actionEntropy, SIGNAL(triggered()), this, SLOT(programEntropy()));
        actionHex.setEnabled(bIsEnable);
        contextMenu.addAction(&actionEntropy);

        contextMenu.exec(ui->tableView_Elf_Phdr->viewport()->mapToGlobal(pos));
    }
}

void ELFWidget::on_tableView_DynamicArrayTags_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editDynamicArrayTag();
}

void ELFWidget::on_tableView_DynamicArrayTags_customContextMenuRequested(const QPoint &pos)
{
    int nRow=ui->tableView_DynamicArrayTags->currentIndex().row();

    if(nRow!=-1)
    {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"),this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editDynamicArrayTag()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_DynamicArrayTags->viewport()->mapToGlobal(pos));
    }
}

void ELFWidget::on_tableView_Notes_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    // TODO
}

void ELFWidget::on_tableView_Rela_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editRelaHeaderTag();
}

void ELFWidget::on_tableView_Rela_customContextMenuRequested(const QPoint &pos)
{
    int nRow=ui->tableView_Rela->currentIndex().row();

    if(nRow!=-1)
    {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"),this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editRelaHeaderTag()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_Rela->viewport()->mapToGlobal(pos));
    }
}

void ELFWidget::editRelaHeaderTag()
{
    showSectionHeader(SELF::TYPE_Elf_Rela,ui->tableView_Rela);
}

void ELFWidget::editRelHeaderTag()
{
    showSectionHeader(SELF::TYPE_Elf_Rel,ui->tableView_Rel);
}

void ELFWidget::on_tableView_Rel_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editRelHeaderTag();
}

void ELFWidget::on_tableView_Rel_customContextMenuRequested(const QPoint &pos)
{
    int nRow=ui->tableView_Rel->currentIndex().row();

    if(nRow!=-1)
    {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"),this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editRelHeaderTag()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_Rel->viewport()->mapToGlobal(pos));
    }
}
