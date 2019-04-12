// copyright (c) 2017-2019 hors<horsicq@gmail.com>
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

#include "pe_defs.h"

namespace Ui {
class PEWidget;
}

class PEWidget : public FormatWidget
{
    Q_OBJECT

public:
    PEWidget(QWidget *parent = nullptr);
    PEWidget(QIODevice *pDevice, OPTIONS *pOptions, QWidget *parent = nullptr);
    ~PEWidget();
    virtual void setData(QIODevice *pDevice,OPTIONS *pOptions);
protected:
    virtual bool _setValue(QVariant vValue,int nStype,int nNdata,int nVtype,int nPosition);
    virtual void setReadonly(bool bState);
    virtual void blockSignals(bool bState);
    virtual void adjustHeaderTable(int type,QTableWidget *pTableWidget);
private slots:
    void on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void widgetValueChanged(quint64 nValue);
    void widgetAction();
    void on_checkBoxReadonly_toggled(bool checked);

    void editSectionHeader();
    void reloadData();
    void on_tableWidget_Sections_customContextMenuRequested(const QPoint &pos);
    void loadImportLibrary(int nNumber);
    void loadRelocs(qint64 nOffset);

    void on_tableWidget_ImportLibraries_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_tableWidget_Relocs_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    bool createSectionTable(int type, QTableWidget *pTableWidget, const FormatWidget::HEADER_RECORD *pRecords, int nRecordCount);
    void on_tableWidget_Sections_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_treeWidgetResource_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

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
        __CB_size
    };
    enum PB
    {
        PB_CalculateChecksum,
        __PB_size
    };

    enum DT
    {
        DT_DateTimeStamp,
        __DT_size
    };

private:
    Ui::PEWidget *ui;

    XLineEditHEX *lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::__data_size];
    XLineEditHEX *lineEdit_IMAGE_NT_HEADERS[N_IMAGE_NT_HEADERS::__data_size];
    XLineEditHEX *lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::__data_size];
    XLineEditHEX *lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::__data_size];
    XLineEditHEX *lineEdit_IMAGE_DIRECTORY_ADDRESS[N_IMAGE_DIRECORIES::__data_size];
    XLineEditHEX *lineEdit_IMAGE_DIRECTORY_SIZE[N_IMAGE_DIRECORIES::__data_size];
    XLineEditHEX *lineEdit_EXPORT[N_IMAGE_EXPORT::__data_size];
    XComboBoxEx *comboBox[__CB_size];
    QPushButton *pushButton[__PB_size];
    XDateTimeEditX *dateTimeEdit[__DT_size];

    bool bInit[SPE::__TYPE_size];

    SubDevice *pSubDeviceSection;
    SubDevice *pSubDeviceOverlay;
    SubDevice *pSubDeviceResource;
};

#endif // PEWIDGET_H
