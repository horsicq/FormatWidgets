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
#include "xfilesystemmodel.h"

#include <QFileInfo>
#include <QLocale>
#include <QVariant>

#include <algorithm>

XFileSystemModel::XFileSystemModel(QObject *pParent)
    : QAbstractTableModel(pParent),
      m_filters(QDir::AllEntries | QDir::AllDirs | QDir::NoDotAndDotDot),
      m_sortOrder(Qt::AscendingOrder),
      m_nSortColumn(COLUMN_NAME),
      m_bNameFilterDisables(true)
{
}

void XFileSystemModel::setFilter(QDir::Filters filters)
{
    if (m_filters != filters) {
        m_filters = filters;
        reload();
    }
}

QDir::Filters XFileSystemModel::filter() const
{
    return m_filters;
}

QModelIndex XFileSystemModel::setRootPath(const QString &sRootPath)
{
    QString sPath = normalizePath(sRootPath);
    QFileInfo fileInfo(sPath);

    if (fileInfo.exists() && fileInfo.isFile()) {
        sPath = fileInfo.absolutePath();
    }

    QDir dir(sPath);

    if (dir.exists()) {
        m_sRootPath = dir.absolutePath();
    } else {
        m_sRootPath.clear();
    }

    reload();

    return QModelIndex();
}

QString XFileSystemModel::rootPath() const
{
    return m_sRootPath;
}

QModelIndex XFileSystemModel::index(const QString &sPath) const
{
    QString sNormalizedPath = normalizePath(sPath);

    for (int i = 0; i < m_listRecordInfo.count(); i++) {
        if (isSamePath(m_listRecordInfo.at(i).sFilePath, sNormalizedPath)) {
            return QAbstractTableModel::index(i, COLUMN_NAME);
        }
    }

    return QModelIndex();
}

QString XFileSystemModel::filePath(const QModelIndex &index) const
{
    const RecordInfo *pRecordInfo = recordInfo(index);

    return pRecordInfo ? pRecordInfo->sFilePath : QString();
}

void XFileSystemModel::setNameFilters(const QStringList &listNameFilters)
{
    bool bChanged = (m_listNameFilters.count() != listNameFilters.count());

    for (int i = 0; !bChanged && i < m_listNameFilters.count(); i++) {
        bChanged = (m_listNameFilters.at(i) != listNameFilters.at(i));
    }

    if (bChanged) {
        m_listNameFilters = listNameFilters;
        reload();
    }
}

QStringList XFileSystemModel::nameFilters() const
{
    return m_listNameFilters;
}

void XFileSystemModel::setNameFilterDisables(bool bState)
{
    if (m_bNameFilterDisables != bState) {
        m_bNameFilterDisables = bState;
        reload();
    }
}

bool XFileSystemModel::nameFilterDisables() const
{
    return m_bNameFilterDisables;
}

void XFileSystemModel::reload()
{
    beginResetModel();

    m_listRecordInfo.clear();

    if (!m_sRootPath.isEmpty()) {
        QDir dir(m_sRootPath);
        QFileInfoList listFileInfo = dir.entryInfoList(m_filters, QDir::NoSort);

        for (int i = 0; i < listFileInfo.count(); i++) {
            const QFileInfo &fileInfo = listFileInfo.at(i);
            RecordInfo recordInfo;

            recordInfo.sFileName = fileInfo.fileName();
            recordInfo.sFilePath = fileInfo.absoluteFilePath();
            recordInfo.sType = m_iconProvider.type(fileInfo);
            recordInfo.dtLastModified = fileInfo.lastModified();
            recordInfo.icon = m_iconProvider.icon(fileInfo);
            recordInfo.nSize = fileInfo.size();
            recordInfo.bIsDir = fileInfo.isDir();
            recordInfo.bEnabled = matchesNameFilters(recordInfo);

            if (m_bNameFilterDisables || recordInfo.bEnabled) {
                m_listRecordInfo.append(recordInfo);
            }
        }

        sortEntries();
    }

    endResetModel();
}

int XFileSystemModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_listRecordInfo.count();
}

int XFileSystemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return COLUMN_COUNT;
}

QVariant XFileSystemModel::data(const QModelIndex &index, int nRole) const
{
    const RecordInfo *pRecordInfo = recordInfo(index);

    if (!pRecordInfo) {
        return QVariant();
    }

    if (nRole == Qt::DisplayRole || nRole == Qt::EditRole) {
        if (index.column() == COLUMN_NAME) {
            return pRecordInfo->sFileName;
        } else if (index.column() == COLUMN_SIZE) {
            return pRecordInfo->bIsDir ? QString() : sizeToString(pRecordInfo->nSize);
        } else if (index.column() == COLUMN_TYPE) {
            return pRecordInfo->sType;
        } else if (index.column() == COLUMN_MODIFIED) {
            return QLocale().toString(pRecordInfo->dtLastModified, QLocale::ShortFormat);
        }
    } else if (nRole == Qt::DecorationRole) {
        if (index.column() == COLUMN_NAME) {
            return pRecordInfo->icon;
        }
    } else if (nRole == Qt::TextAlignmentRole) {
        if (index.column() == COLUMN_SIZE) {
            return QVariant(Qt::AlignRight | Qt::AlignVCenter);
        }
    } else if (nRole == Qt::ToolTipRole) {
        return QDir::toNativeSeparators(pRecordInfo->sFilePath);
    } else if (nRole == Qt::UserRole) {
        return pRecordInfo->sFilePath;
    }

    return QVariant();
}

QVariant XFileSystemModel::headerData(int nSection, Qt::Orientation orientation, int nRole) const
{
    if (orientation != Qt::Horizontal || nRole != Qt::DisplayRole) {
        return QVariant();
    }

    if (nSection == COLUMN_NAME) {
        return tr("Name");
    } else if (nSection == COLUMN_SIZE) {
        return tr("Size");
    } else if (nSection == COLUMN_TYPE) {
        return tr("Type");
    } else if (nSection == COLUMN_MODIFIED) {
        return tr("Date modified");
    }

    return QVariant();
}

Qt::ItemFlags XFileSystemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    Qt::ItemFlags flags = Qt::ItemIsSelectable;
    const RecordInfo *pRecordInfo = recordInfo(index);

    if (pRecordInfo && pRecordInfo->bEnabled) {
        flags |= Qt::ItemIsEnabled;
    }

    return flags;
}

void XFileSystemModel::sort(int nColumn, Qt::SortOrder order)
{
    if ((nColumn < 0) || (nColumn >= COLUMN_COUNT)) {
        nColumn = COLUMN_NAME;
    }

    m_nSortColumn = nColumn;
    m_sortOrder = order;

    if (m_listRecordInfo.isEmpty()) {
        return;
    }

    emit layoutAboutToBeChanged();
    sortEntries();
    emit layoutChanged();
}

QString XFileSystemModel::normalizePath(const QString &sPath) const
{
    QString sResult = QDir::fromNativeSeparators(sPath.trimmed());

    if (sResult.isEmpty()) {
        return QString();
    }

    return QDir::cleanPath(QFileInfo(sResult).absoluteFilePath());
}

const XFileSystemModel::RecordInfo *XFileSystemModel::recordInfo(const QModelIndex &index) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_listRecordInfo.count()) {
        return nullptr;
    }

    return &(m_listRecordInfo.at(index.row()));
}

bool XFileSystemModel::isSamePath(const QString &sPath1, const QString &sPath2) const
{
    QString sCleanPath1 = QDir::cleanPath(QDir::fromNativeSeparators(sPath1));
    QString sCleanPath2 = QDir::cleanPath(QDir::fromNativeSeparators(sPath2));

#ifdef Q_OS_WIN
    return QString::compare(sCleanPath1, sCleanPath2, Qt::CaseInsensitive) == 0;
#else
    return sCleanPath1 == sCleanPath2;
#endif
}

bool XFileSystemModel::matchesNameFilters(const RecordInfo &recordInfo) const
{
    if (recordInfo.bIsDir || m_listNameFilters.isEmpty()) {
        return true;
    }

    return QDir::match(m_listNameFilters, recordInfo.sFileName);
}

int XFileSystemModel::compareRecordInfo(const RecordInfo &recordInfo1, const RecordInfo &recordInfo2) const
{
    if (recordInfo1.bIsDir != recordInfo2.bIsDir) {
        return recordInfo1.bIsDir ? -1 : 1;
    }

    int nResult = 0;

    if (m_nSortColumn == COLUMN_SIZE) {
        if (recordInfo1.nSize < recordInfo2.nSize) {
            nResult = -1;
        } else if (recordInfo1.nSize > recordInfo2.nSize) {
            nResult = 1;
        }
    } else if (m_nSortColumn == COLUMN_TYPE) {
        nResult = compareText(recordInfo1.sType, recordInfo2.sType);
    } else if (m_nSortColumn == COLUMN_MODIFIED) {
        if (recordInfo1.dtLastModified < recordInfo2.dtLastModified) {
            nResult = -1;
        } else if (recordInfo1.dtLastModified > recordInfo2.dtLastModified) {
            nResult = 1;
        }
    }

    if (nResult == 0) {
        nResult = compareText(recordInfo1.sFileName, recordInfo2.sFileName);
    }

    if (m_sortOrder == Qt::DescendingOrder) {
        nResult = -nResult;
    }

    return nResult;
}

int XFileSystemModel::compareText(const QString &sValue1, const QString &sValue2) const
{
    return QString::localeAwareCompare(sValue1.toCaseFolded(), sValue2.toCaseFolded());
}

QString XFileSystemModel::sizeToString(qint64 nSize) const
{
    return QLocale().formattedDataSize(nSize);
}

void XFileSystemModel::sortEntries()
{
    std::sort(m_listRecordInfo.begin(), m_listRecordInfo.end(), [this](const RecordInfo &recordInfo1, const RecordInfo &recordInfo2) {
        return compareRecordInfo(recordInfo1, recordInfo2) < 0;
    });
}
