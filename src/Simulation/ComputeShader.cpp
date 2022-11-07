#include "ComputeShader.hpp"

#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLVersionFunctionsFactory>

static QOpenGLFunctions_4_3_Core 
*gl()
{
    QSurfaceFormat fmt;
    fmt.setVersion(4,3);
    fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
    fmt.setOptions(QSurfaceFormat::DeprecatedFunctions);
    QSurfaceFormat::setDefaultFormat(fmt);

    return QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_3_Core>(QOpenGLContext::currentContext());
}

void
SSBO::initialize(int binding_index)
{
    _qtBuffer.create();
    _qtBuffer.setUsagePattern(QOpenGLBuffer::StreamRead);
    gl()->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_index,
                           _qtBuffer.bufferId());
}

void
SSBO::uploadData(void *data, int size_bytes)
{
    _qtBuffer.bind();
    _qtBuffer.allocate(data, size_bytes);
    _qtBuffer.release();
}

void
SSBO::downloadData(void *data, int size_bytes)
{
    _qtBuffer.bind();
    _qtBuffer.read(0, data, size_bytes);
    _qtBuffer.release();
}


void 
ComputeShader::initialize(QString shader_file)
{
    _qtProgram.addShaderFromSourceFile(QOpenGLShader::Compute, shader_file);
    _qtProgram.link();
}

void
ComputeShader::compute(GLuint nbComputations)
{
    _qtProgram.setUniformValue("NumberOfComputations", nbComputations);
    gl()->glDispatchCompute((nbComputations - 1) / 256 + 1, 1, 1);
    gl()->glFinish();
}