/* Copyright (c) 2019-2025 hors<horsicq@gmail.com>
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
#include "invwidget.h"

InvWidget::InvWidget(QWidget *pParent, TYPE type) : XShortcutsWidget(pParent)
{
    m_pHexPushButton = nullptr;
    m_pDisasmPushButton = nullptr;

    QHBoxLayout *pLayot = new QHBoxLayout(this);
    pLayot->setContentsMargins(0, 0, 0, 0);

    if (type == TYPE_HEX) {
        m_pHexPushButton = new QPushButton(tr("Hex"), this);

        connect(m_pHexPushButton, SIGNAL(clicked()), this, SLOT(showHexSlot()));

        pLayot->addWidget(m_pHexPushButton);
    } else if (type == TYPE_DISASM) {
        m_pDisasmPushButton = new QPushButton(tr("Disasm"), this);

        connect(m_pDisasmPushButton, SIGNAL(clicked()), this, SLOT(showDisasmSlot()));

        pLayot->addWidget(m_pDisasmPushButton);
    }

    setLayout(pLayot);

    m_nOffset = 0;
    m_nAddress = 0;
    m_nSize = 0;
}

InvWidget::~InvWidget()
{
}

void InvWidget::setOffsetAndSize(XBinary *pBinary, qint64 nOffset, qint64 nSize, bool bNotNull)
{
    bool bValid = false;

    if ((bNotNull) && (nOffset == 0)) {
        bValid = false;
    } else if (pBinary->isOffsetValid(nOffset)) {
        bValid = true;
    }

    if (bValid) {
        _setEnabled(true);

        this->m_nOffset = nOffset;
        this->m_nSize = nSize;
    } else {
        _setEnabled(false);

        this->m_nOffset = 0;
        this->m_nSize = 0;
    }
}

void InvWidget::setAddressAndSize(XBinary *pBinary, XADDR nAddress, qint64 nSize, bool bNotNull)
{
    bool bValid = false;

    this->m_nAddress = nAddress;

    XBinary::_MEMORY_MAP memoryMap = pBinary->getMemoryMap();

    if ((bNotNull) && (nAddress == 0)) {
        bValid = false;
    } else if (pBinary->isAddressPhysical(&memoryMap, nAddress)) {
        bValid = true;
    }

    if (bValid) {
        _setEnabled(true);

        this->m_nOffset = pBinary->addressToOffset(&memoryMap, nAddress);
        this->m_nSize = nSize;
    } else {
        _setEnabled(false);

        this->m_nOffset = 0;
        this->m_nSize = 0;
    }
}

void InvWidget::_setEnabled(bool bState)
{
    if (m_pHexPushButton) {
        m_pHexPushButton->setEnabled(bState);
    }

    if (m_pDisasmPushButton) {
        m_pDisasmPushButton->setEnabled(bState);
    }
}

void InvWidget::adjustView()
{
    // TODO
}

void InvWidget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)
}

void InvWidget::showHexSlot()
{
    emit showHex(m_nOffset, m_nSize);
}

void InvWidget::showDisasmSlot()
{
    emit showDisasm(m_nAddress);
}

void InvWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
