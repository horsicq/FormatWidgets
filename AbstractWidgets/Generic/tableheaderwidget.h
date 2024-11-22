/* Copyright (c) 2024 hors<horsicq@gmail.com>
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
#ifndef TABLEHEADERWIDGET_H
#define TABLEHEADERWIDGET_H

#include "formatwidget.h"

namespace Ui {
class TableHeaderWidget;
}

class TableHeaderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TableHeaderWidget(QWidget *parent = nullptr);
    ~TableHeaderWidget();

    void setData(QTableWidget *pTableWidget, QList<FormatWidget::RECWIDGET> *pListRecWidget, FW_DEF::CWOPTIONS *pCwOptions);

signals:
    void reload();

private slots:
    void on_toolButtonTableReload_clicked();

private:
    Ui::TableHeaderWidget *ui;
    QTableWidget *g_pTableWidget;
    QList<FormatWidget::RECWIDGET> *g_pListRecWidget;
    FW_DEF::CWOPTIONS *g_pCwOptions;
};

#endif // TABLEHEADERWIDGET_H
