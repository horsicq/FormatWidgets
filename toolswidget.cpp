// copyright (c) 2019-2021 hors<horsicq@gmail.com>
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
#include "toolswidget.h"
#include "ui_toolswidget.h"

ToolsWidget::ToolsWidget(QWidget *pParent) :
    XShortcutsWidget(pParent),
    ui(new Ui::ToolsWidget)
{
    ui->setupUi(this);

    g_pDevice=nullptr;

    connect(ui->widgetHex,SIGNAL(editState(bool)),this,SIGNAL(editState(bool)));
    connect(ui->widgetHex,SIGNAL(showOffsetDisasm(qint64)),this,SLOT(_showDisasm(qint64)));
    connect(ui->widgetHex,SIGNAL(showOffsetMemoryMap(qint64)),this,SLOT(_showMemoryMap(qint64)));
    connect(ui->widgetStrings,SIGNAL(showHex(qint64,qint64)),this,SLOT(_showHex(qint64,qint64)));
    connect(ui->widgetStrings,SIGNAL(showDemangle(QString)),this,SIGNAL(showDemangle(QString)));
}

void ToolsWidget::setData(QIODevice *pDevice, FW_DEF::OPTIONS options)
{
    g_pDevice=pDevice;

    XHexView::OPTIONS hexOptions={};
    hexOptions.nStartAddress=options.nImageBase;
    hexOptions.bMenu_MemoryMap=true;
    hexOptions.bMenu_Disasm=true;
    hexOptions.bIsOffsetTitle=options.bOffset;

    ui->widgetHex->enableReadOnly(false);

    ui->widgetHex->setData(pDevice,hexOptions);

    g_stringsOptions={};
    g_stringsOptions.nBaseAddress=options.nImageBase;
    g_stringsOptions.bMenu_Hex=true;
    g_stringsOptions.bMenu_Demangle=true;
    g_stringsOptions.bAnsi=true;
    g_stringsOptions.bUnicode=true;
    g_stringsOptions.bCStrings=true;

    ui->widgetStrings->setData(pDevice,g_stringsOptions,false);

    reload();
}

void ToolsWidget::setEdited(bool bState)
{
    ui->widgetHex->setEdited(bState);
}

void ToolsWidget::setReadonly(bool bState)
{
    ui->widgetHex->setReadonly(bState);
}

void ToolsWidget::reload()
{
    int nIndex=ui->tabWidgetMain->currentIndex();

    if(nIndex==0) // Hex
    {
        ui->widgetHex->reload(true);
    }
    else if(nIndex==1) // Strings
    {
        if(!ui->widgetStrings->getInitStatus())
        {
            ui->widgetStrings->reload();
        }
    }
}

qint64 ToolsWidget::getStartAddress()
{
    return ui->widgetHex->getStartAddress();
}

void ToolsWidget::setSelection(qint64 nOffset, qint64 nSize)
{
    ui->widgetHex->setSelection(nOffset,nSize);
}

ToolsWidget::~ToolsWidget()
{
    delete ui;
}

void ToolsWidget::setGlobal(XShortcuts *pShortcuts,XOptions *pXOptions)
{
    ui->widgetHex->setGlobal(pShortcuts,pXOptions);
    ui->widgetStrings->setGlobal(pShortcuts,pXOptions);
}

void ToolsWidget::on_tabWidgetMain_currentChanged(int nIndex)
{
    Q_UNUSED(nIndex)
    reload();
}

void ToolsWidget::_showHex(qint64 nOffset,qint64 nSize)
{
    SubDevice *pSubDevice=qobject_cast<SubDevice *>(g_pDevice);

    if(pSubDevice)
    {
        nOffset+=pSubDevice->getInitOffset();
    }

    emit showOffsetHex(nOffset,nSize);
}

void ToolsWidget::_showDisasm(qint64 nOffset)
{
    SubDevice *pSubDevice=qobject_cast<SubDevice *>(g_pDevice);

    if(pSubDevice)
    {
        nOffset+=pSubDevice->getInitOffset();
    }

    emit showOffsetDisasm(nOffset);
}

void ToolsWidget::_showMemoryMap(qint64 nOffset)
{
    SubDevice *pSubDevice=qobject_cast<SubDevice *>(g_pDevice);

    if(pSubDevice)
    {
        nOffset+=pSubDevice->getInitOffset();
    }

    emit showOffsetMemoryMap(nOffset);
}

void ToolsWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
