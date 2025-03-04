/* Copyright (c) 2024 hors<horsicq@gmail.com>
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
#include "xmainwidgetadvanced.h"

XMainWidgetAdvanced::XMainWidgetAdvanced(QWidget *pParent) : XMainWidget(pParent)
{
    setGlobalHexEnable(true);
}

QTreeWidgetItem *XMainWidgetAdvanced::_addBaseItems(QTreeWidget *pTreeWidget, XBinary::FT fileType)
{
    QTreeWidgetItem *pResult = XMainWidget::_addBaseItems(pTreeWidget, fileType);

    pResult->addChild(createNewItem(XFW_DEF::TYPE_NFDSCAN, XFW_DEF::WIDGETMODE_UNKNOWN, XOptions::ICONTYPE_NFD, 0, -1, 0, 0, 0, XBinary::MODE_UNKNOWN,
                                    XBinary::ENDIAN_UNKNOWN, "", ""));
    pResult->addChild(createNewItem(XFW_DEF::TYPE_DIESCAN, XFW_DEF::WIDGETMODE_UNKNOWN, XOptions::ICONTYPE_DIE, 0, -1, 0, 0, 0, XBinary::MODE_UNKNOWN,
                                    XBinary::ENDIAN_UNKNOWN, "", ""));
#ifdef USE_YARA
    pResult->addChild(createNewItem(XFW_DEF::TYPE_YARASCAN, XFW_DEF::WIDGETMODE_UNKNOWN, XOptions::ICONTYPE_YARA, 0, -1, 0, 0, 0, XBinary::MODE_UNKNOWN,
                                    XBinary::ENDIAN_UNKNOWN, "", ""));
#endif
    pResult->addChild(createNewItem(XFW_DEF::TYPE_VIRUSTOTAL, XFW_DEF::WIDGETMODE_UNKNOWN, XOptions::ICONTYPE_VIRUSTOTAL, 0, -1, 0, 0, 0, XBinary::MODE_UNKNOWN,
                                    XBinary::ENDIAN_UNKNOWN, "", ""));
    pResult->addChild(createNewItem(XFW_DEF::TYPE_VISUALIZATION, XFW_DEF::WIDGETMODE_UNKNOWN, XOptions::ICONTYPE_VISUALIZATION, 0, -1, 0, 0, 0, XBinary::MODE_UNKNOWN,
                                    XBinary::ENDIAN_UNKNOWN, "", ""));
    pResult->addChild(
        createNewItem(XFW_DEF::TYPE_HEX, XFW_DEF::WIDGETMODE_UNKNOWN, XOptions::ICONTYPE_HEX, 0, -1, 0, 0, 0, XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN, "", ""));

    if (fileType != XBinary::FT_DEX) {
        pResult->addChild(createNewItem(XFW_DEF::TYPE_DISASM, XFW_DEF::WIDGETMODE_UNKNOWN, XOptions::ICONTYPE_DISASM, 0, -1, 0, 0, 0, XBinary::MODE_UNKNOWN,
                                        XBinary::ENDIAN_UNKNOWN, "", ""));
    }

    pResult->addChild(
        createNewItem(XFW_DEF::TYPE_HASH, XFW_DEF::WIDGETMODE_UNKNOWN, XOptions::ICONTYPE_HASH, 0, -1, 0, 0, 0, XBinary::MODE_UNKNOWN, XBinary::ENDIAN_UNKNOWN, "", ""));
    pResult->addChild(createNewItem(XFW_DEF::TYPE_STRINGS, XFW_DEF::WIDGETMODE_UNKNOWN, XOptions::ICONTYPE_STRING, 0, -1, 0, 0, 0, XBinary::MODE_UNKNOWN,
                                    XBinary::ENDIAN_UNKNOWN, "", ""));
    pResult->addChild(createNewItem(XFW_DEF::TYPE_SIGNATURES, XFW_DEF::WIDGETMODE_UNKNOWN, XOptions::ICONTYPE_SIGNATURE, 0, -1, 0, 0, 0, XBinary::MODE_UNKNOWN,
                                    XBinary::ENDIAN_UNKNOWN, "", ""));
    pResult->addChild(createNewItem(XFW_DEF::TYPE_REGIONS, XFW_DEF::WIDGETMODE_UNKNOWN, XOptions::ICONTYPE_SEGMENT, 0, -1, 0, 0, 0, XBinary::MODE_UNKNOWN,
                                    XBinary::ENDIAN_UNKNOWN, "", ""));
    pResult->addChild(createNewItem(XFW_DEF::TYPE_MEMORYMAP, XFW_DEF::WIDGETMODE_UNKNOWN, XOptions::ICONTYPE_MEMORYMAP, 0, -1, 0, 0, 0, XBinary::MODE_UNKNOWN,
                                    XBinary::ENDIAN_UNKNOWN, "", ""));
    pResult->addChild(createNewItem(XFW_DEF::TYPE_SYMBOLS, XFW_DEF::WIDGETMODE_UNKNOWN, XOptions::ICONTYPE_SYMBOL, 0, -1, 0, 0, 0, XBinary::MODE_UNKNOWN,
                                    XBinary::ENDIAN_UNKNOWN, "", ""));
    pResult->addChild(createNewItem(XFW_DEF::TYPE_ENTROPY, XFW_DEF::WIDGETMODE_UNKNOWN, XOptions::ICONTYPE_ENTROPY, 0, -1, 0, 0, 0, XBinary::MODE_UNKNOWN,
                                    XBinary::ENDIAN_UNKNOWN, "", ""));
    pResult->addChild(createNewItem(XFW_DEF::TYPE_EXTRACTOR, XFW_DEF::WIDGETMODE_UNKNOWN, XOptions::ICONTYPE_EXTRACTOR, 0, -1, 0, 0, 0, XBinary::MODE_UNKNOWN,
                                    XBinary::ENDIAN_UNKNOWN, "", ""));
    pResult->addChild(createNewItem(XFW_DEF::TYPE_SEARCH, XFW_DEF::WIDGETMODE_UNKNOWN, XOptions::ICONTYPE_SEARCH, 0, -1, 0, 0, 0, XBinary::MODE_UNKNOWN,
                                    XBinary::ENDIAN_UNKNOWN, "", ""));

    return pResult;
}

XShortcutsWidget *XMainWidgetAdvanced::createWidget(const XFW_DEF::CWOPTIONS &cwOptions)
{
    XShortcutsWidget *pResult = nullptr;

    if (cwOptions._type == XFW_DEF::TYPE_INFO) {
        XFileInfoWidget *_pWidget = new XFileInfoWidget(cwOptions.pParent);
        _pWidget->setData(cwOptions.pDevice, cwOptions.fileType, "Info", false);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_NFDSCAN) {
        NFDWidgetAdvanced *_pWidget = new NFDWidgetAdvanced(cwOptions.pParent);
        _pWidget->setData(cwOptions.pDevice, false, cwOptions.fileType);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_DIESCAN) {
        DIEWidgetAdvanced *_pWidget = new DIEWidgetAdvanced(cwOptions.pParent);
        _pWidget->setData(cwOptions.pDevice, false, cwOptions.fileType);
        pResult = _pWidget;
    }
#ifdef USE_YARA
    else if (cwOptions._type == XFW_DEF::TYPE_YARASCAN) {
        YARAWidgetAdvanced *_pWidget = new YARAWidgetAdvanced(cwOptions.pParent);
        _pWidget->setData(XBinary::getDeviceFileName(cwOptions.pDevice), false);
        pResult = _pWidget;
#endif
    } else if (cwOptions._type == XFW_DEF::TYPE_VIRUSTOTAL) {
        XVirusTotalWidget *_pWidget = new XVirusTotalWidget(cwOptions.pParent);
        _pWidget->setData(cwOptions.pDevice);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_VISUALIZATION) {
        XVisualizationWidget *_pWidget = new XVisualizationWidget(cwOptions.pParent);
        _pWidget->setData(cwOptions.pDevice, cwOptions.fileType, false);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_HEX) {
        XHexViewWidget *_pWidget = new XHexViewWidget(cwOptions.pParent);
        XHexViewWidget::OPTIONS options = {};
        options.fileType = cwOptions.fileType;
        options.bMenu_Disasm = true;
        options.bMenu_MemoryMap = true;
        _pWidget->setXInfoDB(cwOptions.pXInfoDB, cwOptions.profile);
        _pWidget->setData(cwOptions.pDevice, options);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_DISASM) {
        XMultiDisasmWidget *_pWidget = new XMultiDisasmWidget(cwOptions.pParent);
        XMultiDisasmWidget::OPTIONS options = {};
        options.fileType = cwOptions.fileType;
        options.nInitAddress = XFormats::getEntryPointAddress(cwOptions.fileType, cwOptions.pDevice, cwOptions.bIsImage, cwOptions.nImageBase);
        options.bMenu_Hex = true;
        _pWidget->setXInfoDB(cwOptions.pXInfoDB, cwOptions.profile);
        _pWidget->setData(cwOptions.pDevice, options);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_HASH) {
        XHashWidget *_pWidget = new XHashWidget(cwOptions.pParent);
        _pWidget->setData(cwOptions.pDevice, cwOptions.fileType, cwOptions.nDataOffset, cwOptions.nDataSize, false);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_STRINGS) {
        SearchStringsWidget *_pWidget = new SearchStringsWidget(cwOptions.pParent);
        SearchStringsWidget::OPTIONS stringsOptions = {};
        stringsOptions.bMenu_Hex = true;
        stringsOptions.bMenu_Disasm = true;
        stringsOptions.bMenu_Demangle = true;
        stringsOptions.bAnsi = true;
        stringsOptions.bUnicode = true;
        stringsOptions.bNullTerminated = false;
        _pWidget->setData(cwOptions.pDevice, cwOptions.fileType, stringsOptions, false);

        connect(_pWidget, SIGNAL(showDemangle(QString)), this, SLOT(showDemangleSlot(QString)));

        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_SIGNATURES) {
        SearchSignaturesWidget *_pWidget = new SearchSignaturesWidget(cwOptions.pParent);
        SearchSignaturesWidget::OPTIONS signaturesOptions = {};
        signaturesOptions.bMenu_Hex = true;
        signaturesOptions.bMenu_Disasm = true;
        signaturesOptions.fileType = cwOptions.fileType;
        _pWidget->setData(cwOptions.pDevice, signaturesOptions, false);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_REGIONS) {
        XRegionsWidget *_pWidget = new XRegionsWidget(cwOptions.pParent);
        _pWidget->setData(cwOptions.pDevice, cwOptions.fileType, cwOptions.pXInfoDB, cwOptions.profile);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_SYMBOLS) {
        XSymbolsWidget *_pWidget = new XSymbolsWidget(cwOptions.pParent);
        XSymbolsWidget::OPTIONS options = {};
        options.fileType = cwOptions.fileType;
        options.symbolMode = XInfoDB::SYMBOL_MODE_ALL;
        _pWidget->setData(cwOptions.pDevice, options, cwOptions.pXInfoDB, cwOptions.profile, true);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_MEMORYMAP) {
        XMemoryMapWidget *_pWidget = new XMemoryMapWidget(cwOptions.pParent);
        XMemoryMapWidget::OPTIONS options = {};
        options.fileType = cwOptions.fileType;
        options.bIsSearchEnable = true;
        _pWidget->setData(cwOptions.pDevice, options, cwOptions.pXInfoDB, cwOptions.profile);

        connect(_pWidget, SIGNAL(findValue(quint64, XBinary::ENDIAN)), this, SLOT(findValue(quint64, XBinary::ENDIAN)));

        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_ENTROPY) {
        XEntropyWidget *_pWidget = new XEntropyWidget(cwOptions.pParent);
        _pWidget->setData(cwOptions.pDevice, cwOptions.nDataOffset, cwOptions.nDataSize, cwOptions.fileType, false);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_EXTRACTOR) {
        XExtractorWidget *_pWidget = new XExtractorWidget(cwOptions.pParent);
        XExtractor::OPTIONS extractorOptions = XExtractor::getDefaultOptions();
        extractorOptions.fileType = cwOptions.fileType;
        extractorOptions.bMenu_Hex = true;
        _pWidget->setData(cwOptions.pDevice, extractorOptions, false);
        pResult = _pWidget;
    } else if (cwOptions._type == XFW_DEF::TYPE_SEARCH) {
        SearchValuesWidget *_pWidget = new SearchValuesWidget(cwOptions.pParent);
        SearchValuesWidget::OPTIONS options = {};
        options.fileType = cwOptions.fileType;
        options.bMenu_Hex = true;
        options.bMenu_Disasm = true;
        _pWidget->setData(cwOptions.pDevice, options);
        pResult = _pWidget;
    } else {
        pResult = XMainWidget::createWidget(cwOptions);
    }

    return pResult;
}

void XMainWidgetAdvanced::showDemangleSlot(const QString &sString)
{
    DialogDemangle dialogDemangle(this, sString);
    dialogDemangle.setGlobal(getShortcuts(), getGlobalOptions());

    dialogDemangle.exec();
}

void XMainWidgetAdvanced::findValue(quint64 nValue, XBinary::ENDIAN endian)
{
    setTreeItem(getTreeWidgetNavi(), XFW_DEF::TYPE_SEARCH);
    SearchValuesWidget *pWidget = dynamic_cast<SearchValuesWidget *>(getCurrentWidget());

    if (pWidget) {
        pWidget->findValue(nValue, endian);
    }
}

void XMainWidgetAdvanced::_followLocation(quint64 nLocation, qint32 nLocationType, qint64 nSize, qint32 nWidgetType)
{
    if (nWidgetType == XOptions::WIDGETTYPE_HEX) {
        if (isGlobalHexSyncEnabled()) {
            getGlobalHexView()->setLocation(nLocation, nLocationType, nSize);
        } else {
            setTreeItem(getTreeWidgetNavi(), XFW_DEF::TYPE_HEX);
            getCurrentWidget()->setLocation(nLocation, nLocationType, nSize);
        }
    } else if (nWidgetType == XOptions::WIDGETTYPE_DISASM) {
        setTreeItem(getTreeWidgetNavi(), XFW_DEF::TYPE_DISASM);
        getCurrentWidget()->setLocation(nLocation, nLocationType, nSize);
    } else if (nWidgetType == XOptions::WIDGETTYPE_MEMORYMAP) {
        setTreeItem(getTreeWidgetNavi(), XFW_DEF::TYPE_MEMORYMAP);
        getCurrentWidget()->setLocation(nLocation, nLocationType, nSize);
    } else {
        XMainWidget::_followLocation(nLocation, nLocationType, nSize, nWidgetType);
    }
}
