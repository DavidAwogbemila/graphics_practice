#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

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

  std::cout << "This is the master branch" << std::endl;
  if (glewInit() != GLEW_OK) {
    std::cout << "Glew not ok" << std::endl;
  } else {
    std::cout << "Glew ok!" << std::endl;
    std::cout << "On this run the OpenGL version is: " << glGetString(GL_VERSION) << std::endl;
  }
  {
    /* Let's make a triangle, and then squares.*/
    float triangle_positions[16] = {
      -0.5f, -0.5f, 0.0f, 0.0f,
      0.5f, -0.5f, 1.0f, 0.0f,
      0.5f, 0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f, 0.0f, 1.0f
    };

    //GLCall(glEnable(GL_BLEND));
    //GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ));

    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    VertexArray va;
    VertexBuffer vb(triangle_positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    Shader shader("res/shader/Basic.shader");
    shader.Bind();
    //shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

    Texture texture("res/textures/wall.jpg");
    texture.Bind(0);
    shader.SetUniform1i("u_Texture", 0);

    va.UnBind();
    vb.UnBind();
    ib.UnBind();
    shader.UnBind();

    Renderer renderer;
    float red_color = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
      /* Render here */
      renderer.Clear();

      shader.Bind();
      //shader.SetUniform4f("u_Color", red_color, 0.3f, 0.8f, 1.0f);
      renderer.Draw(va, ib, shader);

      if (red_color > 1.0f) {
        increment = -0.05f;
      } else if (red_color < 0.0f) {
        increment = 0.05f;
      }

      red_color += increment;

      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }
  }
  glfwTerminate();
  return 0;
}
