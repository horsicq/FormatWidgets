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
#ifndef TOOLSWIDGET_H
#define TOOLSWIDGET_H

#include "formatwidget_def.h"
#include "searchstringswidget.h"
#include "xhexview.h"

namespace Ui {
class ToolsWidget;
}

class ToolsWidget : public XShortcutsWidget {
    Q_OBJECT

public:
    explicit ToolsWidget(QWidget *pParent = nullptr);
    ~ToolsWidget();

    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions);
    void setData(QIODevice *pDevice, FW_DEF::OPTIONS options, bool bDisasm, bool bFollow, XInfoDB *pXInfoDB);
    void resetWidget();
    void setEdited(qint64 nDeviceOffset, qint64 nDeviceSize);
    void setReadonly(bool bState);
    void reload();
    // qint64 getStartLocation();
    void setSelection(qint64 nOffset, qint64 nSize, bool bGoto);
    virtual void adjustView();
    virtual void reloadData(bool bSaveSelection);

signals:
    void showOffsetHex(qint64 nOffset, qint64 nSize);
    void showOffsetDisasm(qint64 nOffset);
    void showOffsetMemoryMap(qint64 nOffset);
    void showDemangle(const QString &sString);

private slots:
    void on_tabWidgetMain_currentChanged(int nIndex);

protected:
    virtual void registerShortcuts(bool bState);

private:
    Ui::ToolsWidget *ui;
    QIODevice *m_pDevice;
    SearchStringsWidget::OPTIONS g_stringsOptions;
};

#endif  // TOOLSWIDGET_H
