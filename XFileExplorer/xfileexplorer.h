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
#ifndef XFILEEXPLORER_H
#define XFILEEXPLORER_H

#include <QFileSystemModel>
#include <QModelIndex>
#include <QStringList>
#include <QWidget>

class QTreeView;

namespace Ui {
class XFileExplorer;
}

class XFileExplorer : public QWidget {
    Q_OBJECT

public:
    explicit XFileExplorer(QWidget *pParent = nullptr);
    ~XFileExplorer();

    void setRootPath(const QString &sRootPath);
    QString getRootPath() const;

    void setCurrentPath(const QString &sPath);
    QString getCurrentPath() const;

    void setNameFilters(const QStringList &listNameFilters);
    QStringList getNameFilters() const;

    void setNameFilterDisables(bool bState);
    bool getNameFilterDisables() const;

    QFileSystemModel *getModel();
    QTreeView *getTreeView();

public slots:
    void reload();

signals:
    void rootPathChanged(const QString &sPath);
    void currentPathChanged(const QString &sPath);
    void fileActivated(const QString &sFileName);
    void directoryActivated(const QString &sDirectoryName);

private slots:
    void on_toolButtonBrowse_clicked();
    void on_toolButtonUp_clicked();
    void on_toolButtonRefresh_clicked();
    void on_lineEditPath_returnPressed();
    void on_treeViewFileSystem_clicked(const QModelIndex &index);
    void on_treeViewFileSystem_doubleClicked(const QModelIndex &index);
    void on_treeViewFileSystem_customContextMenuRequested(const QPoint &pos);
    void onCurrentChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    QString normalizePath(const QString &sPath) const;
    bool setRootPathInternal(const QString &sRootPath, bool bEmitSignal);
    bool selectPath(const QString &sPath);
    void updateCurrentPath(const QModelIndex &index);
    void updatePathControls(const QString &sPath);
    void activateIndex(const QModelIndex &index);
    void showContextMenu(const QModelIndex &index, const QPoint &pos);

    Ui::XFileExplorer *ui;
    QFileSystemModel *m_pModel;
    QString m_sRootPath;
    QString m_sCurrentPath;
    bool m_bNameFilterDisables;
};

#endif  // XFILEEXPLORER_H
