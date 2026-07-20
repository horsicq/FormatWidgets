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

#ifndef XFWIDGET_REGIONS_H
#define XFWIDGET_REGIONS_H

#include <QWidget>
#include "xformats.h"
#include "xregionswidget.h"

namespace Ui {
class XFWidget_Regions;
}

class XFWidget_Regions : public QWidget {
    Q_OBJECT

public:
    explicit XFWidget_Regions(QWidget *pParent = nullptr);
    virtual ~XFWidget_Regions();

    void setData(const XBinary::INDATA &inData);
    void setData(const XBinary::INDATA &inData, const XRegionsModel::OPTIONS &options);
    void clear();
    void reload();

    void setReadonly(bool bIsReadonly);

    XRegionsWidget *getRegionsWidget();

private:
    Ui::XFWidget_Regions *ui;
    XBinary::INDATA m_inData;
    XRegionsModel::OPTIONS m_options;
    QIODevice *m_pCurrentDevice;
    XInfoDB *m_pXInfoDB;
    bool m_bIsReadonly;
};

#endif  // XFWIDGET_REGIONS_H
