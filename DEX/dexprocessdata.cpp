/* Copyright (c) 2017-2022 hors<horsicq@gmail.com>
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
#include "dexprocessdata.h"

DEXProcessData::DEXProcessData(int nType, QStandardItemModel **ppModel, XDEX *pDEX, qint64 nOffset, qint64 nSize)
    : ProcessData()
{
    this->g_nType = nType;
    this->g_ppModel = ppModel;
    this->g_pDEX = pDEX;
    this->g_nOffset = nOffset;
    this->g_nSize = nSize;
}

void DEXProcessData::_process()
{
    if (g_nType == SDEX::TYPE_MAPITEMS) {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append("");

        listLabels.append(getStructList(N_DEX_MAP_ITEM::records, N_DEX_MAP_ITEM::__data_size));

        QList<XDEX_DEF::MAP_ITEM> listMapItems = g_pDEX->getMapItems();

        qint32 nNumberOfMapItems = listMapItems.count();

        *g_ppModel = new QStandardItemModel(nNumberOfMapItems, listLabels.count());

        setMaximum(nNumberOfMapItems);

        setHeader(*g_ppModel, &listLabels);

        QMap<quint64, QString> mapTypes = XDEX::getTypesS();

        for (qint32 i = 0; (i < nNumberOfMapItems) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem(QString::number(i));

            //            pItem->setData(nStringTableOffset,Qt::UserRole+FW_DEF::SECTION_DATA_STRINGTABLEOFFSET);
            //            pItem->setData(nStringTableSize,Qt::UserRole+FW_DEF::SECTION_DATA_STRINGTABLESIZE);

            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, 1, new QStandardItem(XBinary::valueToHex(listMapItems.at(i).nType)));
            (*g_ppModel)->setItem(i, N_DEX_MAP_ITEM::type + 2, new QStandardItem(mapTypes.value(listMapItems.at(i).nType, XBinary::valueToHex(listMapItems.at(i).nType))));
            (*g_ppModel)->setItem(i, N_DEX_MAP_ITEM::count + 2, new QStandardItem(XBinary::valueToHex(listMapItems.at(i).nCount)));
            (*g_ppModel)->setItem(i, N_DEX_MAP_ITEM::offset + 2, new QStandardItem(XBinary::valueToHex(listMapItems.at(i).nOffset)));

            incValue();
        }
    } else if (g_nType == SDEX::TYPE_STRING_ID_ITEM) {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append("");
        listLabels.append(getStructList(N_DEX_STRING_ID::records, N_DEX_STRING_ID::__data_size));
        listLabels.append("");

        QList<XDEX_DEF::STRING_ITEM_ID> listStringItemIds = g_pDEX->getList_STRING_ITEM_ID();
        qint64 nDataOffset = g_pDEX->getHeader_data_off();
        qint64 nDataSize = g_pDEX->getHeader_data_size();
        QByteArray baData = g_pDEX->read_array(nDataOffset, nDataSize);

        qint32 nNumberOfIds = listStringItemIds.count();

        *g_ppModel = new QStandardItemModel(nNumberOfIds, listLabels.count());

        setMaximum(nNumberOfIds);

        setHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfIds) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem(QString::number(i));

            QString sString = g_pDEX->getStringItemIdString(listStringItemIds.at(i), baData.data(), nDataSize, nDataOffset);

            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, 1, new QStandardItem(XBinary::valueToHex(i)));
            (*g_ppModel)->setItem(i, N_DEX_STRING_ID::string_data_off + 2, new QStandardItem(XBinary::valueToHex(listStringItemIds.at(i).string_data_off)));
            (*g_ppModel)->setItem(i, N_DEX_STRING_ID::string_data_off + 3, new QStandardItem(sString));

            incValue();
        }
    } else if (g_nType == SDEX::TYPE_TYPE_ID_ITEM) {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append("");
        listLabels.append(getStructList(N_DEX_TYPE_ID::records, N_DEX_TYPE_ID::__data_size));
        listLabels.append("");

        QList<XDEX_DEF::MAP_ITEM> listMapItems = g_pDEX->getMapItems();
        qint64 nDataOffset = g_pDEX->getHeader_data_off();
        qint64 nDataSize = g_pDEX->getHeader_data_size();
        QByteArray baData = g_pDEX->read_array(nDataOffset, nDataSize);

        QList<XDEX_DEF::TYPE_ITEM_ID> listTypeItemIds = g_pDEX->getList_TYPE_ITEM_ID(&listMapItems);

        XDEX_DEF::MAP_ITEM mapItemStrings = g_pDEX->getMapItem(XDEX_DEF::TYPE_STRING_ID_ITEM, &listMapItems);

        qint32 nNumberOfIds = listTypeItemIds.count();

        *g_ppModel = new QStandardItemModel(nNumberOfIds, listLabels.count());

        setMaximum(nNumberOfIds);

        setHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfIds) && (isRun()); i++) {
            QString sString = g_pDEX->getTypeItemIdString(listTypeItemIds.at(i), &mapItemStrings, baData.data(), nDataSize, nDataOffset);

            QStandardItem *pItem = new QStandardItem(QString::number(i));

            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, 1, new QStandardItem(XBinary::valueToHex(i)));
            (*g_ppModel)->setItem(i, N_DEX_TYPE_ID::descriptor_idx + 2, new QStandardItem(XBinary::valueToHex(listTypeItemIds.at(i).descriptor_idx)));
            (*g_ppModel)->setItem(i, N_DEX_TYPE_ID::descriptor_idx + 3, new QStandardItem(sString));

            incValue();
        }
    } else if (g_nType == SDEX::TYPE_PROTO_ID_ITEM) {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append("");
        listLabels.append(getStructList(N_DEX_PROTO_ID::records, N_DEX_PROTO_ID::__data_size));
        listLabels.append("shorty");
        listLabels.append("return_type");

        QList<XDEX_DEF::MAP_ITEM> listMapItems = g_pDEX->getMapItems();
        XDEX_DEF::MAP_ITEM mapItemString = g_pDEX->getMapItem(XDEX_DEF::TYPE_STRING_ID_ITEM, &listMapItems);

        QList<XDEX_DEF::STRING_ITEM_ID> listStringID = g_pDEX->getList_STRING_ITEM_ID(&listMapItems);
        QList<XDEX_DEF::TYPE_ITEM_ID> listItemID = g_pDEX->getList_TYPE_ITEM_ID(&listMapItems);

        qint64 nDataOffset = g_pDEX->getHeader_data_off();
        qint64 nDataSize = g_pDEX->getHeader_data_size();
        QByteArray baData = g_pDEX->read_array(nDataOffset, nDataSize);

        QList<XDEX_DEF::PROTO_ITEM_ID> listProtoIDs = g_pDEX->getList_PROTO_ITEM_ID(&listMapItems);

        qint32 nNumberOfIds = listProtoIDs.count();

        *g_ppModel = new QStandardItemModel(nNumberOfIds, listLabels.count());

        setMaximum(nNumberOfIds);

        setHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfIds) && (isRun()); i++) {
            //            QString
            //            sString=pDEX->getProtoItemIdString(listProtoIDs.at(i),&mapItemStrings);

            QStandardItem *pItem = new QStandardItem(QString::number(i));

            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, 1, new QStandardItem(XBinary::valueToHex(i)));
            (*g_ppModel)->setItem(i, N_DEX_PROTO_ID::shorty_idx + 2, new QStandardItem(XBinary::valueToHex(listProtoIDs.at(i).shorty_idx)));
            (*g_ppModel)->setItem(i, N_DEX_PROTO_ID::return_type_idx + 2, new QStandardItem(XBinary::valueToHex(listProtoIDs.at(i).return_type_idx)));
            (*g_ppModel)->setItem(i, N_DEX_PROTO_ID::parameters_off + 2, new QStandardItem(XBinary::valueToHex(listProtoIDs.at(i).parameters_off)));

            QString sShorty = g_pDEX->getStringItemIdString(&listStringID, listProtoIDs.at(i).shorty_idx, baData.data(), nDataSize, nDataOffset);
            QString sReturnType = g_pDEX->getTypeItemIdString(&listItemID, listProtoIDs.at(i).return_type_idx, &mapItemString, baData.data(), nDataSize, nDataOffset);

            (*g_ppModel)->setItem(i, N_DEX_PROTO_ID::parameters_off + 3, new QStandardItem(sShorty));
            (*g_ppModel)->setItem(i, N_DEX_PROTO_ID::parameters_off + 4, new QStandardItem(sReturnType));

            incValue();
        }
    } else if (g_nType == SDEX::TYPE_FIELD_ID_ITEM) {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append("");
        listLabels.append(getStructList(N_DEX_FIELD_ID::records, N_DEX_FIELD_ID::__data_size));
        // TODO type
        listLabels.append("name");

        QList<XDEX_DEF::MAP_ITEM> listMapItems = g_pDEX->getMapItems();

        QList<XDEX_DEF::STRING_ITEM_ID> listStringID = g_pDEX->getList_STRING_ITEM_ID(&listMapItems);

        qint64 nDataOffset = g_pDEX->getHeader_data_off();
        qint64 nDataSize = g_pDEX->getHeader_data_size();
        QByteArray baData = g_pDEX->read_array(nDataOffset, nDataSize);

        QList<XDEX_DEF::FIELD_ITEM_ID> listFieldIDs = g_pDEX->getList_FIELD_ITEM_ID(&listMapItems);

        qint32 nNumberOfIds = listFieldIDs.count();

        *g_ppModel = new QStandardItemModel(nNumberOfIds, listLabels.count());

        setMaximum(nNumberOfIds);

        setHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfIds) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem(QString::number(i));

            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, 1, new QStandardItem(XBinary::valueToHex(i)));
            (*g_ppModel)->setItem(i, N_DEX_FIELD_ID::class_idx + 2, new QStandardItem(XBinary::valueToHex(listFieldIDs.at(i).class_idx)));
            (*g_ppModel)->setItem(i, N_DEX_FIELD_ID::type_idx + 2, new QStandardItem(XBinary::valueToHex(listFieldIDs.at(i).type_idx)));
            (*g_ppModel)->setItem(i, N_DEX_FIELD_ID::name_idx + 2, new QStandardItem(XBinary::valueToHex(listFieldIDs.at(i).name_idx)));

            QString sName = g_pDEX->getStringItemIdString(&listStringID, listFieldIDs.at(i).name_idx, baData.data(), nDataSize, nDataOffset);

            (*g_ppModel)->setItem(i, N_DEX_FIELD_ID::name_idx + 3, new QStandardItem(sName));

            incValue();
        }
    } else if (g_nType == SDEX::TYPE_METHOD_ID_ITEM) {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append("");
        listLabels.append(getStructList(N_DEX_METHOD_ID::records, N_DEX_METHOD_ID::__data_size));
        listLabels.append("name");

        QList<XDEX_DEF::MAP_ITEM> listMapItems = g_pDEX->getMapItems();

        QList<XDEX_DEF::STRING_ITEM_ID> listStringID = g_pDEX->getList_STRING_ITEM_ID(&listMapItems);

        qint64 nDataOffset = g_pDEX->getHeader_data_off();
        qint64 nDataSize = g_pDEX->getHeader_data_size();
        QByteArray baData = g_pDEX->read_array(nDataOffset, nDataSize);

        QList<XDEX_DEF::METHOD_ITEM_ID> listMethodIDs = g_pDEX->getList_METHOD_ITEM_ID(&listMapItems);

        qint32 nNumberOfIds = listMethodIDs.count();

        *g_ppModel = new QStandardItemModel(nNumberOfIds, listLabels.count());

        setMaximum(nNumberOfIds);

        setHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfIds) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem(QString::number(i));

            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, 1, new QStandardItem(XBinary::valueToHex(i)));
            (*g_ppModel)->setItem(i, N_DEX_METHOD_ID::class_idx + 2, new QStandardItem(XBinary::valueToHex(listMethodIDs.at(i).class_idx)));
            (*g_ppModel)->setItem(i, N_DEX_METHOD_ID::proto_idx + 2, new QStandardItem(XBinary::valueToHex(listMethodIDs.at(i).proto_idx)));
            (*g_ppModel)->setItem(i, N_DEX_METHOD_ID::name_idx + 2, new QStandardItem(XBinary::valueToHex(listMethodIDs.at(i).name_idx)));

            QString sName = g_pDEX->getStringItemIdString(&listStringID, listMethodIDs.at(i).name_idx, baData.data(), nDataSize, nDataOffset);

            (*g_ppModel)->setItem(i, N_DEX_METHOD_ID::name_idx + 3, new QStandardItem(sName));

            incValue();
        }
    } else if (g_nType == SDEX::TYPE_CLASS_DEF_ITEM) {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append("");
        listLabels.append(getStructList(N_DEX_CLASS_ITEM_DEF::records, N_DEX_CLASS_ITEM_DEF::__data_size));

        QList<XDEX_DEF::MAP_ITEM> listMapItems = g_pDEX->getMapItems();

        QList<XDEX_DEF::CLASS_ITEM_DEF> listClassDefss = g_pDEX->getList_CLASS_ITEM_DEF(&listMapItems);

        qint32 nNumberOfDefs = listClassDefss.count();

        *g_ppModel = new QStandardItemModel(nNumberOfDefs, listLabels.count());

        setMaximum(nNumberOfDefs);

        setHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; (i < nNumberOfDefs) && (isRun()); i++) {
            QStandardItem *pItem = new QStandardItem(QString::number(i));

            (*g_ppModel)->setItem(i, 0, pItem);
            (*g_ppModel)->setItem(i, 1, new QStandardItem(XBinary::valueToHex(i)));
            (*g_ppModel)->setItem(i, N_DEX_CLASS_ITEM_DEF::class_idx + 2, new QStandardItem(XBinary::valueToHex(listClassDefss.at(i).class_idx)));
            (*g_ppModel)->setItem(i, N_DEX_CLASS_ITEM_DEF::access_flags + 2, new QStandardItem(XBinary::valueToHex(listClassDefss.at(i).access_flags)));
            (*g_ppModel)->setItem(i, N_DEX_CLASS_ITEM_DEF::superclass_idx + 2, new QStandardItem(XBinary::valueToHex(listClassDefss.at(i).superclass_idx)));
            (*g_ppModel)->setItem(i, N_DEX_CLASS_ITEM_DEF::interfaces_off + 2, new QStandardItem(XBinary::valueToHex(listClassDefss.at(i).interfaces_off)));
            (*g_ppModel)->setItem(i, N_DEX_CLASS_ITEM_DEF::source_file_idx + 2, new QStandardItem(XBinary::valueToHex(listClassDefss.at(i).source_file_idx)));
            (*g_ppModel)->setItem(i, N_DEX_CLASS_ITEM_DEF::annotations_off + 2, new QStandardItem(XBinary::valueToHex(listClassDefss.at(i).annotations_off)));
            (*g_ppModel)->setItem(i, N_DEX_CLASS_ITEM_DEF::class_data_off + 2, new QStandardItem(XBinary::valueToHex(listClassDefss.at(i).class_data_off)));
            (*g_ppModel)->setItem(i, N_DEX_CLASS_ITEM_DEF::static_values_off + 2, new QStandardItem(XBinary::valueToHex(listClassDefss.at(i).static_values_off)));

            incValue();
        }
    }
}

void DEXProcessData::ajustTableView(QWidget *pWidget, QTableView *pTableView)
{
    XBinary::MODE mode = g_pDEX->getMode();

    if (g_nType == SDEX::TYPE_MAPITEMS) {
        pTableView->setColumnWidth(0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGMID, mode));
        pTableView->setColumnWidth(3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(4, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
    } else if (g_nType == SDEX::TYPE_STRING_ID_ITEM) {
        pTableView->setColumnWidth(0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGLONG, mode));
    } else if (g_nType == SDEX::TYPE_TYPE_ID_ITEM) {
        pTableView->setColumnWidth(0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_STRINGLONG, mode));
    } else if (g_nType == SDEX::TYPE_PROTO_ID_ITEM) {
        pTableView->setColumnWidth(0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(4, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
    } else if (g_nType == SDEX::TYPE_FIELD_ID_ITEM) {
        pTableView->setColumnWidth(0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(4, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
    } else if (g_nType == SDEX::TYPE_METHOD_ID_ITEM) {
        pTableView->setColumnWidth(0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(4, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
    } else if (g_nType == SDEX::TYPE_CLASS_DEF_ITEM) {
        pTableView->setColumnWidth(0, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT16, mode));
        pTableView->setColumnWidth(1, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(2, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(3, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(4, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(5, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(6, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(7, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(8, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
        pTableView->setColumnWidth(9, FormatWidget::getColumnWidth(pWidget, FormatWidget::CW_UINT32, mode));
    }
}
