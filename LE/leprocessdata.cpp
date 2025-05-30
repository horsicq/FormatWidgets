/* Copyright (c) 2019-2025 hors<horsicq@gmail.com>
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
#include "leprocessdata.h"

LEProcessData::LEProcessData(qint32 nType, QStandardItemModel **ppModel, XLE *pLE, qint64 nOffset, qint64 nSize) : ProcessData()
{
    this->g_nType = nType;
    this->g_ppModel = ppModel;
    this->g_pLE = pLE;
    this->g_nOffset = nOffset;
    this->g_nSize = nSize;
}

void LEProcessData::_process()
{
    if (g_nType == SLE::TYPE_OBJECTS) {
        QList<QString> listLabels;
        listLabels.append("");
        listLabels.append(getStructList(N_O32_OBJ::records, N_O32_OBJ::__data_size));

        QList<XLE_DEF::o32_obj> listObjects = g_pLE->getObjects();

        qint32 nNumberOfObjects = listObjects.count();

        *g_ppModel = new QStandardItemModel(nNumberOfObjects, listLabels.count());

        setMaximum(nNumberOfObjects);

        setTableHeader(*g_ppModel, &listLabels);

        for (qint32 i = 0; i < nNumberOfObjects; i++) {
            QStandardItem *pItemNumber = new QStandardItem;
            pItemNumber->setData(i, Qt::DisplayRole);

            (*g_ppModel)->setItem(i, 0, pItemNumber);

            (*g_ppModel)->setItem(i, N_O32_OBJ::o32_size + 1, new QStandardItem(XBinary::valueToHex(listObjects.at(i).o32_size)));
            (*g_ppModel)->setItem(i, N_O32_OBJ::o32_base + 1, new QStandardItem(XBinary::valueToHex(listObjects.at(i).o32_base)));
            (*g_ppModel)->setItem(i, N_O32_OBJ::o32_flags + 1, new QStandardItem(XBinary::valueToHex(listObjects.at(i).o32_flags)));
            (*g_ppModel)->setItem(i, N_O32_OBJ::o32_pagemap + 1, new QStandardItem(XBinary::valueToHex(listObjects.at(i).o32_pagemap)));
            (*g_ppModel)->setItem(i, N_O32_OBJ::o32_mapsize + 1, new QStandardItem(XBinary::valueToHex(listObjects.at(i).o32_mapsize)));
            (*g_ppModel)->setItem(i, N_O32_OBJ::o32_reserved + 1, new QStandardItem(XBinary::valueToHex(listObjects.at(i).o32_reserved)));

            incValue();
        }
    }
}

void LEProcessData::ajustTableView(qint32 nType, QTableView *pTableView)
{
    // TODO like MACH !!!
    XBinary::MODE mode = g_pLE->getMode();

    if (nType == SLE::TYPE_OBJECTS) {
        XOptions::setTableViewHeaderWidth(pTableView, 0, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT16, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 1, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 2, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 3, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 4, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 5, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
        XOptions::setTableViewHeaderWidth(pTableView, 6, FormatWidget::getColumnWidth(pTableView, FormatWidget::CW_UINT32, mode));
    }
}
