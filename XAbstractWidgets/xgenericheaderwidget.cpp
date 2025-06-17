/* Copyright (c) 2025 hors<horsicq@gmail.com>
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
#include "xgenericheaderwidget.h"
#include "ui_xgenericheaderwidget.h"

XGenericHeaderWidget::XGenericHeaderWidget(QWidget *parent) : XGenericAbstractWidget(parent), ui(new Ui::XGenericHeaderWidget)
{
    ui->setupUi(this);

    g_nDataSize = 0;

    XOptions::adjustToolButton(ui->toolButtonTableReload, XOptions::ICONTYPE_RELOAD);
    XOptions::adjustToolButton(ui->toolButtonTableSize, XOptions::ICONTYPE_SIZE);
    XOptions::adjustToolButton(ui->toolButtonTableSave, XOptions::ICONTYPE_SAVE);

    ui->toolButtonTableReload->setToolTip(tr("Reload"));
    ui->toolButtonTableSize->setToolTip(tr("Size"));
    ui->toolButtonTableSave->setToolTip(tr("Save"));
}

XGenericHeaderWidget::~XGenericHeaderWidget()
{
    delete ui;
}

void XGenericHeaderWidget::adjustView()
{
    // XGenericAbstractWidget::adjustView();
    getGlobalOptions()->adjustTableWidget(ui->tableWidgetMain, XOptions::ID_VIEW_FONT_TABLEVIEWS);
}

void XGenericHeaderWidget::reloadData(bool bSaveSelection)
{
    QList<QVariant> listVariants;

    XBinary::getDataRecordValues(getDevice(), getRecordsOptions(), &listVariants, nullptr);

    qint32 nCurrentRow = 0;

    if (bSaveSelection) {
        nCurrentRow = ui->tableWidgetMain->currentRow();
    }

    QStringList slHeader;
    slHeader.append(tr("Name"));
    slHeader.append(tr("Offset"));
    slHeader.append(tr("Size"));
    slHeader.append(tr("Type"));
    slHeader.append(tr("Value"));
    slHeader.append(tr(""));
    slHeader.append(tr("Comment"));

    ui->tableWidgetMain->setColumnCount(slHeader.count());
    ui->tableWidgetMain->setHorizontalHeaderLabels(slHeader);
    ui->tableWidgetMain->setSortingEnabled(false);
    ui->tableWidgetMain->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetMain->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetMain->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // ui->tableWidgetMain->setShowGrid(true);
    // ui->tableWidgetMain->setAlternatingRowColors(true);
    ui->tableWidgetMain->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidgetMain->verticalHeader()->setVisible(false);
    ui->tableWidgetMain->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableWidgetMain->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableWidgetMain->setWordWrap(false);
    ui->tableWidgetMain->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->tableWidgetMain->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // Set last column to stretch
    ui->tableWidgetMain->horizontalHeader()->setStretchLastSection(true);

    QList<XBinary::DATA_RECORD> listDataRecords = getRecordsOptions().dataHeader.listRecords;

    g_nDataSize = getRecordsOptions().dataHeader.nSize;

    // Example: Populate the tableWidget with the data records
    qint32 nNumberOfRecords = listDataRecords.count();

    ui->tableWidgetMain->setRowCount(nNumberOfRecords);
    ui->tableWidgetMain->clearContents();

    g_listLineEditsHEX.clear();
    g_listComboBoxes.clear();

    bool bIsReadonly = isReadonly();

    for (qint32 i = 0; i < nNumberOfRecords; ++i) {
        const XBinary::DATA_RECORD &record = listDataRecords.at(i);

        {
            QTableWidgetItem *itemName = new QTableWidgetItem(record.sName);
            itemName->setData(Qt::UserRole + UR_RELOFFSET, record.nRelOffset);  // TODO
            itemName->setData(Qt::UserRole + UR_SIZE, record.nSize);
            ui->tableWidgetMain->setItem(i, HEADER_COLUMN_NAME, itemName);
        }
        {
            QTableWidgetItem *itemOffset = new QTableWidgetItem(QString::number(record.nRelOffset, 16));
            itemOffset->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            ui->tableWidgetMain->setItem(i, HEADER_COLUMN_OFFSET, itemOffset);
        }
        {
            QTableWidgetItem *itemSize = new QTableWidgetItem(QString::number(record.nSize, 16));
            itemSize->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            ui->tableWidgetMain->setItem(i, HEADER_COLUMN_SIZE, itemSize);
        }
        {
            QTableWidgetItem *itemType = new QTableWidgetItem(XBinary::valueTypeToString(record.valType, record.nSize));
            ui->tableWidgetMain->setItem(i, HEADER_COLUMN_TYPE, itemType);
        }
        {
            QTableWidgetItem *itemValue = new QTableWidgetItem(listVariants.at(i).toString());
            ui->tableWidgetMain->setItem(i, HEADER_COLUMN_VALUE, itemValue);
        }
        {
            QTableWidgetItem *itemComment = new QTableWidgetItem("");
            ui->tableWidgetMain->setItem(i, HEADER_COLUMN_COMMENT, itemComment);
        }

        if (XBinary::isIntegerType(record.valType)) {
            XLineEditHEX *pLineEdit = new XLineEditHEX(this);

            XLineEditValidator::MODE mode = XLineEditValidator::MODE_HEX_32;

            if (record.nSize == 1) {
                mode = XLineEditValidator::MODE_HEX_8;
            } else if (record.nSize == 2) {
                mode = XLineEditValidator::MODE_HEX_16;
            } else if (record.nSize == 4) {
                mode = XLineEditValidator::MODE_HEX_32;
            } else if (record.nSize == 8) {
                mode = XLineEditValidator::MODE_HEX_64;
            }

            pLineEdit->setValidatorModeValue(mode, listVariants.at(i));
            pLineEdit->setReadOnly(bIsReadonly);

            ui->tableWidgetMain->setCellWidget(i, HEADER_COLUMN_VALUE, pLineEdit);

            g_listLineEditsHEX.append(pLineEdit);
        }

        qint32 nNumberOfComboBoxes = record.listDataValueSets.count();

        if (nNumberOfComboBoxes) {
            QWidget *pWidget = new QWidget(ui->tableWidgetMain);
            QLayout *pLayout = new QHBoxLayout(pWidget);
            pLayout->setContentsMargins(0, 0, 0, 0);
            pLayout->setSpacing(0);
            pWidget->setLayout(pLayout);

            for (qint32 j = 0; j < nNumberOfComboBoxes; j++) {
                XComboBoxEx *pComboBox = new XComboBoxEx(this);

                if (record.listDataValueSets.at(j).bFlags) {
                    pComboBox->setData(record.listDataValueSets.at(j).mapValues, XComboBoxEx::CBTYPE_FLAGS, record.listDataValueSets.at(j).nMask);
                } else {
                    pComboBox->setData(record.listDataValueSets.at(j).mapValues, XComboBoxEx::CBTYPE_LIST, record.listDataValueSets.at(j).nMask);
                }

                pComboBox->setValue(listVariants.at(i));
                pComboBox->setReadOnly(bIsReadonly);

                g_listComboBoxes.append(pComboBox);

                pLayout->addWidget(pComboBox);
            }

            ui->tableWidgetMain->setCellWidget(i, HEADER_COLUMN_INFO, pWidget);
        }
    }

    // Optionally resize columns to fit contents
    ui->tableWidgetMain->resizeColumnsToContents();  // TODO only init

    ui->tableWidgetMain->setCurrentCell(nCurrentRow, 0);
}

void XGenericHeaderWidget::setReadonly(bool bState)
{
    qint32 nNumberOfLineedis = g_listLineEditsHEX.count();

    for (qint32 i = 0; i < nNumberOfLineedis; i++) {
        XLineEditHEX *pLineEdit = g_listLineEditsHEX.at(i);
        pLineEdit->setReadOnly(bState);
    }

    XShortcutsObject::setReadonly(bState);
}

void XGenericHeaderWidget::on_toolButtonTableReload_clicked()
{
    reloadData(true);
}

void XGenericHeaderWidget::on_toolButtonTableSize_clicked()
{
    emit followLocation(getRecordsOptions().dataHeader.nLocation, getRecordsOptions().dataHeader.locType, g_nDataSize, XOptions::WIDGETTYPE_HEX);
}

void XGenericHeaderWidget::on_toolButtonTableSave_clicked()
{
    // saveModel(ui->tableWidgetMain->model(), getTypeTitle(getCwOptions()));
}

void XGenericHeaderWidget::on_tableWidgetMain_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);
    Q_UNUSED(currentColumn);
    Q_UNUSED(currentRow);

    setHeaderSelection();
}

void XGenericHeaderWidget::on_tableWidgetMain_cellClicked(int row, int column)
{
    Q_UNUSED(column);
    Q_UNUSED(row);

    setHeaderSelection();
}

void XGenericHeaderWidget::setHeaderSelection()
{
    qint32 nCurrentRow = ui->tableWidgetMain->currentRow();

    if (nCurrentRow >= 0) {
        QTableWidgetItem *pItem = ui->tableWidgetMain->item(nCurrentRow, HEADER_COLUMN_NAME);
        if (pItem) {
            qint64 nRelOffset = pItem->data(Qt::UserRole + UR_RELOFFSET).toLongLong();
            qint64 nSize = pItem->data(Qt::UserRole + UR_SIZE).toLongLong();

            emit currentLocationChanged(getRecordsOptions().dataHeader.nLocation + nRelOffset, getRecordsOptions().dataHeader.locType, nSize);
        }
    }
}
