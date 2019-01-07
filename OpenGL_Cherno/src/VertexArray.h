#pragma once
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {
private:
  unsigned int m_RendererID;

public:
  VertexArray();
  ~VertexArray();

  void Bind();
  void UnBind();

  void AddBuffer(VertexBuffer& vb, VertexBufferLayout& layout) const;
};

