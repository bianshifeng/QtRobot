#include "qmlcamera.h"
#include <QSGSimpleRectNode>
#include <QQuickWindow>
#include <QSGGeometryNode>
#include <QSGSimpleTextureNode>
#include <QSGSimpleMaterial>
#include <QSGGeometry>
#include <QSGSimpleMaterialShader>

#define VERTEX_COUNT 36


QImage cvMat2QImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        qDebug() << "CV_8UC1";
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        qDebug() << "CV_8UC3";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}



QmlCamera::~QmlCamera()
{
    QMutexLocker t_locker(&m_frameMutex);
    Q_UNUSED(t_locker);

    if(m_ptrCamera != Q_NULLPTR){
        m_ptrCamera->exit();
        m_ptrCamera->deleteLater();
        qDebug() << "QmlCamera";
    }
    if(m_ptrTexture != Q_NULLPTR){
        m_ptrTexture->deleteLater();
    }

}

void QmlCamera::slot_cameraLoad(int cameraIndex)
{
    if(m_ptrCamera){
        m_ptrCamera->deleteLater();
        m_ptrCamera = Q_NULLPTR;
    }

    qDebug()<<"init new camera work";
    if(cameraIndex > -1){
        m_ptrCamera = new TmCameraWork(cameraIndex);
    }

}


QmlCamera::QmlCamera(QQuickItem *parent):
    QQuickItem(parent),
    m_ptrCamera(0),
    m_ptrTexture(0),
    m_frameChanged(false),
    m_glContext(0),
    m_cameraIndex(0)
{
    setFlag(ItemHasContents, true);
    connect(this,SIGNAL(cameraIndexChanged(int)),this,SLOT(slot_cameraLoad(int)));

}


void QmlCamera::updateBack()
{
    slot_refresh();
}

void QmlCamera::close()
{
    this->deleteLater();
}



void QmlCamera::slot_refresh()
{
    QCoreApplication::postEvent(this,new QEvent(QEvent::User));
}

bool QmlCamera::event(QEvent *e)
{
    if(e->type() != QEvent::User)
        return QQuickItem::event(e);
    update();
    return true;
}

void QmlCamera::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    Q_UNUSED(newGeometry);
    Q_UNUSED(oldGeometry);
    QQuickItem::geometryChanged(newGeometry,oldGeometry); //geometry will be updated
    update();

}
//class  QSGVideoNode : public QSGGeometryNode
//{
//public:
//    enum FrameFlag {
//        FrameFiltered = 0x01
//    };
//    Q_DECLARE_FLAGS(FrameFlags, FrameFlag)

//    QSGVideoNode();

//    virtual void setCurrentFrame(const QVideoFrame &frame, FrameFlags flags) = 0;
//    virtual QVideoFrame::PixelFormat pixelFormat() const = 0;
//    virtual QAbstractVideoBuffer::HandleType handleType() const = 0;

//    void setTexturedRectGeometry(const QRectF &boundingRect, const QRectF &textureRect, int orientation);

//private:
//    QRectF m_rect;
//    QRectF m_textureRect;
//    int m_orientation;
//};

struct TexturedCubeVertex
{
    QVector3D               position;
    QVector2D               texCoord;
};

QSGNode *QmlCamera::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *data)
{

    QMutexLocker t_locker(&m_frameMutex);
    Q_UNUSED(t_locker);

    QSGSimpleTextureNode* node = Q_NULLPTR;

    if(m_ptrCamera == Q_NULLPTR){
        return oldNode;
    }
    cv::Mat matImg;
    if(!m_ptrCamera->getCameraSrc(matImg))
    {
        return oldNode;
    }


    QImage m_image = cvMat2QImage(matImg);

//        QImage m_image(":/image/qml/Images/page/loading/loading_pro_f.png");

    if(m_ptrTexture)
        delete m_ptrTexture;
     m_ptrTexture = window()->createTextureFromImage(m_image);

    if(oldNode == Q_NULLPTR)
    {
        node = new QSGSimpleTextureNode();
        node->setRect(boundingRect());
        node->setTexture(m_ptrTexture);
        node->markDirty(QSGNode::DirtyGeometry);

    }else{
        node = static_cast<QSGSimpleTextureNode*> (oldNode);
        QSGNode::DirtyState dirtyState = QSGNode::DirtyGeometry;
        node->setRect(boundingRect());
        node->setTexture(m_ptrTexture);
        node->markDirty(dirtyState);

    }

    return node;
}

QSGNode *QmlCamera::updatePaintNode_old(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *data)
{

    //创建几何体
    QSGGeometryNode* node = Q_NULLPTR;
    QSGGeometry* geometry = Q_NULLPTR;

    if ( oldNode == Q_NULLPTR )
    {
        // 创建几何体
        static QSGGeometry::Attribute attributes[] =
        {
            QSGGeometry::Attribute::create(
            0,                  // 数组元素的下标
            3,                  // 元的个数
            GL_FLOAT,           // 元的类型
            true ),             // 是不是顶点位置元素
            QSGGeometry::Attribute::create(
            1,                  // 数组元素的下标
            2,                  // 元的个数
            GL_FLOAT,           // 元的类型
            false )             // 是不是顶点位置元素
        };
        static QSGGeometry::AttributeSet attributeSet =
        {
            2,                              // 属性的个数
            sizeof( TexturedCubeVertex ),   // 个性化顶点结构的大小
            attributes                      // 属性的数组
        };

        geometry = new QSGGeometry(
                    attributeSet,
                    VERTEX_COUNT);
        geometry->setDrawingMode(GL_TRIANGLES);
        geometry->setVertexDataPattern(QSGGeometry::DynamicPattern);
        geometry->allocate(VERTEX_COUNT);


        //创建材质
        QImage image(":/image/qml/Images/page/loading/loading_pro_f.png");
        QSGTexture* texture = window()->createTextureFromImage(image.mirrored());
        texture->setParent(window());
        QSGOpaqueTextureMaterial* material = new QSGOpaqueTextureMaterial;
        material->setTexture(texture);

        //创建节点
        node = new QSGGeometryNode;
        node->setGeometry(geometry);
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsGeometry);
        node->setFlag(QSGNode::OwnsOpaqueMaterial);
        node->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
    }
    else
    {
        node = static_cast<QSGGeometryNode*> (oldNode);
        geometry = node->geometry();
        QSGNode::DirtyState dirtyState = QSGNode::DirtyGeometry;
        node->markDirty(dirtyState);
    }


    TexturedCubeVertex* v = static_cast<TexturedCubeVertex*>( geometry->vertexData( ) );


    const QRectF bounding = boundingRect();
    const float factor = 1.0;

    qreal semi = 50;

    const QVector3D basicVertices[]=
    {
        QVector3D( semi, -semi, semi ),
        QVector3D( semi, -semi, -semi ),
        QVector3D( -semi, -semi, -semi ),
        QVector3D( -semi, -semi, semi ),

        QVector3D( semi, semi, semi ),
        QVector3D( semi, semi, -semi ),
        QVector3D( -semi, semi, -semi ),
        QVector3D( -semi, semi, semi )
    };

    // 前面
    v[0].position = basicVertices[6]; v[0].texCoord = QVector2D( 0.0, 0.0 );
    v[1].position = basicVertices[2]; v[1].texCoord = QVector2D( 0.0, 1.0 );
    v[2].position = basicVertices[5]; v[2].texCoord = QVector2D( 1.0, 0.0 );
    v[3].position = basicVertices[2]; v[3].texCoord = QVector2D( 0.0, 1.0 );
    v[4].position = basicVertices[1]; v[4].texCoord = QVector2D( 1.0, 1.0 );
    v[5].position = basicVertices[5]; v[5].texCoord = QVector2D( 1.0, 0.0 );

    // 后面
    v[6].position = basicVertices[4]; v[6].texCoord = QVector2D( 0.0, 0.0 );
    v[7].position = basicVertices[0]; v[7].texCoord = QVector2D( 0.0, 1.0 );
    v[8].position = basicVertices[7]; v[8].texCoord = QVector2D( 1.0, 0.0 );
    v[9].position = basicVertices[0]; v[9].texCoord = QVector2D( 0.0, 1.0 );
    v[10].position = basicVertices[3]; v[10].texCoord = QVector2D( 1.0, 1.0 );
    v[11].position = basicVertices[7]; v[11].texCoord = QVector2D( 1.0, 0.0 );

    // 上面
    v[12].position = basicVertices[2]; v[12].texCoord = QVector2D( 0.0, 0.0 );
    v[13].position = basicVertices[3]; v[13].texCoord = QVector2D( 0.0, 1.0 );
    v[14].position = basicVertices[1]; v[14].texCoord = QVector2D( 1.0, 0.0 );
    v[15].position = basicVertices[3]; v[15].texCoord = QVector2D( 0.0, 1.0 );
    v[16].position = basicVertices[0]; v[16].texCoord = QVector2D( 1.0, 1.0 );
    v[17].position = basicVertices[1]; v[17].texCoord = QVector2D( 1.0, 0.0 );

    // 下面
    v[18].position = basicVertices[7]; v[18].texCoord = QVector2D( 0.0, 0.0 );
    v[19].position = basicVertices[6]; v[19].texCoord = QVector2D( 0.0, 1.0 );
    v[20].position = basicVertices[4]; v[20].texCoord = QVector2D( 1.0, 0.0 );
    v[21].position = basicVertices[6]; v[21].texCoord = QVector2D( 0.0, 1.0 );
    v[22].position = basicVertices[5]; v[22].texCoord = QVector2D( 1.0, 1.0 );
    v[23].position = basicVertices[4]; v[23].texCoord = QVector2D( 1.0, 0.0 );

    // 左面
    v[24].position = basicVertices[7]; v[24].texCoord = QVector2D( 0.0, 0.0 );
    v[25].position = basicVertices[3]; v[25].texCoord = QVector2D( 0.0, 1.0 );
    v[26].position = basicVertices[6]; v[26].texCoord = QVector2D( 1.0, 0.0 );
    v[27].position = basicVertices[3]; v[27].texCoord = QVector2D( 0.0, 1.0 );
    v[28].position = basicVertices[2]; v[28].texCoord = QVector2D( 1.0, 1.0 );
    v[29].position = basicVertices[6]; v[29].texCoord = QVector2D( 1.0, 0.0 );

    // 右面
    v[30].position = basicVertices[5]; v[30].texCoord = QVector2D( 0.0, 0.0 );
    v[31].position = basicVertices[1]; v[31].texCoord = QVector2D( 0.0, 1.0 );
    v[32].position = basicVertices[4]; v[32].texCoord = QVector2D( 1.0, 0.0 );
    v[33].position = basicVertices[1]; v[33].texCoord = QVector2D( 0.0, 1.0 );
    v[34].position = basicVertices[0]; v[34].texCoord = QVector2D( 1.0, 1.0 );
    v[35].position = basicVertices[4]; v[35].texCoord = QVector2D( 1.0, 0.0 );




    for ( int i = 0; i < VERTEX_COUNT; ++i )// 调整位置
    {
        // 这里由于坐标系x向右增大，y向下增大，符合屏幕坐标系，那么根据OpenGL右手坐标系，
        // Z轴朝里面增大。
        float x = v[i].position.x( ) + bounding.width( ) / 2;
        float y = v[i].position.y( ) + bounding.height( ) / 2;
        float z = ( v[i].position.z( ) + semi ) / factor;
        v[i].position.setX( x );
        v[i].position.setY( y );
        v[i].position.setZ( z );
    }

    return node;

}






//struct MyPoint2D {
//    float x;
//    float y;
//    float r;
//    float g;
//    float b;
//    float a;

//    void set(float x_, float y_, float r_, float g_, float b_, float a_) {
//        x = x_;
//        y = y_;
//        r = r_;
//        g = g_;
//        b = b_;
//        a = a_;
//    }
//};

//QSGGeometry::Attribute MyPoint2D_Attributes[] = {
//    QSGGeometry::Attribute::create(0, 2, GL_FLOAT, true),
//    QSGGeometry::Attribute::create(1, 4, GL_FLOAT, false)
//};

//QSGGeometry::AttributeSet MyPoint2D_AttributeSet = {
//    2,
//    sizeof(MyPoint2D),
//    MyPoint2D_Attributes
//};



//QSGGeometry geometry = new QSGGeometry(MyPoint2D_AttributeSet, 2);
//geometry->setDrawingMode(GL_LINES);




//// 创建几何体
// static QSGGeometry::Attribute attributes[] =
// {
//     QSGGeometry::Attribute::create(
//     0,                  // 数组元素的下标
//     3,                  // 元的个数
//     GL_FLOAT,           // 元的类型
//     true ),             // 是不是顶点位置元素
//     QSGGeometry::Attribute::create(
//     1,                  // 数组元素的下标
//     2,                  // 元的个数
//     GL_FLOAT,           // 元的类型
//     false )             // 是不是顶点位置元素
// };
// static QSGGeometry::AttributeSet attributeSet =
// {
//     2,                              // 属性的个数
//     sizeof( TexturedCubeVertex ),   // 个性化顶点结构的大小
//     attributes                      // 属性的数组
// };
// geometry = new QSGGeometry(
//             attributeSet,       // 属性集
//             VERTEX_COUNT );     // 顶点个数
// geometry->setDrawingMode( GL_TRIANGLES );
// geometry->setVertexDataPattern( QSGGeometry::DynamicPattern );
// geometry->allocate( VERTEX_COUNT );


// // 创建材质
// QImage image( ":/2000 (3).jpg");
// QSGTexture* texture = window( )->createTextureFromImage(
//             image.mirrored( ) );
// texture->setParent( window( ) );
// QSGOpaqueTextureMaterial* material = new QSGOpaqueTextureMaterial;
// material->setTexture( texture );


//// 创建节点
//if(!node){
//     n = new QSGGeometryNode();
//}
//n->setGeometry( geometry );
//n->setMaterial( material );
//n->setFlag( QSGNode::OwnsGeometry );
//n->setFlag( QSGNode::OwnsOpaqueMaterial );
//n->markDirty( QSGNode::DirtyGeometry | QSGNode::DirtyMaterial );

//return n;



//void QmlCamera::drawFrame()
//{






//    if(m_texture)
//        delete m_texture;

//    cv::Mat matImg;
//    m_ptrCamera->getCameraSrc(matImg);
//    m_image = cvMat2QImage(matImg);
//    m_texture = window()->createTextureFromImage(m_image);

//    static_cast<QSGSimpleTextureNode*>(m_node)->setRect(boundingRect());
//    static_cast<QSGSimpleTextureNode*>(m_node)->setTexture(m_texture);
//    m_node->markDirty(QSGNode::DirtyGeometry);
//}




//void QmlCamera::drawFrameTest(QSGNode *node)
//{
//    QMutexLocker t_locker(&m_img_mutex);
//    Q_UNUSED(t_locker);
//    qDebug() << "sdfsdfsfsdfsfsdfsdf11111";


//    QSGSimpleRectNode *n = static_cast<QSGSimpleRectNode *>(node);
//    if (!n) {
//        n = new QSGSimpleRectNode();
//        n->setColor(Qt::red);
//    }
//    n->setRect(boundingRect());

//}




//class GraphNode : public QSGNode
//{
//public:
//    QSGSimpleRectNode *background;
//    QSGGeometryNode *frontframe;
//};


//    if(m_texture)
//        delete m_texture;

//    cv::Mat matImg;
//    m_ptrCamera->getCameraSrc(matImg);
//    m_image = cvMat2QImage(matImg);

//    m_image = QImage(":/image/qml/Images/page/base/btn_ret_pressed.png");

//    if(!m_image.isNull()){
//        m_texture = window()->createTextureFromImage(m_image);

//        QSGSimpleTextureNode *bg = new QSGSimpleTextureNode();

//        m_out_rect.moveCenter(boundingRect().center().toPoint());
//        bg->setRect(boundingRect());
//        bg->setTexture(m_texture);
//        n->appendChildNode(bg);

//    }



