//
//  Application.cpp
//  OpenGLFramework
//
//  Created by Aybars Acar on 21/1/21.
//

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <cstring>


/**
 * Abstract the compiling shader logic to avoid repetitios code
 * source string must not be nullptr
 */
static unsigned int CompileShader(unsigned int type, const std::string &source)
{
  unsigned int id = glCreateShader(type);
  
//  get a pointer to the source data
  const char* src = source.c_str();   // &source[0]
  
  glShaderSource(id, 1, &src, nullptr);
  
  glCompileShader(id);
  
//  Error handling
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);

  if (result == GL_FALSE)
  {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char *message = new char[length];
    glGetShaderInfoLog(id, length, &length, message);
    
    std::cout << "Failed to compile the " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
    std::cout << message << std::endl;
    
    delete [] message;
    
    glDeleteShader(id); // because compilation did not work
    return 0;
  }
  
  return id;
}

/**
 * Shader function
 * to render both shaders
 */
static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
//  create a shader program
  unsigned int program = glCreateProgram();
  
//  create 2 shader obj
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
  
//  link the shaders
  glAttachShader(program, vs);
  glAttachShader(program, fs);

//  link and validate the program
  glLinkProgram(program);
  glValidateProgram(program);
  
//  delete your shaders since they are now linked to the program
  glDeleteShader(vs);
  glDeleteShader(fs);
 
  return program;
}

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
  std::cout << "Supported GLSL version is " << (char *)glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  

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
  
//  we need to enable the vertex attribute with the index you want to enable
  glEnableVertexAttribArray(0);
//  specify the attribute for hte first vertex whihc is at index 0, last 0 is 0 bytes
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
  
  
//  write the shader - instructions on how to draw on the window
  std::string vertexShader = R"glsl(
      
    #version 120
  
    attribute vec4 position;
  
    void main()
    {
      gl_Position = position;
    }
  
  )glsl";
  
  std::string fragmentShader = R"glsl(
      
    #version 120
  
    void main()
    {
      gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
  
  )glsl";
  

  unsigned int shader = CreateShader(vertexShader, fragmentShader);
  
//  bind the shader
  glUseProgram(shader);
  
  
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
  
  glDeleteProgram(shader);
  
  glfwTerminate();
  
  return 0;
}
