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
#include <QFont>
#include <QLocale>
#include <QVariant>

#include <algorithm>

XFileSystemModel::XFileSystemModel(QObject *pParent)
    : QAbstractTableModel(pParent),
      m_filters(QDir::AllEntries | QDir::AllDirs | QDir::NoDotDot | QDir::Hidden),
      m_sortOrder(Qt::AscendingOrder),
      m_nSortColumn(0),
      m_bNameFilterDisables(true)
{
    m_pData = nullptr;
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

void XFileSystemModel::setData(XFileInfoValues::XFIDATA *pData)
{
    m_pData = pData;
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

    if (!m_pData) {
        return QModelIndex();
    }

    for (int i = 0; i < m_pData->listRecords.count(); i++) {
        if (isSamePath(m_pData->listRecords.at(i).sFilePath, sNormalizedPath)) {
            return QAbstractTableModel::index(i, 0);
        }
    }

    return QModelIndex();
}

QString XFileSystemModel::filePath(const QModelIndex &index) const
{
    const XFileInfoValues::RecordInfo *pRecordInfo = recordInfo(index);

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

    if (m_pData) {
        m_pData->listRecords.clear();
    }

    if (m_pData && !m_sRootPath.isEmpty()) {
        QDir dir(m_sRootPath);
        QFileInfoList listFileInfo = dir.entryInfoList(m_filters, QDir::NoSort);

        for (int i = 0; i < listFileInfo.count(); i++) {
            const QFileInfo &fileInfo = listFileInfo.at(i);
            XFileInfoValues::RecordInfo recordInfo;

            recordInfo.sFileName = fileInfo.fileName();
            recordInfo.sFilePath = fileInfo.absoluteFilePath();
            recordInfo.icon = m_iconProvider.icon(fileInfo);
            recordInfo.bIsDir = fileInfo.isDir();
            recordInfo.bIsHidden = fileInfo.isHidden();
            recordInfo.bEnabled = matchesNameFilters(recordInfo);
            recordInfo.mapValues.clear();

            if (m_bNameFilterDisables || recordInfo.bEnabled) {
                m_pData->listRecords.append(recordInfo);
            }
        }

        sortEntries();
    }

    endResetModel();
}

void XFileSystemModel::updateFileInfoValues()
{
    if (!m_pData || m_pData->listRecords.isEmpty()) {
        return;
    }

    emit layoutAboutToBeChanged();
    sortEntries();
    emit layoutChanged();

    emit dataChanged(index(0, 0), index(m_pData->listRecords.count() - 1, m_pData->listFIV.count() - 1));
}

int XFileSystemModel::rowCount(const QModelIndex &parent) const
{
    int nResult = 0;

    if (parent.isValid() || !m_pData) {
        nResult = 0;
    } else {
        nResult = m_pData->listRecords.count();
    }

    return nResult;
}

int XFileSystemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    int nResult = 0;

    if (m_pData) {
        nResult =m_pData->listFIV.count();
    }

    if (nResult == 0) {
        nResult = 1; // at least one column for file name
    }

    return nResult;
}

QVariant XFileSystemModel::data(const QModelIndex &index, int nRole) const
{
    const XFileInfoValues::RecordInfo *pRecordInfo = recordInfo(index);

    if (!pRecordInfo) {
        return QVariant();
    }

    if (nRole == Qt::DisplayRole || nRole == Qt::EditRole) {
        qint32 nValueIndex = index.column();

        if (m_pData && (nValueIndex >= 0)) {
            XFileInfoValues::XFIV value = XFileInfoValues::XFIV_NAME;

            if (nValueIndex < m_pData->listFIV.count()) {
                value = m_pData->listFIV.at(nValueIndex);
            }

            if (value == XFileInfoValues::XFIV_NAME) {
                return pRecordInfo->sFileName;
            }

            QVariant varValue = pRecordInfo->mapValues.value(value);

            if (nRole == Qt::DisplayRole) {
                return XFileInfoValues::getDisplayRole(varValue, value);
            }

            return varValue;
        }
    } else if (nRole == Qt::TextAlignmentRole) {
        qint32 nValueIndex = index.column();

        if (m_pData && (nValueIndex >= 0) && (nValueIndex < m_pData->listFIV.count())) {
            XFileInfoValues::XFIV value = XFileInfoValues::XFIV_NAME;

            if (nValueIndex < m_pData->listFIV.count()) {
                value = m_pData->listFIV.at(nValueIndex);
            }

            int alignment = XFileInfoValues::getTextAlignmentRole(value);

            if (alignment) {
                return QVariant(alignment);
            }
        }
    } else if (nRole == Qt::FontRole) {
        if (pRecordInfo->bIsDir || pRecordInfo->bIsHidden) {
            QFont font;
            font.setBold(pRecordInfo->bIsDir);
            font.setItalic(pRecordInfo->bIsHidden);

            return font;
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

    qint32 nValueIndex = nSection;

    if (m_pData && (nValueIndex >= 0) && (nValueIndex < m_pData->listFIV.count())) {
        return XFileInfoValues::valueIdToString(m_pData->listFIV.at(nValueIndex));
    }

    return QVariant();
}

Qt::ItemFlags XFileSystemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    Qt::ItemFlags flags = Qt::ItemIsSelectable;
    const XFileInfoValues::RecordInfo *pRecordInfo = recordInfo(index);

    if (pRecordInfo && pRecordInfo->bEnabled) {
        flags |= Qt::ItemIsEnabled;
    }

    return flags;
}

void XFileSystemModel::sort(int nColumn, Qt::SortOrder order)
{
    if (nColumn < 0) {
        nColumn = 0;
    }

    m_nSortColumn = nColumn;
    m_sortOrder = order;

    if (!m_pData || m_pData->listRecords.isEmpty()) {
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

const XFileInfoValues::RecordInfo *XFileSystemModel::recordInfo(const QModelIndex &index) const
{
    if (!m_pData || !index.isValid() || index.row() < 0 || index.row() >= m_pData->listRecords.count()) {
        return nullptr;
    }

    return &(m_pData->listRecords.at(index.row()));
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

bool XFileSystemModel::matchesNameFilters(const XFileInfoValues::RecordInfo &recordInfo) const
{
    if (recordInfo.bIsDir || m_listNameFilters.isEmpty()) {
        return true;
    }

    return QDir::match(m_listNameFilters, recordInfo.sFileName);
}

int XFileSystemModel::compareRecordInfo(const XFileInfoValues::RecordInfo &recordInfo1, const XFileInfoValues::RecordInfo &recordInfo2) const
{
    if (recordInfo1.bIsDir != recordInfo2.bIsDir) {
        return recordInfo1.bIsDir ? -1 : 1;
    }

    int nResult = 0;

    // if (m_nSortColumn == COLUMN_SIZE) {
    //     if (recordInfo1.nSize < recordInfo2.nSize) {
    //         nResult = -1;
    //     } else if (recordInfo1.nSize > recordInfo2.nSize) {
    //         nResult = 1;
    //     }
    // } else if (m_nSortColumn == COLUMN_TYPE) {
    //     nResult = compareText(recordInfo1.sType, recordInfo2.sType);
    // } else if (m_nSortColumn == COLUMN_MODIFIED) {
    //     if (recordInfo1.dtLastModified < recordInfo2.dtLastModified) {
    //         nResult = -1;
    //     } else if (recordInfo1.dtLastModified > recordInfo2.dtLastModified) {
    //         nResult = 1;
    //     }
    // }

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
    XFileInfoValues::XFIV value = XFileInfoValues::XFIV_NAME;
    qint32 nValueIndex = m_nSortColumn - 1;

    if (m_pData && (nValueIndex >= 0) && (nValueIndex < m_pData->listFIV.count())) {
        value = m_pData->listFIV.at(nValueIndex);
    }

    XFileInfoValues_Sort xfiv;
    xfiv.sortOrder = m_sortOrder;
    xfiv.xFIV = value;

    std::sort(m_pData->listRecords.begin(), m_pData->listRecords.end(), xfiv);
}
