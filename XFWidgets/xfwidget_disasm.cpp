/* Copyright (c) 2025-2026 hors<horsicq@gmail.com>
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

#include "xfwidget_disasm.h"

#include <QVBoxLayout>

#include "xinfodb.h"

XFWidget_Disasm::XFWidget_Disasm(QWidget *pParent) : XShortcutsWidget(pParent)
{
    m_pXInfoDB = nullptr;
    m_pDevice = nullptr;
    m_pDisasmWidget = new XMultiDisasmWidget(this);

    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setContentsMargins(0, 0, 0, 0);
    pLayout->addWidget(m_pDisasmWidget);
}

XFWidget_Disasm::~XFWidget_Disasm()
{
    clear();
}

void XFWidget_Disasm::setData(const XBinary::INDATA &inData)
{
    clear();

    m_inData = inData;
    m_pDevice = XFormats::createDevice(m_inData);

    XMultiDisasmWidget::OPTIONS options = {};
    options.fileType = m_inData.fileType;
    options.nInitAddress = XFormats::getEntryPointAddress(m_inData.fileType, m_pDevice, m_inData.bIsImage, m_inData.nModuleAddress);
    options.bMenu_Hex = true;

    if (m_pDevice) {
        m_pDisasmWidget->setData(XFormats::createINDATA(m_inData.fileType, m_pDevice, m_inData.bIsImage, m_inData.nModuleAddress), options);

        m_pXInfoDB = new XInfoDB(this);
        m_pXInfoDB->setData(m_pDevice, m_inData.fileType);
        m_pDisasmWidget->setXInfoDB(m_pXInfoDB);
    } else {
        m_pDisasmWidget->setData(m_inData, options);
    }
}

void XFWidget_Disasm::clear()
{
    if (m_pXInfoDB) {
        delete m_pXInfoDB;
        m_pXInfoDB = nullptr;
    }

    m_pDisasmWidget->setData(XBinary::INDATA(), XMultiDisasmWidget::OPTIONS());

    XFormats::removeDevice(m_pDevice, m_inData);
    m_pDevice = nullptr;

    m_inData = {};
}

void XFWidget_Disasm::reload()
{
    if (m_inData.inDataMode != XBinary::INDATA_MODE_UNKNOWN) {
        m_pDisasmWidget->reloadData(false);
    }
}

void XFWidget_Disasm::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
    m_pDisasmWidget->setGlobal(pShortcuts, pXOptions);
}

void XFWidget_Disasm::setReadonly(bool bIsReadonly)
{
    XShortcutsWidget::setReadonly(bIsReadonly);
    m_pDisasmWidget->setReadonly(bIsReadonly);
}

XMultiDisasmWidget *XFWidget_Disasm::getDisasmWidget()
{
    return m_pDisasmWidget;
}
