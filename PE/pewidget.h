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
#ifndef PEWIDGET_H
#define PEWIDGET_H

#include "../formatwidget.h"
#include "dialogsectionheader.h"
#include "sectionheaderwidget.h"
#include "pe_defs.h"
#include "peprocessdata.h"

namespace Ui
{
class PEWidget;
}

class PEWidget : public FormatWidget
{
    Q_OBJECT

public:
    PEWidget(QWidget *parent=nullptr);
    PEWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, QWidget *parent=nullptr);
    ~PEWidget();
    virtual void clear();
    virtual void cleanup();
    virtual void reset();
    virtual void reload();

protected:
    virtual bool _setValue(QVariant vValue,int nStype,int nNdata,int nVtype,int nPosition,qint64 nOffset);
    virtual void setReadonly(bool bState);
    virtual void blockSignals(bool bState);
    virtual void adjustHeaderTable(int type,QTableWidget *pTableWidget);
    virtual QString typeIdToString(int type);

private slots:
    void on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void widgetValueChanged(quint64 nValue);
    void widgetAction();
    void on_checkBoxReadonly_toggled(bool checked);
    void editSectionHeader();
    void sectionHex();
    void reloadData();

    void loadImportLibrary(int nNumber);
    void loadRelocs(int nNumber);
    void loadSection(int nRow);
    void loadException(int nRow);
    void loadDirectory(int nNumber);
    void loadDebug(int nRow);
    bool createSectionTable(int type, QTableWidget *pTableWidget, const FW_DEF::HEADER_RECORD *pRecords, int nRecordCount);
    void on_treeWidgetResources_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_pushButtonReload_clicked();
    void enableButton();
    void on_tableWidget_IMAGE_DIRECTORY_ENTRIES_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_tableWidget_IMAGE_DOS_HEADER_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_tableWidget_IMAGE_NT_HEADERS_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_tableWidget_IMAGE_FILE_HEADER_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_tableWidget_IMAGE_OPTIONAL_HEADER_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_tableWidget_LoadConfig_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_tableWidget_NetHeader_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_tableWidget_TLS_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void editImportHeader();
    void editDebugHeader();
    void editRelocsHeader();
    void editExceptionHeader();

    void showSectionHeader(int type,QTableView *pTableView);

    void on_tableView_Sections_customContextMenuRequested(const QPoint &pos);
    void on_tableView_Sections_doubleClicked(const QModelIndex &index);
    void onTableView_Sections_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void on_tableView_ImportLibraries_customContextMenuRequested(const QPoint &pos);
    void onTableView_ImportLibraries_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void on_tableView_ImportLibraries_doubleClicked(const QModelIndex &index);
    void on_tableView_Relocs_customContextMenuRequested(const QPoint &pos);
    void onTableView_Relocs_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void on_tableView_Relocs_doubleClicked(const QModelIndex &index);
    void on_tableView_Debug_customContextMenuRequested(const QPoint &pos);
    void onTableView_Debug_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void on_tableView_Debug_doubleClicked(const QModelIndex &index);
    void on_tableView_Exceptions_customContextMenuRequested(const QPoint &pos);
    void onTableView_Exceptions_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void on_tableView_Exceptions_doubleClicked(const QModelIndex &index);

private:
    enum CB
    {
        CB_IMAGE_DOS_HEADER_e_magic=0,
        CB_IMAGE_NT_HEADERS_Signature,
        CB_IMAGE_FILE_HEADER_Machine,
        CB_IMAGE_FILE_HEADER_Characteristics,
        CB_IMAGE_OPTIONAL_HEADER_Magic,
        CB_IMAGE_OPTIONAL_HEADER_Subsystem,
        CB_IMAGE_OPTIONAL_HEADER_DllCharacteristics,
        CB_IMAGE_NETHEADER_FLAGS,
        __CB_size
    };
    enum PB
    {
        PB_CalculateChecksum,
        __PB_size
    };

    enum TD
    {
        TD_IMAGE_FILE_HEADER_TimeDateStamp,
        TD_IMAGE_EXPORT_TimeDateStamp,
        __TD_size
    };

    enum INV
    {
        INV_IMAGE_DOS_HEADER_e_lfanew,
        INV_IMAGE_FILE_HEADER_PointerToSymbolTable,
        INV_IMAGE_OPTIONAL_HEADER_AddressOfEntryPoint,
        INV_IMAGE_OPTIONAL_HEADER_BaseOfCode,
        INV_IMAGE_OPTIONAL_HEADER_BaseOfData,
        INV_IMAGE_TLS_StartAddressOfRawData,
        INV_IMAGE_TLS_EndAddressOfRawData,
        INV_IMAGE_TLS_AddressOfIndex,
        INV_IMAGE_TLS_AddressOfCallBacks,
        INV_IMAGE_EXPORT_Name,
        INV_IMAGE_EXPORT_AddressOfFunctions,
        INV_IMAGE_EXPORT_AddressOfNames,
        INV_IMAGE_EXPORT_AddressOfNameOrdinals,
        INV_IMAGE_LOADCONFIG_SecurityCookie,
        INV_IMAGE_LOADCONFIG_SEHandlerTable,
        __INV_size
    };

    Ui::PEWidget *ui;
    XLineEditHEX *lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::__data_size];
    XLineEditHEX *lineEdit_IMAGE_NT_HEADERS[N_IMAGE_NT_HEADERS::__data_size];
    XLineEditHEX *lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::__data_size];
    XLineEditHEX *lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::__data_size];
    XLineEditHEX *lineEdit_TLS[N_IMAGE_TLS::__data_size];
    XLineEditHEX *lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::__data_size];
    XLineEditHEX *lineEdit_NetHeader[N_IMAGE_NETHEADER::__data_size];
    XLineEditHEX *lineEdit_EXPORT[N_IMAGE_EXPORT::__data_size];
    XLineEditHEX *lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::__data_size];
    XComboBoxEx *comboBox[__CB_size];
    QPushButton *pushButton[__PB_size];
    XDateTimeEditX *dateTimeEdit[__TD_size];
    InvWidget *invWidget[__INV_size];
    SubDevice *subDevice[SPE::__TYPE_size];
    bool bInit[SPE::__TYPE_size];
    QStandardItemModel *tvModel[SPE::__TYPE_size_ext];
};

#endif // PEWIDGET_H
