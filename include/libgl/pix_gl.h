#ifndef PIX_GL_H
#define PIX_GL_H 1

#include <stdint.h>

#include "libbase/pix_error.h"
#include "libbase/pix_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GLAPI extern

pxResult 
pxInitGL(void); 

void *
pxLoadGLFunction(const pxChar *name); 

void 
pxLoadGLFunctions(void); 

typedef unsigned char GLboolean; 
typedef char GLbyte; 
typedef unsigned char GLubyte; 
typedef short GLshort; 
typedef unsigned short GLushort; 
typedef int GLint;
typedef unsigned int GLuint; 
typedef int GLfixed;
typedef int64_t GLint64;
typedef uint64_t GLuint64;
typedef unsigned int GLsizei;
typedef unsigned int GLenum; 
typedef void * GLintptr;
typedef void * GLsizeiptr;
typedef void * GLsync;
typedef unsigned int GLbitfield;
typedef unsigned short GLhalf;
typedef float GLfloat; 
typedef float GLclampf;
typedef double GLdouble; 
typedef double GLclampd; 
typedef void GLvoid; 

#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_TRIANGLES        0x00000004

/* TEXTURE */
typedef void (*PFNGLACTIVETEXTUREPROC)(GLenum);
GLAPI PFNGLACTIVETEXTUREPROC glActiveTexture; 
typedef void (*PFNGLBINDTEXTUREPROC)(GLenum, GLuint);
GLAPI PFNGLBINDTEXTUREPROC glBindTexture; 
typedef void (*PFNGLCOMPRESSEDTEXIMAGE1DPROC)(GLenum, GLint, GLenum, GLsizei, GLint, GLsizei, const GLvoid *);
GLAPI PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D;
typedef void (*PFNGLCOMPRESSEDTEXIMAGE2DPROC)(GLenum, GLint, GLenum, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *);
GLAPI PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
typedef void (*PFNGLCOMPRESSEDTEXIMAGE3DPROC)(GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *);
GLAPI PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D;
typedef void (*PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)(GLenum, GLint, GLint, GLsizei, GLenum, GLsizei, const GLvoid *);
GLAPI PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D; 
typedef void (*PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *);
GLAPI PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D; 
typedef void (*PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)(GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *);
GLAPI PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D; 
typedef void (*PFNGLCOPYTEXIMAGE1DPROC)(GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLint);
GLAPI PFNGLCOPYTEXIMAGE1DPROC glCopyTexImage1D; 
typedef void (*PFNGLCOPYTEXIMAGE2DPROC)(GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLsizei, GLint);
GLAPI PFNGLCOPYTEXIMAGE2DPROC glCopyTexImage2D; 
typedef void (*PFNGLCOPYTEXSUBIMAGE1DPROC)(GLenum, GLint, GLint, GLint, GLint, GLsizei); 
GLAPI PFNGLCOPYTEXSUBIMAGE1DPROC glCopyTexSubImage1D; 
typedef void (*PFNGLCOPYTEXSUBIMAGE2DPROC)(GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei); 
GLAPI PFNGLCOPYTEXSUBIMAGE2DPROC glCopyTexSubImage2D; 
typedef void (*PFNGLCOPYTEXSUBIMAGE3DPROC)(GLenum, GLint, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei); 
GLAPI PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D; 
typedef void (*PFNGLDELETETEXTURESPROC)(GLsizei, const GLuint *); 
GLAPI PFNGLDELETETEXTURESPROC glDeleteTextures; 
typedef void (*PFNGLGENTEXTURESPROC)(GLsizei, GLuint *); 
GLAPI PFNGLGENTEXTURESPROC glGenTextures;
typedef void (*PFNGLGETCOMPRESSEDTEXIMAGEPROC)(GLenum, GLint, GLvoid *); 
GLAPI PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage;
typedef void (*PFNGLGETTEXIMAGEPROC)(GLenum, GLint, GLenum, GLenum, GLvoid *); 
GLAPI PFNGLGETTEXIMAGEPROC glGetTexImage; 
typedef void (*PFNGLGETTEXLEVELPARAMETERFVPROC)(GLenum, GLint, GLenum, GLfloat *);
GLAPI PFNGLGETTEXLEVELPARAMETERFVPROC glGetTexLevelParameterfv; 
typedef void (*PFNGLGETTEXLEVELPARAMETERIVPROC)(GLenum, GLint, GLenum, GLint *);
GLAPI PFNGLGETTEXLEVELPARAMETERIVPROC glGetTexLevelParameteriv; 
typedef void (*PFNGLGETTEXPARAMETERFVPROC)(GLenum, GLenum, GLfloat *); 
GLAPI PFNGLGETTEXPARAMETERFVPROC glGetTexParameterfv;
typedef void (*PFNGLGETTEXPARAMETERIVPROC)(GLenum, GLenum, GLint *); 
GLAPI PFNGLGETTEXPARAMETERIVPROC glGetTexParameteriv;
typedef GLboolean (*PFNGLISTEXTUREPROC)(GLuint);
GLAPI PFNGLISTEXTUREPROC glIsTexture;
typedef void (*PFNGLTEXIMAGE1DPROC)(GLenum, GLint, GLint, GLsizei, GLint, GLenum, GLenum, GLenum, const GLvoid *); 
GLAPI PFNGLTEXIMAGE1DPROC glTexImage1D; 
typedef void (*PFNGLTEXIMAGE2DPROC)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, GLenum, const GLvoid *); 
GLAPI PFNGLTEXIMAGE2DPROC glTexImage2D; 
typedef void (*PFNGLTEXIMAGE3DPROC)(GLenum, GLint, GLint, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, GLenum, const GLvoid *); 
GLAPI PFNGLTEXIMAGE3DPROC glTexImage3D; 
typedef void (*PFNGLTEXPARAMETERFPROC)(GLenum, GLenum, GLfloat); 
GLAPI PFNGLTEXPARAMETERFPROC glTexParameterf; 
typedef void (*PFNGLTEXPARAMETERIPROC)(GLenum, GLenum, GLint); 
GLAPI PFNGLTEXPARAMETERIPROC glTexParameteri; 
typedef void (*PFNGLTEXSUBIMAGE1DPROC)(GLenum, GLint, GLint, GLsizei, GLenum, GLenum, const GLvoid *); 
GLAPI PFNGLTEXSUBIMAGE1DPROC glTexSubImage1D;
typedef void (*PFNGLTEXSUBIMAGE2DPROC)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *); 
GLAPI PFNGLTEXSUBIMAGE2DPROC glTexSubImage2D;
typedef void (*PFNGLTEXSUBIMAGE3DPROC)(GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *); 
GLAPI PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D;
/* RENDER */
typedef void (*PFNGLCLEARPROC)(GLbitfield); 
GLAPI PFNGLCLEARPROC glClear;
typedef void (*PFNGLCLEARCOLORPROC)(GLclampf, GLclampf, GLclampf, GLclampf);
GLAPI PFNGLCLEARCOLORPROC glClearColor; 
typedef void (*PFNGLCLEARDEPTHPROC)(GLclampf);
GLAPI PFNGLCLEARDEPTHPROC glClearDepth; 
typedef void (*PFNGLCLEARSTENCILPROC)(GLint);
GLAPI PFNGLCLEARSTENCILPROC glClearStencil; 
typedef void (*PFNGLDRAWBUFFERPROC)(GLenum); 
GLAPI PFNGLDRAWBUFFERPROC glDrawBuffer;
typedef void (*PFNGLFINISHPROC)(void);
GLAPI PFNGLFINISHPROC glFinish;
typedef void (*PFNGLFLUSHPROC)(void);
GLAPI PFNGLFLUSHPROC glFlush;
typedef void (*PFNGLREADBUFFERPROC)(GLenum);
GLAPI PFNGLREADBUFFERPROC glReadBuffer; 
typedef void (*PFNGLREADPIXELSPROC)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLvoid *); 
GLAPI PFNGLREADPIXELSPROC glReadPixels; 
/* FRAME BUFFERS */
typedef void (*PFNGLDRAWBUFFERSPROC)(GLsizei, const GLenum *);
GLAPI PFNGLDRAWBUFFERSPROC glDrawBuffers;
/* SHADERS */
/* BUFFER OBJECTS */
/* STATE MANAGEMENT */
typedef void (*PFNGLVIEWPORTPROC)(GLint, GLint, GLsizei, GLsizei);
GLAPI PFNGLVIEWPORTPROC glViewport; 
/* UTILITY */
/* QUERIES */
/* IMMEDIATE MODE */
typedef void (*PFNGLARRAYELEMENTPROC)(GLint);
GLAPI PFNGLARRAYELEMENTPROC glArrayElement; 
typedef void (*PFNGLBEGINPROC)(GLenum);
GLAPI PFNGLBEGINPROC glBegin; 
typedef void (*PFNGLCOLOR3BPROC)(GLbyte, GLbyte, GLbyte);
GLAPI PFNGLCOLOR3BPROC glColor3b; 
typedef void (*PFNGLCOLOR3SPROC)(GLshort, GLshort, GLshort);
GLAPI PFNGLCOLOR3SPROC glColor3s;
typedef void (*PFNGLCOLOR3IPROC)(GLint, GLint, GLint);
GLAPI PFNGLCOLOR3IPROC glColor3i;
typedef void (*PFNGLCOLOR3FPROC)(GLfloat, GLfloat, GLfloat);
GLAPI PFNGLCOLOR3FPROC glColor3f;
typedef void (*PFNGLCOLOR3DPROC)(GLdouble, GLdouble, GLdouble);
GLAPI PFNGLCOLOR3DPROC glColor3d;
typedef void (*PFNGLCOLOR3UBPROC)(GLubyte, GLubyte, GLubyte);
GLAPI PFNGLCOLOR3UBPROC glColor3ub; 
typedef void (*PFNGLCOLOR3USPROC)(GLushort, GLushort, GLushort);
GLAPI PFNGLCOLOR3USPROC glColor3us;
typedef void (*PFNGLCOLOR3UIPROC)(GLuint, GLuint, GLuint);
GLAPI PFNGLCOLOR3UIPROC glColor3ui;
typedef void (*PFNGLCOLOR4BPROC)(GLbyte, GLbyte, GLbyte, GLbyte);
GLAPI PFNGLCOLOR4BPROC glColor4b; 
typedef void (*PFNGLCOLOR4SPROC)(GLshort, GLshort, GLshort, GLshort);
GLAPI PFNGLCOLOR4SPROC glColor4s;
typedef void (*PFNGLCOLOR4IPROC)(GLint, GLint, GLint, GLint);
GLAPI PFNGLCOLOR4IPROC glColor4i;
typedef void (*PFNGLCOLOR4FPROC)(GLfloat, GLfloat, GLfloat, GLfloat);
GLAPI PFNGLCOLOR4FPROC glColor4f;
typedef void (*PFNGLCOLOR4DPROC)(GLdouble, GLdouble, GLdouble, GLdouble);
GLAPI PFNGLCOLOR4DPROC glColor4d;
typedef void (*PFNGLCOLOR4UBPROC)(GLubyte, GLubyte, GLubyte, GLubyte);
GLAPI PFNGLCOLOR4UBPROC glColor4ub; 
typedef void (*PFNGLCOLOR4USPROC)(GLushort, GLushort, GLushort, GLushort);
GLAPI PFNGLCOLOR4USPROC glColor4us;
typedef void (*PFNGLCOLOR4UIPROC)(GLuint, GLuint, GLuint, GLuint);
GLAPI PFNGLCOLOR4UIPROC glColor4ui;
typedef void (*PFNGLENDPROC)(void);
GLAPI PFNGLENDPROC glEnd;
typedef void (*PFNGLEVALCOORD1FPROC)(GLfloat);
GLAPI PFNGLEVALCOORD1FPROC glEvalCoord1f; 
typedef void (*PFNGLEVALCOORD1DPROC)(GLdouble);
GLAPI PFNGLEVALCOORD1DPROC glEvalCoord1d;
typedef void (*PFNGLEVALCOORD2FPROC)(GLfloat, GLfloat);
GLAPI PFNGLEVALCOORD2FPROC glEvalCoord2f;
typedef void (*PFNGLEVALCOORD2DPROC)(GLdouble, GLdouble);
GLAPI PFNGLEVALCOORD2DPROC glEvalCoord2d;
typedef void (*PFNGLEVALMESH1PROC)(GLenum, GLint, GLint);
GLAPI PFNGLEVALMESH1PROC glEvalMesh1;
typedef void (*PFNGLEVALPOINT1PROC)(GLint);
GLAPI PFNGLEVALPOINT1PROC glEvalPoint1;
typedef void (*PFNGLEVALPOINT2PROC)(GLint, GLint);
GLAPI PFNGLEVALPOINT2PROC glEvalPoint2; 
typedef void (*PFNGLFOGCOORDFPROC)(GLfloat);
GLAPI PFNGLFOGCOORDFPROC glFogCoordf; 
typedef void (*PFNGLFOGCOORDDPROC)(GLdouble);
GLAPI PFNGLFOGCOORDDPROC glFogCoordd; 
typedef void (*PFNGLINDEXSPROC)(GLshort);
GLAPI PFNGLINDEXSPROC glIndexs;
typedef void (*PFNGLINDEXIPROC)(GLint);
GLAPI PFNGLINDEXIPROC glIndexi;
typedef void (*PFNGLINDEXFPROC)(GLfloat);
GLAPI PFNGLINDEXFPROC glIndexf;
typedef void (*PFNGLINDEXDPROC)(GLdouble);
GLAPI PFNGLINDEXDPROC glIndexd;
typedef void (*PFNGLINDEXUBPROC)(GLubyte);
GLAPI PFNGLINDEXUBPROC glIndexub;
typedef void (*PFNGLMATERIALFPROC)(GLenum, GLenum, GLfloat);
GLAPI PFNGLMATERIALFPROC glMaterialf;
typedef void (*PFNGLMATERIALIPROC)(GLenum, GLenum, GLint);
GLAPI PFNGLMATERIALIPROC glMateriali; 
typedef void (*PFNGLMULTITEXCOORD1SPROC)(GLenum, GLshort);
GLAPI PFNGLMULTITEXCOORD1SPROC glMultiTexCoord1s; 
typedef void (*PFNGLMULTITEXCOORD1IPROC)(GLenum, GLint);
GLAPI PFNGLMULTITEXCOORD1IPROC glMultiTexCoord1i; 
typedef void (*PFNGLMULTITEXCOORD1FPROC)(GLenum, GLfloat);
GLAPI PFNGLMULTITEXCOORD1FPROC glMultiTexCoord1f; 
typedef void (*PFNGLMULTITEXCOORD1DPROC)(GLenum, GLdouble);
GLAPI PFNGLMULTITEXCOORD1DPROC glMultiTexCoord1d; 
typedef void (*PFNGLMULTITEXCOORD2SPROC)(GLenum, GLshort, GLshort);
GLAPI PFNGLMULTITEXCOORD2SPROC glMultiTexCoord2s; 
typedef void (*PFNGLMULTITEXCOORD2IPROC)(GLenum, GLint, GLint);
GLAPI PFNGLMULTITEXCOORD2IPROC glMultiTexCoord2i; 
typedef void (*PFNGLMULTITEXCOORD2FPROC)(GLenum, GLfloat, GLfloat);
GLAPI PFNGLMULTITEXCOORD2FPROC glMultiTexCoord2f; 
typedef void (*PFNGLMULTITEXCOORD2DPROC)(GLenum, GLdouble, GLdouble);
GLAPI PFNGLMULTITEXCOORD2DPROC glMultiTexCoord2d; 
typedef void (*PFNGLMULTITEXCOORD3SPROC)(GLenum, GLshort, GLshort, GLshort);
GLAPI PFNGLMULTITEXCOORD3SPROC glMultiTexCoord3s; 
typedef void (*PFNGLMULTITEXCOORD3IPROC)(GLenum, GLint, GLint, GLint);
GLAPI PFNGLMULTITEXCOORD3IPROC glMultiTexCoord3i; 
typedef void (*PFNGLMULTITEXCOORD3FPROC)(GLenum, GLfloat, GLfloat, GLfloat);
GLAPI PFNGLMULTITEXCOORD3FPROC glMultiTexCoord3f; 
typedef void (*PFNGLMULTITEXCOORD3DPROC)(GLenum, GLdouble, GLdouble, GLdouble);
GLAPI PFNGLMULTITEXCOORD3DPROC glMultiTexCoord3d; 
typedef void (*PFNGLMULTITEXCOORD4SPROC)(GLenum, GLshort, GLshort, GLshort, GLshort);
GLAPI PFNGLMULTITEXCOORD4SPROC glMultiTexCoord4s; 
typedef void (*PFNGLMULTITEXCOORD4IPROC)(GLenum, GLint, GLint, GLint, GLint);
GLAPI PFNGLMULTITEXCOORD4IPROC glMultiTexCoord4i; 
typedef void (*PFNGLMULTITEXCOORD4FPROC)(GLenum, GLfloat, GLfloat, GLfloat, GLfloat);
GLAPI PFNGLMULTITEXCOORD4FPROC glMultiTexCoord4f; 
typedef void (*PFNGLMULTITEXCOORD4DPROC)(GLenum, GLdouble, GLdouble, GLdouble, GLdouble);
GLAPI PFNGLMULTITEXCOORD4DPROC glMultiTexCoord4d; 
typedef void (*PFNGLNORMAL3BPROC)(GLbyte, GLbyte, GLbyte);
GLAPI PFNGLNORMAL3BPROC glNormal3b; 
typedef void (*PFNGLNORMAL3DPROC)(GLdouble, GLdouble, GLdouble);
GLAPI PFNGLNORMAL3DPROC glNormal3d; 
typedef void (*PFNGLNORMAL3FPROC)(GLfloat, GLfloat, GLfloat);
GLAPI PFNGLNORMAL3FPROC glNormal3f; 
typedef void (*PFNGLNORMAL3IPROC)(GLint, GLint, GLint);
GLAPI PFNGLNORMAL3IPROC glNormal3i; 
typedef void (*PFNGLNORMAL3SPROC)(GLshort, GLshort, GLshort);
GLAPI PFNGLNORMAL3SPROC glNormal3s; 
typedef void (*PFNGLSECONDARYCOLOR3BPROC)(GLbyte, GLbyte, GLbyte);
GLAPI PFNGLSECONDARYCOLOR3BPROC glSecondaryColor3b;
typedef void (*PFNGLSECONDARYCOLOR3SPROC)(GLshort, GLshort, GLshort);
GLAPI PFNGLSECONDARYCOLOR3SPROC glSecondaryColor3s;
typedef void (*PFNGLSECONDARYCOLOR3IPROC)(GLint, GLint, GLint);
GLAPI PFNGLSECONDARYCOLOR3IPROC glSecondaryColor3i;
typedef void (*PFNGLSECONDARYCOLOR3FPROC)(GLfloat, GLfloat, GLfloat);
GLAPI PFNGLSECONDARYCOLOR3FPROC glSecondaryColor3f;
typedef void (*PFNGLSECONDARYCOLOR3DPROC)(GLdouble, GLdouble, GLdouble);
GLAPI PFNGLSECONDARYCOLOR3DPROC glSecondaryColor3d;
typedef void (*PFNGLSECONDARYCOLOR3UBPROC)(GLubyte, GLubyte, GLubyte);
GLAPI PFNGLSECONDARYCOLOR3UBPROC glSecondaryColor3ub;
typedef void (*PFNGLSECONDARYCOLOR3USPROC)(GLushort, GLushort, GLushort);
GLAPI PFNGLSECONDARYCOLOR3USPROC glSecondaryColor3us;
typedef void (*PFNGLSECONDARYCOLOR3UIPROC)(GLuint, GLuint, GLuint);
GLAPI PFNGLSECONDARYCOLOR3UIPROC glSecondaryColor3ui;
typedef void (*PFNGLTEXCOORD1SPROC)(GLshort);
GLAPI PFNGLTEXCOORD1SPROC glTexCoord1s; 
typedef void (*PFNGLTEXCOORD1IPROC)(GLint);
GLAPI PFNGLTEXCOORD1IPROC glTexCoord1i; 
typedef void (*PFNGLTEXCOORD1FPROC)(GLfloat);
GLAPI PFNGLTEXCOORD1FPROC glTexCoord1f; 
typedef void (*PFNGLTEXCOORD1DPROC)(GLdouble);
GLAPI PFNGLTEXCOORD1DPROC glTexCoord1d; 
typedef void (*PFNGLTEXCOORD2SPROC)(GLshort, GLshort);
GLAPI PFNGLTEXCOORD2SPROC glTexCoord2s; 
typedef void (*PFNGLTEXCOORD2IPROC)(GLint, GLint);
GLAPI PFNGLTEXCOORD2IPROC glTexCoord2i; 
typedef void (*PFNGLTEXCOORD2FPROC)(GLfloat, GLfloat);
GLAPI PFNGLTEXCOORD2FPROC glTexCoord2f; 
typedef void (*PFNGLTEXCOORD2DPROC)(GLdouble, GLdouble);
GLAPI PFNGLTEXCOORD2DPROC glTexCoord2d; 
typedef void (*PFNGLTEXCOORD3SPROC)(GLshort, GLshort, GLshort);
GLAPI PFNGLTEXCOORD3SPROC glTexCoord3s; 
typedef void (*PFNGLTEXCOORD3IPROC)(GLint, GLint, GLint);
GLAPI PFNGLTEXCOORD3IPROC glTexCoord3i; 
typedef void (*PFNGLTEXCOORD3FPROC)(GLfloat, GLfloat, GLfloat);
GLAPI PFNGLTEXCOORD3FPROC glTexCoord3f; 
typedef void (*PFNGLTEXCOORD3DPROC)(GLdouble, GLdouble, GLdouble);
GLAPI PFNGLTEXCOORD3DPROC glTexCoord3d; 
typedef void (*PFNGLTEXCOORD4SPROC)(GLshort, GLshort, GLshort, GLshort);
GLAPI PFNGLTEXCOORD4SPROC glTexCoord4s; 
typedef void (*PFNGLTEXCOORD4IPROC)(GLint, GLint, GLint, GLint);
GLAPI PFNGLTEXCOORD4IPROC glTexCoord4i; 
typedef void (*PFNGLTEXCOORD4FPROC)(GLfloat, GLfloat, GLfloat, GLfloat);
GLAPI PFNGLTEXCOORD4FPROC glTexCoord4f; 
typedef void (*PFNGLTEXCOORD4DPROC)(GLdouble, GLdouble, GLdouble, GLdouble);
GLAPI PFNGLTEXCOORD4DPROC glTexCoord4d; 
typedef void (*PFNGLVERTEX2SPROC)(GLshort, GLshort);
GLAPI PFNGLVERTEX2SPROC glVertex2s; 
typedef void (*PFNGLVERTEX2IPROC)(GLint, GLint);
GLAPI PFNGLVERTEX2IPROC glVertex2i; 
typedef void (*PFNGLVERTEX2FPROC)(GLfloat, GLfloat);
GLAPI PFNGLVERTEX2FPROC glVertex2f; 
typedef void (*PFNGLVERTEX2DPROC)(GLdouble, GLdouble);
GLAPI PFNGLVERTEX2DPROC glVertex2d; 
typedef void (*PFNGLVERTEX3SPROC)(GLshort, GLshort, GLshort);
GLAPI PFNGLVERTEX3SPROC glVertex3s; 
typedef void (*PFNGLVERTEX3IPROC)(GLint, GLint, GLint);
GLAPI PFNGLVERTEX3IPROC glVertex3i; 
typedef void (*PFNGLVERTEX3FPROC)(GLfloat, GLfloat, GLfloat);
GLAPI PFNGLVERTEX3FPROC glVertex3f; 
typedef void (*PFNGLVERTEX3DPROC)(GLdouble, GLdouble, GLdouble);
GLAPI PFNGLVERTEX3DPROC glVertex3d; 
typedef void (*PFNGLVERTEX4S)(GLshort, GLshort, GLshort, GLshort);
GLAPI PFNGLVERTEX4S glVertex4s; 
typedef void (*PFNGLVERTEX4IPROC)(GLint, GLint, GLint, GLint);
GLAPI PFNGLVERTEX4IPROC glVertex4i; 
typedef void (*PFNGLVERTEX4FPROC)(GLfloat, GLfloat, GLfloat, GLfloat);
GLAPI PFNGLVERTEX4FPROC glVertex4f; 
typedef void (*PFNGLVERTEX4DPROC)(GLdouble, GLdouble, GLdouble, GLdouble);
GLAPI PFNGLVERTEX4DPROC glVertex4d; 

#ifdef __cplusplus
}
#endif

#endif
