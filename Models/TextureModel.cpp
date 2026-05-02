#include "StdAfx.h"
#include "TextureModel.h"
#include "RawModel.h"
#include "Texture.h"

TextureModel::TextureModel(RawModel *model, Texture *texture)
{
	m_modelTexture = texture;
	m_rawModel = model;
}