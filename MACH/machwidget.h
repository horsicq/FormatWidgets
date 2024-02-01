/* Copyright (c) 2019-2023 hors<horsicq@gmail.com>
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
#ifndef MACHWIDGET_H
#define MACHWIDGET_H

#include "../formatwidget.h"
#include "dialogsectionheader.h"
#include "mach_defs.h"
#include "machprocessdata.h"
#include "machsectionheaderwidget.h"

namespace Ui {
class MACHWidget;
}

class MACHWidget : public FormatWidget {
    Q_OBJECT

public:
    MACHWidget(QWidget *pParent = nullptr);
    MACHWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, QWidget *pParent = nullptr);
    ~MACHWidget();

    virtual void clear();
    virtual void cleanup();
    virtual void reload();

protected:
    virtual SV _setValue(QVariant vValue, qint32 nStype, qint32 nNdata, qint32 nVtype, qint32 nPosition, qint64 nOffset);
    virtual void setReadonly(bool bState);
    virtual void blockSignals(bool bState);
    virtual void adjustHeaderTable(qint32 nType, QTableWidget *pTableWidget);
    virtual QString typeIdToString(qint32 nType);
    virtual void _showInDisasmWindowAddress(XADDR nAddress);
    virtual void _showInDisasmWindowOffset(qint64 nOffset);
    virtual void _showInMemoryMapWindowOffset(qint64 nOffset);
    virtual void _showInHexWindow(qint64 nOffset, qint64 nSize);
    virtual void _findValue(quint64 nValue, bool bIsBigEndian);

private slots:
    virtual void reloadData();
    void widgetValueChanged(quint64 nValue);
    void on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pItemCurrent, QTreeWidgetItem *pItemPrevious);
    void on_checkBoxReadonly_toggled(bool bChecked);
    void on_pushButtonReload_clicked();
    void enableButton();
    void on_tableWidget_mach_header_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);
    void on_tableWidget_dyld_info_only_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);
    void on_tableWidget_symtab_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);
    void on_tableWidget_dysymtab_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);
    void on_tableWidget_version_min_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);
    void on_tableWidget_build_version_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);
    void on_tableWidget_source_version_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);
    void on_tableWidget_encryption_info_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);
    void on_tableWidget_function_starts_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);
    void on_tableWidget_data_in_code_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);
    void on_tableWidget_code_signature_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);
    void on_tableWidget_SuperBlob_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);
    void on_tableWidget_main_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);
    void on_tableWidget_unix_thread_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);

    void on_tableWidget_unix_thread_x86_32_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);
    void on_tableWidget_unix_thread_x86_64_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);
    void on_tableWidget_unix_thread_arm_32_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);
    void on_tableWidget_unix_thread_arm_64_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);
    void on_tableWidget_unix_thread_ppc_32_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn);

    void on_toolButtonPrev_clicked();
    void on_toolButtonNext_clicked();
    void onTableView_commands_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void onTableView_segments_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void onTableView_sections_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void onTableView_libraries_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void onTableView_weak_libraries_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void onTableView_id_library_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void onTableView_LOADFVMLIB_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void onTableView_IDFVMLIB_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void on_tableView_commands_doubleClicked(const QModelIndex &index);
    void on_tableView_commands_customContextMenuRequested(const QPoint &pos);
    void on_tableView_segments_doubleClicked(const QModelIndex &index);
    void on_tableView_segments_customContextMenuRequested(const QPoint &pos);
    void on_tableView_sections_doubleClicked(const QModelIndex &index);
    void on_tableView_sections_customContextMenuRequested(const QPoint &pos);
    void on_tableView_libraries_doubleClicked(const QModelIndex &index);
    void on_tableView_libraries_customContextMenuRequested(const QPoint &pos);
    void on_tableView_weak_libraries_doubleClicked(const QModelIndex &index);
    void on_tableView_weak_libraries_customContextMenuRequested(const QPoint &pos);
    void on_tableView_id_library_doubleClicked(const QModelIndex &index);
    void on_tableView_id_library_customContextMenuRequested(const QPoint &pos);
    void on_tableView_LOADFVMLIB_doubleClicked(const QModelIndex &index);
    void on_tableView_LOADFVMLIB_customContextMenuRequested(const QPoint &pos);
    void on_tableView_IDFVMLIB_doubleClicked(const QModelIndex &index);
    void on_tableView_IDFVMLIB_customContextMenuRequested(const QPoint &pos);
    void on_tableView_SymbolTable_doubleClicked(const QModelIndex &index);
    void on_tableView_SymbolTable_customContextMenuRequested(const QPoint &pos);
    void on_tableView_Functions_doubleClicked(const QModelIndex &index);
    void on_tableView_Functions_customContextMenuRequested(const QPoint &pos);
    void on_tableView_data_in_code_entry_doubleClicked(const QModelIndex &index);
    void on_tableView_data_in_code_entry_customContextMenuRequested(const QPoint &pos);
    void on_tableView_DYSYMTAB_modtab_doubleClicked(const QModelIndex &index);
    void on_tableView_DYSYMTAB_modtab_customContextMenuRequested(const QPoint &pos);
    void on_tableView_DYSYMTAB_toc_doubleClicked(const QModelIndex &index);
    void on_tableView_DYSYMTAB_toc_customContextMenuRequested(const QPoint &pos);
    void on_tableView_DYSYMTAB_extrel_doubleClicked(const QModelIndex &index);
    void on_tableView_DYSYMTAB_extrel_customContextMenuRequested(const QPoint &pos);
    void on_tableView_DYSYMTAB_locrel_doubleClicked(const QModelIndex &index);
    void on_tableView_DYSYMTAB_locrel_customContextMenuRequested(const QPoint &pos);
    void on_tableView_DYSYMTAB_indirectsyms_doubleClicked(const QModelIndex &index);
    void on_tableView_DYSYMTAB_indirectsyms_customContextMenuRequested(const QPoint &pos);
    void on_tableView_DYSYMTAB_extrefsyms_doubleClicked(const QModelIndex &index);
    void on_tableView_DYSYMTAB_extrefsyms_customContextMenuRequested(const QPoint &pos);
    void onTableView_DYLD_INFO_rebase_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void onTableView_DYLD_INFO_bind_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void onTableView_DYLD_INFO_weak_bind_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void onTableView_DYLD_INFO_lazy_bind_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void onTableView_DYLD_INFO_export_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void editCommandHeader();
    void editSegmentHeader();
    void segmentHex();
    void segmentDisasm();
    void segmentEntropy();
    void editSectionHeader();
    void sectionHex();
    void sectionDisasm();
    void sectionEntropy();
    void editLibraryHeader();
    void editWeakLibraryHeader();
    void editIdLibraryHeader();
    void editIDFVMLIBHeader();
    void editLOADFVMLIBHeader();
    void editSymbolHeader();
    void functionHex();
    void functionDisasm();
    void functionDemangle();
    void editDiceHeader();
    void editModTabHeader();
    void editTocHeader();
    void editExtrelHeader();
    void editLocrelHeader();
    void editIndirectSymbolHeader();
    void editExtRefSymbolHeader();
    void diceHex();
    void showSectionHeader(qint32 nType, QTableView *pTableView);

    void symbolDemangle();
    void indirectsymsDemangle();
    void tocDemangle();
    void extrefsymsDemangle();

    void on_pushButtonHex_clicked();
    void on_pushButtonDisasm_clicked();
    void on_pushButtonStrings_clicked();
    void on_pushButtonMemoryMap_clicked();
    void on_pushButtonEntropy_clicked();
    void on_pushButtonHeuristicScan_clicked();

private:
    enum CB {
        CB_mach_header_magic = 0,
        CB_mach_header_cputype,
        CB_mach_header_cpusubtype,
        CB_mach_header_filetype,
        CB_mach_header_flags,
        __CB_size
    };

    enum INV {
        INV_rebase_off,
        INV_bind_off,
        INV_weak_bind_off,
        INV_lazy_bind_off,
        INV_export_off,
        INV_symoff,
        INV_stroff,
        INV_tocoff,
        INV_modtaboff,
        INV_extrefsymoff,
        INV_indirectsymoff,
        INV_extreloff,
        INV_locreloff,
        INV_cryptoff,
        INV_FUNCTION_STARTS_dataoff,
        INV_DATA_IN_CODE_dataoff,
        INV_CODE_SIGNATURE_dataoff,
        INV_CHAINED_FIXUPS_dataoff,
        INV_EXPORTS_TREE_dataoff,
        INV_MAIN_entryoff,
        __INV_size
    };

    Ui::MACHWidget *ui;

    XLineEditHEX *g_lineEdit_mach_header[N_mach_header::__data_size];
    XLineEditHEX *g_lineEdit_mach_dyld_info_only[N_mach_dyld_info::__data_size];
    XLineEditHEX *g_lineEdit_mach_uuid[N_mach_uuid::__data_size];
    XLineEditHEX *g_lineEdit_mach_dylinker[N_mach_dylinker::__data_size];
    XLineEditHEX *g_lineEdit_mach_rpath[N_mach_rpath::__data_size];
    XLineEditHEX *g_lineEdit_mach_symtab[N_mach_symtab::__data_size];
    XLineEditHEX *g_lineEdit_mach_dysymtab[N_mach_dysymtab::__data_size];
    XLineEditHEX *g_lineEdit_mach_version_min[N_mach_version_min::__data_size];
    XLineEditHEX *g_lineEdit_mach_build_version[N_mach_build_version::__data_size];
    XLineEditHEX *g_lineEdit_mach_source_version[N_mach_source_version::__data_size];
    XLineEditHEX *g_lineEdit_mach_encryption_info[N_mach_encryption_info::__data_size];
    XLineEditHEX *g_lineEdit_mach_function_starts[N_mach_linkedit_data::__data_size];
    XLineEditHEX *g_lineEdit_mach_data_in_code[N_mach_linkedit_data::__data_size];
    XLineEditHEX *g_lineEdit_mach_code_signature[N_mach_linkedit_data::__data_size];
    XLineEditHEX *g_lineEdit_mach_dyld_chained_fixups[N_mach_linkedit_data::__data_size];
    XLineEditHEX *g_lineEdit_mach_dyld_exports_trie[N_mach_linkedit_data::__data_size];
    XLineEditHEX *g_lineEdit_mach_SuperBlob[N_mach_SuperBlob::__data_size];
    XLineEditHEX *g_lineEdit_mach_main[N_mach_main::__data_size];
    XLineEditHEX *g_lineEdit_mach_unix_thread[N_mach_unix_thread::__data_size];
    XLineEditHEX *g_lineEdit_mach_unix_thread_x86_32[N_mach_unix_thread_x86_32::__data_size];
    XLineEditHEX *g_lineEdit_mach_unix_thread_x86_64[N_mach_unix_thread_x86_64::__data_size];
    XLineEditHEX *g_lineEdit_mach_unix_thread_arm_32[N_mach_unix_thread_arm_32::__data_size];
    XLineEditHEX *g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::__data_size];
    XLineEditHEX *g_lineEdit_mach_unix_thread_m68k_32[N_mach_unix_thread_m68k_32::__data_size];
    XLineEditHEX *g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::__data_size];

    XComboBoxEx *g_comboBox[__CB_size];
    InvWidget *g_invWidget[__INV_size];

    SubDevice *g_subDevice[SMACH::__TYPE_size];
    QStandardItemModel *tvModel[SMACH::__TYPE_size];
};

#endif  // MACHWIDGET_H
