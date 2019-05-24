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
#include "formatwidget.h"

FormatWidget::FormatWidget(QWidget *parent):
    QWidget(parent)
{
    bIsReadonly=false;
}

FormatWidget::FormatWidget(QIODevice *pDevice, OPTIONS *pOptions, QWidget *parent):
    QWidget(parent)
{
    setData(pDevice,pOptions);
}

FormatWidget::~FormatWidget()
{

}

void FormatWidget::setData(QIODevice *pDevice, FormatWidget::OPTIONS *pOptions)
{
    this->pDevice=pDevice;
    this->pOptions=pOptions;

    bIsReadonly=!(pDevice->isWritable());
}

QIODevice *FormatWidget::getDevice()
{
    return this->pDevice;
}

FormatWidget::OPTIONS *FormatWidget::getOptions()
{
    return this->pOptions;
}

bool FormatWidget::isReadonly()
{
    return bIsReadonly;
}

QTreeWidgetItem *FormatWidget::createNewItem(int nUserData,QString sTitle)
{
    QTreeWidgetItem *result=new QTreeWidgetItem;
    result->setText(0,sTitle);
    result->setData(0,Qt::UserRole,nUserData);

    return result;
}

int FormatWidget::getSymbolWidth()
{
    QFontMetrics fm(font());
    return fm.width("W");
}

void FormatWidget::setValue(QVariant vValue, int nStype, int nNdata, int nVtype,int nPosition)
{
    saveBackup();

    if(_setValue(vValue,nStype,nNdata,nVtype,nPosition))
    {
        pOptions->bEdited=true;
    }
}

void FormatWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{

}

void FormatWidget::hexValueChanged(quint64 nValue)
{
    XLineEditHEX *lineEdit=qobject_cast<XLineEditHEX *>(sender());
    int nStype=lineEdit->property("STYPE").toInt();
    int nNdata=lineEdit->property("NDATA").toInt();
    int nVtype=lineEdit->property("VTYPE").toInt();
    int nPosition=lineEdit->property("POSITION").toInt();

    setValue(nValue,nStype,nNdata,nVtype,nPosition);
}

void FormatWidget::textValueChanged(QString sText)
{
    XLineEditHEX *lineEdit=qobject_cast<XLineEditHEX *>(sender());
    int nStype=lineEdit->property("STYPE").toInt();
    int nNdata=lineEdit->property("NDATA").toInt();
    int nVtype=lineEdit->property("VTYPE").toInt();
    int nPosition=lineEdit->property("POSITION").toInt();

    setValue(sText,nStype,nNdata,nVtype,nPosition);
}

void FormatWidget::saveBackup()
{
    if(!pOptions->bEdited)
    {
        // Save backup
        if(pOptions->sBackupFileName!="")
        {
            if(!QFile::exists(pOptions->sBackupFileName))
            {
                if(pDevice->metaObject()->className()==QString("QFile"))
                {
                    QString sFileName=((QFile *)pDevice)->fileName();

                    if(!QFile::copy(sFileName,pOptions->sBackupFileName))
                    {
                        QMessageBox::critical(this,tr("Error"),tr("Cannot save file")+QString(": %1").arg(pOptions->sBackupFileName));
                    }
                }
            }
        }
    }
}

bool FormatWidget::createHeaderTable(int type, QTableWidget *pTableWidget, const HEADER_RECORD *pRecords, XLineEditHEX **ppLineEdits, int nRecordCount, int nPosition)
{
    pTableWidget->setColumnCount(4);
    pTableWidget->setRowCount(nRecordCount);

    adjustHeaderTable(type,pTableWidget);

    QStringList slHeader;
    slHeader.append(tr("Name"));
    slHeader.append(tr("Type"));
    slHeader.append(tr("Value"));
    slHeader.append(tr(""));

    pTableWidget->setHorizontalHeaderLabels(slHeader);
    pTableWidget->horizontalHeader()->setVisible(true);

    for(int i=0; i<nRecordCount; i++)
    {
        QTableWidgetItem *newItemName = new QTableWidgetItem;
        newItemName->setText(pRecords[i].pszName);
        pTableWidget->setItem(i,HEADER_COLUMN_NAME,newItemName);

        QTableWidgetItem *newItemType = new QTableWidgetItem;
        newItemType->setText(pRecords[i].pszType);
        pTableWidget->setItem(i,HEADER_COLUMN_TYPE,newItemType);

        ppLineEdits[i]=new XLineEditHEX(this);
        ppLineEdits[i]->setProperty("STYPE",type);
        ppLineEdits[i]->setProperty("NDATA",pRecords[i].nData);
        ppLineEdits[i]->setProperty("POSITION",nPosition);

        if(pRecords[i].vtype==VAL_TYPE_TEXT)
        {
            connect(ppLineEdits[i],SIGNAL(textChanged(QString)),this,SLOT(textValueChanged(QString)));
        }
        else
        {
            connect(ppLineEdits[i],SIGNAL(valueChanged(quint64)),this,SLOT(hexValueChanged(quint64)));
        }

        pTableWidget->setCellWidget(i,HEADER_COLUMN_VALUE,ppLineEdits[i]);

        if(pRecords[i].nSize==0)
        {
            ppLineEdits[i]->setEnabled(false);
        }
    }

    return true;
}

bool FormatWidget::createDirectoryTable(int type, QTableWidget *pTableWidget, const DIRECTORY_ENTRY_RECORD *pRecords, int nRecordCount)
{
    pTableWidget->setColumnCount(4);
    pTableWidget->setRowCount(nRecordCount);

    int nSymbolWidth=getSymbolWidth();

    pTableWidget->setColumnWidth(0,nSymbolWidth*3);
    pTableWidget->setColumnWidth(1,nSymbolWidth*12);
    pTableWidget->setColumnWidth(2,nSymbolWidth*8);
    pTableWidget->setColumnWidth(3,nSymbolWidth*8);

    QStringList slHeader;
    slHeader.append(tr(""));
    slHeader.append(tr("Name"));
    slHeader.append(tr("Address"));
    slHeader.append(tr("Size"));

    pTableWidget->setHorizontalHeaderLabels(slHeader);
    pTableWidget->horizontalHeader()->setVisible(true);

    for(int i=0; i<nRecordCount; i++)
    {
        QTableWidgetItem *newItemNumber = new QTableWidgetItem;
        newItemNumber->setText(QString("%1").arg(i));
        pTableWidget->setItem(i,DIRECTORY_COLUMN_NUMBER,newItemNumber);

        QTableWidgetItem *newItemName = new QTableWidgetItem;
        newItemName->setText(pRecords[i].pszName);
        pTableWidget->setItem(i,DIRECTORY_COLUMN_NAME,newItemName);

//        ppLineEdits1[i]=new XLineEditHEX(this);
//        ppLineEdits1[i]->setProperty("STYPE",type);
//        ppLineEdits1[i]->setProperty("NDATA",pRecords[i].nData);
//        ppLineEdits1[i]->setProperty("VTYPE",pRecords[i].vtype[0]);

//        connect(ppLineEdits1[i],SIGNAL(valueChanged(quint64)),this,SLOT(hexValueChanged(quint64)));

//        pTableWidget->setCellWidget(i,DIRECTORY_COLUMN_ADDRESS,ppLineEdits1[i]);

//        ppLineEdits2[i]=new XLineEditHEX(this);
//        ppLineEdits2[i]->setProperty("STYPE",type);
//        ppLineEdits2[i]->setProperty("NDATA",pRecords[i].nData);
//        ppLineEdits2[i]->setProperty("VTYPE",pRecords[i].vtype[1]);

//        connect(ppLineEdits2[i],SIGNAL(valueChanged(quint64)),this,SLOT(hexValueChanged(quint64)));

//        pTableWidget->setCellWidget(i,DIRECTORY_COLUMN_SIZE,ppLineEdits2[i]);
    }

    return true;
}

bool FormatWidget::createSectionTable(int type, QTableWidget *pTableWidget, const FormatWidget::HEADER_RECORD *pRecords, int nRecordCount)
{
    QStringList slHeader;

    pTableWidget->setRowCount(0);

    for(int i=0; i<nRecordCount; i++)
    {
        slHeader.append(pRecords[i].pszName);
    }

    pTableWidget->setColumnCount(nRecordCount);
    pTableWidget->setHorizontalHeaderLabels(slHeader);
    pTableWidget->horizontalHeader()->setVisible(true);

    return true;
}

void FormatWidget::setLineEditsReadOnly(XLineEditHEX **ppLineEdits, int nCount, bool bState)
{
    for(int i=0; i<nCount; i++)
    {
        if(ppLineEdits[i])
        {
            ppLineEdits[i]->setReadOnly(bState);
        }
    }
}

void FormatWidget::setComboBoxesReadOnly(XComboBoxEx **ppComboBoxes, int nCount, bool bState)
{
    for(int i=0; i<nCount; i++)
    {
        if(ppComboBoxes[i])
        {
            ppComboBoxes[i]->setReadOnly(bState);
        }
    }
}

void FormatWidget::setPushButtonReadOnly(QPushButton **ppPushButtons, int nCount, bool bState)
{
    for(int i=0; i<nCount; i++)
    {
        if(ppPushButtons[i])
        {
            ppPushButtons[i]->setEnabled(!bState);
        }
    }
}

void FormatWidget::setDateTimeEditReadOnly(XDateTimeEditX **ppDateTimeEdits, int nCount, bool bState)
{
    for(int i=0; i<nCount; i++)
    {
        if(ppDateTimeEdits[i])
        {
            ppDateTimeEdits[i]->setReadOnly(bState);
        }
    }
}

void FormatWidget::_blockSignals(QObject **ppObjects, int nCount, bool bState)
{
    for(int i=0; i<nCount; i++)
    {
        if(ppObjects[i])
        {
            ppObjects[i]->blockSignals(bState);
        }
    }
}

XComboBoxEx *FormatWidget::createComboBox(QTableWidget *pTableWidget, QMap<quint64, QString> mapData, int type, int nData, XComboBoxEx::CBTYPE cbtype, quint64 nMask)
{
    XComboBoxEx *result=new XComboBoxEx(this);
    result->setData(mapData,cbtype,nMask);

    result->setProperty("STYPE",type);
    result->setProperty("NDATA",nData);

    connect(result,SIGNAL(valueChanged(quint64)),this,SLOT(widgetValueChanged(quint64)));

    pTableWidget->setCellWidget(nData,HEADER_COLUMN_INFO,result);

    return result;
}

XDateTimeEditX *FormatWidget::createDateTimeEdit(QTableWidget *pTableWidget, int type, int nData, XDateTimeEditX::DT_TYPE dtType)
{
    XDateTimeEditX *result=new XDateTimeEditX(this);
    result->setType(dtType);

    result->setProperty("STYPE",type);
    result->setProperty("NDATA",nData);

    connect(result,SIGNAL(valueChanged(quint64)),this,SLOT(widgetValueChanged(quint64)));

    pTableWidget->setCellWidget(nData,HEADER_COLUMN_INFO,result);

    return result;
}

QPushButton *FormatWidget::createPushButton(QTableWidget *pTableWidget, int type, int nData, QString sText)
{
    QPushButton *result=new QPushButton(this);

    result->setText(sText);
    result->setProperty("STYPE",type);
    result->setProperty("NDATA",nData);

    connect(result,SIGNAL(clicked()),this,SLOT(widgetAction()));

    pTableWidget->setCellWidget(nData,HEADER_COLUMN_INFO,result);

    return result;
}
