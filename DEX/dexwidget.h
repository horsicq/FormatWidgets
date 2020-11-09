// copyright (c) 2017-2020 hors<horsicq@gmail.com>
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
#ifndef DEXWIDGET_H
#define DEXWIDGET_H

#include "../formatwidget.h"
#include "dialogsectionheader.h"
#include "dexsectionheaderwidget.h"
#include "dex_defs.h"
#include "dexprocessdata.h"

namespace Ui
{
class DEXWidget;
}

class DEXWidget : public FormatWidget
{
    Q_OBJECT

public:
    DEXWidget(QWidget *pParent=nullptr);
    DEXWidget(QIODevice *pDevice,FW_DEF::OPTIONS *pOptions,QWidget *pParent=nullptr);
    ~DEXWidget();
    virtual void clear();
    virtual void cleanup();
    virtual void reset();
    virtual void reload();

protected:
    virtual bool _setValue(QVariant vValue,int nStype,int nNdata,int nVtype,int nPosition,qint64 nOffset);
    virtual void setReadonly(bool bState);
    virtual void blockSignals(bool bState);
    virtual void adjustHeaderTable(int nType,QTableWidget *pTableWidget);
    virtual QString typeIdToString(int nType);

private slots:
    virtual void reloadData();
    void widgetValueChanged(quint64 nValue);
    void on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pCurrent,QTreeWidgetItem *pPrevious);
    void on_checkBoxReadonly_toggled(bool bChecked);
    bool createSectionTable(int nType, QTableWidget *pTableWidget,const FW_DEF::HEADER_RECORD *pHeaderRecord,int nNumberOfRecords);
    void on_pushButtonReload_clicked();
    void enableButton();
    void on_tableWidget_Header_currentCellChanged(int nCurrentRow,int nCurrentColumn,int nPreviousRow,int nPreviousColumn);
    void on_lineEditFilterStrings_textChanged(const QString &sString);
    void on_lineEditFilterTypes_textChanged(const QString &sString);

private:
    // TODO rename
    enum CB
    {
        CB_Dex_Header_magic=0,
        CB_Dex_Header_version,
        CB_Dex_Header_endian_tag,
        __CB_size
    };

    enum INV
    {
        INV_link=0,
        INV_map,
        INV_string_ids,
        INV_type_ids,
        INV_proto_ids,
        INV_field_ids,
        INV_method_ids,
        INV_class_defs,
        INV_data,
        __INV_size
    };

    Ui::DEXWidget *ui;

    XLineEditHEX *lineEdit_HEADER[N_DEX_HEADER::__data_size];

    XComboBoxEx *comboBox[__CB_size];
    InvWidget *invWidget[__INV_size];

    SubDevice *subDevice[SDEX::__TYPE_size];
    QStandardItemModel *tvModel[SDEX::__TYPE_size];

    QSet<QString> stInit;

    qint32 nLastType;
    QString sLastInit;

    QSortFilterProxyModel *pFilterStrings;
    QSortFilterProxyModel *pFilterTypes;
};

#endif // DEXWIDGET_H
