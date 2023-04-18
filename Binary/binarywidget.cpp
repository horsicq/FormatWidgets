/* Copyright (c) 2017-2023 hors<horsicq@gmail.com>
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

BinaryWidget::BinaryWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, QWidget *pParent) : BinaryWidget(pParent)
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
    // TODO Hex
    BinaryWidget::clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XBinary binary(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (binary.isValid()) {
        setFileType(binary.getFileType());

        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_INFO, tr("Info")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_VISUALIZATION, tr("Visualization")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_VIRUSTOTAL, "VirusTotal"));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_HEX, tr("Hex")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_DISASM, tr("Disasm")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_HASH, tr("Hash")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_STRINGS, tr("Strings")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_SIGNATURES, tr("Signatures")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_MEMORYMAP, tr("Memory map")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_ENTROPY, tr("Entropy")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_HEURISTICSCAN, tr("Heuristic scan")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_EXTRACTOR, tr("Extractor")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SBINARY::TYPE_SEARCH, tr("Search")));

        ui->treeWidgetNavi->expandAll();

        setTreeItem(ui->treeWidgetNavi, getOptions().nStartType);

        setDisasmInitAddress(binary.getEntryPointAddress());
    }
}

FormatWidget::SV BinaryWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
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

void BinaryWidget::adjustHeaderTable(int nType, QTableWidget *pTableWidget)
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

void BinaryWidget::_findValue(quint64 nValue, bool bIsBigEndian)
{
    setTreeItem(ui->treeWidgetNavi, SBINARY::TYPE_SEARCH);
    ui->widgetSearch->findValue(nValue, bIsBigEndian);
}

void BinaryWidget::reloadData()
{
    qint32 nType = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::SECTION_DATA_TYPE).toInt();

    QString sInit = getInitString(ui->treeWidgetNavi->currentItem());

    ui->stackedWidgetInfo->setCurrentIndex(nType);

    XBinary binary(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    QSet<XBinary::FT> stFT = binary.getFileTypes(true);
    XBinary::FT fileType = binary._getPrefFileType(&stFT);

    if (binary.isValid()) {
        if (nType == SBINARY::TYPE_INFO) {
            if (!isInitPresent(sInit)) {
                ui->widgetInfo->setData(getDevice(), fileType, "Info", true);
            }
        } else if (nType == SBINARY::TYPE_VISUALIZATION) {
            if (!isInitPresent(sInit)) {
                ui->widgetVisualization->setData(getDevice(), fileType, true);
            }
        } else if (nType == SBINARY::TYPE_VIRUSTOTAL) {
            if (!isInitPresent(sInit)) {
                ui->widgetVirusTotal->setData(getDevice());
            }
        } else if (nType == SBINARY::TYPE_HEX) {
            if (!isInitPresent(sInit)) {
                XHexView::OPTIONS options = {};
                options.bMenu_Disasm = true;
                options.bMenu_MemoryMap = true;

                ui->widgetHex->setData(getDevice(), options, getXInfoDB());
                ui->widgetHex->setBackupDevice(getBackupDevice());
                //                ui->widgetHex->setBackupFileName(getOptions().sBackupFileName);
                //                ui->widgetHex->enableReadOnly(false);

                ui->widgetHex->reload();
            }
        } else if (nType == SBINARY::TYPE_DISASM) {
            if (!isInitPresent(sInit)) {
                XMultiDisasmWidget::OPTIONS options = {};
                options.fileType = fileType;
                options.nInitAddress = getDisasmInitAddress();
                options.bMenu_Hex = true;

                ui->widgetDisasm->setData(getDevice(), options, getXInfoDB());
                ui->widgetDisasm->setBackupDevice(getBackupDevice());

                setDisasmInitAddress(-1);
            }
        } else if (nType == SBINARY::TYPE_HASH) {
            if (!isInitPresent(sInit)) {
                ui->widgetHash->setData(getDevice(), fileType, 0, -1, true);
            }
        } else if (nType == SBINARY::TYPE_STRINGS) {
            if (!isInitPresent(sInit)) {
                SearchStringsWidget::OPTIONS stringsOptions = {};
                stringsOptions.bMenu_Hex = true;
                stringsOptions.bMenu_Demangle = true;
                stringsOptions.bAnsi = true;
                stringsOptions.bUTF8 = false;
                stringsOptions.bUnicode = true;
                stringsOptions.bCStrings = false;

                ui->widgetStrings->setData(getDevice(), stringsOptions, true);
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
                options.fileType = fileType;
                options.bIsSearchEnable = true;

                ui->widgetMemoryMap->setData(getDevice(), options, getXInfoDB());
            }
        } else if (nType == SBINARY::TYPE_ENTROPY) {
            if (!isInitPresent(sInit)) {
                ui->widgetEntropy->setData(getDevice(), 0, getDevice()->size(), fileType, true);
            }
        } else if (nType == SBINARY::TYPE_HEURISTICSCAN) {
            if (!isInitPresent(sInit)) {
                ui->widgetHeuristicScan->setData(getDevice(), true, fileType);
            }
        } else if (nType == SBINARY::TYPE_EXTRACTOR) {
            if (!isInitPresent(sInit)) {
                XExtractor::OPTIONS extractorOptions = XExtractor::getDefaultOptions();
                extractorOptions.fileType = fileType;
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

void BinaryWidget::widgetValueChanged(quint64 nValue)
{
    Q_UNUSED(nValue)
    //    QWidget *pWidget=qobject_cast<QWidget *>(sender());
    //    int nStype=pWidget->property("STYPE").toInt();
    //    int nNdata=pWidget->property("NDATA").toInt();
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

void BinaryWidget::on_pushButtonHex_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SBINARY::TYPE_HEX);
}

void BinaryWidget::on_pushButtonDisasm_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SBINARY::TYPE_DISASM);
}

void BinaryWidget::on_pushButtonStrings_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SBINARY::TYPE_STRINGS);
}

void BinaryWidget::on_pushButtonEntropy_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SBINARY::TYPE_ENTROPY);
}

void BinaryWidget::on_pushButtonHeuristicScan_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SBINARY::TYPE_HEURISTICSCAN);
}

void BinaryWidget::on_pushButtonMemoryMap_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SBINARY::TYPE_MEMORYMAP);
}

void BinaryWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pItemCurrent, QTreeWidgetItem *pItemPrevious)
{
    Q_UNUSED(pItemPrevious)

    if (pItemCurrent) {
        reloadData();
        addPage(pItemCurrent);
        ui->toolButtonPrev->setEnabled(isPrevPageAvailable());
        ui->toolButtonNext->setEnabled(isNextPageAvailable());
    }
}
