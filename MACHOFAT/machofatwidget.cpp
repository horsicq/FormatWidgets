/* Copyright (c) 2017-2025 hors<horsicq@gmail.com>
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
#include "machofatwidget.h"

#include "ui_machofatwidget.h"

MACHOFATWidget::MACHOFATWidget(QWidget *pParent) : FormatWidget(pParent), ui(new Ui::MACHOFATWidget)
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

    QObjectList listWidgets = ui->stackedWidget->children();

    for (qint32 i = 0; i < listWidgets.count(); i++) {
        QWidget *pWidget = qobject_cast<QWidget *>(listWidgets.at(i));

        ui->stackedWidget->removeWidget(pWidget);

        delete pWidget;
    }

    qint32 nNumberOfRecords = g_listDevices.count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        g_listDevices.at(i)->close();
        delete g_listDevices.at(i);
    }

    g_listDevices.clear();
}

void MACHOFATWidget::cleanup()
{
    MACHOFATWidget::clear();
}

void MACHOFATWidget::reload()
{
    // Q_UNUSED(ui->comboBoxFilePart);

    MACHOFATWidget::clear();

    //    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XMACHOFat machofat(getDevice());

    if (machofat.isValid()) {
        XBinary::PDSTRUCT pdStruct = XBinary::createPdStruct();

        QList<XArchive::RECORD> listRecords = machofat.getRecords(-1, &pdStruct);

        qint32 nNumberOfRecords = listRecords.count();

        for (qint32 i = 0; i < nNumberOfRecords; i++) {
            SubDevice *pSubDevice = new SubDevice(getDevice(), listRecords.at(i).nDataOffset, listRecords.at(i).nDataSize);

            if (pSubDevice->open(getDevice()->openMode())) {
                ui->comboBoxFilePart->addItem(listRecords.at(i).spInfo.sRecordName);

                MACHWidget *pMachWidget = new MACHWidget(pSubDevice, getOptions(), this);

                pMachWidget->setGlobal(getShortcuts(), getGlobalOptions());

                XInfoDB *pXInfoDB = getXInfoDB();
                pMachWidget->setXInfoDB(pXInfoDB);

                ui->stackedWidget->addWidget(pMachWidget);

                g_listDevices.append(pSubDevice);
            }
        }
    }
}

FormatWidget::SV MACHOFATWidget::_setValue(QVariant vValue, qint32 nStype, qint32 nNdata, qint32 nVtype, qint32 nPosition, qint64 nOffset)
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
    //    qint32 nNumberOfRecords=g_listDevices.count();

    //    for(qint32 i=0;i<nNumberOfRecords;i++)
    //    {
    //        MACHWidget *pWidget=qobject_cast<MACHWidget
    //        *>(ui->stackedWidget->widget(i));

    //        pWidget->setReadonly(bState);
    //    }
}

void MACHOFATWidget::blockSignals(bool bState)
{
    Q_UNUSED(bState)
}

void MACHOFATWidget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)
}

void MACHOFATWidget::on_comboBoxFilePart_currentIndexChanged(int nIndex)
{
    ui->stackedWidget->setCurrentIndex(nIndex);
}
