/* Copyright (c) 2017-2022 hors<horsicq@gmail.com>
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
#include "pesectionheaderwidget.h"

#include "ui_pesectionheaderwidget.h"

PESectionHeaderWidget::PESectionHeaderWidget(QWidget *pParent) : FormatWidget(pParent), ui(new Ui::PESectionHeaderWidget)
{
    ui->setupUi(this);
}

PESectionHeaderWidget::PESectionHeaderWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType, QWidget *pParent)
    : PESectionHeaderWidget(pParent)
{
    PESectionHeaderWidget::setData(pDevice, options, nNumber, nOffset, nType);

    g_ppLinedEdit = 0;
    g_nLineEditSize = 0;
    g_ppComboBox = 0;
    g_nComboBoxSize = 0;
    g_ppInvWidget = 0;
    g_nInvWidgetSize = 0;

    if (nType == SPE::TYPE_IMAGE_SECTION_HEADER) {
        g_nLineEditSize = N_IMAGE_SECTION_HEADER::__data_size + 1;
        g_nComboBoxSize = N_IMAGE_SECTION_HEADER::__CB_size;
        g_nInvWidgetSize = N_IMAGE_SECTION_HEADER::__INV_size;
    } else if (nType == SPE::TYPE_IMPORT) {
        g_nLineEditSize = N_IMAGE_IMPORT::__data_size;
        g_nComboBoxSize = N_IMAGE_IMPORT::__CB_size;
        g_nInvWidgetSize = N_IMAGE_IMPORT::__INV_size;
    } else if (nType == SPE::TYPE_DEBUG) {
        g_nLineEditSize = N_IMAGE_DEBUG::__data_size;
        g_nComboBoxSize = N_IMAGE_DEBUG::__CB_size;
        g_nInvWidgetSize = N_IMAGE_DEBUG::__INV_size;
    } else if (nType == SPE::TYPE_RELOCS) {
        g_nLineEditSize = N_IMAGE_RELOCS::__data_size;
        g_nComboBoxSize = N_IMAGE_RELOCS::__CB_size;
        g_nInvWidgetSize = N_IMAGE_RELOCS::__INV_size;
    } else if (nType == SPE::TYPE_EXCEPTION) {
        g_nLineEditSize = N_IMAGE_EXCEPTIONS::__data_size;
        g_nComboBoxSize = N_IMAGE_EXCEPTIONS::__CB_size;
        g_nInvWidgetSize = N_IMAGE_EXCEPTIONS::__INV_size;
    } else if (nType == SPE::TYPE_DELAYIMPORT) {
        g_nLineEditSize = N_IMAGE_DELAYIMPORT::__data_size;
        g_nComboBoxSize = N_IMAGE_DELAYIMPORT::__CB_size;
        g_nInvWidgetSize = N_IMAGE_DELAYIMPORT::__INV_size;
    } else if (nType == SPE::TYPE_BOUNDIMPORT) {
        g_nLineEditSize = N_IMAGE_BOUNDIMPORT::__data_size;
        g_nComboBoxSize = N_IMAGE_BOUNDIMPORT::__CB_size;
        g_nInvWidgetSize = N_IMAGE_BOUNDIMPORT::__INV_size;
    } else if (nType == SPE::TYPE_IMAGE_DIRECTORY_ENTRIES) {
        g_nLineEditSize = N_IMAGE_DATA_DIRECTORY::__data_size;
        g_nComboBoxSize = N_IMAGE_DATA_DIRECTORY::__CB_size;
        g_nInvWidgetSize = N_IMAGE_DATA_DIRECTORY::__INV_size;
    }

    if (g_nLineEditSize) {
        g_ppLinedEdit = new PXLineEditHEX[g_nLineEditSize];
    }

    if (g_nComboBoxSize) {
        g_ppComboBox = new PXComboBoxEx[g_nComboBoxSize];
    }

    if (g_nInvWidgetSize) {
        g_ppInvWidget = new PInvWidget[g_nInvWidgetSize];
    }
}

PESectionHeaderWidget::~PESectionHeaderWidget()
{
    if (g_ppLinedEdit) {
        delete[] g_ppLinedEdit;
    }

    if (g_ppComboBox) {
        delete[] g_ppComboBox;
    }

    if (g_ppInvWidget) {
        delete[] g_ppInvWidget;
    }

    delete ui;
}

void PESectionHeaderWidget::clear()
{
    reset();

    memset(g_ppLinedEdit, 0, g_nLineEditSize * sizeof(XLineEditHEX *));
    memset(g_ppComboBox, 0, g_nComboBoxSize * sizeof(XComboBoxEx *));
    memset(g_ppInvWidget, 0, g_nInvWidgetSize * sizeof(InvWidget *));

    g_pSubDevice = nullptr;

    ui->checkBoxReadonly->setChecked(true);
}

void PESectionHeaderWidget::cleanup()
{
    PESectionHeaderWidget::clear();
}

void PESectionHeaderWidget::reload()
{
    PESectionHeaderWidget::clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    reloadData();
}

void PESectionHeaderWidget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    ui->widgetHex->setGlobal(pShortcuts, pXOptions);
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
}

FormatWidget::SV PESectionHeaderWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    Q_UNUSED(nVtype)
    Q_UNUSED(nOffset)

    SV result = SV_NONE;

    quint64 nValue = vValue.toULongLong();
    QString sValue = vValue.toString();

    if (getDevice()->isWritable()) {
        XPE pe(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

        if (pe.isValid()) {
            switch (nStype) {
                case SPE::TYPE_IMAGE_SECTION_HEADER:
                    switch (nNdata) {
                        case N_IMAGE_SECTION_HEADER::VirtualAddress:
                            g_ppInvWidget[N_IMAGE_SECTION_HEADER::INV_VirtualAddress]->setAddressAndSize(&pe, pe.getBaseAddress() + (quint32)nValue, 0);
                            break;
                        case N_IMAGE_SECTION_HEADER::PointerToRawData:
                            g_ppInvWidget[N_IMAGE_SECTION_HEADER::INV_PointerToRawData]->setOffsetAndSize(&pe, (quint32)nValue, 0);
                            break;
                        case N_IMAGE_SECTION_HEADER::Characteristics:
                        case N_IMAGE_SECTION_HEADER::Characteristics + 1:
                            g_ppComboBox[N_IMAGE_SECTION_HEADER::CB_CHARACTERISTICS]->setValue((quint32)nValue);
                            g_ppComboBox[N_IMAGE_SECTION_HEADER::CB_ALIGH]->setValue((quint32)nValue);
                            break;
                    }
                    break;

                case SPE::TYPE_IMPORT:
                    switch (nNdata) {
                        case N_IMAGE_IMPORT::OriginalFirstThunk:
                            g_ppInvWidget[N_IMAGE_IMPORT::INV_OriginalFirstThunk]->setAddressAndSize(&pe, pe.getBaseAddress() + (quint32)nValue, 0);
                            break;
                        case N_IMAGE_IMPORT::Name:
                            g_ppInvWidget[N_IMAGE_IMPORT::INV_Name]->setAddressAndSize(&pe, pe.getBaseAddress() + (quint32)nValue, 0);
                            break;
                        case N_IMAGE_IMPORT::FirstThunk:
                            g_ppInvWidget[N_IMAGE_IMPORT::INV_FirstThunk]->setAddressAndSize(&pe, pe.getBaseAddress() + (quint32)nValue, 0);
                            break;
                    }
                    break;

                case SPE::TYPE_DEBUG:
                    switch (nNdata) {
                        case N_IMAGE_DEBUG::Type:
                            g_ppComboBox[N_IMAGE_DEBUG::CB_TYPE]->setValue((quint32)nValue);
                            break;
                    }
                    break;
            }

            switch (nStype) {
                case SPE::TYPE_IMAGE_SECTION_HEADER:
                    switch (nNdata) {
                        case N_IMAGE_SECTION_HEADER::Name:
                            pe.setSection_NameAsString((quint32)nPosition, sValue);
                            break;
                        case N_IMAGE_SECTION_HEADER::VirtualSize:
                            pe.setSection_VirtualSize((quint32)nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_SECTION_HEADER::VirtualAddress:
                            pe.setSection_VirtualAddress((quint32)nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_SECTION_HEADER::SizeOfRawData:
                            pe.setSection_SizeOfRawData((quint32)nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_SECTION_HEADER::PointerToRawData:
                            pe.setSection_PointerToRawData((quint32)nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_SECTION_HEADER::PointerToRelocations:
                            pe.setSection_PointerToRelocations((quint32)nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_SECTION_HEADER::PointerToLinenumbers:
                            pe.setSection_PointerToLinenumbers((quint32)nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_SECTION_HEADER::NumberOfRelocations:
                            pe.setSection_NumberOfRelocations((quint32)nPosition, (quint16)nValue);
                            break;
                        case N_IMAGE_SECTION_HEADER::NumberOfLinenumbers:
                            pe.setSection_NumberOfLinenumbers((quint32)nPosition, (quint16)nValue);
                            break;
                        case N_IMAGE_SECTION_HEADER::Characteristics:
                            pe.setSection_Characteristics((quint32)nPosition, (quint32)nValue);
                            break;
                    }
                    break;

                case SPE::TYPE_IMPORT:
                    switch (nNdata) {
                        case N_IMAGE_IMPORT::OriginalFirstThunk:
                            pe.setImportDescriptor_OriginalFirstThunk(nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_IMPORT::TimeDateStamp:
                            pe.setImportDescriptor_TimeDateStamp(nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_IMPORT::ForwarderChain:
                            pe.setImportDescriptor_ForwarderChain(nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_IMPORT::Name:
                            pe.setImportDescriptor_Name(nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_IMPORT::FirstThunk:
                            pe.setImportDescriptor_FirstThunk(nPosition, (quint32)nValue);
                            break;
                    }
                    break;

                case SPE::TYPE_DEBUG:
                    switch (nNdata) {
                        case N_IMAGE_DEBUG::Characteristics:
                            pe.setDebugHeader_Characteristics(nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_DEBUG::TimeDateStamp:
                            pe.setDebugHeader_TimeDateStamp(nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_DEBUG::MajorVersion:
                            pe.setDebugHeader_MajorVersion(nPosition, (quint16)nValue);
                            break;
                        case N_IMAGE_DEBUG::MinorVersion:
                            pe.setDebugHeader_MinorVersion(nPosition, (quint16)nValue);
                            break;
                        case N_IMAGE_DEBUG::Type:
                            pe.setDebugHeader_Type(nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_DEBUG::SizeOfData:
                            pe.setDebugHeader_SizeOfData(nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_DEBUG::AddressOfRawData:
                            pe.setDebugHeader_AddressOfRawData(nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_DEBUG::PointerToRawData:
                            pe.setDebugHeader_PointerToRawData(nPosition, (quint32)nValue);
                            break;
                    }
                    break;

                case SPE::TYPE_RELOCS:
                    switch (nNdata) {
                        case N_IMAGE_RELOCS::VirtualAddress:
                            pe.setRelocsVirtualAddress(nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_RELOCS::SizeOfBlock:
                            pe.setRelocsSizeOfBlock(nPosition, (quint32)nValue);
                            break;
                    }
                    break;

                case SPE::TYPE_EXCEPTION:
                    switch (nNdata) {
                        case N_IMAGE_EXCEPTIONS::BeginAddress:
                            pe.setException_BeginAddress(nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_EXCEPTIONS::EndAddress:
                            pe.setException_EndAddress(nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_EXCEPTIONS::UnwindInfoAddress:
                            pe.setException_UnwindInfoAddress(nPosition, (quint16)nValue);
                            break;
                    }
                    break;

                case SPE::TYPE_DELAYIMPORT:
                    switch (nNdata) {
                        case N_IMAGE_DELAYIMPORT::AllAttributes:
                            pe.setDelayImport_AllAttributes(nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_DELAYIMPORT::DllNameRVA:
                            pe.setDelayImport_DllNameRVA(nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_DELAYIMPORT::ModuleHandleRVA:
                            pe.setDelayImport_ModuleHandleRVA(nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_DELAYIMPORT::ImportAddressTableRVA:
                            pe.setDelayImport_ImportAddressTableRVA(nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_DELAYIMPORT::ImportNameTableRVA:
                            pe.setDelayImport_ImportNameTableRVA(nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_DELAYIMPORT::BoundImportAddressTableRVA:
                            pe.setDelayImport_BoundImportAddressTableRVA(nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_DELAYIMPORT::UnloadInformationTableRVA:
                            pe.setDelayImport_UnloadInformationTableRVA(nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_DELAYIMPORT::TimeDateStamp:
                            pe.setDelayImport_TimeDateStamp(nPosition, (quint32)nValue);
                            break;
                    }
                    break;

                case SPE::TYPE_BOUNDIMPORT:
                    switch (nNdata) {
                        case N_IMAGE_BOUNDIMPORT::TimeDateStamp:
                            pe.setBoundImport_TimeDateStamp(nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_BOUNDIMPORT::OffsetModuleName:
                            pe.setBoundImport_OffsetModuleName(nPosition, (quint16)nValue);
                            break;
                        case N_IMAGE_BOUNDIMPORT::NumberOfModuleForwarderRefs:
                            pe.setBoundImport_NumberOfModuleForwarderRefs(nPosition, (quint16)nValue);
                            break;
                    }
                    break;

                case SPE::TYPE_IMAGE_DIRECTORY_ENTRIES:
                    switch (nNdata) {
                        case N_IMAGE_DATA_DIRECTORY::Address:
                            pe.setOptionalHeader_DataDirectory_VirtualAddress(nPosition, (quint32)nValue);
                            break;
                        case N_IMAGE_DATA_DIRECTORY::Size:
                            pe.setOptionalHeader_DataDirectory_Size(nPosition, (quint32)nValue);
                            break;
                    }
                    break;
            }

            switch (nStype) {
                case SPE::TYPE_IMPORT:
                    switch (nNdata) {
                        case N_IMAGE_IMPORT::OriginalFirstThunk:
                            addComment(ui->tableWidget, N_IMAGE_IMPORT::OriginalFirstThunk, HEADER_COLUMN_COMMENT, pe.getMemoryRecordInfoByRelAddress((quint32)nValue));
                            break;
                        case N_IMAGE_IMPORT::Name:
                            addComment(ui->tableWidget, N_IMAGE_IMPORT::Name, HEADER_COLUMN_COMMENT, pe.read_ansiString(pe.relAddressToOffset((quint32)nValue)));
                            break;
                            break;
                        case N_IMAGE_IMPORT::FirstThunk:
                            addComment(ui->tableWidget, N_IMAGE_IMPORT::FirstThunk, HEADER_COLUMN_COMMENT, pe.getMemoryRecordInfoByRelAddress((quint32)nValue));
                            break;
                    }
                    break;
            }

            ui->widgetHex->reload();

            result = SV_EDITED;
        }
    }

    return result;
}
void PESectionHeaderWidget::setReadonly(bool bState)
{
    if (ui->checkBoxReadonly->isChecked() != bState) {
        const bool bBlocked1 = ui->checkBoxReadonly->blockSignals(true);
        ui->checkBoxReadonly->setChecked(bState);
        ui->checkBoxReadonly->blockSignals(bBlocked1);
    }

    setLineEditsReadOnly(g_ppLinedEdit, g_nLineEditSize, bState);
    setComboBoxesReadOnly(g_ppComboBox, g_nComboBoxSize, bState);
}

void PESectionHeaderWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)g_ppLinedEdit, g_nLineEditSize, bState);
    _blockSignals((QObject **)g_ppComboBox, g_nComboBoxSize, bState);
}

void PESectionHeaderWidget::adjustHeaderTable(int nType, QTableWidget *pTableWidget)
{
    XBinary::MODE mode = XPE::getMode(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET, getColumnWidth(this, CW_UINT16, mode));
    pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE, getColumnWidth(this, CW_TYPE, mode));

    switch (nType) {
        case SPE::TYPE_IMAGE_SECTION_HEADER:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGMID, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;

        case SPE::TYPE_IMPORT:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGMID, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;

        case SPE::TYPE_DEBUG:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGMID, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;

        case SPE::TYPE_RELOCS:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGMID, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;

        case SPE::TYPE_EXCEPTION:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGMID, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;

        case SPE::TYPE_DELAYIMPORT:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGMID, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;

        case SPE::TYPE_BOUNDIMPORT:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGMID, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;

        case SPE::TYPE_IMAGE_DIRECTORY_ENTRIES:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGMID, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;
    }
}

void PESectionHeaderWidget::_showInHexWindow(qint64 nOffset, qint64 nSize)
{
    showHex(nOffset, nSize);
}

void PESectionHeaderWidget::on_checkBoxReadonly_toggled(bool bChecked)
{
    setReadonly(bChecked);
}

void PESectionHeaderWidget::reloadData()
{
    qint32 nType = getType();

    XPE pe(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (pe.isValid()) {
        if (nType == SPE::TYPE_IMAGE_SECTION_HEADER) {
            createHeaderTable(SPE::TYPE_IMAGE_SECTION_HEADER, ui->tableWidget, N_IMAGE_SECTION_HEADER::records, g_ppLinedEdit, N_IMAGE_SECTION_HEADER::__data_size + 1,
                              getNumber());
            g_ppComboBox[N_IMAGE_SECTION_HEADER::CB_CHARACTERISTICS] = createComboBox(ui->tableWidget, XPE::getImageSectionHeaderFlagsS(), SPE::TYPE_IMAGE_SECTION_HEADER,
                                                                                      N_IMAGE_SECTION_HEADER::Characteristics, XComboBoxEx::CBTYPE_FLAGS);
            g_ppComboBox[N_IMAGE_SECTION_HEADER::CB_ALIGH] =
                createComboBox(ui->tableWidget, XPE::getImageSectionHeaderAlignsS(), SPE::TYPE_IMAGE_SECTION_HEADER, N_IMAGE_SECTION_HEADER::Characteristics + 1,
                               XComboBoxEx::CBTYPE_ELIST, XPE_DEF::S_IMAGE_SCN_ALIGN_MASK);

            g_ppInvWidget[N_IMAGE_SECTION_HEADER::INV_VirtualAddress] =
                createInvWidget(ui->tableWidget, SPE::TYPE_IMAGE_SECTION_HEADER, N_IMAGE_SECTION_HEADER::VirtualAddress, InvWidget::TYPE_HEX);
            g_ppInvWidget[N_IMAGE_SECTION_HEADER::INV_PointerToRawData] =
                createInvWidget(ui->tableWidget, SPE::TYPE_IMAGE_SECTION_HEADER, N_IMAGE_SECTION_HEADER::PointerToRawData, InvWidget::TYPE_HEX);

            blockSignals(true);

            XPE_DEF::IMAGE_SECTION_HEADER ish = pe.getSectionHeader(getNumber());

            QString sName = QString((char *)ish.Name);
            sName.resize(qMin(sName.length(), XPE_DEF::S_IMAGE_SIZEOF_SHORT_NAME));

            g_ppLinedEdit[N_IMAGE_SECTION_HEADER::Name]->setStringValue(sName, XPE_DEF::S_IMAGE_SIZEOF_SHORT_NAME);  // TODO Check
            g_ppLinedEdit[N_IMAGE_SECTION_HEADER::VirtualSize]->setValue(ish.Misc.VirtualSize);
            g_ppLinedEdit[N_IMAGE_SECTION_HEADER::VirtualAddress]->setValue(ish.VirtualAddress);
            g_ppLinedEdit[N_IMAGE_SECTION_HEADER::SizeOfRawData]->setValue(ish.SizeOfRawData);
            g_ppLinedEdit[N_IMAGE_SECTION_HEADER::PointerToRawData]->setValue(ish.PointerToRawData);
            g_ppLinedEdit[N_IMAGE_SECTION_HEADER::PointerToRelocations]->setValue(ish.PointerToRelocations);
            g_ppLinedEdit[N_IMAGE_SECTION_HEADER::PointerToLinenumbers]->setValue(ish.PointerToLinenumbers);
            g_ppLinedEdit[N_IMAGE_SECTION_HEADER::NumberOfRelocations]->setValue(ish.NumberOfRelocations);
            g_ppLinedEdit[N_IMAGE_SECTION_HEADER::NumberOfLinenumbers]->setValue(ish.NumberOfLinenumbers);
            g_ppLinedEdit[N_IMAGE_SECTION_HEADER::Characteristics]->setValue(ish.Characteristics);

            g_ppComboBox[N_IMAGE_SECTION_HEADER::CB_CHARACTERISTICS]->setValue(ish.Characteristics);
            g_ppComboBox[N_IMAGE_SECTION_HEADER::CB_ALIGH]->setValue(ish.Characteristics);

            g_ppInvWidget[N_IMAGE_SECTION_HEADER::INV_VirtualAddress]->setAddressAndSize(&pe, pe.getBaseAddress() + ish.VirtualAddress, 0);
            g_ppInvWidget[N_IMAGE_SECTION_HEADER::INV_PointerToRawData]->setOffsetAndSize(&pe, ish.PointerToRawData, 0);

            qint64 nOffset = pe.getSectionHeaderOffset(getNumber());
            qint64 nSize = pe.getSectionHeaderSize();
            qint64 nAddress = pe.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &g_pSubDevice, ui->widgetHex);

            blockSignals(false);
        } else if (nType == SPE::TYPE_IMPORT) {
            createHeaderTable(SPE::TYPE_IMPORT, ui->tableWidget, N_IMAGE_IMPORT::records, g_ppLinedEdit, N_IMAGE_IMPORT::__data_size, getNumber());

            g_ppInvWidget[N_IMAGE_IMPORT::INV_OriginalFirstThunk] =
                createInvWidget(ui->tableWidget, SPE::TYPE_IMPORT, N_IMAGE_IMPORT::OriginalFirstThunk, InvWidget::TYPE_HEX);
            g_ppInvWidget[N_IMAGE_IMPORT::INV_Name] = createInvWidget(ui->tableWidget, SPE::TYPE_IMPORT, N_IMAGE_IMPORT::Name, InvWidget::TYPE_HEX);
            g_ppInvWidget[N_IMAGE_IMPORT::INV_FirstThunk] = createInvWidget(ui->tableWidget, SPE::TYPE_IMPORT, N_IMAGE_IMPORT::FirstThunk, InvWidget::TYPE_HEX);

            blockSignals(true);

            XBinary::_MEMORY_MAP memoryMap = pe.getMemoryMap();

            XPE_DEF::IMAGE_IMPORT_DESCRIPTOR idh = pe.getImportDescriptor(getNumber());

            g_ppLinedEdit[N_IMAGE_IMPORT::OriginalFirstThunk]->setValue(idh.OriginalFirstThunk);
            g_ppLinedEdit[N_IMAGE_IMPORT::TimeDateStamp]->setValue(idh.TimeDateStamp);
            g_ppLinedEdit[N_IMAGE_IMPORT::ForwarderChain]->setValue(idh.ForwarderChain);
            g_ppLinedEdit[N_IMAGE_IMPORT::Name]->setValue(idh.Name);
            g_ppLinedEdit[N_IMAGE_IMPORT::FirstThunk]->setValue(idh.FirstThunk);

            g_ppInvWidget[N_IMAGE_IMPORT::INV_OriginalFirstThunk]->setAddressAndSize(&pe, pe.getBaseAddress() + idh.OriginalFirstThunk, 0);
            g_ppInvWidget[N_IMAGE_IMPORT::INV_Name]->setAddressAndSize(&pe, pe.getBaseAddress() + idh.Name, 0);
            g_ppInvWidget[N_IMAGE_IMPORT::INV_FirstThunk]->setAddressAndSize(&pe, pe.getBaseAddress() + idh.FirstThunk, 0);

            addComment(ui->tableWidget, N_IMAGE_IMPORT::OriginalFirstThunk, HEADER_COLUMN_COMMENT, pe.getMemoryRecordInfoByRelAddress(&memoryMap, idh.OriginalFirstThunk));
            addComment(ui->tableWidget, N_IMAGE_IMPORT::Name, HEADER_COLUMN_COMMENT, pe.read_ansiString(pe.relAddressToOffset(&memoryMap, idh.Name)));
            addComment(ui->tableWidget, N_IMAGE_IMPORT::FirstThunk, HEADER_COLUMN_COMMENT, pe.getMemoryRecordInfoByRelAddress(&memoryMap, idh.FirstThunk));

            qint64 nOffset = pe.getImportDescriptorOffset(getNumber());
            qint64 nSize = pe.getImportDescriptorSize();
            qint64 nAddress = pe.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &g_pSubDevice, ui->widgetHex);

            blockSignals(false);
        } else if (nType == SPE::TYPE_DEBUG) {
            createHeaderTable(SPE::TYPE_DEBUG, ui->tableWidget, N_IMAGE_DEBUG::records, g_ppLinedEdit, N_IMAGE_DEBUG::__data_size, getNumber());
            g_ppComboBox[N_IMAGE_DEBUG::CB_TYPE] = createComboBox(ui->tableWidget, XPE::getDebugTypesS(), SPE::TYPE_DEBUG, N_IMAGE_DEBUG::Type, XComboBoxEx::CBTYPE_LIST);

            blockSignals(true);

            XPE_DEF::S_IMAGE_DEBUG_DIRECTORY idd = pe.getDebugHeader(getNumber());

            g_ppLinedEdit[N_IMAGE_DEBUG::Characteristics]->setValue(idd.Characteristics);
            g_ppLinedEdit[N_IMAGE_DEBUG::TimeDateStamp]->setValue(idd.TimeDateStamp);
            g_ppLinedEdit[N_IMAGE_DEBUG::MajorVersion]->setValue(idd.MajorVersion);
            g_ppLinedEdit[N_IMAGE_DEBUG::MinorVersion]->setValue(idd.MinorVersion);
            g_ppLinedEdit[N_IMAGE_DEBUG::Type]->setValue(idd.Type);
            g_ppLinedEdit[N_IMAGE_DEBUG::SizeOfData]->setValue(idd.SizeOfData);
            g_ppLinedEdit[N_IMAGE_DEBUG::AddressOfRawData]->setValue(idd.AddressOfRawData);
            g_ppLinedEdit[N_IMAGE_DEBUG::PointerToRawData]->setValue(idd.PointerToRawData);

            g_ppComboBox[N_IMAGE_DEBUG::CB_TYPE]->setValue(idd.Type);

            qint64 nOffset = pe.getDebugHeaderOffset(getNumber());
            qint64 nSize = pe.getDebugHeaderSize();
            qint64 nAddress = pe.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &g_pSubDevice, ui->widgetHex);

            blockSignals(false);
        } else if (nType == SPE::TYPE_RELOCS) {
            createHeaderTable(SPE::TYPE_RELOCS, ui->tableWidget, N_IMAGE_RELOCS::records, g_ppLinedEdit, N_IMAGE_RELOCS::__data_size, getNumber());

            blockSignals(true);

            qint64 nOffset = getOffset();

            quint32 nVirtualAddress = pe.getRelocsVirtualAddress(nOffset);
            quint32 nSizeOfBlock = pe.getRelocsSizeOfBlock(nOffset);

            g_ppLinedEdit[N_IMAGE_RELOCS::VirtualAddress]->setValue(nVirtualAddress);
            g_ppLinedEdit[N_IMAGE_RELOCS::SizeOfBlock]->setValue(nSizeOfBlock);

            qint64 nSize = nSizeOfBlock;
            qint64 nAddress = pe.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &g_pSubDevice, ui->widgetHex);

            blockSignals(false);
        } else if (nType == SPE::TYPE_EXCEPTION) {
            createHeaderTable(SPE::TYPE_EXCEPTION, ui->tableWidget, N_IMAGE_EXCEPTIONS::records, g_ppLinedEdit, N_IMAGE_EXCEPTIONS::__data_size, getNumber());

            blockSignals(true);

            qint64 nOffset = pe.getExceptionRecordOffset(getNumber());

            XPE_DEF::S_IMAGE_RUNTIME_FUNCTION_ENTRY exception = pe._read_IMAGE_RUNTIME_FUNCTION_ENTRY(nOffset);

            g_ppLinedEdit[N_IMAGE_EXCEPTIONS::BeginAddress]->setValue(exception.BeginAddress);
            g_ppLinedEdit[N_IMAGE_EXCEPTIONS::EndAddress]->setValue(exception.EndAddress);
            g_ppLinedEdit[N_IMAGE_EXCEPTIONS::UnwindInfoAddress]->setValue(exception.UnwindInfoAddress);

            qint64 nSize = pe.getExceptionRecordSize();
            qint64 nAddress = pe.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &g_pSubDevice, ui->widgetHex);

            blockSignals(false);
        } else if (nType == SPE::TYPE_DELAYIMPORT) {
            createHeaderTable(SPE::TYPE_DELAYIMPORT, ui->tableWidget, N_IMAGE_DELAYIMPORT::records, g_ppLinedEdit, N_IMAGE_DELAYIMPORT::__data_size, getNumber());

            blockSignals(true);

            qint64 nOffset = pe.getDelayImportRecordOffset(getNumber());

            XPE_DEF::S_IMAGE_DELAYLOAD_DESCRIPTOR delayImport = pe._read_IMAGE_DELAYLOAD_DESCRIPTOR(nOffset);

            g_ppLinedEdit[N_IMAGE_DELAYIMPORT::AllAttributes]->setValue(delayImport.AllAttributes);
            g_ppLinedEdit[N_IMAGE_DELAYIMPORT::DllNameRVA]->setValue(delayImport.DllNameRVA);
            g_ppLinedEdit[N_IMAGE_DELAYIMPORT::ModuleHandleRVA]->setValue(delayImport.ModuleHandleRVA);
            g_ppLinedEdit[N_IMAGE_DELAYIMPORT::ImportAddressTableRVA]->setValue(delayImport.ImportAddressTableRVA);
            g_ppLinedEdit[N_IMAGE_DELAYIMPORT::ImportNameTableRVA]->setValue(delayImport.ImportNameTableRVA);
            g_ppLinedEdit[N_IMAGE_DELAYIMPORT::BoundImportAddressTableRVA]->setValue(delayImport.BoundImportAddressTableRVA);
            g_ppLinedEdit[N_IMAGE_DELAYIMPORT::UnloadInformationTableRVA]->setValue(delayImport.UnloadInformationTableRVA);
            g_ppLinedEdit[N_IMAGE_DELAYIMPORT::TimeDateStamp]->setValue(delayImport.TimeDateStamp);

            qint64 nSize = pe.getDelayImportRecordSize();
            qint64 nAddress = pe.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &g_pSubDevice, ui->widgetHex);

            blockSignals(false);
        } else if (nType == SPE::TYPE_BOUNDIMPORT) {
            createHeaderTable(SPE::TYPE_BOUNDIMPORT, ui->tableWidget, N_IMAGE_BOUNDIMPORT::records, g_ppLinedEdit, N_IMAGE_BOUNDIMPORT::__data_size, getNumber());

            blockSignals(true);

            qint64 nOffset = pe.getBoundImportRecordOffset(getNumber());

            XPE_DEF::IMAGE_BOUND_IMPORT_DESCRIPTOR boundImport = pe._read_IMAGE_BOUND_IMPORT_DESCRIPTOR(nOffset);

            g_ppLinedEdit[N_IMAGE_BOUNDIMPORT::TimeDateStamp]->setValue(boundImport.TimeDateStamp);
            g_ppLinedEdit[N_IMAGE_BOUNDIMPORT::OffsetModuleName]->setValue(boundImport.OffsetModuleName);
            g_ppLinedEdit[N_IMAGE_BOUNDIMPORT::NumberOfModuleForwarderRefs]->setValue(boundImport.NumberOfModuleForwarderRefs);

            qint64 nSize = pe.getBoundImportRecordSize();
            qint64 nAddress = pe.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &g_pSubDevice, ui->widgetHex);

            blockSignals(false);
        } else if (nType == SPE::TYPE_IMAGE_DIRECTORY_ENTRIES) {
            createHeaderTable(SPE::TYPE_IMAGE_DIRECTORY_ENTRIES, ui->tableWidget, N_IMAGE_DATA_DIRECTORY::records, g_ppLinedEdit, N_IMAGE_DATA_DIRECTORY::__data_size,
                              getNumber());

            blockSignals(true);

            qint64 nOffset = pe.getDataDirectoryHeaderOffset(getNumber());

            XPE_DEF::IMAGE_DATA_DIRECTORY dataDirectory = pe.read_IMAGE_DATA_DIRECTORY(nOffset);

            g_ppLinedEdit[N_IMAGE_DATA_DIRECTORY::Address]->setValue(dataDirectory.VirtualAddress);
            g_ppLinedEdit[N_IMAGE_DATA_DIRECTORY::Size]->setValue(dataDirectory.Size);

            qint64 nSize = pe.getDataDirectoryHeaderSize();
            qint64 nAddress = pe.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset, nSize, nAddress, &g_pSubDevice, ui->widgetHex);

            blockSignals(false);
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void PESectionHeaderWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget = qobject_cast<QWidget *>(sender());
    int nStype = pWidget->property("STYPE").toInt();
    int nNdata = pWidget->property("NDATA").toInt();

    switch (nStype) {
        case SPE::TYPE_IMAGE_SECTION_HEADER:
            switch (nNdata) {
                case N_IMAGE_SECTION_HEADER::Characteristics:
                case N_IMAGE_SECTION_HEADER::Characteristics + 1:
                    g_ppLinedEdit[N_IMAGE_SECTION_HEADER::Characteristics]->setValue((quint32)nValue);
                    g_ppComboBox[N_IMAGE_SECTION_HEADER::CB_CHARACTERISTICS]->setValue(nValue);
                    g_ppComboBox[N_IMAGE_SECTION_HEADER::CB_ALIGH]->setValue(nValue);
                    break;
            }
            break;

        case SPE::TYPE_DEBUG:
            switch (nNdata) {
                case N_IMAGE_DEBUG::Type:
                    g_ppLinedEdit[N_IMAGE_DEBUG::Type]->setValue((quint32)nValue);
                    break;
            }
            break;
    }
}

void PESectionHeaderWidget::on_tableWidget_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow)
    Q_UNUSED(nCurrentColumn)
    Q_UNUSED(nPreviousRow)
    Q_UNUSED(nPreviousColumn)

    setHeaderTableSelection(ui->widgetHex, ui->tableWidget);
}
