/* Copyright (c) 2024-2025 hors<horsicq@gmail.com>
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
#ifndef XMAINWIDGETADVANCED_H
#define XMAINWIDGETADVANCED_H

#include "xmainwidget.h"
#include "xextractorwidget.h"
#include "xhexviewwidget.h"
#include "xregionswidget.h"
#include "xmemorymapwidget.h"
#include "xmultidisasmwidget.h"
#include "xsymbolswidget.h"
#include "xvisualizationwidget.h"
#include "yarawidgetadvanced.h"
#include "nfdwidgetadvanced.h"
#include "diewidgetadvanced.h"
#include "xvirustotalwidget.h"
#include "xhashwidget.h"
#include "xentropywidget.h"

class XMainWidgetAdvanced : public XMainWidget {
    Q_OBJECT
public:
    explicit XMainWidgetAdvanced(QWidget *pParent = nullptr);

    virtual QTreeWidgetItem *_addBaseItems(QTreeWidget *pTreeWidget, XBinary::FT fileType);
    virtual XShortcutsWidget *createWidget(const XFW_DEF::CWOPTIONS &cwOptions);

private slots:
    void showDemangleSlot(const QString &sString);
    void findValue(quint64 nValue, XBinary::ENDIAN endian);

protected:
    virtual void _followLocation(quint64 nLocation, qint32 nLocationType, qint64 nSize, qint32 nWidgetType);
};

#endif  // XMAINWIDGETADVANCED_H
