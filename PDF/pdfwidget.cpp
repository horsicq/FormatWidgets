/* Copyright (c) 2022 hors<horsicq@gmail.com>
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
#include "pdfwidget.h"
#include "ui_pdfwidget.h"

PDFWidget::PDFWidget(QWidget *pParent) :
    FormatWidget(pParent),
    ui(new Ui::PDFWidget)
{
    ui->setupUi(this);

    initWidget();
}

PDFWidget::PDFWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, QWidget *pParent) :
    PDFWidget(pParent)
{
    PDFWidget::setData(pDevice,options,0,0,0);
    PDFWidget::reload();
}

PDFWidget::~PDFWidget()
{
    delete ui;
}

void PDFWidget::clear()
{
    reset();

    ui->checkBoxReadonly->setChecked(true);

    ui->treeWidgetNavi->clear();
}

void PDFWidget::cleanup()
{

}

void PDFWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XPDF pdf(getDevice());

    if(pdf.isValid())
    {
        XBinary::_MEMORY_MAP memoryMap=pdf.getMemoryMap();

        setFileType(pdf.getFileType());

        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPDF::TYPE_INFO,tr("Info")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPDF::TYPE_HEX,tr("Hex")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPDF::TYPE_HASH,tr("Hash")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPDF::TYPE_STRINGS,tr("Strings")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPDF::TYPE_SIGNATURES,tr("Signatures")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPDF::TYPE_MEMORYMAP,tr("Memory map")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPDF::TYPE_ENTROPY,tr("Entropy")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPDF::TYPE_HEURISTICSCAN,tr("Heuristic scan")));

        qint64 nStarthref=pdf.findStartxref();

        ui->treeWidgetNavi->expandAll();

        setTreeItem(ui->treeWidgetNavi,getOptions().nStartType);
    }
}

FormatWidget::SV PDFWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    return SV_NONE;
}

void PDFWidget::setReadonly(bool bState)
{
    ui->widgetHex->setReadonly(bState);
}

void PDFWidget::blockSignals(bool bState)
{

}

void PDFWidget::reloadData()
{
    int nType=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_TYPE).toInt();
    qint64 nDataOffset=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
    qint64 nDataSize=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();

    QString sInit=getInitString(ui->treeWidgetNavi->currentItem());

    ui->stackedWidgetInfo->setCurrentIndex(nType);

    XPDF pdf(getDevice());

    if(pdf.isValid())
    {
        if(nType==SPDF::TYPE_INFO)
        {
            if(!isInitPresent(sInit))
            {
                ui->widgetInfo->setData(getDevice(),pdf.getFileType(),true);
            }
        }
        else if(nType==SPDF::TYPE_HEX)
        {
            if(!isInitPresent(sInit))
            {
                XHexView::OPTIONS options={};
                options.bMenu_Disasm=true;
                options.bMenu_MemoryMap=true;
                ui->widgetHex->setData(getDevice(),options);
                // TODO save directory
                ui->widgetHex->enableReadOnly(false);

                ui->widgetHex->reload();
            }
        }
        else if(nType==SPDF::TYPE_HASH)
        {
            if(!isInitPresent(sInit))
            {
                ui->widgetHash->setData(getDevice(),pdf.getFileType(),0,-1,true);
            }
        }
        else if(nType==SPDF::TYPE_STRINGS)
        {
            if(!isInitPresent(sInit))
            {
                SearchStringsWidget::OPTIONS stringsOptions={};
                stringsOptions.bMenu_Hex=true;
                stringsOptions.bMenu_Demangle=true;
                stringsOptions.bAnsi=true;
                stringsOptions.bUTF8=false;
                stringsOptions.bUnicode=true;
                stringsOptions.bCStrings=true;

                ui->widgetStrings->setData(getDevice(),stringsOptions,true);
            }
        }
        else if(nType==SPDF::TYPE_SIGNATURES)
        {
            if(!isInitPresent(sInit))
            {
                SearchSignaturesWidget::OPTIONS signaturesOptions={};
                signaturesOptions.bMenu_Hex=true;

                ui->widgetSignatures->setData(getDevice(),pdf.getFileType(),signaturesOptions,false);
            }
        }
        else if(nType==SPDF::TYPE_MEMORYMAP)
        {
            if(!isInitPresent(sInit))
            {
                ui->widgetMemoryMap->setData(getDevice(),pdf.getFileType());
            }
        }
        else if(nType==SPDF::TYPE_ENTROPY)
        {
            if(!isInitPresent(sInit))
            {
                ui->widgetEntropy->setData(getDevice(),0,getDevice()->size(),pdf.getFileType(),true); // TODO save last directory
            }
        }
        else if(nType==SPDF::TYPE_HEURISTICSCAN)
        {
            if(!isInitPresent(sInit))
            {
                ui->widgetHeuristicScan->setData(getDevice(),true,pdf.getFileType());
            }
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }

    addInit(sInit);
}

void PDFWidget::on_checkBoxReadonly_toggled(bool bChecked)
{
    setReadonly(bChecked);
}

void PDFWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pCurrent, QTreeWidgetItem *pPrevious)
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
