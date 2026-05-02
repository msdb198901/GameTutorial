#pragma once

// 原始模型 仅包含模型数据
class RawModel
{
public:
	RawModel(int id, int vertexCount);

public:
	int GetVAOID() 
	{
		return m_iVAOID;
	}

	int GetVertexCount()
	{
		return m_vertexCount;
	}

private:
	int m_iVAOID;		// VAO ID
	int m_vertexCount;  // 该模型顶点数量
};