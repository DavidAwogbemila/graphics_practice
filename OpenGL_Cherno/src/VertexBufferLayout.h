#pragma once
#include <vector>
#include <GL/glew.h>

#include "Renderer.h"

struct VertexBufferElement {
  const unsigned int type;
  const unsigned int count;
  const unsigned int normalized;
};

class VertexBufferLayout {
private:
  unsigned int m_Stride;
  std::vector<VertexBufferElement> m_Elements;

public:
  VertexBufferLayout() {};
  ~VertexBufferLayout() {};
  std::vector<VertexBufferElement> getElements() const { return m_Elements; }
  unsigned int getStride() const { return m_Stride;  }

  template<typename T>
  void Push(unsigned int count);

  template<>
  void Push<float>(unsigned int count);

  template<>
  void Push<unsigned int>(unsigned int count);

  template<>
  void Push<unsigned char>(unsigned int count);
};

