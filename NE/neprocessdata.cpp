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
#include "neprocessdata.h"

NEProcessData::NEProcessData(int nType,QStandardItemModel **ppModel,XNE *pNE,qint64 nOffset,qint64 nSize) : ProcessData()
{
    this->g_nType=nType;
    this->g_ppModel=ppModel;
    this->g_pNE=pNE;
    this->g_nOffset=nOffset;
    this->g_nSize=nSize;
}

void NEProcessData::_process()
{
    if(g_nType==SNE::TYPE_SEGMENTS)
    {
        QList<QString> listLabels;
        listLabels.append("");

        listLabels.append(getStructList(N_NE_SEGMENT::records,N_NE_SEGMENT::__data_size));

        QList<XNE_DEF::NE_SEGMENT> listSegments=g_pNE->getSegmentList();

        quint16 nShift=g_pNE->getImageOS2Header_align();

        qint32 nNumberOfSegments=listSegments.count();

        *g_ppModel=new QStandardItemModel(nNumberOfSegments,listLabels.count());

        setMaximum(nNumberOfSegments);

        setHeader(*g_ppModel,&listLabels);

        for(qint32 i=0;(i<nNumberOfSegments)&&(isRun());i++)
        {
            QStandardItem *pItem=new QStandardItem(QString::number(i));

            pItem->setData((listSegments.at(i).dwFileOffset<<nShift),Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET);
//            pItem->setData(listSegments.at(i).dwFileOffset,Qt::UserRole+FW_DEF::SECTION_DATA_HEADEROFFSET);
            pItem->setData((i+1)*0x10000,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS);
            pItem->setData(listSegments.at(i).dwFileSize,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE);

            (*g_ppModel)->setItem(i,0,                                      pItem);
            (*g_ppModel)->setItem(i,N_NE_SEGMENT::dwFileOffset+1,           new QStandardItem(XBinary::valueToHex(listSegments.at(i).dwFileOffset)));
            (*g_ppModel)->setItem(i,N_NE_SEGMENT::dwFileSize+1,             new QStandardItem(XBinary::valueToHex(listSegments.at(i).dwFileSize)));
            (*g_ppModel)->setItem(i,N_NE_SEGMENT::dwFlags+1,                new QStandardItem(XBinary::valueToHex(listSegments.at(i).dwFlags)));
            (*g_ppModel)->setItem(i,N_NE_SEGMENT::dwMinAllocSize+1,         new QStandardItem(XBinary::valueToHex(listSegments.at(i).dwMinAllocSize)));

            incValue();
        }
    }
}

void NEProcessData::ajustTableView(QWidget *pWidget,QTableView *pTableView)
{
    Q_UNUSED(pWidget)
    Q_UNUSED(pTableView)
}
