/* Copyright (c) 2025-2026 hors<horsicq@gmail.com>
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
#ifndef XMAINWIDGET_H
#define XMAINWIDGET_H

#include "xstructwidget.h"

#include "xfileinfowidget.h"
#include "xhexviewwidget.h"
#include "nfdwidgetadvanced.h"
#include "xmemorymapwidget.h"
#include "xextractorwidget.h"
#include "diewidgetadvanced.h"
#include "yarawidgetadvanced.h"
#include "xvirustotalwidget.h"
#include "xmultidisasmwidget.h"
#include "xregionswidget.h"
#include "xentropywidget.h"
#include "xhashwidget.h"

class XMainWidget : public XStructWidget {
    Q_OBJECT

public:
    explicit XMainWidget(QWidget *pParent = nullptr);
    ~XMainWidget();

    virtual XShortcutsWidget *createWidget(const QString &sGUID);
};

#endif  // XMAINWIDGET_H
