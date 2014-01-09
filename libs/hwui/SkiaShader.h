/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_HWUI_SKIA_SHADER_H
#define ANDROID_HWUI_SKIA_SHADER_H

#include <SkShader.h>
#include <SkXfermode.h>

#include <GLES2/gl2.h>

#include <cutils/compiler.h>

#include "Extensions.h"
#include "ProgramCache.h"
#include "TextureCache.h"
#include "GradientCache.h"

namespace android {
namespace uirenderer {

class Caches;
class Layer;

/**
 * Type of Skia shader in use.
 */
enum SkiaShaderType {
    kNone_SkiaShaderType,
    kBitmap_SkiaShaderType,
    kGradient_SkiaShaderType,
    kCompose_SkiaShaderType,
    kLayer_SkiaShaderType
};

class SkiaShader {
public:
    static SkiaShaderType getType(const SkShader& shader);
    static void describe(Caches* caches, ProgramDescription& description,
            const Extensions& extensions, const SkShader& shader);
    static void setupProgram(Caches* caches, const mat4& modelViewMatrix,
            GLuint* textureUnit, const Extensions& extensions, const SkShader& shader);
};

class InvalidSkiaShader {
public:
    static void describe(Caches* caches, ProgramDescription& description,
            const Extensions& extensions, const SkShader& shader) {
        // This shader is unsupported. Skip it.
    }
    static void setupProgram(Caches* caches, const mat4& modelViewMatrix,
            GLuint* textureUnit, const Extensions& extensions, const SkShader& shader) {
        // This shader is unsupported. Skip it.

    void setMatrix(SkMatrix* matrix) {
        updateLocalMatrix(matrix);
        mGenerationId++;
    }

    void updateLocalMatrix(const SkMatrix* matrix) {
        if (matrix) {
            mat4 localMatrix(*matrix);
            mShaderMatrix.loadInverse(localMatrix);
        } else {
            mShaderMatrix.loadIdentity();
        }
    }

    void computeScreenSpaceMatrix(mat4& screenSpace, const mat4& modelView);

protected:
    SkiaShader();

    /**
     * The appropriate texture unit must have been activated prior to invoking
     * this method.
     */
    inline void bindTexture(Texture* texture, GLenum wrapS, GLenum wrapT);

    Type mType;
    SkShader* mKey;
    SkShader::TileMode mTileX;
    SkShader::TileMode mTileY;
    bool mBlend;

    Caches* mCaches;

    mat4 mUnitMatrix;
    mat4 mShaderMatrix;

private:
    uint32_t mGenerationId;
}; // struct SkiaShader


///////////////////////////////////////////////////////////////////////////////
// Implementations
///////////////////////////////////////////////////////////////////////////////

/**
 * A shader that draws a bitmap.
 */
struct SkiaBitmapShader: public SkiaShader {
    ANDROID_API SkiaBitmapShader(SkBitmap* bitmap, SkShader* key, SkShader::TileMode tileX,
            SkShader::TileMode tileY, SkMatrix* matrix, bool blend);
    SkiaShader* copy();

    void describe(ProgramDescription& description, const Extensions& extensions);
    void setupProgram(Program* program, const mat4& modelView, const Snapshot& snapshot,
            GLuint* textureUnit);

private:
    SkiaBitmapShader() : mBitmap(NULL), mTexture(NULL) {
    }

};
/**
 * A shader that draws a layer.
 */
class SkiaLayerShader {
public:
    static void describe(Caches* caches, ProgramDescription& description,
            const Extensions& extensions, const SkShader& shader);
    static void setupProgram(Caches* caches, const mat4& modelViewMatrix,
            GLuint* textureUnit, const Extensions& extensions, const SkShader& shader);
}; // class SkiaLayerShader

/**
 * A shader that draws a bitmap.
 */
class SkiaBitmapShader {
public:
    static void describe(Caches* caches, ProgramDescription& description,
            const Extensions& extensions, const SkShader& shader);
    static void setupProgram(Caches* caches, const mat4& modelViewMatrix,
            GLuint* textureUnit, const Extensions& extensions, const SkShader& shader);


}; // class SkiaBitmapShader

/**
 * A shader that draws one of three types of gradient, depending on shader param.
 */
class SkiaGradientShader {
public:
    static void describe(Caches* caches, ProgramDescription& description,
            const Extensions& extensions, const SkShader& shader);
    static void setupProgram(Caches* caches, const mat4& modelViewMatrix,
            GLuint* textureUnit, const Extensions& extensions, const SkShader& shader);
};

/**
 * A shader that draws two shaders, composited with an xfermode.
 */
class SkiaComposeShader {
public:
    static void describe(Caches* caches, ProgramDescription& description,
            const Extensions& extensions, const SkShader& shader);
    static void setupProgram(Caches* caches, const mat4& modelViewMatrix,
            GLuint* textureUnit, const Extensions& extensions, const SkShader& shader);
}; // class SkiaComposeShader

}; // namespace uirenderer
}; // namespace android

#endif // ANDROID_HWUI_SKIA_SHADER_H
