#ifndef NV12RENDER_H
#define NV12RENDER_H
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include <QtGui/qvector3d.h>
#include <QtGui/qmatrix4x4.h>
#include <QtGui/qopenglshaderprogram.h>
#include <QtGui/qopenglfunctions.h>
#include <QtGui/QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QTime>
#include <QVector>

class Nv12Render : protected QOpenGLFunctions
{
public:
    Nv12Render();
    ~Nv12Render();

    void render();
    void renderFrame(uchar* imgPtr, uint videoW,uint videoH);
    void initialize();

private:

    qreal   m_fAngle;
    qreal   m_fScale;

    QOpenGLShaderProgram program1;

private:
    QOpenGLShaderProgram *program;
    QOpenGLBuffer vbo;
    GLuint textureUniformY,textureUniformU,textureUniformV; //opengl中y、u、v分量位置
    QOpenGLTexture *textureY = nullptr,*textureU = nullptr,*textureV = nullptr;
    GLuint idY,idU,idV; //自己创建的纹理对象ID，创建错误返回0
    uint videoW,videoH;
    uchar *yuvPtr = nullptr;
//    QOpenGLShaderProgram *program;
//    QOpenGLBuffer vbo;
//    GLuint textureUniformY,textureUniformU,textureUniformV; //opengl中y、u、v分量位置
//    QOpenGLTexture *textureY = nullptr,*textureU = nullptr,*textureV = nullptr;
//    GLuint idY,idU,idV; //自己创建的纹理对象ID，创建错误返回0
//    uint videoW,videoH;
//    uchar *yuvPtr = nullptr;

//    //QOpenGLBuffer vbo;

//    //QOpenGLTexture *textureY;
//    QOpenGLTexture *textureUV;

//   // GLuint idY;
//    GLuint idUV;
};

#endif // NV12RENDER_H
