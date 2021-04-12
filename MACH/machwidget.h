// copyright (c) 2019-2021 hors<horsicq@gmail.com>
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
#ifndef MACHWIDGET_H
#define MACHWIDGET_H

#include "../formatwidget.h"
#include "dialogsectionheader.h"
#include "mach_defs.h"
#include "machsectionheaderwidget.h"
#include "machprocessdata.h"

namespace Ui
{
class MACHWidget;
}

class MACHWidget : public FormatWidget
{
    Q_OBJECT

public:
    MACHWidget(QWidget *pParent=nullptr);
    MACHWidget(QIODevice *pDevice,FW_DEF::OPTIONS options,QWidget *pParent=nullptr);
    ~MACHWidget();
    void setShortcuts(XShortcuts *pShortcuts);

    virtual void clear();
    virtual void cleanup();
    virtual void reset();
    virtual void reload();

protected:
    virtual SV _setValue(QVariant vValue,int nStype,int nNdata,int nVtype,int nPosition,qint64 nOffset);
    virtual void setReadonly(bool bState);
    virtual void blockSignals(bool bState);
    virtual void adjustHeaderTable(int nType,QTableWidget *pTableWidget);
    virtual QString typeIdToString(int nType);
    virtual void _showInDisasmWindowAddress(qint64 nAddress);
    virtual void _showInDisasmWindowOffset(qint64 nOffset);
    virtual void _showInMemoryMapWindowOffset(qint64 nOffset);
    virtual void _showInHexWindow(qint64 nOffset,qint64 nSize);

private slots:
    virtual void reloadData();
    void widgetValueChanged(quint64 nValue);
    void on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pCurrent,QTreeWidgetItem *pPrevious);
    void on_checkBoxReadonly_toggled(bool bChecked);
    void on_pushButtonReload_clicked();
    void enableButton();
    void on_tableWidget_mach_header_currentCellChanged(int nCurrentRow,int nCurrentColumn,int nPreviousRow,int nPreviousColumn);
    void on_toolButtonPrev_clicked();
    void on_toolButtonNext_clicked();
    void onTableView_commands_currentRowChanged(const QModelIndex &current,const QModelIndex &previous);
    void onTableView_segments_currentRowChanged(const QModelIndex &current,const QModelIndex &previous);
    void onTableView_sections_currentRowChanged(const QModelIndex &current,const QModelIndex &previous);
    void onTableView_libraries_currentRowChanged(const QModelIndex &current,const QModelIndex &previous);
    void onTableView_id_library_currentRowChanged(const QModelIndex &current,const QModelIndex &previous);
    void on_tableView_commands_doubleClicked(const QModelIndex &index);
    void on_tableView_commands_customContextMenuRequested(const QPoint &pos);
    void on_tableView_segments_doubleClicked(const QModelIndex &index);
    void on_tableView_segments_customContextMenuRequested(const QPoint &pos);
    void on_tableView_sections_doubleClicked(const QModelIndex &index);
    void on_tableView_sections_customContextMenuRequested(const QPoint &pos);
    void on_tableView_libraries_doubleClicked(const QModelIndex &index);
    void on_tableView_libraries_customContextMenuRequested(const QPoint &pos);
    void on_tableView_id_library_doubleClicked(const QModelIndex &index);
    void on_tableView_id_library_customContextMenuRequested(const QPoint &pos);
    void editCommandHeader();
    void editSegmentHeader();
    void editSectionHeader();
    void editLibraryHeader();
    void editIdLibraryHeader();
    void showSectionHeader(int nType,QTableView *pTableView);

private:
    enum CB
    {
        CB_mach_header_magic=0,
        CB_mach_header_cputype,
        CB_mach_header_cpusubtype,
        CB_mach_header_filetype,
        CB_mach_header_flags,
        __CB_size
    };

    Ui::MACHWidget *ui;

    XLineEditHEX *g_lineEdit_mach_header[N_mach_header::__data_size];
    XComboBoxEx *g_comboBox[__CB_size];

    SubDevice *g_subDevice[SMACH::__TYPE_size];
    QStandardItemModel *tvModel[SMACH::__TYPE_size];

    QSet<QString> g_stInit;
    qint32 g_nLastType;
    QString g_sLastInit;
};

#endif // MACHWIDGET_H
