#include "opencvgl.hpp"

OpenCVGL::OpenCVGL(QWidget *parent)
    : QGLWidget(parent)
{
    setFormat(QGLFormat(QGL::SampleBuffers));

    rotx = -38.0;
    roty = -58.0;
    rotz = 0.0;
    scaling = 1.0;

    timer = new QTimer();
    timer->setInterval(33);

    connect(timer,SIGNAL(timeout()),this,SLOT(updateGL()));

    if(!cap.open(0))
        exit(-1);
    else
        cap >> cframe;

    timer->start();
}

OpenCVGL::~OpenCVGL() { }

void OpenCVGL::resizeGL(int w, int h)
{
    h = h ? h : 1;

    glViewport( 0, 0, (GLint)w, (GLint)h );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void OpenCVGL::initializeGL()
{
    updateTexture();

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void OpenCVGL::paintGL()
{
    rotx += 0.3f;
    roty += 0.2f;
    rotz += 0.4f;

    updateTexture();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f,0.0f,-5.0f);

    glRotatef(rotx,1.0f,0.0f,0.0f);
    glRotatef(roty,0.0f,1.0f,0.0f);
    glRotatef(rotz,0.0f,0.0f,1.0f);

    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
//    // Back Face
//    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
//    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
//    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
//    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
//    // Top Face
//    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
//    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
//    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
//    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
//    // Bottom Face
//    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
//    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
//    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
//    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
//    // Right face
//    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
//    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
//    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
//    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
//    // Left Face
//    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
//    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
//    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
//    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();
}

void OpenCVGL::updateTexture()
{
    cap >> cframe;

    cv::split(cframe, channels);
    channels.push_back(Mat(cframe.size(), CV_8UC1, cv::Scalar(255)));
    cv::merge(channels, cframe);

    img = QImage(cframe.data,
                 cframe.size().width,
                 cframe.size().height,
                 cframe.step,
                 QImage::Format_ARGB32 );

    t = QGLWidget::convertToGLFormat(img);
    glGenTextures( 1, &texture[0] );
    glBindTexture( GL_TEXTURE_2D, texture[0] );
    glTexImage2D( GL_TEXTURE_2D,
                  0,
                  3,
                  t.width(),
                  t.height(),
                  0,
                  GL_RGBA,
                  GL_UNSIGNED_BYTE,
                  t.bits()
                  );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}
