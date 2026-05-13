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
#include "xfileexplorer.h"

#include "ui_xfileexplorer.h"

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
#include <QUrl>

XFileExplorer::XFileExplorer(QWidget *pParent) : QWidget(pParent), ui(new Ui::XFileExplorer), m_pModel(new XFileSystemModel(this))
{
    ui->setupUi(this);

    m_bNameFilterDisables = true;

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
    ui->treeViewFileSystem->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->treeViewFileSystem->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeViewFileSystem->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->treeViewFileSystem->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    ui->toolButtonBrowse->setToolTip(tr("Browse"));
    ui->toolButtonUp->setToolTip(tr("Up"));
    ui->toolButtonRefresh->setToolTip(tr("Refresh"));
    ui->lineEditPath->setToolTip(tr("Path"));
    ui->treeViewFileSystem->setToolTip(tr("File explorer"));

    connect(ui->treeViewFileSystem->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), this,
            SLOT(onCurrentChanged(QModelIndex, QModelIndex)));

    setRootPath(QDir::homePath());
}

XFileExplorer::~XFileExplorer()
{
    delete ui;
}

void XFileExplorer::setRootPath(const QString &sRootPath)
{
    setRootPathInternal(sRootPath, true);
}

QString XFileExplorer::getRootPath() const
{
    return m_sRootPath;
}

void XFileExplorer::setCurrentPath(const QString &sPath)
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
        selectPath(fileInfo.absoluteFilePath());
    }
}

QString XFileExplorer::getCurrentPath() const
{
    return m_sCurrentPath;
}

void XFileExplorer::setNameFilters(const QStringList &listNameFilters)
{
    m_pModel->setNameFilters(listNameFilters);
}

QStringList XFileExplorer::getNameFilters() const
{
    return m_pModel->nameFilters();
}

void XFileExplorer::setNameFilterDisables(bool bState)
{
    m_bNameFilterDisables = bState;
    m_pModel->setNameFilterDisables(bState);
}

bool XFileExplorer::getNameFilterDisables() const
{
    return m_bNameFilterDisables;
}

XFileSystemModel *XFileExplorer::getModel()
{
    return m_pModel;
}

QTreeView *XFileExplorer::getTreeView()
{
    return ui->treeViewFileSystem;
}

void XFileExplorer::reload()
{
    QString sRootPath = m_sRootPath;
    QString sCurrentPath = m_sCurrentPath;

    if (sRootPath.isEmpty()) {
        sRootPath = QDir::homePath();
    }

    if (setRootPathInternal(sRootPath, false) && !sCurrentPath.isEmpty()) {
        selectPath(sCurrentPath);
    }
}

void XFileExplorer::on_toolButtonBrowse_clicked()
{
    QString sDirectoryName = QFileDialog::getExistingDirectory(this, tr("Open directory"), m_sRootPath);

    if (!sDirectoryName.isEmpty()) {
        setRootPath(sDirectoryName);
    }
}

void XFileExplorer::on_toolButtonUp_clicked()
{
    QDir dir(m_sRootPath);

    if (dir.cdUp()) {
        setRootPath(dir.absolutePath());
    }
}

void XFileExplorer::on_toolButtonRefresh_clicked()
{
    reload();
}

void XFileExplorer::on_lineEditPath_returnPressed()
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

void XFileExplorer::on_treeViewFileSystem_clicked(const QModelIndex &index)
{
    updateCurrentPath(index);
}

void XFileExplorer::on_treeViewFileSystem_doubleClicked(const QModelIndex &index)
{
    activateIndex(index);
}

void XFileExplorer::on_treeViewFileSystem_customContextMenuRequested(const QPoint &pos)
{
    showContextMenu(ui->treeViewFileSystem->indexAt(pos), pos);
}

void XFileExplorer::onCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    updateCurrentPath(current);
}

QString XFileExplorer::normalizePath(const QString &sPath) const
{
    QString sResult = QDir::fromNativeSeparators(sPath.trimmed());

    if (sResult.isEmpty()) {
        sResult = QDir::homePath();
    }

    return QDir::cleanPath(sResult);
}

bool XFileExplorer::setRootPathInternal(const QString &sRootPath, bool bEmitSignal)
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

bool XFileExplorer::selectPath(const QString &sPath)
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

void XFileExplorer::updateCurrentPath(const QModelIndex &index)
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

void XFileExplorer::updatePathControls(const QString &sPath)
{
    ui->lineEditPath->setText(QDir::toNativeSeparators(sPath));

    QDir dir(sPath);
    ui->toolButtonUp->setEnabled(dir.cdUp());
}

void XFileExplorer::activateIndex(const QModelIndex &index)
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
        emit fileActivated(fileInfo.absoluteFilePath());
    }
}

void XFileExplorer::showContextMenu(const QModelIndex &index, const QPoint &pos)
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
