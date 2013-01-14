#ifndef OPENCVGL_HPP
#define OPENCVGL_HPP

#include <QtGui>
#include <QGLWidget>
#include <QTimer>
#include <QDebug>

// opencv2
#include <opencv2/opencv.hpp>
using cv::Mat;
using cv::VideoCapture;

#include <vector>
using std::vector;

class OpenCVGL : public QGLWidget
{
    Q_OBJECT

public:
    OpenCVGL(QWidget *parent = 0);
     ~OpenCVGL();

protected:
    void paintGL();
    void initializeGL();
    void resizeGL(int w, int h);

private:
    void updateTexture();

    GLuint texture[1];
    GLfloat rotx;
    GLfloat roty;
    GLfloat rotz;
    GLfloat scaling;
    QPoint lastPos;

    // opencv2
    VideoCapture cap;
    Mat cframe;
    vector< Mat > channels;

    QTimer *timer;
    QImage img,t;
};

#endif // OPENCVGL_HPP
