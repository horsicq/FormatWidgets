/* Copyright (c) 2026 hors<horsicq@gmail.com>
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
#include "xfileexplorerwidget.h"

#include "ui_xfileexplorerwidget.h"
#include "xcomboboxex.h"
#include "xdialogprocess.h"
#include "xoptions.h"

#include <QAction>
#include <QClipboard>
#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QGuiApplication>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QMenu>
#include <QSignalBlocker>
#include <QUrl>
#include <QVariant>

XFileExplorerWidget::XFileExplorerWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::XFileExplorerWidget), m_pModel(new XFileSystemModel(this))
{
    ui->setupUi(this);

    m_bNameFilterDisables = true;
    m_bColumnsLoadedFromOptions = false;

    m_pModel->setData(&m_fileInfoValuesData);

    ui->comboBoxColumns->addCustomFlags(tr("Columns"), XFileInfoValues::getColumnCustomFlags());

    m_pModel->setFilter(QDir::AllEntries | QDir::AllDirs | QDir::NoDotAndDotDot);
    m_pModel->setNameFilterDisables(m_bNameFilterDisables);

    ui->treeViewFileSystem->setModel(m_pModel);
    ui->treeViewFileSystem->setSortingEnabled(true);
    ui->treeViewFileSystem->sortByColumn(0, Qt::AscendingOrder);
    ui->treeViewFileSystem->setRootIsDecorated(false);
    ui->treeViewFileSystem->setItemsExpandable(false);
    ui->treeViewFileSystem->setExpandsOnDoubleClick(false);
    ui->treeViewFileSystem->setIndentation(0);
    ui->treeViewFileSystem->header()->setStretchLastSection(false);
    // ui->treeViewFileSystem->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    XOptions::setMonoFont(ui->treeViewFileSystem);

    ui->toolButtonBrowse->setToolTip(tr("Browse"));
    ui->toolButtonUp->setToolTip(tr("Up"));
    ui->toolButtonRefresh->setToolTip(tr("Refresh"));
    ui->lineEditPath->setToolTip(tr("Path"));
    ui->comboBoxColumns->setToolTip(tr("Columns"));
    ui->treeViewFileSystem->setToolTip(tr("File explorer"));

    connect(ui->treeViewFileSystem->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), this,
            SLOT(onCurrentChanged(QModelIndex, QModelIndex)));

    // setRootPath(QDir::homePath());
}

XFileExplorerWidget::~XFileExplorerWidget()
{
    delete ui;
}

void XFileExplorerWidget::setRootPath(const QString &sRootPath)
{
    if (setRootPathInternal(sRootPath, true)) {
        reloadData(true);
    }
}

QString XFileExplorerWidget::getRootPath() const
{
    return m_sRootPath;
}

void XFileExplorerWidget::setCurrentPath(const QString &sPath)
{
    QFileInfo fileInfo(normalizePath(sPath));

    if (!fileInfo.exists()) {
        return;
    }

    QString sRootPath = fileInfo.isDir() ? fileInfo.absoluteFilePath() : fileInfo.absolutePath();

    if (!m_sRootPath.isEmpty()) {
        QString sRelativePath = QDir(m_sRootPath).relativeFilePath(fileInfo.absoluteFilePath());
        bool bInsideRoot = (sRelativePath == QString(".")) ||
                           (!QDir::isAbsolutePath(sRelativePath) && (sRelativePath != QString("..")) && !sRelativePath.startsWith(QString("../")) &&
                            !sRelativePath.startsWith(QString("..\\")));

        if (bInsideRoot) {
            sRootPath = m_sRootPath;
        }
    }

    if (setRootPathInternal(sRootPath, (sRootPath != m_sRootPath))) {
        reloadData(true);
        selectPath(fileInfo.absoluteFilePath());
    }
}

QString XFileExplorerWidget::getCurrentPath() const
{
    return m_sCurrentPath;
}

void XFileExplorerWidget::setNameFilters(const QStringList &listNameFilters)
{
    m_pModel->setNameFilters(listNameFilters);
}

QStringList XFileExplorerWidget::getNameFilters() const
{
    return m_pModel->nameFilters();
}

void XFileExplorerWidget::setNameFilterDisables(bool bState)
{
    m_bNameFilterDisables = bState;
    m_pModel->setNameFilterDisables(bState);
}

bool XFileExplorerWidget::getNameFilterDisables() const
{
    return m_bNameFilterDisables;
}

XFileSystemModel *XFileExplorerWidget::getModel()
{
    return m_pModel;
}

QTreeView *XFileExplorerWidget::getTreeView()
{
    return ui->treeViewFileSystem;
}

void XFileExplorerWidget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
    reloadData(true);
}

void XFileExplorerWidget::adjustView()
{
    XOptions *pOptions = getGlobalOptions();

    if ((!m_bColumnsLoadedFromOptions) && pOptions->isIDPresent(XOptions::ID_VIEW_COLUMNS)) {
        QString sColumns = pOptions->getValue(XOptions::ID_VIEW_COLUMNS).toString();

        if (!sColumns.isEmpty()) {
            QSignalBlocker signalBlocker(ui->comboBoxColumns);
            ui->comboBoxColumns->setValueAsString(sColumns);
        }

        m_bColumnsLoadedFromOptions = true;
    }

    pOptions->adjustWidget(this, XOptions::ID_VIEW_FONT_CONTROLS);

    if (pOptions->isIDPresent(XOptions::ID_VIEW_FONT_TREEVIEWS)) {
        pOptions->adjustTreeView(ui->treeViewFileSystem, XOptions::ID_VIEW_FONT_TREEVIEWS);
    }

    QHeaderView *pHeader = ui->treeViewFileSystem->header();

    if (pHeader) {
        pHeader->setStretchLastSection(false);

        qint32 nNumberOfColumns = m_pModel->columnCount();

        if (nNumberOfColumns > 0) {
            for (qint32 i = 0; i < nNumberOfColumns; i++) {
                pHeader->setSectionResizeMode(i, QHeaderView::Interactive);
                ui->treeViewFileSystem->resizeColumnToContents(i);
            }
        }
    }
}

void XFileExplorerWidget::reloadData(bool bSaveSelection)
{
    m_fileInfoValuesData.listFIV.clear();

    QString sColumns = ui->comboBoxColumns->getValueAsString();

    if (getGlobalOptions()->isIDPresent(XOptions::ID_VIEW_COLUMNS)) {
        getGlobalOptions()->setValue(XOptions::ID_VIEW_COLUMNS, sColumns);
    }

    QStringList listColumns = sColumns.split("|", Qt::SkipEmptyParts);
    qint32 nNumberOfColumns = listColumns.count();

    for (qint32 i = 0; i < nNumberOfColumns; i++) {
        QString sColumn = listColumns.at(i).trimmed();
        bool bIsId = false;
        quint64 nValue = sColumn.toULongLong(&bIsId);
        XFileInfoValues::XFIV value = bIsId ? static_cast<XFileInfoValues::XFIV>(nValue) : XFileInfoValues::valueStringToId(sColumn);

        m_fileInfoValuesData.listFIV.append(value);
    }

    QString sRootPath = m_sRootPath;
    QString sCurrentPath = bSaveSelection ? m_sCurrentPath : QString();

    if (sRootPath.isEmpty()) {
        sRootPath = QDir::homePath();
    }

    if (setRootPathInternal(sRootPath, false)) {
        if (!sCurrentPath.isEmpty()) {
            selectPath(sCurrentPath);
        }

        m_pModel->updateFileInfoValues();

        adjustView();
    }
}

void XFileExplorerWidget::reloadValues()
{
    if (!m_fileInfoValuesData.listRecords.isEmpty() && !m_fileInfoValuesData.listFIV.isEmpty()) {
        XFileInfoValues fileInfoValues;
        XDialogProcess dialogProcess(this, &fileInfoValues);
        fileInfoValues.setData(&m_fileInfoValuesData, dialogProcess.getPdStruct(), getGlobalOptions());
        dialogProcess.start();
        dialogProcess.showDialogDelay();

        if (!dialogProcess.isSuccess()) {
            for (qint32 i = 0; i < m_fileInfoValuesData.listRecords.count(); i++) {
                m_fileInfoValuesData.listRecords[i].mapValues.clear();
            }
        }
    }

    m_pModel->updateFileInfoValues();

    adjustView();
}

void XFileExplorerWidget::reload()
{
    reloadData(true);
    reloadValues();
}

void XFileExplorerWidget::on_toolButtonBrowse_clicked()
{
    QString sDirectoryName = QFileDialog::getExistingDirectory(this, tr("Open directory"), m_sRootPath);

    if (!sDirectoryName.isEmpty()) {
        setRootPath(sDirectoryName);
    }
}

void XFileExplorerWidget::on_toolButtonUp_clicked()
{
    QDir dir(m_sRootPath);

    if (dir.cdUp()) {
        setRootPath(dir.absolutePath());
    }
}

void XFileExplorerWidget::on_toolButtonRefresh_clicked()
{
    reloadData(true);
    reloadValues();
}

void XFileExplorerWidget::on_lineEditPath_returnPressed()
{
    QFileInfo fileInfo(normalizePath(ui->lineEditPath->text()));

    if (fileInfo.exists()) {
        if (fileInfo.isDir()) {
            setRootPath(fileInfo.absoluteFilePath());
        } else {
            setRootPath(fileInfo.absolutePath());
            selectPath(fileInfo.absoluteFilePath());
        }
    } else {
        updatePathControls(m_sRootPath);
    }
}

void XFileExplorerWidget::on_treeViewFileSystem_clicked(const QModelIndex &index)
{
    updateCurrentPath(index);
}

void XFileExplorerWidget::on_treeViewFileSystem_doubleClicked(const QModelIndex &index)
{
    activateIndex(index);
}

void XFileExplorerWidget::on_treeViewFileSystem_customContextMenuRequested(const QPoint &pos)
{
    showContextMenu(ui->treeViewFileSystem->indexAt(pos), pos);
}

void XFileExplorerWidget::onCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    updateCurrentPath(current);
}

QString XFileExplorerWidget::normalizePath(const QString &sPath) const
{
    QString sResult = QDir::fromNativeSeparators(sPath.trimmed());

    if (sResult.isEmpty()) {
        sResult = QDir::homePath();
    }

    return QDir::cleanPath(sResult);
}

bool XFileExplorerWidget::setRootPathInternal(const QString &sRootPath, bool bEmitSignal)
{
    QString sPath = normalizePath(sRootPath);
    QFileInfo fileInfo(sPath);

    if (fileInfo.exists() && fileInfo.isFile()) {
        sPath = fileInfo.absolutePath();
    }

    QDir dir(sPath);

    if (!dir.exists()) {
        updatePathControls(m_sRootPath);
        return false;
    }

    sPath = dir.absolutePath();

    QModelIndex indexRoot = m_pModel->setRootPath(sPath);
    ui->treeViewFileSystem->setRootIndex(indexRoot);

    bool bChanged = (m_sRootPath != sPath);
    m_sRootPath = sPath;

    updatePathControls(m_sRootPath);
    selectPath(m_sRootPath);

    if (bEmitSignal && bChanged) {
        emit rootPathChanged(m_sRootPath);
    }

    return true;
}

bool XFileExplorerWidget::selectPath(const QString &sPath)
{
    QFileInfo fileInfo(normalizePath(sPath));

    if (!fileInfo.exists()) {
        return false;
    }

    QString sFilePath = QDir::cleanPath(fileInfo.absoluteFilePath());
    QString sRootPath = QDir::cleanPath(m_sRootPath);
#ifdef Q_OS_WIN
    bool bRootPath = (QString::compare(sFilePath, sRootPath, Qt::CaseInsensitive) == 0);
#else
    bool bRootPath = (sFilePath == sRootPath);
#endif

    if (bRootPath) {
        ui->treeViewFileSystem->clearSelection();

        if (ui->treeViewFileSystem->selectionModel()) {
            ui->treeViewFileSystem->selectionModel()->clearCurrentIndex();
        }

        if (m_sCurrentPath != sFilePath) {
            m_sCurrentPath = sFilePath;
            emit currentPathChanged(m_sCurrentPath);
        }

        return true;
    }

    QModelIndex index = m_pModel->index(fileInfo.absoluteFilePath());

    if (!index.isValid()) {
        return false;
    }

    ui->treeViewFileSystem->setCurrentIndex(index);
    ui->treeViewFileSystem->scrollTo(index);
    updateCurrentPath(index);

    return true;
}

void XFileExplorerWidget::updateCurrentPath(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }

    QString sPath = m_pModel->filePath(index);

    if (!sPath.isEmpty() && (m_sCurrentPath != sPath)) {
        m_sCurrentPath = sPath;
        emit currentPathChanged(m_sCurrentPath);
    }
}

void XFileExplorerWidget::updatePathControls(const QString &sPath)
{
    ui->lineEditPath->setText(QDir::toNativeSeparators(sPath));

    QDir dir(sPath);
    ui->toolButtonUp->setEnabled(dir.cdUp());
}

void XFileExplorerWidget::activateIndex(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }

    QString sPath = m_pModel->filePath(index);
    QFileInfo fileInfo(sPath);

    if (fileInfo.isDir()) {
        setRootPath(fileInfo.absoluteFilePath());
        emit directoryActivated(fileInfo.absoluteFilePath());
    } else if (fileInfo.isFile()) {
        if (XFormats::isArchive(fileInfo.absoluteFilePath())) {
            emit archiveActivated(fileInfo.absoluteFilePath());
        } else {
            emit fileActivated(fileInfo.absoluteFilePath());
        }
    }
}

void XFileExplorerWidget::showContextMenu(const QModelIndex &index, const QPoint &pos)
{
    QMenu contextMenu(this);

    QAction *pActionOpen = contextMenu.addAction(tr("Open"));
    QAction *pActionOpenExternal = contextMenu.addAction(tr("Open externally"));
    QAction *pActionShowInFolder = contextMenu.addAction(tr("Show in folder"));
    QAction *pActionCopyPath = contextMenu.addAction(tr("Copy path"));
    contextMenu.addSeparator();
    QAction *pActionRefresh = contextMenu.addAction(tr("Refresh"));

    bool bHasSelection = index.isValid();

    pActionOpen->setEnabled(bHasSelection);
    pActionOpenExternal->setEnabled(bHasSelection);
    pActionShowInFolder->setEnabled(bHasSelection);
    pActionCopyPath->setEnabled(bHasSelection);

    QAction *pAction = contextMenu.exec(ui->treeViewFileSystem->viewport()->mapToGlobal(pos));

    if (!pAction) {
        return;
    }

    if (pAction == pActionRefresh) {
        reload();
        return;
    }

    if (!bHasSelection) {
        return;
    }

    QString sPath = m_pModel->filePath(index);
    QFileInfo fileInfo(sPath);

    if (pAction == pActionOpen) {
        activateIndex(index);
    } else if (pAction == pActionOpenExternal) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));
    } else if (pAction == pActionShowInFolder) {
        QString sDirectoryName = fileInfo.isDir() ? fileInfo.absoluteFilePath() : fileInfo.absolutePath();
        QDesktopServices::openUrl(QUrl::fromLocalFile(sDirectoryName));
    } else if (pAction == pActionCopyPath) {
        QClipboard *pClipboard = QGuiApplication::clipboard();

        if (pClipboard) {
            pClipboard->setText(QDir::toNativeSeparators(fileInfo.absoluteFilePath()));
        }
    }
}
