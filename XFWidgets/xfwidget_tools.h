#ifndef XFWIDGET_TOOLS_H
#define XFWIDGET_TOOLS_H

#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "xformats.h"
#include "xshortcutswidget.h"

// Generic file-tools panel: resize the file, manage a format's overlay
// (add/replace/remove/dump) when the format has one, and append raw data.
// Every mutation reopens the file read-write, confirms first, then emits
// dataChanged() so the host reloads the whole view from the modified file.
class XFWidget_Tools : public XShortcutsWidget {
    Q_OBJECT

public:
    explicit XFWidget_Tools(QWidget *pParent = nullptr);
    ~XFWidget_Tools() override;

    void setData(const XBinary::INDATA &inData);
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
    void _refresh();
    QFile *_openReadWrite();
    void _finishMutation();
    QString _lastDirectory();

    XBinary::INDATA m_inData;
    qint64 m_nFileSize;
    qint64 m_nOverlayOffset;
    qint64 m_nOverlaySize;
    bool m_bOverlayCapable;

    QLabel *m_pLabelFileSize;
    QLineEdit *m_pLineEditNewSize;
    QPushButton *m_pPushButtonResize;

    QGroupBox *m_pGroupOverlay;
    QLabel *m_pLabelOverlayInfo;
    QPushButton *m_pPushButtonOverlaySet;
    QPushButton *m_pPushButtonOverlayRemove;
    QPushButton *m_pPushButtonOverlayDump;

    QPushButton *m_pPushButtonAppend;
};

#endif  // XFWIDGET_TOOLS_H
