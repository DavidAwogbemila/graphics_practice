#pragma once

class IndexBuffer {
private:
  unsigned int m_RendererID;
  unsigned int m_count;

public:
  IndexBuffer(const unsigned int* data, const unsigned int count);
  ~IndexBuffer();

  void Bind() const;
  void UnBind() const;

  inline unsigned int GetCount() { return m_count; }
};
