#include <stdio.h>

#include "pix.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

static double deltaTime = 0;

#define ONE_THIRD  (1.0 / 3.0)
#define TWO_THIRDS (2.0 / 3.0)

static inline pxColorRgba 
interpolateColors(pxColorRgba a, pxColorRgba b, pxColorRgba c, double t)
{
    t = fmod(t, 1);
    if ( t <= ONE_THIRD ) {
        return pxColorLerp(a, b, 1.0 - t * 3);
    }  else if ( t <= TWO_THIRDS ) {
        return pxColorLerp(b, c, 1.0 - (t - ONE_THIRD) * 3);
    } else {
        return pxColorLerp(c, a, 1.0 - (t - TWO_THIRDS) * 3);
    }
}

static inline void
renderFrame()
{
    static double t = 0;
    pxColorRgba colors[3] = {
        interpolateColors(pxColor_Red, pxColor_Green, pxColor_Blue, t),
        interpolateColors(pxColor_Green, pxColor_Blue, pxColor_Red, t),
        interpolateColors(pxColor_Blue, pxColor_Red, pxColor_Green, t)
    };
    glClear(GL_COLOR_BUFFER_BIT); 
    glBegin(GL_TRIANGLES);
    glColor3f(colors[0].r / 255.0, colors[0].g / 255.0, colors[0].b / 255.0); 
    glVertex3f(-0.5, -0.5, 0.0);
    glColor3f(colors[1].r / 255.0, colors[1].g / 255.0, colors[1].b / 255.0); 
    glVertex3f( 0.0,  0.5, 0.0);
    glColor3f(colors[2].r / 255.0, colors[2].g / 255.0, colors[2].b / 255.0); 
    glVertex3f( 0.5, -0.5, 0.0);
    glEnd();
    t += deltaTime; 
}

static void
onResize(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
}

int
main(void)
{
    GLFWwindow *window;
    unsigned frames = 0; 
    double timeStart, timeCurrent, timeDelta;
    char title[64]; 
    if ( glfwInit() != GLFW_TRUE ) {
        printf("failed to init GLFW\n"); 
        return -1;
    }
    window = glfwCreateWindow(500, 500, "Win32 Demo [GLFW]", NULL, NULL); 
    if ( window == NULL ) {
        const char *desc; 
        glfwGetError(&desc);
        printf("failed to create glfwWindow -> %s\n", desc);
        return -2;
    }
    glfwMakeContextCurrent(window);
    if ( pxInitGL() != PIX_SUCCESS) {
        printf("failed to init pxGL\n");
        return -3;
    }
    pxLoadGLFunctions();
    glfwSetWindowSizeCallback(window, onResize);
    timeStart = glfwGetTime();
    while ( glfwWindowShouldClose(window) == 0 ) {
        frames++; 
        timeCurrent = glfwGetTime(), timeDelta = timeCurrent - timeStart;
        if ( timeDelta >= 1.0 ) {
            timeStart = timeCurrent;
            double fps = frames / timeDelta;
            snprintf(title, sizeof(title), "Win32 Demo - FPS %u/%.2fms [GLFW]", (unsigned) fps, 1000.0 / fps); 
            glfwSetWindowTitle(window, title);
            frames = 0; 
        }
        renderFrame(); 
        glfwSwapBuffers(window);
        glfwPollEvents();
        deltaTime = glfwGetTime() - timeCurrent; 
    }
    glfwDestroyWindow(window);
    glfwTerminate(); 
    return 0;
}