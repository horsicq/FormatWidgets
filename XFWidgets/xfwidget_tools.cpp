#include "xfwidget_tools.h"

#include <limits>

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QTemporaryFile>
#include <QVBoxLayout>

namespace {
void setErrorString(QString *pErrorString, const QString &sErrorString)
{
    if (pErrorString) {
        *pErrorString = sErrorString;
    }
}

bool getOverlayPart(XBinary *pBinary, qint64 nFileSize, qint64 *pOffset, qint64 *pSize)
{
    if (!pBinary || !pBinary->isValid() || (nFileSize < 0)) {
        return false;
    }

    const auto setTailOverlay = [nFileSize, pOffset, pSize](XBinary::FILEPART filePart, qint64 nOffset, qint64 nSize) {
        if ((filePart != XBinary::FILEPART_OVERLAY) || (nOffset <= 0) || (nSize <= 0) || (nOffset > nFileSize) || (nSize != (nFileSize - nOffset))) {
            return false;
        }
        if (pOffset) {
            *pOffset = nOffset;
        }
        if (pSize) {
            *pSize = nSize;
        }
        return true;
    };

    const QList<XBinary::FPART> listOverlayParts = pBinary->getFileParts(XBinary::FILEPART_OVERLAY, 1);
    if (!listOverlayParts.isEmpty()) {
        const XBinary::FPART &overlayPart = listOverlayParts.constFirst();
        if (setTailOverlay(overlayPart.filePart, overlayPart.nFileOffset, overlayPart.nFileSize)) {
            return true;
        }
    }

    // Destructive overlay tools require an explicit file-part contract.
    // A memory-map gap alone can include legitimate non-loaded format data.
    return false;
}
}  // namespace

XFWidget_Tools::XFWidget_Tools(QWidget *pParent) : XShortcutsWidget(pParent)
{
    setObjectName(QStringLiteral("XFWidget_Tools"));

    QVBoxLayout *pMainLayout = new QVBoxLayout(this);

    QGroupBox *pGroupSize = new QGroupBox(tr("File size"), this);
    pGroupSize->setObjectName(QStringLiteral("groupBoxFileSize"));
    QHBoxLayout *pSizeLayout = new QHBoxLayout(pGroupSize);

    m_pLabelFileSize = new QLabel(pGroupSize);
    m_pLabelFileSize->setObjectName(QStringLiteral("labelFileSize"));
    m_pLabelFileSize->setAccessibleName(tr("Current file size"));
    m_pLabelFileSize->setTextInteractionFlags(Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse);

    m_pLabelNewSize = new QLabel(tr("&New size (bytes):"), pGroupSize);
    m_pLabelNewSize->setObjectName(QStringLiteral("labelNewSize"));
    m_pLabelNewSize->setAccessibleName(tr("New file size label"));

    m_pLineEditNewSize = new QLineEdit(pGroupSize);
    m_pLineEditNewSize->setObjectName(QStringLiteral("lineEditNewSize"));
    m_pLineEditNewSize->setAccessibleName(tr("New file size"));
    m_pLineEditNewSize->setAccessibleDescription(tr("Enter the new file size in decimal bytes."));
    m_pLineEditNewSize->setToolTip(tr("Enter a non-negative decimal size in bytes."));
    m_pLineEditNewSize->setValidator(new QRegularExpressionValidator(QRegularExpression(QStringLiteral("[0-9]{1,19}")), m_pLineEditNewSize));
    m_pLabelNewSize->setBuddy(m_pLineEditNewSize);

    m_pPushButtonResize = new QPushButton(tr("&Resize"), pGroupSize);
    m_pPushButtonResize->setObjectName(QStringLiteral("pushButtonResize"));
    m_pPushButtonResize->setToolTip(tr("Resize the loaded file to the specified number of bytes."));
    m_pPushButtonResize->setAutoDefault(false);

    pSizeLayout->addWidget(m_pLabelFileSize);
    pSizeLayout->addStretch();
    pSizeLayout->addWidget(m_pLabelNewSize);
    pSizeLayout->addWidget(m_pLineEditNewSize);
    pSizeLayout->addWidget(m_pPushButtonResize);
    pMainLayout->addWidget(pGroupSize);

    m_pGroupOverlay = new QGroupBox(tr("Overlay"), this);
    m_pGroupOverlay->setObjectName(QStringLiteral("groupBoxOverlay"));
    QVBoxLayout *pOverlayLayout = new QVBoxLayout(m_pGroupOverlay);

    m_pLabelOverlayInfo = new QLabel(m_pGroupOverlay);
    m_pLabelOverlayInfo->setObjectName(QStringLiteral("labelOverlayInfo"));
    m_pLabelOverlayInfo->setAccessibleName(tr("Overlay information"));
    m_pLabelOverlayInfo->setTextInteractionFlags(Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse);
    pOverlayLayout->addWidget(m_pLabelOverlayInfo);

    QHBoxLayout *pOverlayButtons = new QHBoxLayout();
    m_pPushButtonOverlaySet = new QPushButton(tr("&Replace..."), m_pGroupOverlay);
    m_pPushButtonOverlaySet->setObjectName(QStringLiteral("pushButtonOverlaySet"));
    m_pPushButtonOverlaySet->setToolTip(tr("Replace the current overlay with data from another file."));
    m_pPushButtonOverlayRemove = new QPushButton(tr("Re&move"), m_pGroupOverlay);
    m_pPushButtonOverlayRemove->setObjectName(QStringLiteral("pushButtonOverlayRemove"));
    m_pPushButtonOverlayRemove->setToolTip(tr("Remove the current overlay from the file."));
    m_pPushButtonOverlayDump = new QPushButton(tr("&Save As..."), m_pGroupOverlay);
    m_pPushButtonOverlayDump->setObjectName(QStringLiteral("pushButtonOverlayDump"));
    m_pPushButtonOverlayDump->setToolTip(tr("Save the current overlay to a separate file."));
    m_pPushButtonOverlaySet->setAutoDefault(false);
    m_pPushButtonOverlayRemove->setAutoDefault(false);
    m_pPushButtonOverlayDump->setAutoDefault(false);
    pOverlayButtons->addWidget(m_pPushButtonOverlaySet);
    pOverlayButtons->addWidget(m_pPushButtonOverlayRemove);
    pOverlayButtons->addWidget(m_pPushButtonOverlayDump);
    pOverlayButtons->addStretch();
    pOverlayLayout->addLayout(pOverlayButtons);
    pMainLayout->addWidget(m_pGroupOverlay);

    QGroupBox *pGroupData = new QGroupBox(tr("Data"), this);
    pGroupData->setObjectName(QStringLiteral("groupBoxData"));
    QHBoxLayout *pDataLayout = new QHBoxLayout(pGroupData);
    m_pPushButtonAppend = new QPushButton(tr("&Append data from file..."), pGroupData);
    m_pPushButtonAppend->setObjectName(QStringLiteral("pushButtonAppend"));
    m_pPushButtonAppend->setToolTip(tr("Append the contents of another file to the loaded file."));
    m_pPushButtonAppend->setAutoDefault(false);
    pDataLayout->addWidget(m_pPushButtonAppend);
    pDataLayout->addStretch();
    pMainLayout->addWidget(pGroupData);

    m_pLabelStatus = new QLabel(this);
    m_pLabelStatus->setObjectName(QStringLiteral("labelStatus"));
    m_pLabelStatus->setAccessibleName(tr("Tools status"));
    m_pLabelStatus->setWordWrap(true);
    m_pLabelStatus->setTextInteractionFlags(Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse);
    pMainLayout->addWidget(m_pLabelStatus);
    pMainLayout->addStretch();

    connect(m_pPushButtonResize, &QPushButton::clicked, this, &XFWidget_Tools::onResize);
    connect(m_pPushButtonOverlaySet, &QPushButton::clicked, this, &XFWidget_Tools::onOverlaySet);
    connect(m_pPushButtonOverlayRemove, &QPushButton::clicked, this, &XFWidget_Tools::onOverlayRemove);
    connect(m_pPushButtonOverlayDump, &QPushButton::clicked, this, &XFWidget_Tools::onOverlayDump);
    connect(m_pPushButtonAppend, &QPushButton::clicked, this, &XFWidget_Tools::onAppendData);
    connect(m_pLineEditNewSize, &QLineEdit::returnPressed, this, &XFWidget_Tools::onResize);

    _updateControls();
}

XFWidget_Tools::~XFWidget_Tools() = default;

void XFWidget_Tools::setData(const XBinary::INDATA &inData)
{
    m_inData = inData;
    _reloadState();
}

bool XFWidget_Tools::hasData() const
{
    return m_bHasData;
}

qint64 XFWidget_Tools::fileSize() const
{
    return m_nFileSize;
}

bool XFWidget_Tools::canModifyFile() const
{
    return m_bHasData && !isReadonly() && (m_inData.inDataMode == XBinary::INDATA_MODE_FILE);
}

void XFWidget_Tools::_reloadState()
{
    m_bHasData = false;
    m_nFileSize = -1;
    m_nOverlayOffset = -1;
    m_nOverlaySize = 0;
    m_bHasOverlay = false;

    QIODevice *pDevice = XFormats::createDevice(m_inData);
    if (pDevice) {
        const qint64 nDeviceSize = pDevice->size();
        if (nDeviceSize >= 0) {
            m_bHasData = true;
            m_nFileSize = nDeviceSize;

            XBinary *pBinary = XFormats::createClass(m_inData.fileType, pDevice, m_inData.bIsImage, m_inData.nModuleAddress);
            m_bHasOverlay = getOverlayPart(pBinary, m_nFileSize, &m_nOverlayOffset, &m_nOverlaySize);
            delete pBinary;
        }
        XFormats::removeDevice(pDevice, m_inData);
    }

    _updateControls();
}

void XFWidget_Tools::_updateControls()
{
    const bool bCanEdit = canModifyFile();

    if (m_bHasData) {
        m_pLabelFileSize->setText(tr("Current size: %1 bytes (0x%2)").arg(m_nFileSize).arg(QString::number(m_nFileSize, 16).toUpper()));
        if (!m_pLineEditNewSize->hasFocus() || !bCanEdit || m_pLineEditNewSize->text().trimmed().isEmpty()) {
            m_pLineEditNewSize->setText(QString::number(m_nFileSize));
        }
    } else {
        m_pLabelFileSize->setText(tr("Current size: unavailable"));
        m_pLineEditNewSize->clear();
    }

    m_pLineEditNewSize->setReadOnly(!bCanEdit);
    m_pLabelNewSize->setEnabled(bCanEdit);
    m_pPushButtonResize->setEnabled(bCanEdit);
    m_pPushButtonAppend->setEnabled(bCanEdit);

    m_pGroupOverlay->setVisible(m_bHasData && m_bHasOverlay);
    if (m_bHasOverlay) {
        m_pLabelOverlayInfo->setText(tr("Offset: 0x%1 | Size: %2 bytes (0x%3) | %4")
                                         .arg(QString::number(m_nOverlayOffset, 16).toUpper())
                                         .arg(m_nOverlaySize)
                                         .arg(QString::number(m_nOverlaySize, 16).toUpper())
                                         .arg(tr("Present")));
    } else {
        m_pLabelOverlayInfo->clear();
    }
    m_pPushButtonOverlaySet->setEnabled(m_bHasOverlay && bCanEdit);
    m_pPushButtonOverlayRemove->setEnabled(m_bHasOverlay && bCanEdit);
    m_pPushButtonOverlayDump->setEnabled(m_bHasOverlay);

    if (!m_bHasData) {
        m_pLabelStatus->setText(m_inData.inDataMode == XBinary::INDATA_MODE_UNKNOWN ? tr("No file loaded.") : tr("File is unavailable."));
    } else if (!bCanEdit) {
        m_pLabelStatus->setText(tr("Read-only view."));
    } else {
        m_pLabelStatus->clear();
    }
}

bool XFWidget_Tools::_openReadWrite(QFile *pFile, QString *pErrorString) const
{
    if (!pFile || !canModifyFile() || m_inData.sFileName.isEmpty()) {
        setErrorString(pErrorString, tr("No writable file is loaded."));
        return false;
    }

    pFile->setFileName(m_inData.sFileName);
    if (!pFile->open(QIODevice::ReadWrite)) {
        setErrorString(pErrorString, tr("Cannot open file: %1").arg(m_inData.sFileName));
        return false;
    }

    return true;
}

bool XFWidget_Tools::_saveBackup(QFile *pFile, QString *pErrorString)
{
    if (!pFile) {
        setErrorString(pErrorString, tr("Cannot create backup."));
        return false;
    }
    if (getGlobalOptions() && getGlobalOptions()->isSaveBackup() && !XBinary::saveBackup(pFile)) {
        setErrorString(pErrorString, tr("Cannot create backup: %1").arg(XBinary::getBackupFileName(pFile)));
        return false;
    }

    return true;
}

void XFWidget_Tools::_finishMutation(qint64 nOldSize, qint64 nNewSize, qint64 nChangeOffset, bool bContentsChanged)
{
    _reloadState();
    const qint64 nObservedNewSize = m_bHasData ? m_nFileSize : nNewSize;
    qint64 nAffectedSize = bContentsChanged ? qMax<qint64>(0, qMax(nOldSize, nObservedNewSize) - nChangeOffset) : qAbs(nObservedNewSize - nOldSize);
    if (nAffectedSize > 0) {
        emit dataChanged(nChangeOffset, nAffectedSize);
    }
}

bool XFWidget_Tools::resizeFile(qint64 nNewSize, QString *pErrorString)
{
    if (pErrorString) {
        pErrorString->clear();
    }
    if (nNewSize < 0) {
        setErrorString(pErrorString, tr("The new size must be zero or greater."));
        return false;
    }
    if (!canModifyFile()) {
        setErrorString(pErrorString, tr("No writable file is loaded."));
        return false;
    }
    QFile file;
    if (!_openReadWrite(&file, pErrorString)) {
        _reloadState();
        return false;
    }

    const qint64 nCurrentSize = file.size();
    if (nCurrentSize < 0) {
        file.close();
        setErrorString(pErrorString, tr("Cannot determine the current file size."));
        _reloadState();
        return false;
    }
    if (nNewSize == nCurrentSize) {
        file.close();
        _reloadState();
        return true;
    }
    if (!_saveBackup(&file, pErrorString)) {
        file.close();
        return false;
    }

    const bool bResult = XBinary::resize(&file, nNewSize);
    file.close();
    if (!bResult) {
        setErrorString(pErrorString, tr("Cannot resize file."));
        _reloadState();
        return false;
    }

    _finishMutation(nCurrentSize, nNewSize, qMin(nCurrentSize, nNewSize));
    return true;
}

bool XFWidget_Tools::_rewriteOverlay(const QString *pSourceFileName, QString *pErrorString)
{
    if (pErrorString) {
        pErrorString->clear();
    }
    if (!canModifyFile() || !m_bHasOverlay) {
        setErrorString(pErrorString, tr("This file has no editable overlay."));
        return false;
    }
    if (pSourceFileName && pSourceFileName->isEmpty()) {
        setErrorString(pErrorString, tr("No overlay source file was selected."));
        return false;
    }

    QFile file;
    if (!_openReadWrite(&file, pErrorString)) {
        _reloadState();
        return false;
    }

    const qint64 nCurrentSize = file.size();
    XBinary *pBinary = XFormats::createClass(m_inData.fileType, &file, m_inData.bIsImage, m_inData.nModuleAddress);
    qint64 nChangeOffset = -1;
    qint64 nCurrentOverlaySize = 0;
    if (!getOverlayPart(pBinary, nCurrentSize, &nChangeOffset, &nCurrentOverlaySize)) {
        delete pBinary;
        file.close();
        setErrorString(pErrorString, tr("The loaded file no longer has an editable overlay."));
        _reloadState();
        return false;
    }
    delete pBinary;

    QFile source;
    qint64 nNewOverlaySize = 0;
    if (pSourceFileName) {
        source.setFileName(*pSourceFileName);
        if (!source.open(QIODevice::ReadOnly)) {
            file.close();
            setErrorString(pErrorString, tr("Cannot open overlay source: %1").arg(*pSourceFileName));
            return false;
        }
        nNewOverlaySize = source.size();
        if (nNewOverlaySize <= 0) {
            source.close();
            file.close();
            setErrorString(pErrorString, tr("The selected overlay source is empty."));
            return false;
        }
        const bool bAliasesLoadedFile = XBinary::devicesAlias(&source, &file);
        if (bAliasesLoadedFile) {
            source.close();
            file.close();
            setErrorString(pErrorString, tr("The loaded file cannot be used as its own overlay source."));
            return false;
        }
        if (nNewOverlaySize > (std::numeric_limits<qint64>::max)() - nChangeOffset) {
            source.close();
            file.close();
            setErrorString(pErrorString, tr("The replacement overlay is too large."));
            return false;
        }
    }

    QTemporaryFile previousOverlay;
    if (!previousOverlay.open() || !previousOverlay.resize(nCurrentOverlaySize) ||
        !XBinary::copyDeviceMemory(&file, nChangeOffset, &previousOverlay, 0, nCurrentOverlaySize)) {
        source.close();
        file.close();
        setErrorString(pErrorString, tr("Cannot stage the current overlay for a safe update."));
        return false;
    }
    if (!_saveBackup(&file, pErrorString)) {
        source.close();
        file.close();
        return false;
    }

    const qint64 nRequestedSize = nChangeOffset + nNewOverlaySize;
    const bool bResult = XBinary::resize(&file, nRequestedSize) && (!pSourceFileName || XBinary::copyDeviceMemory(&source, 0, &file, nChangeOffset, nNewOverlaySize));
    const qint64 nResultSize = file.size();
    source.close();
    if (!bResult) {
        const bool bRestoreResult = XBinary::resize(&file, nCurrentSize) && XBinary::copyDeviceMemory(&previousOverlay, 0, &file, nChangeOffset, nCurrentOverlaySize);
        const qint64 nRestoredSize = file.size();
        file.close();
        if (!bRestoreResult || (nRestoredSize != nCurrentSize)) {
            setErrorString(pErrorString, tr("Overlay update failed and the file may be partially modified."));
            _finishMutation(nCurrentSize, nRestoredSize >= 0 ? nRestoredSize : nResultSize, nChangeOffset, true);
        } else {
            setErrorString(pErrorString, tr("Cannot update overlay."));
            _reloadState();
        }
        return false;
    }
    file.close();

    _finishMutation(nCurrentSize, nResultSize >= 0 ? nResultSize : nRequestedSize, nChangeOffset, true);
    return true;
}

bool XFWidget_Tools::replaceOverlayFromFile(const QString &sFileName, QString *pErrorString)
{
    return _rewriteOverlay(&sFileName, pErrorString);
}

bool XFWidget_Tools::removeOverlay(QString *pErrorString)
{
    if (m_nOverlaySize <= 0) {
        setErrorString(pErrorString, tr("No overlay is present."));
        return false;
    }
    return _rewriteOverlay(nullptr, pErrorString);
}

bool XFWidget_Tools::dumpOverlayToFile(const QString &sFileName, QString *pErrorString) const
{
    if (pErrorString) {
        pErrorString->clear();
    }
    if (!m_bHasData || !m_bHasOverlay || (m_nOverlaySize <= 0) || sFileName.isEmpty()) {
        setErrorString(pErrorString, tr("No overlay is available to save."));
        return false;
    }

    QIODevice *pDevice = XFormats::createDevice(m_inData);
    if (!pDevice) {
        setErrorString(pErrorString, tr("Cannot open the loaded file."));
        return false;
    }

    XBinary *pBinary = XFormats::createClass(m_inData.fileType, pDevice, m_inData.bIsImage, m_inData.nModuleAddress);
    const qint64 nCurrentSize = pDevice->size();
    qint64 nCurrentOverlayOffset = -1;
    qint64 nCurrentOverlaySize = 0;
    if (!getOverlayPart(pBinary, nCurrentSize, &nCurrentOverlayOffset, &nCurrentOverlaySize)) {
        delete pBinary;
        XFormats::removeDevice(pDevice, m_inData);
        setErrorString(pErrorString, tr("The loaded file no longer has an overlay to save."));
        return false;
    }

    QFile existingDestination(sFileName);
    if (existingDestination.exists()) {
        if (!existingDestination.open(QIODevice::ReadOnly)) {
            delete pBinary;
            XFormats::removeDevice(pDevice, m_inData);
            setErrorString(pErrorString, tr("Cannot inspect the existing destination: %1").arg(sFileName));
            return false;
        }
        const bool bAliasesLoadedFile = XBinary::devicesAlias(pDevice, &existingDestination);
        existingDestination.close();
        if (bAliasesLoadedFile) {
            delete pBinary;
            XFormats::removeDevice(pDevice, m_inData);
            setErrorString(pErrorString, tr("The overlay cannot replace the loaded file."));
            return false;
        }
    }

    const bool bResult = pBinary->dumpToFile(sFileName, nCurrentOverlayOffset, nCurrentOverlaySize);
    delete pBinary;
    XFormats::removeDevice(pDevice, m_inData);
    if (!bResult) {
        setErrorString(pErrorString, tr("Cannot save overlay: %1").arg(sFileName));
    }
    return bResult;
}

bool XFWidget_Tools::appendFile(const QString &sFileName, QString *pErrorString)
{
    if (pErrorString) {
        pErrorString->clear();
    }
    if (!canModifyFile() || sFileName.isEmpty()) {
        setErrorString(pErrorString, tr("No writable file is loaded."));
        return false;
    }

    QFile source(sFileName);
    if (!source.open(QIODevice::ReadOnly)) {
        setErrorString(pErrorString, tr("Cannot open file: %1").arg(sFileName));
        return false;
    }
    const qint64 nSourceSize = source.size();
    if (nSourceSize <= 0) {
        setErrorString(pErrorString, tr("The selected file is empty."));
        return false;
    }
    QFile destination;
    if (!_openReadWrite(&destination, pErrorString)) {
        source.close();
        _reloadState();
        return false;
    }
    if (XBinary::devicesAlias(&source, &destination)) {
        destination.close();
        source.close();
        setErrorString(pErrorString, tr("A file cannot be appended to itself."));
        return false;
    }
    const qint64 nCurrentSize = destination.size();
    if ((nCurrentSize < 0) || (nCurrentSize > (std::numeric_limits<qint64>::max)() - nSourceSize)) {
        destination.close();
        source.close();
        setErrorString(pErrorString, tr("The loaded file changed or the resulting file would be too large."));
        _reloadState();
        return false;
    }
    if (!_saveBackup(&destination, pErrorString)) {
        destination.close();
        source.close();
        return false;
    }

    const qint64 nRequestedSize = nCurrentSize + nSourceSize;
    const bool bResult = XBinary::resize(&destination, nRequestedSize) && XBinary::copyDeviceMemory(&source, 0, &destination, nCurrentSize, nSourceSize);
    if (!bResult) {
        const bool bRollbackResult = XBinary::resize(&destination, nCurrentSize);
        const qint64 nResultSize = destination.size();
        destination.close();
        source.close();
        if (!bRollbackResult || (nResultSize != nCurrentSize)) {
            setErrorString(pErrorString, tr("Append failed and the file may be partially modified."));
            _finishMutation(nCurrentSize, nResultSize >= 0 ? nResultSize : nRequestedSize, nCurrentSize, true);
        } else {
            setErrorString(pErrorString, tr("Cannot append data to the file."));
            _reloadState();
        }
        return false;
    }
    destination.close();
    source.close();

    _finishMutation(nCurrentSize, nRequestedSize, nCurrentSize);
    return true;
}

QString XFWidget_Tools::_lastDirectory()
{
    QString sResult;
    if (getGlobalOptions()) {
        sResult = getGlobalOptions()->getLastDirectory();
    }
    if (sResult.isEmpty() && !m_inData.sFileName.isEmpty()) {
        sResult = QFileInfo(m_inData.sFileName).absolutePath();
    }
    return sResult;
}

void XFWidget_Tools::onResize()
{
    bool bOk = false;
    const qint64 nNewSize = m_pLineEditNewSize->text().toLongLong(&bOk, 10);
    if (!bOk || (nNewSize < 0)) {
        QMessageBox::warning(this, tr("Invalid size"), tr("Enter a non-negative decimal size in bytes."));
        m_pLineEditNewSize->selectAll();
        m_pLineEditNewSize->setFocus();
        return;
    }

    // Refresh the actual file size before deciding whether this is a no-op or
    // a destructive shrink; the loaded file may have changed externally.
    _reloadState();
    if (!canModifyFile()) {
        QMessageBox::critical(this, tr("Resize failed"), tr("No writable file is loaded."));
        return;
    }
    if (nNewSize == m_nFileSize) {
        return;
    }

    const QString sQuestion = (nNewSize < m_nFileSize)
                                  ? tr("Shrink the file from %1 to %2 bytes? Data beyond the new end will be permanently removed.").arg(m_nFileSize).arg(nNewSize)
                                  : tr("Grow the file from %1 to %2 bytes?").arg(m_nFileSize).arg(nNewSize);
    if (QMessageBox::question(this, tr("Resize file"), sQuestion) != QMessageBox::Yes) {
        return;
    }

    QString sErrorString;
    if (!resizeFile(nNewSize, &sErrorString)) {
        QMessageBox::critical(this, tr("Resize failed"), sErrorString);
    }
}

void XFWidget_Tools::onOverlaySet()
{
    const QString sFileName = QFileDialog::getOpenFileName(this, tr("Open overlay file"), _lastDirectory(), tr("All files (*)"));
    if (sFileName.isEmpty()) {
        return;
    }
    if (getGlobalOptions()) {
        getGlobalOptions()->setLastDirectory(QFileInfo(sFileName).absolutePath());
    }

    const qint64 nSourceSize = QFileInfo(sFileName).size();
    if (QMessageBox::question(this, tr("Replace overlay"), tr("Replace the current overlay with %1 bytes?").arg(nSourceSize)) != QMessageBox::Yes) {
        return;
    }

    QString sErrorString;
    if (!replaceOverlayFromFile(sFileName, &sErrorString)) {
        QMessageBox::critical(this, tr("Overlay update failed"), sErrorString);
    }
}

void XFWidget_Tools::onOverlayRemove()
{
    _reloadState();
    if (!m_bHasOverlay || !canModifyFile()) {
        QMessageBox::critical(this, tr("Overlay removal failed"), tr("The loaded file no longer has an editable overlay."));
        return;
    }
    if (QMessageBox::question(this, tr("Remove overlay"), tr("Remove the %1-byte overlay from this file?").arg(m_nOverlaySize)) != QMessageBox::Yes) {
        return;
    }

    QString sErrorString;
    if (!removeOverlay(&sErrorString)) {
        QMessageBox::critical(this, tr("Overlay removal failed"), sErrorString);
    }
}

void XFWidget_Tools::onOverlayDump()
{
    const QString sFileName =
        QFileDialog::getSaveFileName(this, tr("Save overlay as"), _lastDirectory() + QDir::separator() + QStringLiteral("overlay.bin"), tr("All files (*)"));
    if (sFileName.isEmpty()) {
        return;
    }
    if (getGlobalOptions()) {
        getGlobalOptions()->setLastDirectory(QFileInfo(sFileName).absolutePath());
    }

    QString sErrorString;
    if (!dumpOverlayToFile(sFileName, &sErrorString)) {
        QMessageBox::critical(this, tr("Save failed"), sErrorString);
    }
}

void XFWidget_Tools::onAppendData()
{
    const QString sFileName = QFileDialog::getOpenFileName(this, tr("Open data file"), _lastDirectory(), tr("All files (*)"));
    if (sFileName.isEmpty()) {
        return;
    }
    if (getGlobalOptions()) {
        getGlobalOptions()->setLastDirectory(QFileInfo(sFileName).absolutePath());
    }

    const qint64 nSourceSize = QFileInfo(sFileName).size();
    if (QMessageBox::question(this, tr("Append data"), tr("Append %1 bytes to this file?").arg(nSourceSize)) != QMessageBox::Yes) {
        return;
    }

    QString sErrorString;
    if (!appendFile(sFileName, &sErrorString)) {
        QMessageBox::critical(this, tr("Append failed"), sErrorString);
    }
}

void XFWidget_Tools::clear()
{
    m_inData = {};
    _reloadState();
}

void XFWidget_Tools::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
    _updateControls();
}

void XFWidget_Tools::setReadonly(bool bIsReadonly)
{
    XShortcutsWidget::setReadonly(bIsReadonly);
    _updateControls();
}
