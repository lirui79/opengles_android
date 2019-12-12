//
// Created by stone on 19-12-12.
//

#include "GLRender.h"


GLRender::GLRender() {}

GLRender::~GLRender() {
}

int GLRender::init(EGLDisplay *display, const EGLint* configAttribs, EGLConfig* config) {
    display[0] = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLint majorVersion;
    EGLint minorVersion;
    eglInitialize(display[0], &majorVersion, &minorVersion);
    EGLint numConfigs = 0;
    eglChooseConfig(display[0], configAttribs, config, 1, &numConfigs);
    return 0;
}

int GLRender::draw() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(programID);
// vertex array
    GLfloat vextexData[] = {
            -1.0f,  -1.0f,
            1.0f, -1.0f,
            1.0f, 1.0f,
            -1.0f,  1.0f,
    };
    glVertexAttribPointer(vertexLoc, 2, GL_FLOAT, GL_FALSE, 0, vextexData);
    glEnableVertexAttribArray(vertexLoc);
//顶点颜色数组
    float colorData[] = {
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
    };
    glVertexAttribPointer(textureLoc, 2, GL_FLOAT, GL_FALSE, 0, colorData);
    glEnableVertexAttribArray(textureLoc);

    glUniform1i(yuvtextLoc, 0);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, textureID);
    GLint drawOrder[] = { 0, 1, 2, 0, 2, 3 };
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, drawOrder);

    glDisableVertexAttribArray(vertexLoc);
    glDisableVertexAttribArray(textureLoc);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, 0);
    glUseProgram(0);
    return 0;
}

GLuint GLRender::getTextureID() {
    GLuint texture = -1;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, texture);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return texture;
}

int GLRender::useProgram() {
    GLuint vertexShader = -1; // vertex shader id
    GLuint fragmentShader = -1; // fragment shader id

    const char* vertexShaderCode =
            "attribute vec4 aPosition;\n"
            "attribute vec2 aTextureCoord;\n"
            "varying vec2   vTextureCoord;\n"
            "void main()\n"
            "{\n"
            "gl_Position = aPosition;\n"
            "vTextureCoord = aTextureCoord;\n"
            "}";

    const char* fragmentShaderCode =
            "#extension GL_OES_EGL_image_external : require\n"
            "precision mediump float;\n"
            "varying vec2 vTextureCoord;\n"
            "uniform samplerExternalOES yuvTextureID;\n"
            "void main() {\n"
            "  gl_FragColor = texture2D(yuvTextureID, vTextureCoord);\n"
            "}";

    if (useShader(vertexShaderCode, GL_VERTEX_SHADER, &vertexShader) < 0) {
        return -1;// vertex shader id
    }

    if (useShader(fragmentShaderCode, GL_FRAGMENT_SHADER, &fragmentShader) < 0) {
        return -1;// fragment shader id
    }

    mProgramID = glCreateProgram() ;// program id

    glAttachShader(mProgramID, vertexShader);
    glAttachShader(mProgramID, fragmentShader);

    glLinkProgram(mProgramID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    vertexShader = -1;
    fragmentShader = -1;

    vertexLoc = glGetAttribLocation(mProgramID, "aPosition");
    textureLoc = glGetAttribLocation(mProgramID, "aTextureCoord");
    yuvtextLoc = glGetUniformLocation(mProgramID, "yuvTextureID");

    return 0;
}

int GLRender::compileShader(GLuint *shaderid, const char *shaderSource, GLenum type) {
//创建着色器对象：顶点着色器
    shaderid[0] = glCreateShader(type);
//将字符数组绑定到对应的着色器对象上
    glShaderSource(shaderid[0], 1, &shaderSource, NULL);
//编译着色器对象
    glCompileShader(shaderid[0]);
//检查编译是否成功
    GLint compileResult;
    glGetShaderiv(shaderid[0], GL_COMPILE_STATUS, &compileResult);
    if (GL_FALSE == compileResult) {
        printf("useShader %s %d %d fail\n", shaderSource, type, shaderid[0]);
        glDeleteShader(shaderid[0]);
        return -1;
    }
    return 0;
}