// copyright (c) 2019-2020 hors<horsicq@gmail.com>
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
#ifndef LEWIDGET_H
#define LEWIDGET_H

#include "../formatwidget.h"
#include "le_defs.h"
#include "leprocessdata.h"
#include "lesectionheaderwidget.h"

namespace Ui
{
class LEWidget;
}

class LEWidget : public FormatWidget
{
    Q_OBJECT

public:
    LEWidget(QWidget *pParent=nullptr);
    LEWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, QWidget *pParent=nullptr);
    ~LEWidget();
    virtual void clear();
    virtual void cleanup();
    virtual void reset();
    virtual void setData(QIODevice *pDevice,FW_DEF::OPTIONS *pOptions);
    virtual void reload();

protected:
    virtual bool _setValue(QVariant vValue,int nStype,int nNdata,int nVtype,int nPosition,qint64 nOffset);
    virtual void setReadonly(bool bState);
    virtual void blockSignals(bool bState);
    virtual void adjustHeaderTable(int nType,QTableWidget *pTableWidget);

private slots:
    virtual void reloadData();
    void widgetValueChanged(quint64 nValue);
    void on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_checkBoxReadonly_toggled(bool bChecked);
    void on_pushButtonReload_clicked();
    void enableButton();
    void on_tableWidget_DOS_HEADER_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_tableWidget_VXD_HEADER_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_tableView_Objects_customContextMenuRequested(const QPoint &pos);
    void on_tableView_Objects_doubleClicked(const QModelIndex &index);
    void onTableView_Objects_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    enum CB
    {
        CB_DOS_HEADER_e_magic=0,
        CB_VXD_HEADER_e32_magic,
        CB_VXD_HEADER_e32_cpu,
        CB_VXD_HEADER_e32_os,
        __CB_size
    };

    enum INV
    {
        INV_DOS_HEADER_e_lfanew,
        __INV_size
    };

    Ui::LEWidget *ui;

    XLineEditHEX *lineEdit_DOS_HEADER[N_LE_DOS_HEADER::__data_size];
    XLineEditHEX *lineEdit_VXD_HEADER[N_VXD_HEADER::__data_size];
    XComboBoxEx *comboBox[__CB_size];

    InvWidget *invWidget[__INV_size];
    SubDevice *subDevice[SLE::__TYPE_size];
    QStandardItemModel *tvModel[SLE::__TYPE_size];

    QSet<QString> stInit;
    qint32 nLastType;
    QString sLastInit;
};

#endif // LEWIDGET_H
