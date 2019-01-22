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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTextures.h"

int main() {
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(960, 960, "Hello World", NULL, NULL);
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
    std::cout << "On this run the OpenGL version is: " <<
      glGetString(GL_VERSION) << std::endl;
  }

  {
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    {
      /* Let's make a triangle, and then squares.*/
      float triangle_positions[16] = {
        -50.0f, -50.0f, 0.0f, 0.0f,
        50.0f, -50.0f, 1.0f, 0.0f,
        50.0f, 50.0f, 1.0f, 1.0f,
        -50.0f, 50.0f, 0.0f, 1.0f
      };

      GLCall(glEnable(GL_BLEND));
      GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

      unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

      VertexArray va;
      VertexBuffer vb(triangle_positions, 4 * 4 * sizeof(float));
      VertexBufferLayout layout;
      layout.Push<float>(2);
      layout.Push<float>(2);
      va.AddBuffer(vb, layout);

      IndexBuffer ib(indices, 6);

      glm::mat4 proj = glm::ortho(-480.0f, 480.0f, -480.0f, 480.0f, -1.0f, 1.0f);
      glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

      glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
      glm::mat4 mvp = proj * view * model;

      Shader shader("res/shader/Basic.shader");
      shader.Bind();

      Renderer renderer;

      ImGui::CreateContext();

      ImGui_ImplGlfwGL3_Init(window, true);
      ImGui::StyleColorsDark();

      test::TestClearColor test;
      test::TestTextures testTextures;

      while (!glfwWindowShouldClose(window)) {
        renderer.Clear();

        test.OnUpdate(0.0f);
        test.OnRender();

        testTextures.OnUpdate(0.0f);
        testTextures.OnRender();

        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));
        glm::mat4 mvp = proj * view * model;
        shader.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(va, ib, shader);

        ImGui_ImplGlfwGL3_NewFrame();
        test.OnImguiRender();
        testTextures.OnImguiRender();

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

        glfwPollEvents();
      }
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
  }
  return 0;
}
