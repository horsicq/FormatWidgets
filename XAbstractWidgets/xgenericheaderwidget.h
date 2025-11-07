/* Copyright (c) 2025 hors<horsicq@gmail.com>
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
#ifndef XGENERICHEADERWIDGET_H
#define XGENERICHEADERWIDGET_H

#include "xgenericabstractwidget.h"

namespace Ui {
class XGenericHeaderWidget;
}

class XGenericHeaderWidget : public XGenericAbstractWidget {
    Q_OBJECT

    enum HEADER_COLUMN {
        HEADER_COLUMN_NAME = 0,
        HEADER_COLUMN_OFFSET,
        HEADER_COLUMN_SIZE,
        HEADER_COLUMN_TYPE,
        HEADER_COLUMN_VALUE,
        HEADER_COLUMN_INFO,
        HEADER_COLUMN_COMMENT
    };

    enum UR {
        UR_RELOFFSET = 0,
        UR_SIZE
    };

public:
    explicit XGenericHeaderWidget(QWidget *parent = nullptr);
    ~XGenericHeaderWidget();

    virtual void adjustView();
    virtual void reloadData(bool bSaveSelection);
    virtual void setReadonly(bool bState);

private slots:
    void on_toolButtonTableReload_clicked();
    void on_toolButtonTableSize_clicked();
    void on_toolButtonTableSave_clicked();
    void on_tableWidgetMain_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_tableWidgetMain_cellClicked(int row, int column);
    void on_tableWidgetMain_customContextMenuRequested(const QPoint &pos);

private:
    void setHeaderSelection();

private:
    Ui::XGenericHeaderWidget *ui;
    qint64 g_nDataSize;
    QList<XLineEditHEX *> g_listLineEditsHEX;
    QList<XComboBoxEx *> g_listComboBoxes;
};

#endif  // XGENERICHEADERWIDGET_H
