/* Copyright (c) 2019-2022 hors<horsicq@gmail.com>
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
#include "searchvalueswidget.h"

#include "ui_searchvalueswidget.h"

SearchValuesWidget::SearchValuesWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::SearchValuesWidget)
{
    ui->setupUi(this);
    g_pDevice = nullptr;

    g_pModel = nullptr;
    g_bInit = false;
    g_bIsReadonly = true;

    memset(shortCuts, 0, sizeof shortCuts);

    ui->tableViewResult->installEventFilter(this);
}

SearchValuesWidget::~SearchValuesWidget()
{
    g_watcher.waitForFinished();

    delete ui;
}

void SearchValuesWidget::setData(QIODevice *pDevice, XBinary::FT fileType)
{
    this->g_pDevice = pDevice;

    if (pDevice) {
        XFormats::setFileTypeComboBox(fileType, g_pDevice, ui->comboBoxType);
    }
}

QIODevice *SearchValuesWidget::getDevice()
{
    return g_pDevice;
}

void SearchValuesWidget::reload()
{
    search();
}

bool SearchValuesWidget::getInitStatus()
{
    return g_bInit;
}

void SearchValuesWidget::on_pushButtonSave_clicked()
{
    if (g_pModel) {
        XShortcutsWidget::saveTableModel(g_pModel, XBinary::getResultFileName(g_pDevice, QString("%1.txt").arg(tr("Values"))));
    }
}

void SearchValuesWidget::on_tableViewResult_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextMenu(this);

    // TODO

    contextMenu.exec(ui->tableViewResult->viewport()->mapToGlobal(pos));
}

void SearchValuesWidget::_copyString()
{
    qint32 nRow = ui->tableViewResult->currentIndex().row();

    if ((nRow != -1) && (g_pModel)) {
        QModelIndex index = ui->tableViewResult->selectionModel()->selectedIndexes().at(3);

        QString sString = ui->tableViewResult->model()->data(index).toString();

        QApplication::clipboard()->setText(sString);
    }
}

void SearchValuesWidget::_copyOffset()
{
    int nRow = ui->tableViewResult->currentIndex().row();

    if ((nRow != -1) && (g_pModel)) {
        QModelIndex index = ui->tableViewResult->selectionModel()->selectedIndexes().at(0);

        QString sString = ui->tableViewResult->model()->data(index).toString();

        QApplication::clipboard()->setText(sString);
    }
}

void SearchValuesWidget::_copySize()
{
    int nRow = ui->tableViewResult->currentIndex().row();

    if ((nRow != -1) && (g_pModel)) {
        QModelIndex index = ui->tableViewResult->selectionModel()->selectedIndexes().at(1);

        QString sString = ui->tableViewResult->model()->data(index).toString();

        QApplication::clipboard()->setText(sString);
    }
}

void SearchValuesWidget::_hex()
{
    int nRow = ui->tableViewResult->currentIndex().row();

    if ((nRow != -1) && (g_pModel)) {
        QModelIndex index = ui->tableViewResult->selectionModel()->selectedIndexes().at(0);

        qint64 nOffset = ui->tableViewResult->model()->data(index, Qt::UserRole + MultiSearch::USERROLE_OFFSET).toLongLong();
        qint64 nSize = ui->tableViewResult->model()->data(index, Qt::UserRole + MultiSearch::USERROLE_SIZE).toLongLong();

        XIODevice *pSubDevice = dynamic_cast<XIODevice *>(g_pDevice);

        if (pSubDevice) {
            nOffset += pSubDevice->getInitOffset();
        }

        emit showHex(nOffset, nSize);
    }
}

void SearchValuesWidget::search()
{
    if (g_pDevice) {

    }
}

void SearchValuesWidget::deleteOldModel()
{
    delete g_pOldModel;
}

void SearchValuesWidget::registerShortcuts(bool bState)
{
    if (bState) {
       // if (!shortCuts[SC_COPYSTRING]) shortCuts[SC_COPYSTRING] = new QShortcut(getShortcuts()->getShortcut(X_ID_STRINGS_COPY_STRING), this, SLOT(_copyString()));
    } else {
        for (qint32 i = 0; i < __SC_SIZE; i++) {
            if (shortCuts[i]) {
                delete shortCuts[i];
                shortCuts[i] = nullptr;
            }
        }
    }
}

void SearchValuesWidget::adjust()
{

}
