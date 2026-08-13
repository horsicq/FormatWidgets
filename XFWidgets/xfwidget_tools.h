#ifndef XFWIDGET_TOOLS_H
#define XFWIDGET_TOOLS_H

#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "xformats.h"
#include "xshortcutswidget.h"

class QFile;

// Generic file-tools panel: resize the file, replace/remove/save a parsed
// format overlay when one is present, and append raw data.
// UI actions confirm destructive changes; programmatic operations return a
// detailed error. Every successful mutation emits dataChanged() so the host
// can reload the affected range from the modified file.
class XFWidget_Tools : public XShortcutsWidget {
    Q_OBJECT

public:
    explicit XFWidget_Tools(QWidget *pParent = nullptr);
    ~XFWidget_Tools() override;

    void setData(const XBinary::INDATA &inData);
    bool hasData() const;
    qint64 fileSize() const;
    bool canModifyFile() const;
    bool resizeFile(qint64 nNewSize, QString *pErrorString = nullptr);
    bool replaceOverlayFromFile(const QString &sFileName, QString *pErrorString = nullptr);
    bool removeOverlay(QString *pErrorString = nullptr);
    bool dumpOverlayToFile(const QString &sFileName, QString *pErrorString = nullptr) const;
    bool appendFile(const QString &sFileName, QString *pErrorString = nullptr);
    void clear();
    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions) override;
    void setReadonly(bool bIsReadonly) override;

private slots:
    void onResize();
    void onOverlaySet();
    void onOverlayRemove();
    void onOverlayDump();
    void onAppendData();

private:
    void _reloadState();
    void _updateControls();
    bool _openReadWrite(QFile *pFile, QString *pErrorString) const;
    bool _saveBackup(QFile *pFile, QString *pErrorString);
    bool _rewriteOverlay(const QString *pSourceFileName, QString *pErrorString);
    void _finishMutation(qint64 nOldSize, qint64 nNewSize, qint64 nChangeOffset, bool bContentsChanged = false);
    QString _lastDirectory();

    XBinary::INDATA m_inData;
    qint64 m_nFileSize = -1;
    qint64 m_nOverlayOffset = -1;
    qint64 m_nOverlaySize = 0;
    bool m_bHasData = false;
    bool m_bHasOverlay = false;

    QLabel *m_pLabelFileSize;
    QLabel *m_pLabelNewSize;
    QLineEdit *m_pLineEditNewSize;
    QPushButton *m_pPushButtonResize;

    QGroupBox *m_pGroupOverlay;
    QLabel *m_pLabelOverlayInfo;
    QPushButton *m_pPushButtonOverlaySet;
    QPushButton *m_pPushButtonOverlayRemove;
    QPushButton *m_pPushButtonOverlayDump;

    QPushButton *m_pPushButtonAppend;
    QLabel *m_pLabelStatus;
};

#endif  // XFWIDGET_TOOLS_H
