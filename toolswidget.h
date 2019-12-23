// copyright (c) 2019 hors<horsicq@gmail.com>
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
#ifndef TOOLSWIDGET_H
#define TOOLSWIDGET_H

#include <QWidget>
#include "searchstringswidget.h"
#include "qhexview.h"
#include "formatwidget_def.h"

namespace Ui
{
class ToolsWidget;
}

class ToolsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ToolsWidget(QWidget *parent=nullptr);
    ~ToolsWidget();

    void setData(QIODevice *pDevice,FW_DEF::OPTIONS *pOptions);
    void setEdited(bool bState);
    void setReadonly(bool bState);
    void reload();
    qint64 getBaseAddress();
    void setSelection(qint64 nAddress,qint64 nSize);

signals:
    void editState(bool bState);

private:
    Ui::ToolsWidget *ui;
    QHexView::OPTIONS hexOptions;
    SearchStrings::OPTIONS stringsOptions;
};

#endif // TOOLSWIDGET_H
