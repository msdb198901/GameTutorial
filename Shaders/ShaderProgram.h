#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const int MAX_LIGHTS = 4;
class ShaderProgram
{
public:
	void LoadShader(const char* vertexPath, const char* fragmentPath);

	void Start();

	void Stop();

	void CleanUp();

	void BindAttribute(int attribute, const char* variable);

	virtual void BindAttributes() = 0;
	virtual void GetAllUniformLocations() = 0;

protected:
	int GetUniformLocation(const char* uniformName);

	void LoadFloat(int location, float value);
	void LoadInt(int location, int value);
	void LoadVector(int location, const glm::vec2& vector);
	void LoadVector(int location, const glm::vec3& vector);
	void LoadVector(int location, const glm::vec4& vector);
	void LoadMatrix(int location, const glm::mat4& matrix);
	void LoadBool(int location, bool value);

public:
	int GetShaderProgramID()
	{
		return m_programID;
	}

private:
	int m_programID;
	int m_vertexID;
	int m_fragmentID;
};