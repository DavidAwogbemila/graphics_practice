#pragma once
class VertexBuffer {
private:
  unsigned int m_RenderID;
public:
  void Bind() const;
  void UnBind() const;
public:
  VertexBuffer(void* data);
  ~VertexBuffer();
};

