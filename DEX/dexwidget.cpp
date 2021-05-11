// copyright (c) 2017-2021 hors<horsicq@gmail.com>
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
#include "dexwidget.h"
#include "ui_dexwidget.h"

DEXWidget::DEXWidget(QWidget *pParent) :
    FormatWidget(pParent),
    ui(new Ui::DEXWidget)
{
    ui->setupUi(this);

    g_pFilterStrings=new QSortFilterProxyModel(this);
    g_pFilterTypes=new QSortFilterProxyModel(this);

    initHexViewWidget(ui->widgetHex);
//    initSearchSignaturesWidget(ui->widgetSignatures);
    initSearchStringsWidget(ui->widgetStrings);
    initToolsWidget(ui->widgetHex_Header);
}

DEXWidget::DEXWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, QWidget *pParent) :
    DEXWidget(pParent)
{
    DEXWidget::setData(pDevice,options,0,0,0);
    DEXWidget::reload();
}

DEXWidget::~DEXWidget()
{
    delete ui;
}

void DEXWidget::setShortcuts(XShortcuts *pShortcuts)
{
    ui->widgetHex->setShortcuts(pShortcuts);
//    ui->widgetDisasm->setShortcuts(pShortcuts);
    ui->widgetStrings->setShortcuts(pShortcuts);
    ui->widgetEntropy->setShortcuts(pShortcuts);
    ui->widgetHeuristicScan->setShortcuts(pShortcuts);
    ui->widgetMemoryMap->setShortcuts(pShortcuts);
    ui->widgetHex_Header->setShortcuts(pShortcuts);

    FormatWidget::setShortcuts(pShortcuts);
}

void DEXWidget::clear()
{
    DEXWidget::reset();

    memset(g_lineEdit_HEADER,0,sizeof g_lineEdit_HEADER);
    memset(g_comboBox,0,sizeof g_comboBox);
    memset(g_invWidget,0,sizeof g_invWidget);
    memset(g_subDevice,0,sizeof g_subDevice);
    memset(g_tvModel,0,sizeof g_tvModel);

    ui->checkBoxReadonly->setChecked(true);

    ui->treeWidgetNavi->clear();
}

void DEXWidget::cleanup()
{

}

void DEXWidget::reload()
{
    DEXWidget::clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XDEX dex(getDevice());

    if(dex.isValid())
    {
        setFileType(dex.getFileType());

        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SDEX::TYPE_HEX,tr("Hex")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SDEX::TYPE_STRINGS,tr("Strings")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SDEX::TYPE_MEMORYMAP,tr("Memory map")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SDEX::TYPE_ENTROPY,tr("Entropy")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SDEX::TYPE_HEURISTICSCAN,tr("Heuristic scan")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SDEX::TYPE_HEADER,"Header"));

        QList<XDEX_DEF::MAP_ITEM> listMapItems=dex.getMapItems();

        if(listMapItems.count())
        {
            QTreeWidgetItem *pItemMapItems=createNewItem(SDEX::TYPE_MAPITEMS,"Map items");

            ui->treeWidgetNavi->addTopLevelItem(pItemMapItems);

            if(dex.isMapItemPresent(XDEX_DEF::TYPE_STRING_ID_ITEM,&listMapItems))
            {
                pItemMapItems->addChild(createNewItem(SDEX::TYPE_STRING_ID_ITEM,"STRING_ID_ITEM"));
            }

            if(dex.isMapItemPresent(XDEX_DEF::TYPE_TYPE_ID_ITEM,&listMapItems))
            {
                pItemMapItems->addChild(createNewItem(SDEX::TYPE_TYPE_ID_ITEM,"TYPE_ID_ITEM"));
            }

            if(dex.isMapItemPresent(XDEX_DEF::TYPE_PROTO_ID_ITEM,&listMapItems))
            {
                pItemMapItems->addChild(createNewItem(SDEX::TYPE_PROTO_ID_ITEM,"PROTO_ID_ITEM"));
            }

            if(dex.isMapItemPresent(XDEX_DEF::TYPE_FIELD_ID_ITEM,&listMapItems))
            {
                pItemMapItems->addChild(createNewItem(SDEX::TYPE_FIELD_ID_ITEM,"TYPE_FIELD_ID_ITEM"));
            }

            if(dex.isMapItemPresent(XDEX_DEF::TYPE_METHOD_ID_ITEM,&listMapItems))
            {
                pItemMapItems->addChild(createNewItem(SDEX::TYPE_METHOD_ID_ITEM,"TYPE_METHOD_ID_ITEM"));
            }

            if(dex.isMapItemPresent(XDEX_DEF::TYPE_CLASS_DEF_ITEM,&listMapItems))
            {
                pItemMapItems->addChild(createNewItem(SDEX::TYPE_CLASS_DEF_ITEM,"TYPE_CLASS_DEF_ITEM"));
            }
        }

        ui->treeWidgetNavi->expandAll();

        setTreeItem(ui->treeWidgetNavi,getOptions().nStartType);
    }
}

FormatWidget::SV DEXWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)

    SV result=SV_NONE;

    blockSignals(true);

    quint64 nValue=vValue.toULongLong();
    QString sValue=vValue.toString();

    if(getDevice()->isWritable())
    {
        XDEX dex(getDevice());

        if(dex.isValid())
        {
            switch(nStype)
            {
                case SDEX::TYPE_HEADER:
                    switch(nNdata)
                    {
                        case N_DEX_HEADER::magic:           g_comboBox[CB_Dex_Header_magic]->setValue(nValue);                                                                                        break;
                        case N_DEX_HEADER::version:         g_comboBox[CB_Dex_Header_version]->setValue(nValue);                                                                                      break;
                        case N_DEX_HEADER::endian_tag:      g_comboBox[CB_Dex_Header_endian_tag]->setValue(nValue);                                                                                   break;
                        case N_DEX_HEADER::link_size:       g_invWidget[INV_link]->setOffsetAndSize(&dex,dex.getHeader_link_off(),(quint32)nValue);                                                   break;
                        case N_DEX_HEADER::link_off:        g_invWidget[INV_link]->setOffsetAndSize(&dex,(quint32)nValue,dex.getHeader_link_size());                                                  break;
                        case N_DEX_HEADER::map_off:         g_invWidget[INV_map]->setOffsetAndSize(&dex,(quint32)nValue,0);                                                                           break;
                        case N_DEX_HEADER::string_ids_size: g_invWidget[INV_string_ids]->setOffsetAndSize(&dex,dex.getHeader_string_ids_off(),(quint32)nValue*sizeof(XDEX_DEF::STRING_ITEM_ID));      break;
                        case N_DEX_HEADER::string_ids_off:  g_invWidget[INV_string_ids]->setOffsetAndSize(&dex,(quint32)nValue,dex.getHeader_string_ids_size()*sizeof(XDEX_DEF::STRING_ITEM_ID));     break;
                        case N_DEX_HEADER::type_ids_size:   g_invWidget[INV_type_ids]->setOffsetAndSize(&dex,dex.getHeader_type_ids_off(),(quint32)nValue*sizeof(XDEX_DEF::TYPE_ITEM_ID));            break;
                        case N_DEX_HEADER::type_ids_off:    g_invWidget[INV_type_ids]->setOffsetAndSize(&dex,(quint32)nValue,dex.getHeader_type_ids_size()*sizeof(XDEX_DEF::TYPE_ITEM_ID));           break;
                        case N_DEX_HEADER::proto_ids_size:  g_invWidget[INV_proto_ids]->setOffsetAndSize(&dex,dex.getHeader_proto_ids_off(),(quint32)nValue*sizeof(XDEX_DEF::PROTO_ITEM_ID));         break;
                        case N_DEX_HEADER::proto_ids_off:   g_invWidget[INV_proto_ids]->setOffsetAndSize(&dex,(quint32)nValue,dex.getHeader_proto_ids_size()*sizeof(XDEX_DEF::PROTO_ITEM_ID));        break;
                        case N_DEX_HEADER::field_ids_size:  g_invWidget[INV_field_ids]->setOffsetAndSize(&dex,dex.getHeader_field_ids_off(),(quint32)nValue*sizeof(XDEX_DEF::FIELD_ITEM_ID));         break;
                        case N_DEX_HEADER::field_ids_off:   g_invWidget[INV_field_ids]->setOffsetAndSize(&dex,(quint32)nValue,dex.getHeader_field_ids_size()*sizeof(XDEX_DEF::FIELD_ITEM_ID));        break;
                        case N_DEX_HEADER::method_ids_size: g_invWidget[INV_method_ids]->setOffsetAndSize(&dex,dex.getHeader_method_ids_off(),(quint32)nValue*sizeof(XDEX_DEF::METHOD_ITEM_ID));      break;
                        case N_DEX_HEADER::method_ids_off:  g_invWidget[INV_method_ids]->setOffsetAndSize(&dex,(quint32)nValue,dex.getHeader_method_ids_size()*sizeof(XDEX_DEF::METHOD_ITEM_ID));     break;
                        case N_DEX_HEADER::class_defs_size: g_invWidget[INV_class_defs]->setOffsetAndSize(&dex,dex.getHeader_class_defs_off(),(quint32)nValue*sizeof(XDEX_DEF::CLASS_ITEM_DEF));      break;
                        case N_DEX_HEADER::class_defs_off:  g_invWidget[INV_class_defs]->setOffsetAndSize(&dex,(quint32)nValue,dex.getHeader_class_defs_size()*sizeof(XDEX_DEF::CLASS_ITEM_DEF));     break;
                        case N_DEX_HEADER::data_size:       g_invWidget[INV_data]->setOffsetAndSize(&dex,dex.getHeader_data_off(),(quint32)nValue);                                                   break;
                        case N_DEX_HEADER::data_off:        g_invWidget[INV_data]->setOffsetAndSize(&dex,(quint32)nValue,dex.getHeader_data_size());                                                  break;
                    }

                    break;
            }

            switch(nStype)
            {
                case SDEX::TYPE_HEADER:
                    switch(nNdata)
                    {
                        case N_DEX_HEADER::magic:           dex.setHeader_magic((quint32)nValue);           break;
                        case N_DEX_HEADER::version:         dex.setHeader_version((quint32)nValue);         break;
                        case N_DEX_HEADER::checksum:        dex.setHeader_checksum((quint32)nValue);        break;
                        case N_DEX_HEADER::file_size:       dex.setHeader_file_size((quint32)nValue);       break;
                        case N_DEX_HEADER::header_size:     dex.setHeader_header_size((quint32)nValue);     break;
                        case N_DEX_HEADER::endian_tag:      dex.setHeader_endian_tag((quint32)nValue);      break;
                        case N_DEX_HEADER::link_size:       dex.setHeader_link_size((quint32)nValue);       break;
                        case N_DEX_HEADER::link_off:        dex.setHeader_link_off((quint32)nValue);        break;
                        case N_DEX_HEADER::map_off:         dex.setHeader_map_off((quint32)nValue);         break;
                        case N_DEX_HEADER::string_ids_size: dex.setHeader_string_ids_size((quint32)nValue); break;
                        case N_DEX_HEADER::string_ids_off:  dex.setHeader_string_ids_off((quint32)nValue);  break;
                        case N_DEX_HEADER::type_ids_size:   dex.setHeader_type_ids_size((quint32)nValue);   break;
                        case N_DEX_HEADER::type_ids_off:    dex.setHeader_type_ids_off((quint32)nValue);    break;
                        case N_DEX_HEADER::proto_ids_size:  dex.setHeader_proto_ids_size((quint32)nValue);  break;
                        case N_DEX_HEADER::proto_ids_off:   dex.setHeader_proto_ids_off((quint32)nValue);   break;
                        case N_DEX_HEADER::field_ids_size:  dex.setHeader_field_ids_size((quint32)nValue);  break;
                        case N_DEX_HEADER::field_ids_off:   dex.setHeader_field_ids_off((quint32)nValue);   break;
                        case N_DEX_HEADER::method_ids_size: dex.setHeader_method_ids_size((quint32)nValue); break;
                        case N_DEX_HEADER::method_ids_off:  dex.setHeader_method_ids_off((quint32)nValue);  break;
                        case N_DEX_HEADER::class_defs_size: dex.setHeader_class_defs_size((quint32)nValue); break;
                        case N_DEX_HEADER::class_defs_off:  dex.setHeader_class_defs_off((quint32)nValue);  break;
                        case N_DEX_HEADER::data_size:       dex.setHeader_data_size((quint32)nValue);       break;
                        case N_DEX_HEADER::data_off:        dex.setHeader_data_off((quint32)nValue);        break;
                    }

                    ui->widgetHex_Header->reload();

                    break;
            }

            result=SV_EDITED;
        }
    }

    blockSignals(false);

    return result;
}

void DEXWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(g_lineEdit_HEADER,N_DEX_HEADER::__data_size,bState);
    setComboBoxesReadOnly(g_comboBox,__CB_size,bState);

    ui->widgetHex->setReadonly(bState);
}

void DEXWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)g_lineEdit_HEADER,N_DEX_HEADER::__data_size,bState);
    _blockSignals((QObject **)g_comboBox,__CB_size,bState);
}

void DEXWidget::adjustHeaderTable(int nType, QTableWidget *pTableWidget)
{
    // TODO like Mach
    int nSymbolWidth=XLineEditHEX::getSymbolWidth(this);

    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_NAME,QHeaderView::ResizeToContents);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_OFFSET,QHeaderView::ResizeToContents);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_TYPE,QHeaderView::ResizeToContents);

    switch(nType)
    {
        // TODO
    }
}

QString DEXWidget::typeIdToString(int nType)
{
    return "";
}

void DEXWidget::reloadData()
{
    int nType=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_TYPE).toInt();
    qint64 nDataOffset=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
    qint64 nDataSize=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();
    qint64 nDataExtraOffset=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_EXTRAOFFSET).toLongLong();
    qint64 nDataExtraSize=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_EXTRASIZE).toLongLong();

    QString sInit=getInitString(ui->treeWidgetNavi->currentItem());

    ui->stackedWidgetInfo->setCurrentIndex(nType);

    XDEX dex(getDevice());

    if(dex.isValid())
    {
        if(nType==SDEX::TYPE_HEX)
        {
            if(!isInitPresent(sInit))
            {
                XHexView::OPTIONS options={};
                options.bMenu_Disasm=true;
                options.bMenu_MemoryMap=true;
                options.sSignaturesPath=getOptions().sSearchSignaturesPath;
                ui->widgetHex->setData(getDevice(),options);
//                ui->widgetHex->setBackupFileName(getOptions().sBackupFileName);
                ui->widgetHex->enableReadOnly(false);

                ui->widgetHex->reload();
            }
        }
        else if(nType==SDEX::TYPE_STRINGS)
        {
            if(!isInitPresent(sInit))
            {
                SearchStringsWidget::OPTIONS stringsOptions={};
                stringsOptions.bMenu_Hex=true;
                stringsOptions.bMenu_Demangle=true;
                stringsOptions.bAnsi=true;
                stringsOptions.bUnicode=true;

                ui->widgetStrings->setData(getDevice(),stringsOptions,true,this);
            }
        }
        else if(nType==SDEX::TYPE_MEMORYMAP)
        {
            if(!isInitPresent(sInit))
            {
                ui->widgetMemoryMap->setData(getDevice(),dex.getFileType(),getOptions().sSearchSignaturesPath);
            }
        }
        else if(nType==SDEX::TYPE_ENTROPY)
        {
            if(!isInitPresent(sInit))
            {
                ui->widgetEntropy->setData(getDevice(),0,getDevice()->size(),dex.getFileType(),true,this);
            }
        }
        else if(nType==SDEX::TYPE_HEURISTICSCAN)
        {
            if(!isInitPresent(sInit))
            {
                ui->widgetHeuristicScan->setData(getDevice(),true,XBinary::FT_DEX,this);
            }
        }
        else if(nType==SDEX::TYPE_HEADER)
        {
            if(!isInitPresent(sInit))
            {
                createHeaderTable(SDEX::TYPE_HEADER,ui->tableWidget_Header,N_DEX_HEADER::records,g_lineEdit_HEADER,N_DEX_HEADER::__data_size,0);

                g_comboBox[CB_Dex_Header_magic]=createComboBox(ui->tableWidget_Header,XDEX::getHeaderMagics(),SDEX::TYPE_HEADER,N_DEX_HEADER::magic,XComboBoxEx::CBTYPE_LIST);
                g_comboBox[CB_Dex_Header_version]=createComboBox(ui->tableWidget_Header,XDEX::getHeaderVersions(),SDEX::TYPE_HEADER,N_DEX_HEADER::version,XComboBoxEx::CBTYPE_LIST);
                g_comboBox[CB_Dex_Header_endian_tag]=createComboBox(ui->tableWidget_Header,XDEX::getHeaderEndianTags(),SDEX::TYPE_HEADER,N_DEX_HEADER::endian_tag,XComboBoxEx::CBTYPE_LIST);

                g_invWidget[INV_link]=createInvWidget(ui->tableWidget_Header,SDEX::TYPE_HEADER,N_DEX_HEADER::link_off,InvWidget::TYPE_HEX);
                g_invWidget[INV_map]=createInvWidget(ui->tableWidget_Header,SDEX::TYPE_HEADER,N_DEX_HEADER::map_off,InvWidget::TYPE_HEX);
                g_invWidget[INV_string_ids]=createInvWidget(ui->tableWidget_Header,SDEX::TYPE_HEADER,N_DEX_HEADER::string_ids_off,InvWidget::TYPE_HEX);
                g_invWidget[INV_type_ids]=createInvWidget(ui->tableWidget_Header,SDEX::TYPE_HEADER,N_DEX_HEADER::type_ids_off,InvWidget::TYPE_HEX);
                g_invWidget[INV_proto_ids]=createInvWidget(ui->tableWidget_Header,SDEX::TYPE_HEADER,N_DEX_HEADER::proto_ids_off,InvWidget::TYPE_HEX);
                g_invWidget[INV_field_ids]=createInvWidget(ui->tableWidget_Header,SDEX::TYPE_HEADER,N_DEX_HEADER::field_ids_off,InvWidget::TYPE_HEX);
                g_invWidget[INV_method_ids]=createInvWidget(ui->tableWidget_Header,SDEX::TYPE_HEADER,N_DEX_HEADER::method_ids_off,InvWidget::TYPE_HEX);
                g_invWidget[INV_class_defs]=createInvWidget(ui->tableWidget_Header,SDEX::TYPE_HEADER,N_DEX_HEADER::class_defs_off,InvWidget::TYPE_HEX);
                g_invWidget[INV_data]=createInvWidget(ui->tableWidget_Header,SDEX::TYPE_HEADER,N_DEX_HEADER::data_off,InvWidget::TYPE_HEX);

                blockSignals(true);

                XDEX_DEF::HEADER header=dex.getHeader();

                g_lineEdit_HEADER[N_DEX_HEADER::magic]->setValue(header.magic);
                g_lineEdit_HEADER[N_DEX_HEADER::version]->setValue(header.version);
                g_lineEdit_HEADER[N_DEX_HEADER::checksum]->setValue(header.checksum);

                g_lineEdit_HEADER[N_DEX_HEADER::file_size]->setValue(header.file_size);
                g_lineEdit_HEADER[N_DEX_HEADER::header_size]->setValue(header.header_size);
                g_lineEdit_HEADER[N_DEX_HEADER::endian_tag]->setValue(header.endian_tag);
                g_lineEdit_HEADER[N_DEX_HEADER::link_size]->setValue(header.link_size);
                g_lineEdit_HEADER[N_DEX_HEADER::link_off]->setValue(header.link_off);
                g_lineEdit_HEADER[N_DEX_HEADER::map_off]->setValue(header.map_off);
                g_lineEdit_HEADER[N_DEX_HEADER::string_ids_size]->setValue(header.string_ids_size);
                g_lineEdit_HEADER[N_DEX_HEADER::string_ids_off]->setValue(header.string_ids_off);
                g_lineEdit_HEADER[N_DEX_HEADER::type_ids_size]->setValue(header.type_ids_size);
                g_lineEdit_HEADER[N_DEX_HEADER::type_ids_off]->setValue(header.type_ids_off);
                g_lineEdit_HEADER[N_DEX_HEADER::proto_ids_size]->setValue(header.proto_ids_size);
                g_lineEdit_HEADER[N_DEX_HEADER::proto_ids_off]->setValue(header.proto_ids_off);
                g_lineEdit_HEADER[N_DEX_HEADER::field_ids_size]->setValue(header.field_ids_size);
                g_lineEdit_HEADER[N_DEX_HEADER::field_ids_off]->setValue(header.field_ids_off);
                g_lineEdit_HEADER[N_DEX_HEADER::method_ids_size]->setValue(header.method_ids_size);
                g_lineEdit_HEADER[N_DEX_HEADER::method_ids_off]->setValue(header.method_ids_off);
                g_lineEdit_HEADER[N_DEX_HEADER::class_defs_size]->setValue(header.class_defs_size);
                g_lineEdit_HEADER[N_DEX_HEADER::class_defs_off]->setValue(header.class_defs_off);
                g_lineEdit_HEADER[N_DEX_HEADER::data_size]->setValue(header.data_size);
                g_lineEdit_HEADER[N_DEX_HEADER::data_off]->setValue(header.data_off);

                g_comboBox[CB_Dex_Header_magic]->setValue(header.magic);
                g_comboBox[CB_Dex_Header_version]->setValue(header.version);
                g_comboBox[CB_Dex_Header_endian_tag]->setValue(header.endian_tag);

                g_invWidget[INV_link]->setOffsetAndSize(&dex,header.link_off,header.link_size);
                g_invWidget[INV_map]->setOffsetAndSize(&dex,header.map_off,0);
                g_invWidget[INV_string_ids]->setOffsetAndSize(&dex,header.string_ids_off,header.string_ids_size*sizeof(XDEX_DEF::STRING_ITEM_ID));
                g_invWidget[INV_type_ids]->setOffsetAndSize(&dex,header.type_ids_off,header.type_ids_size*sizeof(XDEX_DEF::TYPE_ITEM_ID));
                g_invWidget[INV_proto_ids]->setOffsetAndSize(&dex,header.proto_ids_off,header.proto_ids_size*sizeof(XDEX_DEF::PROTO_ITEM_ID));
                g_invWidget[INV_field_ids]->setOffsetAndSize(&dex,header.field_ids_off,header.field_ids_size*sizeof(XDEX_DEF::FIELD_ITEM_ID));
                g_invWidget[INV_method_ids]->setOffsetAndSize(&dex,header.method_ids_off,header.method_ids_size*sizeof(XDEX_DEF::METHOD_ITEM_ID));
                g_invWidget[INV_class_defs]->setOffsetAndSize(&dex,header.class_defs_off,header.class_defs_size*sizeof(XDEX_DEF::CLASS_ITEM_DEF));
                g_invWidget[INV_data]->setOffsetAndSize(&dex,header.data_off,header.data_size);

                quint32 nHeaderSize=dex.getHeaderSize();

                loadHexSubdevice(0,nHeaderSize,0,&g_subDevice[SDEX::TYPE_HEADER],ui->widgetHex_Header);

                blockSignals(false);
            }
        }
        else if(nType==SDEX::TYPE_MAPITEMS)
        {
            if(!isInitPresent(sInit))
            {
                DEXProcessData dexProcessData(SDEX::TYPE_MAPITEMS,&g_tvModel[SDEX::TYPE_MAPITEMS],&dex,nDataOffset,nDataSize);

                ajustTableView(&dexProcessData,&g_tvModel[SDEX::TYPE_MAPITEMS],ui->tableView_MapItems);

                if(g_tvModel[SDEX::TYPE_MAPITEMS]->rowCount())
                {
                    ui->tableView_MapItems->setCurrentIndex(ui->tableView_MapItems->model()->index(0,0));
                }
            }
        }
        else if(nType==SDEX::TYPE_STRING_ID_ITEM)
        {
            if(!isInitPresent(sInit))
            {
                DEXProcessData dexProcessData(SDEX::TYPE_STRING_ID_ITEM,&g_tvModel[SDEX::TYPE_STRING_ID_ITEM],&dex,nDataOffset,nDataSize);

                ajustTableView(&dexProcessData,&g_tvModel[SDEX::TYPE_STRING_ID_ITEM],ui->tableView_STRING_ID_ITEM,g_pFilterStrings);

                if(g_tvModel[SDEX::TYPE_STRING_ID_ITEM]->rowCount())
                {
                    ui->tableView_STRING_ID_ITEM->setCurrentIndex(ui->tableView_STRING_ID_ITEM->model()->index(0,0));
                }
            }
        }
        else if(nType==SDEX::TYPE_TYPE_ID_ITEM)
        {
            if(!isInitPresent(sInit))
            {
                DEXProcessData dexProcessData(SDEX::TYPE_TYPE_ID_ITEM,&g_tvModel[SDEX::TYPE_TYPE_ID_ITEM],&dex,nDataOffset,nDataSize);

                ajustTableView(&dexProcessData,&g_tvModel[SDEX::TYPE_TYPE_ID_ITEM],ui->tableView_TYPE_ID_ITEM,g_pFilterTypes);

                if(g_tvModel[SDEX::TYPE_TYPE_ID_ITEM]->rowCount())
                {
                    ui->tableView_TYPE_ID_ITEM->setCurrentIndex(ui->tableView_TYPE_ID_ITEM->model()->index(0,0));
                }
            }
        }
        else if(nType==SDEX::TYPE_PROTO_ID_ITEM)
        {
            if(!isInitPresent(sInit))
            {
                DEXProcessData dexProcessData(SDEX::TYPE_PROTO_ID_ITEM,&g_tvModel[SDEX::TYPE_PROTO_ID_ITEM],&dex,nDataOffset,nDataSize);

                ajustTableView(&dexProcessData,&g_tvModel[SDEX::TYPE_PROTO_ID_ITEM],ui->tableView_PROTO_ID_ITEM);

                if(g_tvModel[SDEX::TYPE_PROTO_ID_ITEM]->rowCount())
                {
                    ui->tableView_PROTO_ID_ITEM->setCurrentIndex(ui->tableView_PROTO_ID_ITEM->model()->index(0,0));
                }
            }
        }
        else if(nType==SDEX::TYPE_FIELD_ID_ITEM)
        {
            if(!isInitPresent(sInit))
            {
                DEXProcessData dexProcessData(SDEX::TYPE_FIELD_ID_ITEM,&g_tvModel[SDEX::TYPE_FIELD_ID_ITEM],&dex,nDataOffset,nDataSize);

                ajustTableView(&dexProcessData,&g_tvModel[SDEX::TYPE_FIELD_ID_ITEM],ui->tableView_FIELD_ID_ITEM);

                if(g_tvModel[SDEX::TYPE_FIELD_ID_ITEM]->rowCount())
                {
                    ui->tableView_FIELD_ID_ITEM->setCurrentIndex(ui->tableView_FIELD_ID_ITEM->model()->index(0,0));
                }
            }
        }
        else if(nType==SDEX::TYPE_METHOD_ID_ITEM)
        {
            if(!isInitPresent(sInit))
            {
                DEXProcessData dexProcessData(SDEX::TYPE_METHOD_ID_ITEM,&g_tvModel[SDEX::TYPE_METHOD_ID_ITEM],&dex,nDataOffset,nDataSize);

                ajustTableView(&dexProcessData,&g_tvModel[SDEX::TYPE_METHOD_ID_ITEM],ui->tableView_METHOD_ID_ITEM);

                if(g_tvModel[SDEX::TYPE_METHOD_ID_ITEM]->rowCount())
                {
                    ui->tableView_METHOD_ID_ITEM->setCurrentIndex(ui->tableView_METHOD_ID_ITEM->model()->index(0,0));
                }
            }
        }
        else if(nType==SDEX::TYPE_CLASS_DEF_ITEM)
        {
            if(!isInitPresent(sInit))
            {
                DEXProcessData dexProcessData(SDEX::TYPE_CLASS_DEF_ITEM,&g_tvModel[SDEX::TYPE_CLASS_DEF_ITEM],&dex,nDataOffset,nDataSize);

                ajustTableView(&dexProcessData,&g_tvModel[SDEX::TYPE_CLASS_DEF_ITEM],ui->tableView_CLASS_DEF_ITEM);

                if(g_tvModel[SDEX::TYPE_CLASS_DEF_ITEM]->rowCount())
                {
                    ui->tableView_CLASS_DEF_ITEM->setCurrentIndex(ui->tableView_CLASS_DEF_ITEM->model()->index(0,0));
                }
            }
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }

    addInit(sInit);
}


bool DEXWidget::createSectionTable(int nType, QTableWidget *pTableWidget, const FW_DEF::HEADER_RECORD *pHeaderRecord, int nNumberOfRecords)
{
    Q_UNUSED(nType)

    QStringList slHeader;

    pTableWidget->setRowCount(0);

    for(int i=0; i<nNumberOfRecords; i++)
    {
        slHeader.append(pHeaderRecord[i].sName);
    }

    pTableWidget->setHorizontalHeaderLabels(slHeader);
    pTableWidget->horizontalHeader()->setVisible(true);

    return true;
}

void DEXWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget=qobject_cast<QWidget *>(sender());
    int nStype=pWidget->property("STYPE").toInt();
    int nNdata=pWidget->property("NDATA").toInt();

    switch(nStype)
    {
        case SDEX::TYPE_HEADER:
            switch(nNdata)
            {
                case N_DEX_HEADER::magic:      g_lineEdit_HEADER[N_DEX_HEADER::magic]->setValue((quint32)nValue);      break;
            }

            break;
    }
}

void DEXWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pCurrent, QTreeWidgetItem *pPrevious)
{
    Q_UNUSED(pPrevious)

    if(pCurrent)
    {
        reloadData();
        addPage(pCurrent);
        ui->toolButtonPrev->setEnabled(isPrevPageAvailable());
        ui->toolButtonNext->setEnabled(isNextPageAvailable());
    }
}

void DEXWidget::on_checkBoxReadonly_toggled(bool bChecked)
{
    setReadonly(bChecked);
}

void DEXWidget::on_pushButtonReload_clicked()
{
    ui->pushButtonReload->setEnabled(false);
    reload();

    QTimer::singleShot(1000,this,SLOT(enableButton()));
}

void DEXWidget::enableButton()
{
    ui->pushButtonReload->setEnabled(true);
}

void DEXWidget::on_tableWidget_Header_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_Header,ui->tableWidget_Header);
}

void DEXWidget::on_lineEditFilterStrings_textChanged(const QString &sString)
{
    g_pFilterStrings->setFilterRegExp(sString);
    g_pFilterStrings->setFilterCaseSensitivity(Qt::CaseInsensitive);
    g_pFilterStrings->setFilterKeyColumn(3);
}

void DEXWidget::on_lineEditFilterTypes_textChanged(const QString &sString)
{
    g_pFilterTypes->setFilterRegExp(sString);
    g_pFilterTypes->setFilterCaseSensitivity(Qt::CaseInsensitive);
    g_pFilterTypes->setFilterKeyColumn(3);
}

void DEXWidget::on_toolButtonPrev_clicked()
{
    setAddPageEnabled(false);
    ui->treeWidgetNavi->setCurrentItem(getPrevPage());
    setAddPageEnabled(true);
}

void DEXWidget::on_toolButtonNext_clicked()
{
    setAddPageEnabled(false);
    ui->treeWidgetNavi->setCurrentItem(getNextPage());
    setAddPageEnabled(true);
}
