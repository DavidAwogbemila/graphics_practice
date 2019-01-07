#pragma once
#include "Renderer.h"

class IndexBuffer {
private: 
  unsigned int m_RendererID;

public:
  IndexBuffer(const void * data, const unsigned int size);
  ~IndexBuffer();

  void Bind() const;
  void UnBind() const;
};

