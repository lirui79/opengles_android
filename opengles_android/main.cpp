/*main.cpp*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sys/prctl.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
 
#include "GLUtil.h"

using namespace android;


int initDisplay(EGLDisplay *display, const EGLint* configAttribs, EGLConfig* config) {
    display[0] = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLint majorVersion;
    EGLint minorVersion;
    eglInitialize(display[0], &majorVersion, &minorVersion);
    EGLint numConfigs = 0;
    eglChooseConfig(display[0], configAttribs, config, 1, &numConfigs);
    return 0;
}

int main(int argc, const char* argv[])
{
    const EGLint configAttribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_NONE };

    const EGLint contextAttribs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE };

    EGLDisplay mEglDisplay = EGL_NO_DISPLAY;
    EGLSurface mEglSurface = EGL_NO_SURFACE;
    EGLContext mEglContext = EGL_NO_CONTEXT;
    EGLConfig  mGlConfig = NULL;

    if (argc < 3) {
        printf(" w  h \n");
        return -1;
    }

    int width = atoi(argv[1]);
    int height = atoi(argv[2]);

    if (initDisplay(&mEglDisplay, configAttribs, &mGlConfig) < 0) {
        printf("opengl initdisplay error!\n");
        return -2;
    }

    GLUtil util;
    ANativeWindow* window = util.getNativeWindow(width, height);

    mEglSurface = eglCreateWindowSurface(mEglDisplay, mGlConfig, window , NULL);
    mEglContext = eglCreateContext(mEglDisplay, mGlConfig, EGL_NO_CONTEXT, contextAttribs);

    eglSurfaceAttrib(mEglDisplay, mEglSurface, EGL_RENDER_BUFFER, EGL_SINGLE_BUFFER);

    eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext);
    const char *vertexShaderSource = "attribute vec4 vPosition;\n"
                                     "attribute vec4 vFragColor;\n"
                                     "varying   vec4 fragColor;\n"
                                     "void main()\n"
                                     "{\n"
                                     "gl_Position = vPosition;\n"
                                     "fragColor = vFragColor;\n"
                                     "}";
    const char *fragmentShaderSource = "varying vec4   fragColor;\n"
                                       "void main() {\n"
                                       "  gl_FragColor = fragColor;\n"
                                       "}";

    GLint programID =  util.createProgram(vertexShaderSource, fragmentShaderSource);
    if (programID <= 0) {
        printf("create program\n");
        return -1;
    }

    GLint positionHandle = glGetAttribLocation(programID, "vPosition");
    GLint fragcolorHandle = glGetAttribLocation(programID, "vFragColor");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float vertices[] = {
         0.5f, -0.5f, 0.0f,  1.0f,  // bottom righ
        -0.5f, -0.5f, 0.0f,  1.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  1.0f  // top
    };

    float colors[] = {
         1.0f, 0.0f, 0.0f, 1.0f, // bottom right
         0.0f, 1.0f, 0.0f, 1.0f, // bottom left
         0.0f, 0.0f, 1.0f, 1.0f // top
    };

    glVertexAttribPointer(positionHandle, 4, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(positionHandle);

    glVertexAttribPointer(fragcolorHandle, 4, GL_FLOAT, GL_FALSE, 0, colors);
    glEnableVertexAttribArray(fragcolorHandle);

    /*
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);*/


    glViewport(0, 0, width, height);

    while(1) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glUseProgram(programID);
      //  glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        eglSwapBuffers(mEglDisplay, mEglSurface);
       // glBindVertexArray(0);
        glUseProgram(0);
        usleep(16000);
        printf("ondraw\n");
    }

    
    return 0;
}

