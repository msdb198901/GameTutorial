#pragma once



class RawModel;
class Texture; // 用于给原始模型贴图的纹理类
// 纹理模型 包含纹理数据 （带纹理模式类：同时包含模型和纹理数据）
class TextureModel
{
public:
	TextureModel(RawModel *model, Texture* texture);

public:
	RawModel* GetRawModel()
	{
		return m_rawModel;
	}

	Texture* GetTexture()
	{
		return m_modelTexture;
	}

private:
	RawModel* m_rawModel;		// 原始模型包含模型数据
	Texture*  m_modelTexture;	// 纹理模型包含纹理数据
};