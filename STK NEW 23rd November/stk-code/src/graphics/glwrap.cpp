#include "graphics/glwrap.hpp"
#include "irr_driver.hpp"
#include <fstream>
#include <string>

#ifdef _IRR_WINDOWS_API_
#define IRR_OGL_LOAD_EXTENSION(X) wglGetProcAddress(reinterpret_cast<const char*>(X))
PFNGLGENTRANSFORMFEEDBACKSPROC glGenTransformFeedbacks;
PFNGLBINDTRANSFORMFEEDBACKPROC glBindTransformFeedback;
PFNGLDRAWTRANSFORMFEEDBACKPROC glDrawTransformFeedback;
PFNGLBEGINTRANSFORMFEEDBACKPROC glBeginTransformFeedback;
PFNGLENDTRANSFORMFEEDBACKPROC glEndTransformFeedback;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC glTransformFeedbackVaryings;
PFNGLBINDBUFFERBASEPROC glBindBufferBase;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORM3FPROC glUniform3f;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLUNIFORM2FPROC glUniform2f;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLUNIFORM3IPROC glUniform3i;
PFNGLUNIFORM4IPROC glUniform4i;
PFNGLUNIFORM1FVPROC glUniform1fv;
PFNGLUNIFORM4FVPROC glUniform4fv;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLBLENDEQUATIONPROC glBlendEquation;
PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor;
PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLTEXBUFFERPROC glTexBuffer;
PFNGLBUFFERSUBDATAPROC glBufferSubData;
PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer;
PFNGLDEBUGMESSAGECALLBACKARBPROC glDebugMessageCallbackARB;
#endif

static GLuint quad_buffer;
static GLuint ColoredVertex;
static bool is_gl_init = false;

//#define ARB_DEBUG_OUTPUT
#ifdef ARB_DEBUG_OUTPUT
static
void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
  const GLchar* msg, const void *userparam)
{
    switch(source)
    {
    case GL_DEBUG_SOURCE_API_ARB:
        printf("[API]");
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
        printf("[WINDOW_SYSTEM]");
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
        printf("[SHADER_COMPILER]");
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
        printf("[THIRD_PARTY]");
        break;
    case GL_DEBUG_SOURCE_APPLICATION_ARB:
        printf("[APPLICATION]");
        break;
    case GL_DEBUG_SOURCE_OTHER_ARB:
        printf("[OTHER]");
        break;
    }

    switch(type)
    {
    case GL_DEBUG_TYPE_ERROR_ARB:
        printf("[ERROR]");
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
        printf("[DEPRECATED_BEHAVIOR]");
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
        printf("[UNDEFINED_BEHAVIOR]");
        break;
    case GL_DEBUG_TYPE_PORTABILITY_ARB:
        printf("[PORTABILITY]");
        break;
    case GL_DEBUG_TYPE_PERFORMANCE_ARB:
        printf("[PERFORMANCE]");
        break;
    case GL_DEBUG_TYPE_OTHER_ARB:
        printf("[OTHER]");
        break;
    }

    switch(severity)
    {
    case GL_DEBUG_SEVERITY_HIGH_ARB:
        printf("[HIGH]");
        break;
    case GL_DEBUG_SEVERITY_MEDIUM_ARB:
        printf("[MEDIUM]");
        break;
    case GL_DEBUG_SEVERITY_LOW_ARB:
        printf("[LOW]");
        break;
    }
    printf("%s\n", msg);
}
#endif

void initGL()
{
	if (is_gl_init)
		return;
	is_gl_init = true;
#ifdef _IRR_WINDOWS_API_
	glGenTransformFeedbacks = (PFNGLGENTRANSFORMFEEDBACKSPROC)IRR_OGL_LOAD_EXTENSION("glGenTransformFeedbacks");
	glBindTransformFeedback = (PFNGLBINDTRANSFORMFEEDBACKPROC)IRR_OGL_LOAD_EXTENSION("glBindTransformFeedback");
	glDrawTransformFeedback = (PFNGLDRAWTRANSFORMFEEDBACKPROC)IRR_OGL_LOAD_EXTENSION("glDrawTransformFeedback");
	glBeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACKPROC)IRR_OGL_LOAD_EXTENSION("glBeginTransformFeedback");
	glEndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACKPROC)IRR_OGL_LOAD_EXTENSION("glEndTransformFeedback");
	glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)IRR_OGL_LOAD_EXTENSION("glBindBufferBase");
	glGenBuffers = (PFNGLGENBUFFERSPROC)IRR_OGL_LOAD_EXTENSION("glGenBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)IRR_OGL_LOAD_EXTENSION("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)IRR_OGL_LOAD_EXTENSION("glBufferData");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)IRR_OGL_LOAD_EXTENSION("glVertexAttribPointer");
	glCreateShader = (PFNGLCREATESHADERPROC)IRR_OGL_LOAD_EXTENSION("glCreateShader");
	glCompileShader = (PFNGLCOMPILESHADERPROC)IRR_OGL_LOAD_EXTENSION("glCompileShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)IRR_OGL_LOAD_EXTENSION("glShaderSource");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)IRR_OGL_LOAD_EXTENSION("glCreateProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)IRR_OGL_LOAD_EXTENSION("glAttachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)IRR_OGL_LOAD_EXTENSION("glLinkProgram");
	glUseProgram = (PFNGLUSEPROGRAMPROC)IRR_OGL_LOAD_EXTENSION("glUseProgram");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)IRR_OGL_LOAD_EXTENSION("glEnableVertexAttribArray");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)IRR_OGL_LOAD_EXTENSION("glGetUniformLocation");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)IRR_OGL_LOAD_EXTENSION("glUniformMatrix4fv");
	glUniform1f = (PFNGLUNIFORM1FPROC)IRR_OGL_LOAD_EXTENSION("glUniform1f");
	glUniform3f = (PFNGLUNIFORM3FPROC)IRR_OGL_LOAD_EXTENSION("glUniform3f");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)IRR_OGL_LOAD_EXTENSION("glDisableVertexAttribArray");
	glDeleteShader = (PFNGLDELETESHADERPROC)IRR_OGL_LOAD_EXTENSION("glDeleteShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)IRR_OGL_LOAD_EXTENSION("glGetShaderiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)IRR_OGL_LOAD_EXTENSION("glGetShaderInfoLog");
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)IRR_OGL_LOAD_EXTENSION("glActiveTexture");
	glUniform2f = (PFNGLUNIFORM2FPROC)IRR_OGL_LOAD_EXTENSION("glUniform2f");
	glUniform4i = (PFNGLUNIFORM4IPROC)IRR_OGL_LOAD_EXTENSION("glUniform4i");
	glUniform3i = (PFNGLUNIFORM3IPROC)IRR_OGL_LOAD_EXTENSION("glUniform3i");
	glUniform1i = (PFNGLUNIFORM1IPROC)IRR_OGL_LOAD_EXTENSION("glUniform1i");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)IRR_OGL_LOAD_EXTENSION("glGetProgramiv");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)IRR_OGL_LOAD_EXTENSION("glGetProgramInfoLog");
	glTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC)IRR_OGL_LOAD_EXTENSION("glTransformFeedbackVaryings");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)IRR_OGL_LOAD_EXTENSION("glGetAttribLocation");
	glBlendEquation = (PFNGLBLENDEQUATIONPROC)IRR_OGL_LOAD_EXTENSION("glBlendEquation");
	glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)IRR_OGL_LOAD_EXTENSION("glVertexAttribDivisor");
	glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)IRR_OGL_LOAD_EXTENSION("glDrawArraysInstanced");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)IRR_OGL_LOAD_EXTENSION("glDeleteBuffers");
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)IRR_OGL_LOAD_EXTENSION("glGenVertexArrays");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)IRR_OGL_LOAD_EXTENSION("glBindVertexArray");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)IRR_OGL_LOAD_EXTENSION("glDeleteVertexArrays");
	glTexBuffer = (PFNGLTEXBUFFERPROC)IRR_OGL_LOAD_EXTENSION("glTexBuffer");
	glUniform1fv = (PFNGLUNIFORM1FVPROC)IRR_OGL_LOAD_EXTENSION("glUniform1fv");
	glUniform4fv = (PFNGLUNIFORM4FVPROC)IRR_OGL_LOAD_EXTENSION("glUniform4fv");
	glBufferSubData = (PFNGLBUFFERSUBDATAPROC)IRR_OGL_LOAD_EXTENSION("glBufferSubData");
	glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)IRR_OGL_LOAD_EXTENSION("glVertexAttribIPointer");
#ifdef DEBUG
	glDebugMessageCallbackARB = (PFNGLDEBUGMESSAGECALLBACKARBPROC)IRR_OGL_LOAD_EXTENSION("glDebugMessageCallbackARB");
#endif
#endif
#ifdef ARB_DEBUG_OUTPUT
	glDebugMessageCallbackARB((GLDEBUGPROCARB)debugCallback, NULL);
#endif
	const float quad_vertex[] = {
		-1., -1., -1., 1., // UpperLeft
		-1., 1., -1., -1., // LowerLeft
		1., -1., 1., 1., // UpperRight
		1., 1., 1., -1., // LowerRight
	};
	glGenBuffers(1, &quad_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), quad_vertex, GL_STATIC_DRAW);

	const unsigned quad_color[] = {
		0, 0, 0, 255,
		255, 0, 0, 255,
		0, 255, 0, 255,
		0, 0, 255, 255,
	};
	glGenBuffers(1, &ColoredVertex);
	glBindBuffer(GL_ARRAY_BUFFER, ColoredVertex);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(unsigned), quad_color, GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Mostly from shader tutorial
static
GLuint LoadShader(const char * file, unsigned type) {
	GLuint Id = glCreateShader(type);
	std::string Code;
	std::ifstream Stream(file, std::ios::in);
	if (Stream.is_open())
	{
		std::string Line = "";
		while (getline(Stream, Line))
			Code += "\n" + Line;
		Stream.close();
	}
	GLint Result = GL_FALSE;
	int InfoLogLength;
	printf("Compiling shader : %s\n", file);
	char const * SourcePointer = Code.c_str();
	int length = strlen(SourcePointer);
	glShaderSource(Id, 1, &SourcePointer, &length);
	glCompileShader(Id);

	glGetShaderiv(Id, GL_COMPILE_STATUS, &Result);
	if (Result == GL_FALSE) {
		glGetShaderiv(Id, GL_INFO_LOG_LENGTH, &InfoLogLength);
		char *ErrorMessage = new char[InfoLogLength];
		glGetShaderInfoLog(Id, InfoLogLength, NULL, ErrorMessage);
		printf(ErrorMessage);
		delete[] ErrorMessage;
	}

	return Id;
}

GLuint LoadProgram(const char * vertex_file_path, const char * fragment_file_path) {
	GLuint VertexShaderID = LoadShader(vertex_file_path, GL_VERTEX_SHADER);
	GLuint FragmentShaderID = LoadShader(fragment_file_path, GL_FRAGMENT_SHADER);

	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	GLint Result = GL_FALSE;
	int InfoLogLength;
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	if (Result == GL_FALSE) {
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		char *ErrorMessage = new char[InfoLogLength];
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, ErrorMessage);
		printf(ErrorMessage);
		delete[] ErrorMessage;
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

GLuint LoadTFBProgram(const char * vertex_file_path, const char **varyings, unsigned varyingscount) {
	GLuint Shader = LoadShader(vertex_file_path, GL_VERTEX_SHADER);
	GLuint Program = glCreateProgram();
	glAttachShader(Program, Shader);
	glTransformFeedbackVaryings(Program, varyingscount, varyings, GL_INTERLEAVED_ATTRIBS);
	glLinkProgram(Program);

	GLint Result = GL_FALSE;
	int InfoLogLength;
	glGetProgramiv(Program, GL_LINK_STATUS, &Result);
	if (Result == GL_FALSE) {
		glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &InfoLogLength);
		char *ErrorMessage = new char[InfoLogLength];
		glGetProgramInfoLog(Program, InfoLogLength, NULL, ErrorMessage);
		printf(ErrorMessage);
		delete[] ErrorMessage;
	}
	glDeleteShader(Shader);
	return Program;
}



void bindUniformToTextureUnit(GLuint location, GLuint texid, unsigned textureUnit) {
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, texid);
	glUniform1i(location, textureUnit);
}

void setTexture(unsigned TextureUnit, GLuint TextureId, GLenum MagFilter, GLenum MinFilter)
{
	glActiveTexture(GL_TEXTURE0 + TextureUnit);
	glBindTexture(GL_TEXTURE_2D, TextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

static GLuint TexturedQuadShader;
static GLuint TexturedQuadAttribPosition;
static GLuint TexturedQuadAttribTexCoord;
static GLuint TexturedQuadUniformTex;
static GLuint TexturedQuadUniformCenter;
static GLuint TexturedQuadUniformSize;
static GLuint TexturedQuadUniformTexcenter;
static GLuint TexturedQuadUniformTexsize;

static GLuint TQvao;

static GLuint ColorTexturedQuadShader;
static GLuint ColorTexturedQuadAttribPosition;
static GLuint ColorTexturedQuadAttribTexCoord;
static GLuint ColorTexturedQuadAttribColor;
static GLuint ColorTexturedQuadUniformTex;
static GLuint ColorTexturedQuadUniformCenter;
static GLuint ColorTexturedQuadUniformSize;
static GLuint ColorTexturedQuadUniformTexcenter;
static GLuint ColorTexturedQuadUniformTexsize;

static GLuint CTQvao;

static void drawTexColoredQuad(const video::ITexture *texture, const video::SColor *col, float width, float height,
    float center_pos_x, float center_pos_y, float tex_center_pos_x, float tex_center_pos_y,
    float tex_width, float tex_height)
{
  unsigned colors[] = {
  col[0].getRed(), col[0].getGreen(), col[0].getBlue(), col[0].getAlpha(),
  col[1].getRed(), col[1].getGreen(), col[1].getBlue(), col[1].getAlpha(),
  col[2].getRed(), col[2].getGreen(), col[2].getBlue(), col[2].getAlpha(),
  col[3].getRed(), col[3].getGreen(), col[3].getBlue(), col[3].getAlpha(),
  };

  if (!ColorTexturedQuadShader) {
     ColorTexturedQuadShader = LoadProgram(file_manager->getAsset("shaders/colortexturedquad.vert").c_str(), file_manager->getAsset("shaders/colortexturedquad.frag").c_str());

	  ColorTexturedQuadAttribPosition = glGetAttribLocation(ColorTexturedQuadShader, "position");
	  ColorTexturedQuadAttribTexCoord = glGetAttribLocation(ColorTexturedQuadShader, "texcoord");
	  ColorTexturedQuadAttribColor = glGetAttribLocation(ColorTexturedQuadShader, "color");
	  ColorTexturedQuadUniformTex = glGetUniformLocation(ColorTexturedQuadShader, "tex");
	  ColorTexturedQuadUniformCenter = glGetUniformLocation(ColorTexturedQuadShader, "center");
	  ColorTexturedQuadUniformSize = glGetUniformLocation(ColorTexturedQuadShader, "size");
	  ColorTexturedQuadUniformTexcenter = glGetUniformLocation(ColorTexturedQuadShader, "texcenter");
	  ColorTexturedQuadUniformTexsize = glGetUniformLocation(ColorTexturedQuadShader, "texsize");
	  glGenVertexArrays(1, &CTQvao);
	  glBindVertexArray(CTQvao);
	  glEnableVertexAttribArray(ColorTexturedQuadAttribPosition);
	  glEnableVertexAttribArray(ColorTexturedQuadAttribTexCoord);
	  glEnableVertexAttribArray(ColorTexturedQuadAttribColor);
	  glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
	  glVertexAttribPointer(ColorTexturedQuadAttribPosition, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	  glVertexAttribPointer(ColorTexturedQuadAttribTexCoord, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid *)(2 * sizeof(float)));
	  glBindBuffer(GL_ARRAY_BUFFER, ColoredVertex);
	  glVertexAttribIPointer(ColorTexturedQuadAttribColor, 4, GL_UNSIGNED_INT, 4 * sizeof(unsigned), 0);
	  glBindVertexArray(0);
  }
  glBindBuffer(GL_ARRAY_BUFFER, ColoredVertex);
  glBufferSubData(GL_ARRAY_BUFFER, 0, 16 * sizeof(unsigned), colors);
  glUseProgram(ColorTexturedQuadShader);
  glBindVertexArray(CTQvao);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, static_cast<const irr::video::COpenGLTexture*>(texture)->getOpenGLTextureName());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glUniform1i(ColorTexturedQuadUniformTex, 0);
  glUniform2f(ColorTexturedQuadUniformCenter, center_pos_x, center_pos_y);
  glUniform2f(ColorTexturedQuadUniformSize, width, height);
  glUniform2f(ColorTexturedQuadUniformTexcenter, tex_center_pos_x, tex_center_pos_y);
  glUniform2f(ColorTexturedQuadUniformTexsize, tex_width, tex_height);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void drawTexQuad(const video::ITexture *texture, float width, float height,
    float center_pos_x, float center_pos_y, float tex_center_pos_x, float tex_center_pos_y,
    float tex_width, float tex_height)
{
  if (!TexturedQuadShader) {
     TexturedQuadShader = LoadProgram(file_manager->getAsset("shaders/texturedquad.vert").c_str(), file_manager->getAsset("shaders/texturedquad.frag").c_str());

	  TexturedQuadAttribPosition = glGetAttribLocation(TexturedQuadShader, "position");
	  TexturedQuadAttribTexCoord = glGetAttribLocation(TexturedQuadShader, "texcoord");
	  TexturedQuadUniformTex = glGetUniformLocation(TexturedQuadShader, "tex");
	  TexturedQuadUniformCenter = glGetUniformLocation(TexturedQuadShader, "center");
	  TexturedQuadUniformSize = glGetUniformLocation(TexturedQuadShader, "size");
	  TexturedQuadUniformTexcenter = glGetUniformLocation(TexturedQuadShader, "texcenter");
	  TexturedQuadUniformTexsize = glGetUniformLocation(TexturedQuadShader, "texsize");
	  glGenVertexArrays(1, &TQvao);
	  glBindVertexArray(TQvao);
	  glEnableVertexAttribArray(TexturedQuadAttribPosition);
	  glEnableVertexAttribArray(TexturedQuadAttribTexCoord);
	  glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
	  glVertexAttribPointer(TexturedQuadAttribPosition, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	  glVertexAttribPointer(TexturedQuadAttribTexCoord, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid *)(2 * sizeof(float)));
     glBindVertexArray(0);
  }
  glUseProgram(TexturedQuadShader);
  glBindVertexArray(TQvao);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, static_cast<const irr::video::COpenGLTexture*>(texture)->getOpenGLTextureName());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glUniform1i(TexturedQuadUniformTex, 0);
  glUniform2f(TexturedQuadUniformCenter, center_pos_x, center_pos_y);
  glUniform2f(TexturedQuadUniformSize, width, height);
  glUniform2f(TexturedQuadUniformTexcenter, tex_center_pos_x, tex_center_pos_y);
  glUniform2f(TexturedQuadUniformTexsize, tex_width, tex_height);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw2DImage(const video::ITexture* texture, const core::rect<s32>& destRect,
	const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect,
	const video::SColor* const colors, bool useAlphaChannelOfTexture)
{
	if (!irr_driver->isGLSL())
	{
		irr_driver->getVideoDriver()->draw2DImage(texture, destRect, sourceRect, clipRect, colors, useAlphaChannelOfTexture);
		return;
	}

	core::dimension2d<u32> frame_size =
		irr_driver->getVideoDriver()->getCurrentRenderTargetSize();
	const int screen_w = frame_size.Width;
	const int screen_h = frame_size.Height;
	float center_pos_x = destRect.UpperLeftCorner.X + destRect.LowerRightCorner.X;
	center_pos_x /= screen_w;
	center_pos_x -= 1.;
	float center_pos_y = destRect.UpperLeftCorner.Y + destRect.LowerRightCorner.Y;
	center_pos_y /= screen_h;
	center_pos_y = 1. - center_pos_y;
	float width = destRect.LowerRightCorner.X - destRect.UpperLeftCorner.X;
	width /= screen_w;
	float height = destRect.LowerRightCorner.Y - destRect.UpperLeftCorner.Y;
	height /= screen_h;

	const core::dimension2d<u32>& ss = texture->getOriginalSize();

	float tex_center_pos_x = sourceRect.UpperLeftCorner.X + sourceRect.LowerRightCorner.X;
	tex_center_pos_x /= ss.Width * 2.;
	//tex_center_pos_x -= 1.;
	float tex_center_pos_y = sourceRect.UpperLeftCorner.Y + sourceRect.LowerRightCorner.Y;
	tex_center_pos_y /= ss.Height * 2.;
	//tex_center_pos_y -= 1.;
	float tex_width = sourceRect.LowerRightCorner.X - sourceRect.UpperLeftCorner.X;
	tex_width /= ss.Width * 2.;
	float tex_height = sourceRect.LowerRightCorner.Y - sourceRect.UpperLeftCorner.Y;
	tex_height /= ss.Height * 2.;

	const f32 invW = 1.f / static_cast<f32>(ss.Width);
	const f32 invH = 1.f / static_cast<f32>(ss.Height);
	const core::rect<f32> tcoords(
		sourceRect.UpperLeftCorner.X * invW,
		sourceRect.UpperLeftCorner.Y * invH,
		sourceRect.LowerRightCorner.X * invW,
		sourceRect.LowerRightCorner.Y *invH);

	initGL();

	if (useAlphaChannelOfTexture)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_BLEND);
	}
	if (colors)
	  drawTexColoredQuad(texture, colors, width, height, center_pos_x, center_pos_y,
	      tex_center_pos_x, tex_center_pos_y, tex_width, tex_height);
	else
	  drawTexQuad(texture, width, height, center_pos_x, center_pos_y,
	      tex_center_pos_x, tex_center_pos_y, tex_width, tex_height);
	glUseProgram(0);
}

static GLuint ColoredQuadShader;
static GLuint ColoredQuadUniformCenter;
static GLuint ColoredQuadUniformSize;
static GLuint ColoredQuadUniformColor;
static GLuint CQvao;

void GL32_draw2DRectangle(video::SColor color, const core::rect<s32>& position,
	const core::rect<s32>* clip)
{

	if (!irr_driver->isGLSL())
	{
		irr_driver->getVideoDriver()->draw2DRectangle(color, position, clip);
		return;
	}

	core::dimension2d<u32> frame_size =
		irr_driver->getVideoDriver()->getCurrentRenderTargetSize();
	const int screen_w = frame_size.Width;
	const int screen_h = frame_size.Height;
	float center_pos_x = position.UpperLeftCorner.X + position.LowerRightCorner.X;
	center_pos_x /= screen_w;
	center_pos_x -= 1;
	float center_pos_y = position.UpperLeftCorner.Y + position.LowerRightCorner.Y;
	center_pos_y /= screen_h;
	center_pos_y = 1 - center_pos_y;
	float width = position.LowerRightCorner.X - position.UpperLeftCorner.X;
	width /= screen_w;
	float height = position.LowerRightCorner.Y - position.UpperLeftCorner.Y;
	height /= screen_h;

	initGL();

	if (color.getAlpha() < 255)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_BLEND);
	}

	if (!ColoredQuadShader)
	{
		ColoredQuadShader = LoadProgram(file_manager->getAsset("shaders/coloredquad.vert").c_str(), file_manager->getAsset("shaders/coloredquad.frag").c_str());
		ColoredQuadUniformColor = glGetUniformLocation(ColoredQuadShader, "color");
		ColoredQuadUniformCenter = glGetUniformLocation(ColoredQuadShader, "center");
		ColoredQuadUniformSize = glGetUniformLocation(ColoredQuadShader, "size");
		glGenVertexArrays(1, &CQvao);
		glBindVertexArray(CQvao);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindVertexArray(0);
	}
	glUseProgram(ColoredQuadShader);
	glBindVertexArray(CQvao);
	glUniform2f(ColoredQuadUniformCenter, center_pos_x, center_pos_y);
	glUniform2f(ColoredQuadUniformSize, width, height);
	glUniform4i(ColoredQuadUniformColor, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}
