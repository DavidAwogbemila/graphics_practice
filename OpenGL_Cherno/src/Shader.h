#pragma once
#include <string>
#include <unordered_map>

struct ShaderProgramSource {
  std::string VertexSoure;
  std::string FragmentSource;
};

class Shader {
private:
  std::string m_FilePath;
  unsigned int m_RendererID;
  ShaderProgramSource ParseShader(const std::string&  file_path);
  unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
  unsigned int compileShader(unsigned int type, const std::string& source);
  int GetUniformLocation(const std::string& name);
  //caching for uniforms
  std::unordered_map<std::string, int> m_UniformLocationCache;

public:
  Shader(const std::string& filename);
  ~Shader();

  void Bind() const;
  void UnBind() const; 

  // Get and set uniforms.
  void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
  void SetUniform1i(const std::string& name, int value);
  //unsigned int GetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
};
