#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>

#include "PixelRenderer.h"

#include <algorithm>

// RGBA channels
#define CHAN_COUNT 4

PixelRenderer::PixelRenderer(int width, int height, void *buffer)
{
        m_program = 0;
	setBufferSize(width, height);
	setPixelBuffer(buffer);
}


PixelRenderer::~PixelRenderer()
{
	// todo setcontext check null  shaders, texture, vertices ? 
	delete m_program;
	delete m_tex_pixels;
}


void PixelRenderer::setBufferSize(int width, int height)
{ 
	// opengl es 2 wants powers of 2 for dimensions
	m_p2width = 1;
	m_p2height = 1;
	while (width > m_p2width) m_p2width <<= 1;
	while (height > m_p2height) m_p2height <<= 1;
	m_buffer_width = width;
	m_buffer_height = height;
	m_tex_pixels = new unsigned char [m_p2width * m_p2height * CHAN_COUNT];
}


void PixelRenderer::initializeGL()
{
    initializeOpenGLFunctions();

    m_texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    m_texture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    m_texture->setSize(m_p2width, m_p2height);
    m_texture->setFormat(QOpenGLTexture::RGBA8_UNorm);
    m_texture->setWrapMode(QOpenGLTexture::Repeat);
    m_texture->allocateStorage();

    GLfloat w = m_buffer_width / (double)m_p2width;
    GLfloat h = m_buffer_height / (double)m_p2height;

    QVector<GLfloat> vertData {
        -1.0f, -1.0f, 0.0f, 	0.0f, h,
         1.0f, -1.0f, 0.0f, 	   w, h,
        -1.0f, 1.0f, 0.0f, 	0.0f, 0.0f,
         1.0f, 1.0f, 0.0f, 	   w, 0.0f
    };
    m_vertices = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vertices->create();
    m_vertices->bind();

    m_vertices->allocate(vertData.constData(), vertData.count() * sizeof (GLfloat));

    glDisable(GL_DEPTH_TEST);

    #define PROGRAM_VERTEX_ATTRIBUTE 0
    #define PROGRAM_TEXCOORD_ATTRIBUTE 1

    const char *vshader_src =
        "attribute vec4 a_position;\n"
        "attribute vec2 a_texCoord;\n"
        "varying vec2 v_texCoord;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = a_position;\n"
        "    v_texCoord = a_texCoord;\n"
        "}\n";

    const char *fshader_src =
        "varying mediump vec2 v_texCoord;\n"
        "uniform sampler2D s_texture;\n"
        "void main()\n"
        "{\n"
        "   gl_FragColor =  texture2D(s_texture, v_texCoord.st);\n"
        "}\n";

    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    vshader->compileSourceCode(vshader_src);
    fshader->compileSourceCode(fshader_src);

    m_program = new QOpenGLShaderProgram;
    m_program->addShader(vshader);
    m_program->addShader(fshader);
    m_program->bindAttributeLocation("a_position", PROGRAM_VERTEX_ATTRIBUTE);
    m_program->bindAttributeLocation("a_texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    m_program->link();
    m_program->bind();
    m_program->setUniformValue("s_texture", 0);
}


void PixelRenderer::readPixels()
{
	m_lock->lock();
	for (int y = 0; y < m_buffer_height; ++y) {
		std::copy(&m_pixels[CHAN_COUNT * m_buffer_width * y],
			&m_pixels[CHAN_COUNT * m_buffer_width * (y + 1)],
			&m_tex_pixels[CHAN_COUNT * m_p2width * y]);
	}
	m_lock->unlock();
}


void PixelRenderer::paintGL()
{
	readPixels();

	m_program->bind();
	m_texture->bind(0, QOpenGLTexture::ResetTextureUnit);
	m_texture->setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, m_tex_pixels);

	m_vertices->bind();

	glClearColor(m_color.redF(), m_color.greenF(), m_color.blueF(), m_color.alphaF());
	glViewport(0, 0, m_width, m_height);
	glClear(GL_COLOR_BUFFER_BIT);

	setViewport();

	m_program->setUniformValue("s_texture", 0);
	m_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
	m_program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
	m_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof (GLfloat));
	m_program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof (GLfloat), 2, 5 * sizeof (GLfloat));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glFinish();
}


void PixelRenderer::resizeGL(int width, int height)
{
	m_width = width;
	m_height = height;
	int xscale = width / m_buffer_width;
	int yscale = height / m_buffer_height;
	int scale = qMin(xscale, yscale);
	int w = m_buffer_width * scale;
	int h = m_buffer_height * scale;
	int x_remaining = width - w;
	int y_remaining = height - h;
	int x = x_remaining / 2;
	int y = y_remaining - qMin(x_remaining, y_remaining) / 8;
	m_viewRect = QRect(x, y, w, h); 
}


void PixelRenderer::setViewport()
{
	glViewport(m_viewRect.x(), m_viewRect.y(), m_viewRect.width(), m_viewRect.height());
}


void PixelRenderer::paint()
{
	if (!m_program) {
		initializeGL();
	}

	paintGL();
	m_window->resetOpenGLState();
}
