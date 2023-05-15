/* Copyright (c) 2017-2023 hors<horsicq@gmail.com>
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
#ifndef DIALOGSECTIONHEADER_H
#define DIALOGSECTIONHEADER_H

#include <QDialog>

#include "formatwidget.h"
#include "xshortcutsdialog.h"

namespace Ui {
class DialogSectionHeader;
}

class DialogSectionHeader : public XShortcutsDialog {
    Q_OBJECT

public:
    explicit DialogSectionHeader(QWidget *pParent);
    ~DialogSectionHeader();

    void setWidget(FormatWidget *pWidget);
    void setData(QIODevice *pDevice, FW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, const QString &sTitle, qint32 nType);  // TODO remove
    void setData(const QString &sTitle);
    void setEdited(qint64 nDeviceOffset, qint64 nDeviceSize);
    virtual void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions);

signals:
    void changed();

private:
    Ui::DialogSectionHeader *ui;
    FormatWidget *g_pWidget;
};

#endif  // DIALOGSECTIONHEADER_H
