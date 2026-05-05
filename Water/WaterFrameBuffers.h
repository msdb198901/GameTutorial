#pragma once

class WaterFrameBuffers {
public:
    static const int REFLECTION_WIDTH;
    static const int REFLECTION_HEIGHT;
    static const int REFRACTION_WIDTH;
    static const int REFRACTION_HEIGHT;

    WaterFrameBuffers();
    ~WaterFrameBuffers();           // 推荐使用 RAII，在析构中清理

    void CleanUp();
    void BindReflectionFrameBuffer();
    void BindRefractionFrameBuffer();
    void UnBindCurrentFrameBuffer(int screenWidth, int screenHeight);  // 传入当前窗口尺寸

    GLuint GetReflectionTexture() const { return reflectionTexture; }
    GLuint GetRefractionTexture() const { return refractionTexture; }
    GLuint GetRefractionDepthTexture() const { return refractionDepthTexture; }

private:
    GLuint reflectionFrameBuffer;
    GLuint reflectionTexture;
    GLuint reflectionDepthBuffer;   // 实际上是 Renderbuffer

    GLuint refractionFrameBuffer;
    GLuint refractionTexture;
    GLuint refractionDepthTexture;   // 是深度纹理（用于折射）

    void InitialiseReflectionFrameBuffer();
    void InitialiseRefractionFrameBuffer();
    void BindFrameBuffer(int frameBuffer, int width, int height);
    GLuint CreateFrameBuffer();
    GLuint CreateTextureAttachment(int width, int height);
    GLuint CreateDepthTextureAttachment(int width, int height);
    GLuint CreateDepthBufferAttachment(int width, int height);
};