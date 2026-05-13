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
#ifndef XFILESYSTEMMODEL_H
#define XFILESYSTEMMODEL_H

#include <QAbstractTableModel>
#include <QDateTime>
#include <QDir>
#include <QFileIconProvider>
#include <QIcon>
#include <QStringList>
#include <QVector>

class XFileSystemModel : public QAbstractTableModel {
public:
    enum COLUMN {
        COLUMN_NAME = 0,
        COLUMN_SIZE,
        COLUMN_TYPE,
        COLUMN_MODIFIED,
        COLUMN_COUNT
    };

    explicit XFileSystemModel(QObject *pParent = nullptr);

    using QAbstractTableModel::index;

    void setFilter(QDir::Filters filters);
    QDir::Filters filter() const;

    QModelIndex setRootPath(const QString &sRootPath);
    QString rootPath() const;

    QModelIndex index(const QString &sPath) const;
    QString filePath(const QModelIndex &index) const;

    void setNameFilters(const QStringList &listNameFilters);
    QStringList nameFilters() const;

    void setNameFilterDisables(bool bState);
    bool nameFilterDisables() const;

    void reload();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int nRole = Qt::DisplayRole) const override;
    QVariant headerData(int nSection, Qt::Orientation orientation, int nRole = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void sort(int nColumn, Qt::SortOrder order = Qt::AscendingOrder) override;

private:
    struct RecordInfo {
        QString sFileName;
        QString sFilePath;
        QString sType;
        QDateTime dtLastModified;
        QIcon icon;
        qint64 nSize;
        bool bIsDir;
        bool bEnabled;
    };

    const RecordInfo *recordInfo(const QModelIndex &index) const;
    QString normalizePath(const QString &sPath) const;
    bool isSamePath(const QString &sPath1, const QString &sPath2) const;
    bool matchesNameFilters(const RecordInfo &recordInfo) const;
    int compareRecordInfo(const RecordInfo &recordInfo1, const RecordInfo &recordInfo2) const;
    int compareText(const QString &sValue1, const QString &sValue2) const;
    QString sizeToString(qint64 nSize) const;
    void sortEntries();

    QFileIconProvider m_iconProvider;
    QVector<RecordInfo> m_listRecordInfo;
    QStringList m_listNameFilters;
    QString m_sRootPath;
    QDir::Filters m_filters;
    Qt::SortOrder m_sortOrder;
    int m_nSortColumn;
    bool m_bNameFilterDisables;
};

#endif  // XFILESYSTEMMODEL_H
