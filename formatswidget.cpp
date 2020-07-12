// Copyright (c) 2020 hors<horsicq@gmail.com>
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
#include "formatswidget.h"
#include "ui_formatswidget.h"

FormatsWidget::FormatsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormatsWidget)
{
    ui->setupUi(this);
}

void FormatsWidget::setFileName(QString sFileName)
{
     const QSignalBlocker blocker(ui->comboBoxType);

    this->sFileName=sFileName;

    ui->comboBoxType->clear();

    QSet<XBinary::FT> stTypes=XBinary::getFileTypes(sFileName);

    XFormats::filterFileTypes(&stTypes);

    QList<XBinary::FT> listFileTypes=XBinary::_getFileTypeListFromSet(stTypes);

    int nCount=listFileTypes.count();

    for(int i=0;i<nCount;i++)
    {
        XBinary::FT ft=listFileTypes.at(i);
        ui->comboBoxType->addItem(XBinary::fileTypeIdToString(ft),ft);
    }

    if(nCount)
    {
        if(listFileTypes.at(0)==XBinary::FT_BINARY)
        {
            ui->comboBoxType->setCurrentIndex(0);
        }
        else
        {
            ui->comboBoxType->setCurrentIndex(nCount-1);
        }

        reload();
    }
}

void FormatsWidget::setBackupFileName(QString sBackupFileName)
{
    this->sBackupFilename=sBackupFileName;
}

FormatsWidget::~FormatsWidget()
{
    delete ui;
}

void FormatsWidget::on_comboBoxType_currentIndexChanged(int index)
{
    Q_UNUSED(index)

    reload();
}

void FormatsWidget::reload()
{
    XBinary::FT ft=(XBinary::FT)ui->comboBoxType->currentData().toInt();

    QFile file;
    file.setFileName(sFileName);

    if(file.open(QIODevice::ReadOnly))
    {
        if(ft==XBinary::FT_BINARY)
        {
            ui->stackedWidgetMain->setCurrentIndex(TAB_BINARY);
        }
        else if(ft==XBinary::FT_COM)
        {
            ui->stackedWidgetMain->setCurrentIndex(TAB_COM);
        }
        else if(ft==XBinary::FT_MSDOS)
        {
            ui->stackedWidgetMain->setCurrentIndex(TAB_MSDOS);

            XMSDOS msdos(&file);

            if(msdos.isValid())
            {
                ui->lineEditEntryPoint->setValue((quint16)msdos.getEntryPointAddress());
                ui->lineEditBaseAddress->setValue((quint16)msdos.getBaseAddress());
            }
        }
        else if((ft==XBinary::FT_PE32)||(ft==XBinary::FT_PE64))
        {
            ui->stackedWidgetMain->setCurrentIndex(TAB_PE);

            XPE pe(&file);

            if(pe.isValid())
            {
                if(pe.is64())
                {
                    ui->lineEditEntryPoint->setValue((quint64)pe.getEntryPointAddress());
                    ui->lineEditBaseAddress->setValue((quint64)pe.getBaseAddress());
                }
                else
                {
                    ui->lineEditEntryPoint->setValue((quint32)pe.getEntryPointAddress());
                    ui->lineEditBaseAddress->setValue((quint32)pe.getBaseAddress());
                }
                ui->lineEditPESections->setValue((quint16)pe.getFileHeader_NumberOfSections());

                ui->pushButtonPEExport->setEnabled(pe.isExportPresent());
                ui->pushButtonPEImport->setEnabled(pe.isImportPresent());
                ui->pushButtonPEResource->setEnabled(pe.isResourcesPresent());
                ui->pushButtonPENET->setEnabled(pe.isNETPresent());
            }
        }
        else if((ft==XBinary::FT_ELF32)||(ft==XBinary::FT_ELF64))
        {
            ui->stackedWidgetMain->setCurrentIndex(TAB_ELF);

            XELF elf(&file);

            if(elf.isValid())
            {
                if(elf.is64())
                {
                    ui->lineEditEntryPoint->setValue((quint64)elf.getEntryPointAddress());
                    ui->lineEditBaseAddress->setValue((quint64)elf.getBaseAddress());
                }
                else
                {
                    ui->lineEditEntryPoint->setValue((quint32)elf.getEntryPointAddress());
                    ui->lineEditBaseAddress->setValue((quint32)elf.getBaseAddress());
                }
            }
        }
        else if((ft==XBinary::FT_MACH32)||(ft==XBinary::FT_MACH64))
        {
            ui->stackedWidgetMain->setCurrentIndex(TAB_MACH);

            XMACH mach(&file);

            if(mach.isValid())
            {
                if(mach.is64())
                {
                    ui->lineEditEntryPoint->setValue((quint64)mach.getEntryPointAddress());
                    ui->lineEditBaseAddress->setValue((quint64)mach.getBaseAddress());
                }
                else
                {
                    ui->lineEditEntryPoint->setValue((quint32)mach.getEntryPointAddress());
                    ui->lineEditBaseAddress->setValue((quint32)mach.getBaseAddress());
                }
            }
        }

        if(ft==XBinary::FT_BINARY)
        {
            ui->groupBoxEntryPoint->hide();
            ui->groupBoxBaseAddress->hide();
        }
        else
        {
            ui->groupBoxEntryPoint->show();
            ui->groupBoxBaseAddress->show();
        }

        file.close();
    }
    else
    {
        // TODO Error message
    }
}

void FormatsWidget::on_pushButtonDisasm_clicked()
{
    // Check x86
    // Open ReadOnly
}

void FormatsWidget::on_pushButtonMemoryMap_clicked()
{
    QFile file;
    file.setFileName(sFileName);

    if(file.open(QIODevice::ReadOnly))
    {
        DialogMemoryMap dialogMemoryMap(this,&file);

        dialogMemoryMap.exec();

        file.close();
    }
}

void FormatsWidget::on_pushButtonPEExport_clicked()
{
    showPE(SPE::TYPE_EXPORT);
}

void FormatsWidget::on_pushButtonPEImport_clicked()
{
    showPE(SPE::TYPE_IMPORT);
}

void FormatsWidget::on_pushButtonPEResource_clicked()
{
    showPE(SPE::TYPE_RESOURCE);
}

void FormatsWidget::on_pushButtonPEOverlay_clicked()
{
    showPE(SPE::TYPE_OVERLAY);
}

void FormatsWidget::on_pushButtonPE_clicked()
{
    showPE(SPE::TYPE_IMAGE_FILE_HEADER);
}

void FormatsWidget::on_pushButtonPESections_clicked()
{
    showPE(SPE::TYPE_SECTIONS);
}

void FormatsWidget::on_pushButtonPENET_clicked()
{
    showPE(SPE::TYPE_NETHEADER);
}

void FormatsWidget::showMSDOS(SMSDOS::TYPE type)
{
    // TODO type
    QFile file;
    file.setFileName(sFileName);

    if(XBinary::tryToOpen(&file))
    {
        FW_DEF::OPTIONS options={};

        if(sBackupFilename!="")
        {
            options.sBackupFileName=sBackupFilename;
        }

        DialogMSDOS dialogMSDOS(this);

        dialogMSDOS.setData(&file,&options);

        dialogMSDOS.exec();

        file.close();
    }
}

void FormatsWidget::showLE(SLE::TYPE type)
{

}

void FormatsWidget::showNE(SNE::TYPE type)
{

}

void FormatsWidget::showPE(SPE::TYPE type)
{
    // TODO type
    QFile file;
    file.setFileName(sFileName);

    if(XBinary::tryToOpen(&file))
    {
        FW_DEF::OPTIONS options={};

        if(sBackupFilename!="")
        {
            options.sBackupFileName=sBackupFilename;
        }

        DialogPE dialogPE(this);

        dialogPE.setData(&file,&options);

        dialogPE.exec();

        file.close();
    }
}

void FormatsWidget::showELF(SELF::TYPE type)
{

}

void FormatsWidget::showMACH(SMACH::TYPE type)
{

}

void FormatsWidget::on_pushButtonMSDOSOverlay_clicked()
{
    showMSDOS(SMSDOS::TYPE_OVERLAY);
}

void FormatsWidget::on_pushButtonMSDOS_clicked()
{
    // TODO
}
