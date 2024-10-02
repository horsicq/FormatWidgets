/* Copyright (c) 2017-2024 hors<horsicq@gmail.com>
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
#include "binarywidget.h"

#include "ui_binarywidget.h"

BinaryWidget::BinaryWidget(QWidget *pParent) : FormatWidget(pParent), ui(new Ui::BinaryWidget)
{
    ui->setupUi(this);

    initWidget();
}

BinaryWidget::BinaryWidget(QIODevice *pDevice, const FW_DEF::OPTIONS &options, QWidget *pParent) : BinaryWidget(pParent)
{
    ui->setupUi(this);

    BinaryWidget::setData(pDevice, options, 0, 0, 0);
    BinaryWidget::reload();
}

BinaryWidget::~BinaryWidget()
{
    delete ui;
}

void BinaryWidget::clear()
{
    BinaryWidget::reset();

    resetWidget();

    ui->checkBoxReadonly->setChecked(true);

    ui->treeWidgetNavi->clear();
}

void BinaryWidget::cleanup()
{
}

void BinaryWidget::reload()
{
    BinaryWidget::clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XBinary binary(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (binary.isValid()) {
        QSet<XBinary::FT> stFT = XFormats::getFileTypes(getDevice(), true);
        setFileType(binary._getPrefFileType(&stFT));

        QTreeWidgetItem *pItemInfo = createNewItem(SBINARY::TYPE_INFO, tr("Info"), XOptions::ICONTYPE_INFO);
        ui->treeWidgetNavi->addTopLevelItem(pItemInfo);
        pItemInfo->addChild(createNewItem(SBINARY::TYPE_NFDSCAN, "Nauz File Detector (NFD)", XOptions::ICONTYPE_NFD));
        pItemInfo->addChild(createNewItem(SBINARY::TYPE_DIESCAN, "Detect It Easy (DiE)", XOptions::ICONTYPE_DIE));
#ifdef USE_YARA
        pItemInfo->addChild(createNewItem(SBINARY::TYPE_YARASCAN, "Yara rules", XOptions::ICONTYPE_YARA));
#endif
        pItemInfo->addChild(createNewItem(SBINARY::TYPE_VIRUSTOTAL, "VirusTotal", XOptions::ICONTYPE_VIRUSTOTAL));

        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_VISUALIZATION, tr("Visualization"), XOptions::ICONTYPE_VISUALIZATION));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_HEX, tr("Hex"), XOptions::ICONTYPE_HEX));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_DISASM, tr("Disasm"), XOptions::ICONTYPE_DISASM));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_HASH, tr("Hash"), XOptions::ICONTYPE_HASH));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_STRINGS, tr("Strings"), XOptions::ICONTYPE_STRING));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_SIGNATURES, tr("Signatures"), XOptions::ICONTYPE_SIGNATURE));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_MEMORYMAP, tr("Memory map"), XOptions::ICONTYPE_MEMORYMAP));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_ENTROPY, tr("Entropy"), XOptions::ICONTYPE_ENTROPY));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_EXTRACTOR, tr("Extractor"), XOptions::ICONTYPE_EXTRACTOR));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_SEARCH, tr("Search"), XOptions::ICONTYPE_SEARCH));

        ui->treeWidgetNavi->expandAll();

        setDisasmInitAddress(binary.getEntryPointAddress());

        setTreeItem(ui->treeWidgetNavi, getOptions().nStartType);
    }
}

FormatWidget::SV BinaryWidget::_setValue(QVariant vValue, qint32 nStype, qint32 nNdata, qint32 nVtype, qint32 nPosition, qint64 nOffset)
{
    Q_UNUSED(vValue)
    Q_UNUSED(nStype)
    Q_UNUSED(nNdata)
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)
    Q_UNUSED(nOffset)

    SV result = SV_NONE;

    blockSignals(true);

    //    quint64 nValue=vValue.toULongLong();

    if (getDevice()->isWritable()) {
    }

    blockSignals(false);

    return result;
}

void BinaryWidget::setReadonly(bool bState)
{
    if (ui->checkBoxReadonly->isChecked() != bState) {
        const bool bBlocked1 = ui->checkBoxReadonly->blockSignals(true);
        ui->checkBoxReadonly->setChecked(bState);
        ui->checkBoxReadonly->blockSignals(bBlocked1);
    }

    ui->widgetHex->setReadonly(bState);
    ui->widgetDisasm->setReadonly(bState);
    ui->widgetStrings->setReadonly(bState);
}

void BinaryWidget::blockSignals(bool bState)
{
    Q_UNUSED(bState)
}

void BinaryWidget::adjustHeaderTable(qint32 nType, QTableWidget *pTableWidget)
{
    Q_UNUSED(nType)
    Q_UNUSED(pTableWidget)
    //    qint32 nSymbolWidth=getSymbolWidth();
}

void BinaryWidget::_showInDisasmWindowAddress(XADDR nAddress)
{
    setTreeItem(ui->treeWidgetNavi, SBINARY::TYPE_DISASM);
    ui->widgetDisasm->goToAddress(nAddress);
}

void BinaryWidget::_showInDisasmWindowOffset(qint64 nOffset)
{
    setTreeItem(ui->treeWidgetNavi, SBINARY::TYPE_DISASM);
    ui->widgetDisasm->goToOffset(nOffset);
}

void BinaryWidget::_showInMemoryMapWindowOffset(qint64 nOffset)
{
    setTreeItem(ui->treeWidgetNavi, SBINARY::TYPE_MEMORYMAP);
    ui->widgetMemoryMap->goToOffset(nOffset);
}

void BinaryWidget::_showInHexWindow(qint64 nOffset, qint64 nSize)
{
    setTreeItem(ui->treeWidgetNavi, SBINARY::TYPE_HEX);
    ui->widgetHex->setSelection(nOffset, nSize);
}

void BinaryWidget::_findValue(quint64 nValue, XBinary::ENDIAN endian)
{
    setTreeItem(ui->treeWidgetNavi, SBINARY::TYPE_SEARCH);
    ui->widgetSearch->findValue(nValue, endian);
}

void BinaryWidget::reloadData(bool bSaveSelection)
{
    qint32 nType = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::SECTION_DATA_TYPE).toInt();

    QString sInit = getInitString(ui->treeWidgetNavi->currentItem());

    ui->stackedWidgetInfo->setCurrentIndex(nType);

    XBinary binary(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (binary.isValid()) {
        if (nType == SBINARY::TYPE_INFO) {
            if (!isInitPresent(sInit)) {
                ui->widgetInfo->setData(getDevice(), getFileType(), "Info", true);
            }
        } else if (nType == SBINARY::TYPE_VISUALIZATION) {
            if (!isInitPresent(sInit)) {
                ui->widgetVisualization->setData(getDevice(), getFileType(), true);
            }
        } else if (nType == SBINARY::TYPE_VIRUSTOTAL) {
            if (!isInitPresent(sInit)) {
                ui->widgetVirusTotal->setData(getDevice());
            }
        } else if (nType == SBINARY::TYPE_HEX) {
            if (!isInitPresent(sInit)) {
                XHexViewWidget::OPTIONS options = {};
                options.bMenu_Disasm = true;
                options.bMenu_MemoryMap = true;
                options.fileType = getFileType();

                if (bSaveSelection) {
                    options.nStartSelectionOffset = -1;
                }

                ui->widgetHex->setXInfoDB(getXInfoDB());
                ui->widgetHex->setData(getDevice(), options);
                ui->widgetHex->setBackupDevice(getBackupDevice());
                //                ui->widgetHex->setBackupFileName(getOptions().sBackupFileName);
                //                ui->widgetHex->enableReadOnly(false);

                ui->widgetHex->reload();
            }
        } else if (nType == SBINARY::TYPE_DISASM) {
            if (!isInitPresent(sInit)) {
                XMultiDisasmWidget::OPTIONS options = {};
                options.fileType = getFileType();
                options.nInitAddress = getDisasmInitAddress();
                options.bMenu_Hex = true;
                ui->widgetDisasm->setXInfoDB(getXInfoDB());
                ui->widgetDisasm->setData(getDevice(), options);
                ui->widgetDisasm->setBackupDevice(getBackupDevice());

                setDisasmInitAddress(-1);
            }
        } else if (nType == SBINARY::TYPE_HASH) {
            if (!isInitPresent(sInit)) {
                ui->widgetHash->setData(getDevice(), getFileType(), 0, -1, true);
            }
        } else if (nType == SBINARY::TYPE_STRINGS) {
            if (!isInitPresent(sInit)) {
                SearchStringsWidget::OPTIONS stringsOptions = {};
                stringsOptions.bMenu_Hex = true;
                stringsOptions.bMenu_Demangle = true;
                stringsOptions.bAnsi = true;
                // stringsOptions.bUTF8 = false;
                stringsOptions.bUnicode = true;
                stringsOptions.bNullTerminated = false;

                ui->widgetStrings->setData(getDevice(), getFileType(), stringsOptions, true);
            }
        } else if (nType == SBINARY::TYPE_SIGNATURES) {
            if (!isInitPresent(sInit)) {
                SearchSignaturesWidget::OPTIONS signaturesOptions = {};
                signaturesOptions.bMenu_Hex = true;

                ui->widgetSignatures->setData(getDevice(), binary.getFileType(), signaturesOptions, false);
            }
        } else if (nType == SBINARY::TYPE_MEMORYMAP) {
            if (!isInitPresent(sInit)) {
                XMemoryMapWidget::OPTIONS options = {};
                options.fileType = getFileType();
                options.bIsSearchEnable = true;

                ui->widgetMemoryMap->setData(getDevice(), options, getXInfoDB());
            }
        } else if (nType == SBINARY::TYPE_ENTROPY) {
            if (!isInitPresent(sInit)) {
                ui->widgetEntropy->setData(getDevice(), 0, getDevice()->size(), getFileType(), true);
            }
        } else if (nType == SBINARY::TYPE_NFDSCAN) {
            if (!isInitPresent(sInit)) {
                ui->widgetHeuristicScan->setData(getDevice(), true, getFileType());
            }
        } else if (nType == SBINARY::TYPE_DIESCAN) {
            if (!isInitPresent(sInit)) {
                ui->widgetDIEScan->setData(getDevice(), true, getFileType());
            }
#ifdef USE_YARA
        } else if (nType == SBINARY::TYPE_YARASCAN) {
            if (!isInitPresent(sInit)) {
                ui->widgetYARAScan->setData(XBinary::getDeviceFileName(getDevice()), true);
            }
#endif
        } else if (nType == SBINARY::TYPE_EXTRACTOR) {
            if (!isInitPresent(sInit)) {
                XExtractor::OPTIONS extractorOptions = XExtractor::getDefaultOptions();
                extractorOptions.fileType = getFileType();
                extractorOptions.bMenu_Hex = true;

                ui->widgetExtractor->setData(getDevice(), extractorOptions, true);
            }
        } else if (nType == SBINARY::TYPE_SEARCH) {
            if (!isInitPresent(sInit)) {
                SearchValuesWidget::OPTIONS options = {};
                options.fileType = binary.getFileType();
                options.bMenu_Hex = true;
                options.bMenu_Disasm = true;

                ui->widgetSearch->setData(getDevice(), options);
            }
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }

    addInit(sInit);
}

void BinaryWidget::_widgetValueChanged(QVariant vValue)
{
    Q_UNUSED(vValue)
    //    QWidget *pWidget=qobject_cast<QWidget *>(sender());
    //    qint32 nStype=pWidget->property("STYPE").toInt();
    //    qint32 nNdata=pWidget->property("NDATA").toInt();
}

void BinaryWidget::on_checkBoxReadonly_toggled(bool bChecked)
{
    setReadonly(bChecked);
}

void BinaryWidget::on_pushButtonReload_clicked()
{
    ui->pushButtonReload->setEnabled(false);
    reload();

    QTimer::singleShot(1000, this, SLOT(enableButton()));
}

void BinaryWidget::enableButton()
{
    ui->pushButtonReload->setEnabled(true);
}

void BinaryWidget::on_toolButtonPrev_clicked()
{
    setAddPageEnabled(false);
    ui->treeWidgetNavi->setCurrentItem(getPrevPage());
    setAddPageEnabled(true);
}

void BinaryWidget::on_toolButtonNext_clicked()
{
    setAddPageEnabled(false);
    ui->treeWidgetNavi->setCurrentItem(getNextPage());
    setAddPageEnabled(true);
}

void BinaryWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pItemCurrent, QTreeWidgetItem *pItemPrevious)
{
    Q_UNUSED(pItemPrevious)

    if (pItemCurrent) {
        reloadData(false);
        addPage(pItemCurrent);
        ui->toolButtonPrev->setEnabled(isPrevPageAvailable());
        ui->toolButtonNext->setEnabled(isNextPageAvailable());
    }
}
