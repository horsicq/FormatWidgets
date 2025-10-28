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
#include "dialogsectionheader.h"

#include "ui_dialogsectionheader.h"

DialogSectionHeader::DialogSectionHeader(QWidget *pParent) : XShortcutsDialog(pParent, false), ui(new Ui::DialogSectionHeader)
{
    ui->setupUi(this);

    this->m_pWidget = nullptr;
}

DialogSectionHeader::~DialogSectionHeader()
{
    delete ui;
}

void DialogSectionHeader::setWidget(FormatWidget *pWidget)
{
    this->m_pWidget = pWidget;
    pWidget->setGlobal(getShortcuts(), getGlobalOptions());

    ui->WidgetLayout->addWidget(pWidget);

    connect(pWidget, SIGNAL(dataChanged(qint64, qint64)), this, SIGNAL(dataChanged(qint64, qint64)));
}

void DialogSectionHeader::setData(QIODevice *pDevice, FW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, const QString &sTitle, qint32 nType)
{
    setWindowTitle(sTitle);

    m_pWidget->setData(pDevice, options, nNumber, nOffset, nType);
    m_pWidget->reload();
}

void DialogSectionHeader::setData(const QString &sTitle)
{
    setWindowTitle(sTitle);

    m_pWidget->reload();
}

void DialogSectionHeader::setEdited(qint64 nDeviceOffset, qint64 nDeviceSize)
{
    m_pWidget->setEdited(nDeviceOffset, nDeviceSize);
}

void DialogSectionHeader::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    if (m_pWidget) {
        m_pWidget->setGlobal(pShortcuts, pXOptions);
    }

    XShortcutsDialog::setGlobal(pShortcuts, pXOptions);
}

void DialogSectionHeader::adjustView()
{
}

void DialogSectionHeader::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
