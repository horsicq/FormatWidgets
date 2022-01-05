// Copyright (c) 2017-2022 hors<horsicq@gmail.com>
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
#include "dialogsectionheader.h"
#include "ui_dialogsectionheader.h"

DialogSectionHeader::DialogSectionHeader(QWidget *pParent) :
    XShortcutsDialog(pParent),
    ui(new Ui::DialogSectionHeader)
{
    ui->setupUi(this);

    this->g_pWidget=nullptr;

    setWindowFlags(Qt::Window);
}

DialogSectionHeader::~DialogSectionHeader()
{
    delete ui;
}

void DialogSectionHeader::setWidget(FormatWidget *pWidget)
{
    this->g_pWidget=pWidget;

    ui->WidgetLayout->addWidget(pWidget);

    connect(pWidget,SIGNAL(editState(bool)),this,SIGNAL(editState(bool)));
}

void DialogSectionHeader::setData(QIODevice *pDevice, FW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, QString sTitle, qint32 nType)
{
    setWindowTitle(sTitle);

    g_pWidget->setData(pDevice,options,nNumber,nOffset,nType);
    g_pWidget->reload();
}

void DialogSectionHeader::setData(QString sTitle)
{
    setWindowTitle(sTitle);

    g_pWidget->reload();
}

void DialogSectionHeader::setEdited(bool bState)
{
    g_pWidget->setEdited(bState);
}

void DialogSectionHeader::setGlobal(XShortcuts *pShortcuts,XOptions *pXOptions)
{
    if(g_pWidget)
    {
        g_pWidget->setGlobal(pShortcuts,pXOptions);
    }
}
