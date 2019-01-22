#include "TestTextures.h"
#include "Renderer.h"
#include "Texture.h"

#include "imgui/imgui.h"

namespace test {

  TestTextures::TestTextures()
    : m_Texture_1(Texture("res/textures/wall.jpg")),
      m_Texture_2(Texture("res/textures/book-icon.png")) {
    m_Texture_1.Bind();
    texture_1_selected = true;
  }

  TestTextures::~TestTextures() {
  }

  void TestTextures::OnUpdate(float deltaTime) {
  }

  void TestTextures::OnRender() {
    if (texture_1_selected) {
      m_Texture_2.Bind(0);
    } else {
      m_Texture_1.Bind(0);
    }
  }

  void TestTextures::OnImGuiRender() {
    ImGui::Checkbox("Texture selector", &texture_1_selected);
  }

}
