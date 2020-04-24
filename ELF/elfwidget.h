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
#ifndef ELFWIDGET_H
#define ELFWIDGET_H

#include "../formatwidget.h"
#include "dialogsectionheader.h"
#include "sectionheaderwidget.h"
#include "elf_defs.h"
#include "elfprocessdata.h"

namespace Ui
{
class ELFWidget;
}

class ELFWidget : public FormatWidget
{
    Q_OBJECT

public:
    ELFWidget(QWidget *parent=nullptr);
    ELFWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, QWidget *parent=nullptr);
    ~ELFWidget();
    virtual void clear();
    virtual void cleanup();
    virtual void reset();
    virtual void reload();

protected:
    virtual bool _setValue(QVariant vValue,int nStype,int nNdata,int nVtype,int nPosition,qint64 nOffset);
    virtual void setReadonly(bool bState);
    virtual void blockSignals(bool bState);
    virtual void adjustHeaderTable(int type,QTableWidget *pTableWidget);

private slots:
    virtual void reloadData();
    void addDatasets(XELF *pElf,QTreeWidgetItem *pParent, QList<XBinary::DATASET> *pList);
    void widgetValueChanged(quint64 nValue);
    void on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_checkBoxReadonly_toggled(bool checked);
    bool createSectionTable(int type, QTableWidget *pTableWidget, const FW_DEF::HEADER_RECORD *pRecords, int nRecordCount);
    void on_pushButtonReload_clicked();
    void enableButton();
    void loadShdr(int nRow);
    void loadPhdr(int nRow);
    void loadNote(int nRow);
    void on_tableWidget_Elf_Ehdr_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void editSectionHeader();
    void sectionHex();
    void editProgramHeader();
    void programHex();
    void editDynamicArrayTag();
    void on_tableView_SymbolTable_customContextMenuRequested(const QPoint &pos);
    void editSymbolHeader();
    void showSectionHeader(int type,QTableView *pTableView);
    static QString typeIdToString(int type);
    void on_tableView_Elf_Shdr_doubleClicked(const QModelIndex &index);
    void on_tableView_Elf_Shdr_customContextMenuRequested(const QPoint &pos);
    void on_tableView_SymbolTable_doubleClicked(const QModelIndex &index);
    void onTableView_Elf_Shdr_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void onTableView_Elf_Phdr_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void onTableView_DynamicArrayTags_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void onTableView_Notes_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void onTableView_Libraries_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void on_tableView_Elf_Phdr_doubleClicked(const QModelIndex &index);
    void on_tableView_Elf_Phdr_customContextMenuRequested(const QPoint &pos);
    void on_tableView_DynamicArrayTags_doubleClicked(const QModelIndex &index);
    void on_tableView_DynamicArrayTags_customContextMenuRequested(const QPoint &pos);
    void on_tableView_Notes_doubleClicked(const QModelIndex &index);
    void on_tableView_Rela_doubleClicked(const QModelIndex &index);
    void on_tableView_Rela_customContextMenuRequested(const QPoint &pos);
    void editRelaHeaderTag();

private:
    // TODO rename
    enum CB
    {
        CB_Elf_Ehdr_iclass=0,
        CB_Elf_Ehdr_idata,
        CB_Elf_Ehdr_iversion,
        CB_Elf_Ehdr_iosabi,
        CB_Elf_Ehdr_type,
        CB_Elf_Ehdr_machine,
        __CB_size
    };

    enum INV
    {
        INV_CB_Elf_e_entry,
        INV_CB_Elf_e_phoff,
        INV_CB_Elf_e_shoff,
        __INV_size
    };

    Ui::ELFWidget *ui;

    XLineEditHEX *lineEdit_Elf_Ehdr[N_Elf_Ehdr::__data_size];
    XLineEditHEX *lineEdit_Elf_Interpreter[N_ELF_INTERPRETER::__data_size];
    XLineEditHEX *lineEdit_Elf_RunPath[N_ELF_RUNPATH::__data_size];
    XComboBoxEx *comboBox[__CB_size];
    InvWidget *invWidget[__INV_size];

    SubDevice *subDevice[SELF::__TYPE_size];
    QStandardItemModel *tvModel[SELF::__TYPE_size];

    QSet<QString> stInit;

    qint32 nLastType;
    QString sLastInit;
};

#endif // ELFWIDGET_H
