#pragma once

#include <string>

#include "Test.h"
#include "Texture.h"

namespace test {

  class TestTextures : public Test {
  public:
    TestTextures();
    ~TestTextures();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;

  private:
    bool texture_1_selected;
    Texture m_Texture_1;
    Texture m_Texture_2;
  };

}

