#include "xfwidget_tools.h"

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QVBoxLayout>

XFWidget_Tools::XFWidget_Tools(QWidget *pParent) : XShortcutsWidget(pParent)
{
    m_inData = {};
    m_nFileSize = 0;
    m_nOverlayOffset = -1;
    m_nOverlaySize = 0;
    m_bOverlayCapable = false;

    QVBoxLayout *pMainLayout = new QVBoxLayout(this);

    // --- File size ---
    QGroupBox *pGroupSize = new QGroupBox(tr("File size"), this);
    QHBoxLayout *pSizeLayout = new QHBoxLayout(pGroupSize);

    m_pLabelFileSize = new QLabel(pGroupSize);
    m_pLineEditNewSize = new QLineEdit(pGroupSize);
    m_pLineEditNewSize->setToolTip(tr("New size (bytes)"));
    m_pPushButtonResize = new QPushButton(tr("Resize"), pGroupSize);

    pSizeLayout->addWidget(m_pLabelFileSize);
    pSizeLayout->addWidget(m_pLineEditNewSize);
    pSizeLayout->addWidget(m_pPushButtonResize);
    pMainLayout->addWidget(pGroupSize);

    // --- Overlay ---
    m_pGroupOverlay = new QGroupBox(tr("Overlay"), this);
    QVBoxLayout *pOverlayLayout = new QVBoxLayout(m_pGroupOverlay);

    m_pLabelOverlayInfo = new QLabel(m_pGroupOverlay);
    pOverlayLayout->addWidget(m_pLabelOverlayInfo);

    QHBoxLayout *pOverlayButtons = new QHBoxLayout();
    m_pPushButtonOverlaySet = new QPushButton(tr("Add / Replace..."), m_pGroupOverlay);
    m_pPushButtonOverlayRemove = new QPushButton(tr("Remove"), m_pGroupOverlay);
    m_pPushButtonOverlayDump = new QPushButton(tr("Dump..."), m_pGroupOverlay);
    pOverlayButtons->addWidget(m_pPushButtonOverlaySet);
    pOverlayButtons->addWidget(m_pPushButtonOverlayRemove);
    pOverlayButtons->addWidget(m_pPushButtonOverlayDump);
    pOverlayButtons->addStretch();
    pOverlayLayout->addLayout(pOverlayButtons);
    pMainLayout->addWidget(m_pGroupOverlay);

    // --- Add data ---
    QGroupBox *pGroupData = new QGroupBox(tr("Data"), this);
    QHBoxLayout *pDataLayout = new QHBoxLayout(pGroupData);
    m_pPushButtonAppend = new QPushButton(tr("Append data from file..."), pGroupData);
    pDataLayout->addWidget(m_pPushButtonAppend);
    pDataLayout->addStretch();
    pMainLayout->addWidget(pGroupData);

    pMainLayout->addStretch();

    connect(m_pPushButtonResize, SIGNAL(clicked()), this, SLOT(onResize()));
    connect(m_pPushButtonOverlaySet, SIGNAL(clicked()), this, SLOT(onOverlaySet()));
    connect(m_pPushButtonOverlayRemove, SIGNAL(clicked()), this, SLOT(onOverlayRemove()));
    connect(m_pPushButtonOverlayDump, SIGNAL(clicked()), this, SLOT(onOverlayDump()));
    connect(m_pPushButtonAppend, SIGNAL(clicked()), this, SLOT(onAppendData()));
}

XFWidget_Tools::~XFWidget_Tools()
{
}

void XFWidget_Tools::setData(const XBinary::INDATA &inData)
{
    m_inData = inData;
    _refresh();
}

void XFWidget_Tools::_refresh()
{
    m_nFileSize = 0;
    m_nOverlayOffset = -1;
    m_nOverlaySize = 0;
    m_bOverlayCapable = false;

    QIODevice *pDevice = XFormats::createDevice(m_inData);

    if (pDevice) {
        m_nFileSize = pDevice->size();

        XBinary *pBinary = XFormats::createClass(m_inData.fileType, pDevice, m_inData.bIsImage, m_inData.nModuleAddress);

        if (pBinary) {
            m_nOverlayOffset = pBinary->getOverlayOffset();
            // Derive the size locally rather than getOverlaySize(), which rebuilds
            // the whole memory map a second time; it is just the tail past the image.
            if ((m_nOverlayOffset > 0) && (m_nFileSize > m_nOverlayOffset)) {
                m_nOverlaySize = m_nFileSize - m_nOverlayOffset;
            } else {
                m_nOverlaySize = 0;
            }
            // The format has an overlay concept if the formatted image ends
            // before EOF (a real overlay) or exactly at EOF (room to add one).
            m_bOverlayCapable = (m_nOverlayOffset > 0) && (m_nOverlayOffset <= m_nFileSize);
            delete pBinary;
        }

        XFormats::removeDevice(pDevice, m_inData);
    }

    m_pLabelFileSize->setText(tr("Current size") + QString(": %1 (0x%2)").arg(m_nFileSize).arg(m_nFileSize, 0, 16));
    m_pLineEditNewSize->setText(QString::number(m_nFileSize));

    m_pGroupOverlay->setVisible(m_bOverlayCapable);

    if (m_bOverlayCapable) {
        bool bPresent = (m_nOverlaySize > 0);
        m_pLabelOverlayInfo->setText(tr("Offset") + QString(": 0x%1  ").arg(m_nOverlayOffset, 0, 16) + tr("Size") +
                                     QString(": 0x%1  ").arg(m_nOverlaySize, 0, 16) + (bPresent ? tr("present") : tr("none")));
        m_pPushButtonOverlayRemove->setEnabled(bPresent && !isReadonly());
        m_pPushButtonOverlayDump->setEnabled(bPresent);
        m_pPushButtonOverlaySet->setEnabled(!isReadonly());
    }

    bool bCanEdit = !isReadonly() && (m_inData.inDataMode == XBinary::INDATA_MODE_FILE);
    m_pPushButtonResize->setEnabled(bCanEdit);
    m_pPushButtonAppend->setEnabled(bCanEdit);
    m_pLineEditNewSize->setReadOnly(!bCanEdit);

    if (m_bOverlayCapable) {
        m_pPushButtonOverlaySet->setEnabled(m_pPushButtonOverlaySet->isEnabled() && bCanEdit);
        m_pPushButtonOverlayRemove->setEnabled(m_pPushButtonOverlayRemove->isEnabled() && bCanEdit);
    }
}

QFile *XFWidget_Tools::_openReadWrite()
{
    if (m_inData.inDataMode != XBinary::INDATA_MODE_FILE) {
        QMessageBox::critical(this, tr("Error"), tr("Cannot open file"));
        return nullptr;
    }

    QFile *pFile = new QFile(m_inData.sFileName);

    if (!pFile->open(QIODevice::ReadWrite)) {
        delete pFile;
        QMessageBox::critical(this, tr("Error"), tr("Cannot open file") + QString(": %1").arg(m_inData.sFileName));
        return nullptr;
    }

    return pFile;
}

void XFWidget_Tools::_finishMutation()
{
    _refresh();
    // Ask the host to rebuild the whole view from the modified file. Queued so
    // this slot returns before the parent tears this widget down on reload.
    emit dataChanged(0, m_nFileSize);
}

QString XFWidget_Tools::_lastDirectory()
{
    QString sResult;

    if (getGlobalOptions()) {
        sResult = getGlobalOptions()->getLastDirectory();
    }

    if (sResult.isEmpty()) {
        sResult = QFileInfo(m_inData.sFileName).absolutePath();
    }

    return sResult;
}

void XFWidget_Tools::onResize()
{
    bool bOk = false;
    qint64 nNewSize = m_pLineEditNewSize->text().toLongLong(&bOk, 10);

    if (!bOk || (nNewSize < 0)) {
        QMessageBox::warning(this, tr("Error"), tr("Invalid value"));
        return;
    }

    if (nNewSize == m_nFileSize) {
        return;
    }

    QString sQuestion = tr("Resize") + QString(": %1 -> %2 ").arg(m_nFileSize).arg(nNewSize) + tr("bytes") + QString("?");

    if (QMessageBox::question(this, tr("Tools"), sQuestion) != QMessageBox::Yes) {
        return;
    }

    QFile *pFile = _openReadWrite();

    if (pFile) {
        bool bResult = XBinary::resize(pFile, nNewSize);
        pFile->close();
        delete pFile;

        if (!bResult) {
            QMessageBox::critical(this, tr("Error"), tr("Cannot save file"));
        }

        _finishMutation();
    }
}

void XFWidget_Tools::onOverlaySet()
{
    if (m_nOverlayOffset <= 0) {
        return;
    }

    QString sFileName = QFileDialog::getOpenFileName(this, tr("Open file") + QString("..."), _lastDirectory(), tr("All files") + QString(" (*)"));

    if (sFileName.isEmpty()) {
        return;
    }

    QFile fileSource(sFileName);

    if (!fileSource.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("Error"), tr("Cannot open file") + QString(": %1").arg(sFileName));
        return;
    }

    QByteArray baData = fileSource.readAll();
    fileSource.close();

    if (getGlobalOptions()) {
        getGlobalOptions()->setLastDirectory(QFileInfo(sFileName).absolutePath());
    }

    QString sQuestion = tr("Overlay") + QString(": %1 ").arg(baData.size()) + tr("bytes") + QString("?");

    if (QMessageBox::question(this, tr("Tools"), sQuestion) != QMessageBox::Yes) {
        return;
    }

    QFile *pFile = _openReadWrite();

    if (pFile) {
        // Write the new overlay first (extends the file if needed), then trim any
        // leftover tail of the old overlay. Writing before shrinking keeps the
        // format image intact if a step fails.
        bool bResult = true;

        if (baData.size() > 0) {
            bResult = (XBinary::write_array(pFile, m_nOverlayOffset, baData) == (qint64)baData.size());
        }

        if (bResult) {
            bResult = XBinary::resize(pFile, m_nOverlayOffset + (qint64)baData.size());
        }

        pFile->close();
        delete pFile;

        if (!bResult) {
            QMessageBox::critical(this, tr("Error"), tr("Cannot save file") + QString(" - ") + tr("the file may be partially modified"));
        }

        _finishMutation();
    }
}

void XFWidget_Tools::onOverlayRemove()
{
    if ((m_nOverlayOffset <= 0) || (m_nOverlaySize <= 0)) {
        return;
    }

    if (QMessageBox::question(this, tr("Tools"), tr("Remove") + QString(" ") + tr("Overlay") + QString("?")) != QMessageBox::Yes) {
        return;
    }

    QFile *pFile = _openReadWrite();

    if (pFile) {
        bool bResult = XBinary::resize(pFile, m_nOverlayOffset);
        pFile->close();
        delete pFile;

        if (!bResult) {
            QMessageBox::critical(this, tr("Error"), tr("Cannot save file"));
        }

        _finishMutation();
    }
}

void XFWidget_Tools::onOverlayDump()
{
    if ((m_nOverlayOffset <= 0) || (m_nOverlaySize <= 0)) {
        return;
    }

    QString sFileName = QFileDialog::getSaveFileName(this, tr("Save file") + QString("..."), _lastDirectory() + QDir::separator() + QString("overlay.bin"),
                                                     tr("All files") + QString(" (*)"));

    if (sFileName.isEmpty()) {
        return;
    }

    QIODevice *pDevice = XFormats::createDevice(m_inData);
    bool bResult = false;

    if (pDevice) {
        XBinary binary(pDevice);
        bResult = binary.dumpToFile(sFileName, m_nOverlayOffset, m_nOverlaySize);
        XFormats::removeDevice(pDevice, m_inData);
    }

    if (getGlobalOptions()) {
        getGlobalOptions()->setLastDirectory(QFileInfo(sFileName).absolutePath());
    }

    if (!bResult) {
        QMessageBox::critical(this, tr("Error"), tr("Cannot save file"));
    }
}

void XFWidget_Tools::onAppendData()
{
    QString sFileName = QFileDialog::getOpenFileName(this, tr("Open file") + QString("..."), _lastDirectory(), tr("All files") + QString(" (*)"));

    if (sFileName.isEmpty()) {
        return;
    }

    QFile fileSource(sFileName);

    if (!fileSource.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("Error"), tr("Cannot open file") + QString(": %1").arg(sFileName));
        return;
    }

    QByteArray baData = fileSource.readAll();
    fileSource.close();

    if (getGlobalOptions()) {
        getGlobalOptions()->setLastDirectory(QFileInfo(sFileName).absolutePath());
    }

    if (baData.isEmpty()) {
        return;
    }

    QString sQuestion = tr("Append") + QString(" %1 ").arg(baData.size()) + tr("bytes") + QString("?");

    if (QMessageBox::question(this, tr("Tools"), sQuestion) != QMessageBox::Yes) {
        return;
    }

    QFile *pFile = _openReadWrite();

    if (pFile) {
        // Writing at EOF extends the file - no pre-resize, so a failed write never
        // grows the file past what was actually written.
        qint64 nEnd = pFile->size();
        bool bResult = (XBinary::write_array(pFile, nEnd, baData) == (qint64)baData.size());

        pFile->close();
        delete pFile;

        if (!bResult) {
            QMessageBox::critical(this, tr("Error"), tr("Cannot save file") + QString(" - ") + tr("the file may be partially modified"));
        }

        _finishMutation();
    }
}

void XFWidget_Tools::clear()
{
    m_inData = {};
}

void XFWidget_Tools::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
}

void XFWidget_Tools::setReadonly(bool bIsReadonly)
{
    XShortcutsWidget::setReadonly(bIsReadonly);
    _refresh();
}
