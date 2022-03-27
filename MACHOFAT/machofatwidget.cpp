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
#include "machofatwidget.h"
#include "ui_machofatwidget.h"

MACHOFATWidget::MACHOFATWidget(QWidget *pParent) :
    FormatWidget(pParent),
    ui(new Ui::MACHOFATWidget)
{
    ui->setupUi(this);
}

MACHOFATWidget::~MACHOFATWidget()
{
    delete ui;
}

void MACHOFATWidget::clear()
{
    ui->comboBoxFilePart->clear();

    QObjectList listWidgets=ui->stackedWidget->children();

    for(int i=0;i<listWidgets.count();i++)
    {
        QWidget *pWidget=qobject_cast<QWidget *>(listWidgets.at(i));

        ui->stackedWidget->removeWidget(pWidget);
    }

    int nNumberOfRecords=g_listDevices.count();

    for(int i=0;i<nNumberOfRecords;i++)
    {
        g_listDevices.at(i)->close();
        delete g_listDevices.at(i);
    }

    g_listDevices.clear();
}

void MACHOFATWidget::cleanup()
{

}

void MACHOFATWidget::reload()
{
    Q_UNUSED(ui->comboBoxFilePart);

    MACHOFATWidget::clear();

//    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XMACHOFat machofat(getDevice());

    if(machofat.isValid())
    {
        QList<XArchive::RECORD> listRecords=machofat.getRecords();

        int nNumberOfRecords=listRecords.count();

        for(int i=0;i<nNumberOfRecords;i++)
        {
            SubDevice *pSubDevice=new SubDevice(getDevice(),listRecords.at(i).nDataOffset,listRecords.at(i).nUncompressedSize);

            if(pSubDevice->open(getDevice()->openMode()))
            {
                ui->comboBoxFilePart->addItem(listRecords.at(i).sFileName);

                MACHWidget *pMachWidget=new MACHWidget(pSubDevice,getOptions(),this);

                pMachWidget->setGlobal(getShortcuts(),getGlobalOptions());

                pMachWidget->setBackupDevice(getDevice());

                ui->stackedWidget->addWidget(pMachWidget);

                g_listDevices.append(pSubDevice);
            }
        }
    }
}

FormatWidget::SV MACHOFATWidget::_setValue(QVariant vValue,int nStype,int nNdata,int nVtype,int nPosition,qint64 nOffset)
{
    Q_UNUSED(vValue)
    Q_UNUSED(nStype)
    Q_UNUSED(nNdata)
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)
    Q_UNUSED(nOffset)

    return SV_NONE;
}

void MACHOFATWidget::setReadonly(bool bState)
{
    Q_UNUSED(bState)
//    // TODO Check
//    int nNumberOfRecords=g_listDevices.count();

//    for(int i=0;i<nNumberOfRecords;i++)
//    {
//        MACHWidget *pWidget=qobject_cast<MACHWidget *>(ui->stackedWidget->widget(i));

//        pWidget->setReadonly(bState);
//    }
}

void MACHOFATWidget::blockSignals(bool bState)
{
    Q_UNUSED(bState)
}

void MACHOFATWidget::reloadData()
{

}

void MACHOFATWidget::on_comboBoxFilePart_currentIndexChanged(int nIndex)
{
    ui->stackedWidget->setCurrentIndex(nIndex);
}
