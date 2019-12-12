//
// Created by stone on 19-12-12.
//

#ifndef PROJECTSTUB_GLRENDER_H
#define PROJECTSTUB_GLRENDER_H

#include <pthread.h>
#include <android/native_window.h>
#include <EGL/egl.h>
#include <GLES/gl.h>

class GLRender {
public:
    GLRender();

    virtual ~GLRender();

    virtual int init(EGLDisplay *display, const EGLint* configAttribs, EGLConfig* config);

    virtual int draw();

    virtual GLuint getTextureID();

    virtual int useProgram();

    virtual int compileShader(GLuint *shaderid, const char *shaderSource, GLenum type);



private:
    GLint  vertexLoc = -1;// gpu vertex handle
    GLint  textureLoc = -1;//gpu texture handle
    GLint  yuvtextLoc = -1;//gpu yuv texture handle
    GLuint     mProgramID ;/*
    EGLDisplay mEglDisplay = EGL_NO_DISPLAY;
    EGLSurface mEglSurface = EGL_NO_SURFACE;
    EGLContext mEglContext = EGL_NO_CONTEXT;
    EGLConfig  mGlConfig = NULL;*/
};


#endif //PROJECTSTUB_GLRENDER_H
