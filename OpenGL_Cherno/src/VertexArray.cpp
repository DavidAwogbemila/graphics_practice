#include <vector>

#include "VertexArray.h"

VertexArray::VertexArray() {
  GLCall(glGenVertexArrays(1, &m_RendererID));
  GLCall(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray() {
  GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::Bind() {
  GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() {
  GLCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(VertexBuffer& vb, VertexBufferLayout& layout) const {
  std::vector<VertexBufferElement> elements = layout.getElements();
  unsigned int offset = 0;
  for (unsigned int i = 0; i < elements.size(); i++) {
    VertexBufferElement element = elements[i];
    GLCall(glVertexAttribPointer(i,
                                 element.count,
                                 element.type,
                                 element.normalized,
                                 layout.getStride(),
                                 (const void*)offset));
    offset += element.count * sizeof(element.type);
    GLCall(glEnableVertexAttribArray(i));
  }
}
