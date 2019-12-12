#include "GLUtil.h"
 
int GLUtil::compileShader(int type, const char * shaderCode)
{
 
    int shader = glCreateShader(type);
    if (shader == 0)
    {
        printf("Create Sharder error %d", glGetError());
        return 0;
    }
 
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);
    GLint compileStatus = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (!compileStatus)
    {
        char infoLog[512];
         glGetShaderInfoLog(shader, 512, NULL, infoLog);
         glDeleteShader(shader);
        printf("ERROR::SHADER::COMPILATION_FAILED %d %s  %s\n",shader,shaderCode,infoLog);
        return 0;
    }
 
    return shader;
}

GLint GLUtil::createProgram(const char *vertexShaderCode, const char *fragmentShaderCode)
{
    GLint program = glCreateProgram();
    if (0 == program) {
        printf("ERROR::PROGRAM::CREATE_FAILED %d \n",program);
        return 0;
    }

    printf("create program success\n");
    int vertexShaderID = compileShader(GL_VERTEX_SHADER, vertexShaderCode);
    int fragmentShaderID = compileShader(GL_FRAGMENT_SHADER, fragmentShaderCode);
 
    glAttachShader(program, vertexShaderID);
    glAttachShader(program, fragmentShaderID);
    glLinkProgram(program);
    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (GL_FALSE == linkStatus) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        glDeleteProgram(program);
        printf("ERROR::PROGRAM::ATTACH_FAILED %d %s\n",program,infoLog);
        return 0;
    }
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    return program;
}
 
EGLNativeWindowType GLUtil::getNativeWindow(int width, int height)
{
    DisplayInfo dinfo;
    
    mComposerClient = new SurfaceComposerClient();
    sp<IBinder> dtoken(SurfaceComposerClient::getBuiltInDisplay(ISurfaceComposer::eDisplayIdMain));
 
    status_t status = SurfaceComposerClient::getDisplayInfo(dtoken, &dinfo);
    printf("w=%d,h=%d,xdpi=%f,ydpi=%f,fps=%f,ds=%f\n", 
                dinfo.w, dinfo.h, dinfo.xdpi, dinfo.ydpi, dinfo.fps, dinfo.density);
    if (status != OK) {
        return  NULL;
    }
 
    mSurfaceControl = mComposerClient->createSurface(
        String8("Test Surface"),
        dinfo.w, dinfo.h,
        PIXEL_FORMAT_RGBA_8888, 0);

    /*
    SurfaceComposerClient::openGlobalTransaction();
    mSurfaceControl->setLayer(INT_MAX);//设定Z坐标
    mSurfaceControl->setPosition((dinfo.w - width) / 2, (dinfo.h - height) / 2);
    mSurfaceControl->setSize(width, height);
 
    SurfaceComposerClient::closeGlobalTransaction();*/

    SurfaceComposerClient::Transaction transaction ;
  //  transaction.setDisplayLayerStack(dtoken, layerStack);
 //   transaction.setDisplayProjection(dtoken, dinfo.orientation, layerRect, dispRect);
 //   transaction.setLayerStack(mSurfaceControl, layerStack);
    transaction.setSize(mSurfaceControl, width, height);
    transaction.setPosition(mSurfaceControl, (dinfo.w - width) / 2, (dinfo.h - height) / 2);//以左上角为(0,0)设定显示位置
    transaction.show(mSurfaceControl);
    transaction.apply();
 
    sp<ANativeWindow> window = mSurfaceControl->getSurface();
 
    return window.get();
}
 
 
void GLUtil::disposeNativeWindow(void)
{
    if (mComposerClient != NULL) 
    {
        mComposerClient->dispose();
        mComposerClient = NULL;
        mSurfaceControl = NULL;
    }
}
