//
//  Application.cpp
//  OpenGLFramework
//
//  Created by Aybars Acar on 21/1/21.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "Renderer.h"

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "VertexBufferLayout.hpp"

int main(void)
{
  GLFWwindow* window;
  
  /* Initialize the library */
  if (!glfwInit())
    return -1;
  
  //  tell glfw we want to create this context with the call profile
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  
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
  
  glfwSwapInterval(1);  // 1 is the default frame rate 60hz etc
  
  //  initialise glew
  if (glewInit() != GLEW_OK) std::cout << "Error" << std::endl;
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "Supported GLSL version is " << (char *)glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  
  
  {
    //  draw a square with 2 trieangles
    float positions[] =
    {
      -0.5f,  -0.5f,    // 0
      0.5f,  -0.5f,    // 1
      0.5f,   0.5f,    // 2
      -0.5f,   0.5f,    // 3
    };
    
    //  indices buffer
    unsigned int indices[] =
    {
      0, 1, 2,      // triangle 1
      2, 3, 0,      // triangle 2
    };
    
//    initialise the vertex array
    VertexArray va;
    
    //  create the vertex buffer - our constructor automatically binds it as well
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    
    VertexBufferLayout layout;
    
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
  
    //  define the index buffer
    IndexBuffer ib(indices, 6);
    
    Shader shader("res/shaders/Basic.shader");
    shader.Bind();
    
    shader.SetUniform4F("u_Color", 0.8f, 0.3f, 0.8f, 0.1f);
    
    //  unbind everything - we have to do this each time we are changing the draw geometry
    va.Unbind();
    ib.Unbind();
    vb.Unbind();
    shader.Unbind();
    
    Renderer renderer;
    
    float redChannel = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
      /* Render here */
      renderer.Clear();
      
      //    bind them back
      shader.Bind();
      
      //    pass down the colour dynamically
      shader.SetUniform4F("u_Color", redChannel, 0.3f, 0.8f, 1.0f);
      
      renderer.Draw(va, ib, shader);
      
      //    draw the triangle specified - draw call
      GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
      
      //    Animate the colour
      if (redChannel > 1.0f)
      {
        increment = -0.05f;
      }
      else if (redChannel < 0.0f)
      {
        increment = 0.05f;
      }
      redChannel += increment;
      
      glEnd();
      
      
      /* Swap front and back buffers */
      glfwSwapBuffers(window);
      
      /* Poll for and process events */
      glfwPollEvents();
    }
  }
  
  glfwTerminate();
  
  return 0;
}
