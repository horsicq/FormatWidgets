/* Copyright (c) 2025 hors<horsicq@gmail.com>
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
#include "xgenericabstractwidget.h"

XGenericAbstractWidget::XGenericAbstractWidget(QWidget *parent)
    : XShortcutsWidget(parent)
{
    g_dataRecordsOptions = {};
    g_pDevice = nullptr;
    g_pXInfoDB = nullptr;
}

void XGenericAbstractWidget::setData(QIODevice *pDevice, XInfoDB *pXInfoDB, const XBinary::DATA_RECORDS_OPTIONS &dataRecordsOptions)
{
    g_pDevice = pDevice;
    g_pXInfoDB = pXInfoDB;
    g_dataRecordsOptions = dataRecordsOptions;

    process();
}

QIODevice *XGenericAbstractWidget::getDevice() const
{
    return g_pDevice;
}

XInfoDB *XGenericAbstractWidget::getXInfoDB() const
{
    return g_pXInfoDB;
}

const XBinary::DATA_RECORDS_OPTIONS &XGenericAbstractWidget::getRecordsOptions() const
{
    return g_dataRecordsOptions;
}

void XGenericAbstractWidget::process()
{
#ifdef QT_DEBUG
    qDebug("XGenericAbstractWidget::process");
#endif
}
