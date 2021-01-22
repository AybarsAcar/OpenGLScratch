//
//  main.cpp
//  OpenGLFramework
//
//  Created by Aybars Acar on 21/1/21.
//

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


int main(void)
{
  GLFWwindow* window;
  
  /* Initialize the library */
  if (!glfwInit())
    return -1;
 
  
  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }
  
//  Create the openGl context
  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  
//  initialise glew
  if (glewInit() != GLEW_OK) std::cout << "Error" << std::endl;
  
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  

  float positions[6] =
  {
    -0.5f,  -0.5f,
     0.0f,   0.5f,
     0.5f,  -0.5f,
  };
  
  
//  define the vertex buffer
  unsigned int buffer;
  glGenBuffers(1, &buffer); // gives us back an id
  
//  select that bufffer
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  
//  specify the data
//  put the data into the buffer
//  6 * sizeof(floa) == sizeof(positions)
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
  
  
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);
    
//    draw the triangle specified - draw call
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glEnd();
    
    
    /* Swap front and back buffers */
    glfwSwapBuffers(window);
    
    /* Poll for and process events */
    glfwPollEvents();
  }
  
  glfwTerminate();
  
  return 0;
}
