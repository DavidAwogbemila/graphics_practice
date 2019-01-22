#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include "TestTexture2D.h"
#include <iostream>
namespace test {

  TestTexture2D::TestTexture2D()
    : m_TranslationA(-100, 0, 0),
    m_TranslationB(100, 0, 0),
    m_Proj(glm::ortho(-480.0f, 480.0f, -480.0f, 480.0f, -1.0f, 1.0f)),
    m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))) {
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

    m_VAO = std::make_unique<VertexArray>();

    m_VertexBuffer = std::make_unique<VertexBuffer>(triangle_positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    m_VAO->AddBuffer(*m_VertexBuffer, layout);

    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);
    m_Texture = std::make_unique<Texture>("res/textures/wall.jpg");

    m_Shader = std::make_unique<Shader>("res/shader/Basic.shader");
    m_Shader->Bind();
    m_Shader->SetUniform1i("u_Texture", 0);
  }

  TestTexture2D::~TestTexture2D() {
  }

  void TestTexture2D::OnUpdate(float deltaTime) {
  }

  void TestTexture2D::OnRender() {
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    Renderer renderer;

    m_Texture->Bind(0);

    glm::mat4 proj = glm::ortho(-480.0f, 480.0f, -480.0f, 480.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
      glm::mat4 mvp = m_Proj * m_View * model;
      m_Shader->Bind();
      m_Shader->SetUniformMat4f("u_MVP", mvp);
      renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
    }

    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
      glm::mat4 mvp = m_Proj * m_View * model;
      m_Shader->Bind();
      m_Shader->SetUniformMat4f("u_MVP", mvp);
      renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
    }
  }

  void TestTexture2D::OnImGuiRender() {
    //ImGui::Checkbox("Texture selector", &texture_1_selected);
    ImGui::SliderFloat3("floatA", &m_TranslationA.x, 0.0f, 960.0f);
    ImGui::SliderFloat3("floatB", &m_TranslationB.x, 0.0f, 960.0f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
      1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  }

}
