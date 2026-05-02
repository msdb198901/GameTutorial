#pragma once

// 模型纹理类 用于模型贴图
class Texture
{
public:
	Texture(int id);

	int GetID()
	{
		return m_textureID;
	}

	void SetShineDamper(float shineDamper)
	{
		m_shineDamper = shineDamper;
	}

	void SetReflectivity(float reflectivity)
	{
		m_reflectivity = reflectivity;
	}

	float GetShineDamper()
	{
		return m_shineDamper;
	}

	float GetReflectivity()
	{
		return m_reflectivity;
	}
private:
	int m_textureID;
	// 光照衰减系数 光照强度衰减 越小越不明显 越接近0 越不明显
	float m_shineDamper = 1;
	// 反射率 越接近1 越明显 越接近0 越不明显
	float m_reflectivity = 0;
};