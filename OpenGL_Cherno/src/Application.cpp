#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout_impl.h"

struct ShaderProgramSource {
  std::string VertexSoure;
  std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string&  file_path) {
  std::ifstream stream(file_path);

  enum class ShaderType {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
  };
  ShaderType type = ShaderType::NONE;
  std::string line;
  std::stringstream ss[2];

  while (getline(stream, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos) {
        type = ShaderType::VERTEX;
      } else if (line.find("fragment") != std::string::npos) {
        type = ShaderType::FRAGMENT;
      }
    } else {
      ss[(int)type] << line << "\n";
    }
  }

  return { ss[(int)ShaderType::VERTEX].str(), ss[(int)ShaderType::FRAGMENT].str() };
}

static unsigned int compileShader(unsigned int type, const std::string& source) {
  unsigned int id = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);

  glCompileShader(id);

  // Error checking
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

    char* msg = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, msg);
    std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
    std::cout << msg << std::endl;
    glDeleteShader(id);
    return 0;
  }
  return id;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

int main() {
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  /* Adjust frame rate. */
  glfwSwapInterval(1);

  std::cout << "This is the experimental branch." << std::endl;
  if (glewInit() != GLEW_OK) {
    std::cout << "Glew not ok" << std::endl;
  } else {
    std::cout << "Glew ok!" << std::endl;
    std::cout << "Our OpenGL version is: " << glGetString(GL_VERSION) << std::endl;
  }

  { // We use this namespace to cause the destructors of
    // the classes to be called before glfwTerminate is called.
    /* Let's make 2 shapes, a rectangle and a triangle. */
    float object0_positions[8] = {
      -0.5f, -0.5f,
      -0.1f, -0.5f,
      -0.1f, 0.5f,
      -0.5f, 0.5f,
    };

    float object1_positions[6] = {
      //0.1f, -0.5f,
      0.5f, -0.5f,
      0.5f, 0.5f,
      0.1f, 0.5f,
    };

    int indices0[] = {
      0, 1, 2,
      2, 3, 0
    };

    int indices1[] = {
      0, 1, 2
    };

    // make vertex buffers for the desired objects.
    VertexArray va0;
    VertexBufferLayout layout0;
    VertexBuffer vb0(object0_positions, 4 * 2 * sizeof(float));
    IndexBuffer ib0(indices0, 6);

    vb0.Bind();
    layout0.Push<float>(2);
    va0.AddBuffer(vb0, layout0);

    VertexArray va1;
    VertexBufferLayout layout1;
    VertexBuffer vb1(object1_positions, 3 * 2 * sizeof(float));
    IndexBuffer ib1(indices1, 3);

    vb1.Bind();
    layout1.Push<float>(2);
    va1.AddBuffer(vb1, layout1);

    ShaderProgramSource source = ParseShader("res/shader/Basic.shader");
    unsigned int shader = createShader(source.VertexSoure, source.FragmentSource);
    GLCall(glUseProgram(shader));

    GLCall(int location = glGetUniformLocation(shader, "u_color"));
    ASSERT(location != -1);
    GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

    float red_color = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
      /* Render here */
      glClear(GL_COLOR_BUFFER_BIT);

      GLCall(glUseProgram(shader));
      GLCall(glUniform4f(location, red_color, 0.3f, 0.8f, 1.0f));

      va0.Bind();
      vb0.Bind();
      ib0.Bind();
      GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); //we can put nullptr because we have bound our ibo earlier.

      if (red_color > 1.0f) {
        increment = -0.05f;
      } else if (red_color < 0.0f) {
        increment = 0.05f;
      }

      va1.Bind();
      vb1.Bind();
      ib1.Bind();
      GLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr)); //we can put nullptr because we have bound our ibo earlier.

      red_color += increment;

      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }

    glDeleteProgram(shader);
  }
  glfwTerminate();
  return 0;
}