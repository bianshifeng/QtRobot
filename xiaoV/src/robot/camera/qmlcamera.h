#ifndef QMLCAMERA_H
#define QMLCAMERA_H

#include <QQuickItem>
#include <QSGTexture>
#include "tmcamerawork.h"
#include <QtQuick/QSGTexture>
#include <QtQuick/QSGSimpleTextureNode>
#include <QtCore/QMutex>

class QmlCamera : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int cameraIndex READ cameraIndex WRITE setCameraIndex NOTIFY cameraIndexChanged)
public:
    QmlCamera(QQuickItem *parent = 0);
    Q_INVOKABLE void updateBack();
    Q_INVOKABLE void close();

    virtual ~QmlCamera();
signals:

    void cameraIndexChanged(int cameraIndex);

public slots:

    void slot_cameraLoad(int cameraIndex);
    void slot_refresh();

    void setCameraIndex(int cameraIndex)
    {
        if (m_cameraIndex == cameraIndex)
            return;

        m_cameraIndex = cameraIndex;
        emit cameraIndexChanged(cameraIndex);
    }

public:
    bool event(QEvent *);


//    void drawFrame();

//    void drawFrameTest(QSGNode *);


    int cameraIndex() const
    {
        return m_cameraIndex;
    }

protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);
    QSGNode *updatePaintNode_old(QSGNode *, UpdatePaintNodeData *);

private:

    TmCameraWork *m_ptrCamera;
    QSGTexture* m_ptrTexture;
    QOpenGLContext *m_glContext;

    bool m_frameChanged;
    QMutex m_frameMutex;

    int m_cameraIndex;
};

#endif // QMLCAMERA_H
