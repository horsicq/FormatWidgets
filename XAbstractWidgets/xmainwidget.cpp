/* Copyright (c) 2025 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "xmainwidget.h"

XMainWidget::XMainWidget(QWidget *pParent) : XStructWidget(pParent)
{
}

XMainWidget::~XMainWidget()
{
}

XShortcutsWidget *XMainWidget::createWidget(const QString &sGUID)
{
    XShortcutsWidget *pResult = nullptr;

    XBinary::DATA_HEADER dataHeader = searchDataHeaderByGuid(sGUID);

    if (dataHeader.dsID.sGUID == sGUID) {
        if (dataHeader.dsID.nID == 0) {
            XFileInfoWidget *_pWidget = new XFileInfoWidget(this);
            _pWidget->setData(getDevice(), dataHeader.dsID.fileType, dataHeader.sName, false);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_NFDSCAN)) {
            NFDWidgetAdvanced *_pWidget = new NFDWidgetAdvanced(this);
            _pWidget->setData(getDevice(), false, dataHeader.dsID_parent.fileType);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_DIESCAN)) {
            DIEWidgetAdvanced *_pWidget = new DIEWidgetAdvanced(this);
            _pWidget->setData(getDevice(), false, dataHeader.dsID_parent.fileType);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_YARASCAN)) {
            YARAWidgetAdvanced *_pWidget = new YARAWidgetAdvanced(this);
            _pWidget->setData(XBinary::getDeviceFileName(getDevice()), false);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_VIRUSTOTALSCAN)) {
            XVirusTotalWidget *_pWidget = new XVirusTotalWidget(this);
            _pWidget->setData(getDevice());
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_VISUALIZATION)) {
            XVisualizationWidget *_pWidget = new XVisualizationWidget(this);
            _pWidget->setData(getDevice(), dataHeader.dsID_parent.fileType, false);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_HEX)) {
            XHexViewWidget *_pWidget = new XHexViewWidget(this);
            XHexViewWidget::OPTIONS options = {};
            options.fileType = dataHeader.dsID_parent.fileType;
            options.bMenu_Disasm = true;
            options.bMenu_MemoryMap = true;
            _pWidget->setXInfoDB(getInfoDB());
            _pWidget->setData(getDevice(), options);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_DISASM)) {
            XMultiDisasmWidget *_pWidget = new XMultiDisasmWidget(this);
            XMultiDisasmWidget::OPTIONS options = {};
            options.fileType = dataHeader.dsID_parent.fileType;
            options.nInitAddress = XFormats::getEntryPointAddress(dataHeader.dsID_parent.fileType, getDevice(), getOptions()->bIsImage, getOptions()->nImageBase);
            options.bMenu_Hex = true;
            _pWidget->setXInfoDB(getInfoDB());
            _pWidget->setData(getDevice(), options);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_HASH)) {
            XHashWidget *_pWidget = new XHashWidget(this);
            _pWidget->setData(getDevice(), dataHeader.dsID_parent.fileType, 0, getDevice()->size(), false);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_STRINGS)) {
            SearchStringsWidget *_pWidget = new SearchStringsWidget(this);
            SearchStringsWidget::OPTIONS stringsOptions = {};
            stringsOptions.bMenu_Hex = true;
            stringsOptions.bMenu_Disasm = true;
            stringsOptions.bMenu_Demangle = true;
            stringsOptions.bAnsi = true;
            stringsOptions.bUnicode = true;
            stringsOptions.bNullTerminated = false;
            _pWidget->setData(getDevice(), dataHeader.dsID_parent.fileType, stringsOptions, false);

            connect(_pWidget, SIGNAL(showDemangle(QString)), this, SLOT(showDemangleSlot(QString)));

            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_SIGNATURES)) {
            SearchSignaturesWidget *_pWidget = new SearchSignaturesWidget(this);
            SearchSignaturesWidget::OPTIONS signaturesOptions = {};
            signaturesOptions.bMenu_Hex = true;
            signaturesOptions.bMenu_Disasm = true;
            signaturesOptions.fileType = dataHeader.dsID_parent.fileType;
            _pWidget->setData(getDevice(), signaturesOptions, false);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_REGIONS)) {
            XRegionsWidget *_pWidget = new XRegionsWidget(this);
            XRegionsModel::OPTIONS options = {};
            options.fileType = dataHeader.dsID_parent.fileType;
            options.bIsImage = getOptions()->bIsImage;
            options.nModuleAddress = getOptions()->nImageBase;
            _pWidget->setData(getDevice(), getInfoDB(), options);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_MEMORYMAP)) {
            XMemoryMapWidget *_pWidget = new XMemoryMapWidget(this);
            XMemoryMapWidget::OPTIONS options = {};
            options.fileType = dataHeader.dsID_parent.fileType;
            options.bIsSearchEnable = true;
            _pWidget->setData(getDevice(), options, getInfoDB());

            connect(_pWidget, SIGNAL(findValue(quint64, XBinary::ENDIAN)), this, SLOT(findValue(quint64, XBinary::ENDIAN)));

            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_SYMBOLS)) {
            XSymbolsWidget *_pWidget = new XSymbolsWidget(this);
            XSymbolsWidget::OPTIONS options = {};
            options.fileType = dataHeader.dsID_parent.fileType;
            options.symbolMode = XInfoDB::SYMBOL_MODE_ALL;
            options.bMenu_Disasm = true;
            options.bMenu_Hex = true;
            _pWidget->setData(getDevice(), options, getInfoDB(), true);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_ENTROPY)) {
            XEntropyWidget *_pWidget = new XEntropyWidget(this);
            _pWidget->setData(getDevice(), 0, getDevice()->size(), dataHeader.dsID_parent.fileType, false);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_EXTRACTOR)) {
            XExtractorWidget *_pWidget = new XExtractorWidget(this);
            XExtractor::OPTIONS extractorOptions = XExtractor::getDefaultOptions();
            extractorOptions.fileType = dataHeader.dsID_parent.fileType;
            extractorOptions.bMenu_Hex = true;
            _pWidget->setData(getDevice(), extractorOptions, false);
            pResult = _pWidget;
        } else if ((dataHeader.dsID.fileType == XBinary::FT_BINARY) && (dataHeader.dsID.nID == XBinary::STRUCTID_SEARCH)) {
            SearchValuesWidget *_pWidget = new SearchValuesWidget(this);
            SearchValuesWidget::OPTIONS options = {};
            options.fileType = dataHeader.dsID_parent.fileType;
            options.bMenu_Hex = true;
            options.bMenu_Disasm = true;
            _pWidget->setData(getDevice(), options);
            pResult = _pWidget;
        } else {
            XShortcutsWidget *_pWidget = XStructWidget::createWidget(sGUID);
            pResult = _pWidget;
        }
    }

    return pResult;
}
