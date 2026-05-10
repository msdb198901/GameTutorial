#pragma once

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <learnopengl/bone.h>
#include <functional>
#include <learnopengl/animdata.h>
#include <learnopengl/model.h>

#include <assimp/ai_assert.h>
#include <assimp/cfileio.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/Importer.hpp>
#include <assimp/LogStream.hpp>

static bool g_bWasFlipped = false;
static float g_smoothAngle = 80.f;
static bool nopointslines = false;
// default pp steps
static unsigned int ppsteps = aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
aiProcess_JoinIdenticalVertices | // join identical vertices/ optimize indexing
aiProcess_ValidateDataStructure | // perform a full validation of the loader's output
aiProcess_ImproveCacheLocality | // improve the cache locality of the output vertices
aiProcess_RemoveRedundantMaterials | // remove redundant materials
aiProcess_FindDegenerates | // remove degenerated polygons from the import
aiProcess_FindInvalidData | // detect invalid model data, such as invalid normal vectors
aiProcess_GenUVCoords | // convert spherical, cylindrical, box and planar mapping to proper UVs
aiProcess_TransformUVCoords | // preprocess UV transformations (scaling, translation ...)
aiProcess_FindInstances | // search for instanced meshes and remove them by references to one master
aiProcess_LimitBoneWeights | // limit bone weights to 4 per vertex
aiProcess_OptimizeMeshes | // join small meshes, if possible;
aiProcess_SplitByBoneCount | // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
0;


struct AssimpNodeData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

class Animation
{
public:
	Animation() = default;

	Animation(const std::string& animationPath, Model* model)
	{

		//aiPropertyStore* props = aiCreatePropertyStore();
		//aiSetImportPropertyInteger(props, AI_CONFIG_IMPORT_TER_MAKE_UVS, 1);
		//aiSetImportPropertyFloat(props, AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, g_smoothAngle);
		//aiSetImportPropertyInteger(props, AI_CONFIG_PP_SBP_REMOVE, nopointslines ? aiPrimitiveType_LINE | aiPrimitiveType_POINT : 0);
		//aiSetImportPropertyInteger(props, AI_CONFIG_GLOB_MEASURE_TIME, 1);

		//const aiScene* scene = (aiScene*)aiImportFileExWithProperties(animationPath.c_str(),
		//	ppsteps | /* configurable pp steps */
		//	aiProcess_GenSmoothNormals | // generate smooth normal vectors if not existing
		//	aiProcess_SplitLargeMeshes | // split large, unrenderable meshes into submeshes
		//	aiProcess_Triangulate | // triangulate polygons with more than 3 edges
		//	aiProcess_ConvertToLeftHanded | // convert everything to D3D left handed space
		//	aiProcess_SortByPType | // make 'clean' meshes which consist of a single typ of primitives
		//	0,
		//	nullptr,
		//	props);

		//aiReleasePropertyStore(props);

		//if (!scene) {
		//	std::string err = aiGetErrorString();  // 获取最后的错误信息
		//	std::cerr << "Assimp error: " << err << std::endl;
		//	// 或者 OutputDebugStringA(err.c_str()); 用于 Visual Studio 输出窗口
		//	return;
		//}

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
		assert(scene && scene->mRootNode);
		auto animation = scene->mAnimations[0];
		m_Duration = animation->mDuration;
		m_TicksPerSecond = animation->mTicksPerSecond;
		aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
		globalTransformation = globalTransformation.Inverse();
		ReadHierarchyData(m_RootNode, scene->mRootNode);
		ReadMissingBones(animation, *model);

		for (auto& pair : model->GetBoneInfoMap())
		{
			std::cout << "Bone: " << pair.first << " id: " << pair.second.id << std::endl;
		}
	}

	~Animation()
	{
	}

	Bone* FindBone(const std::string& name)
	{
		auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
			[&](const Bone& Bone)
			{
				return Bone.GetBoneName() == name;
			}
		);
		if (iter == m_Bones.end()) return nullptr;
		else return &(*iter);
	}

	
	inline float GetTicksPerSecond() { return m_TicksPerSecond; }
	inline float GetDuration() { return m_Duration;}
	inline const AssimpNodeData& GetRootNode() { return m_RootNode; }
	inline const std::map<std::string,BoneInfo>& GetBoneIDMap() 
	{ 
		return m_BoneInfoMap;
	}

private:
	void ReadMissingBones(const aiAnimation* animation, Model& model)
	{
		int size = animation->mNumChannels;

		auto& boneInfoMap = model.GetBoneInfoMap();//getting m_BoneInfoMap from Model class
		int& boneCount = model.GetBoneCount(); //getting the m_BoneCounter from Model class

		//reading channels(bones engaged in an animation and their keyframes)
		for (int i = 0; i < size; i++)
		{
			auto channel = animation->mChannels[i];
			std::string boneName = channel->mNodeName.data;

			if (boneInfoMap.find(boneName) == boneInfoMap.end())
			{
				boneInfoMap[boneName].id = boneCount;
				boneCount++;
			}
			m_Bones.push_back(Bone(channel->mNodeName.data,
				boneInfoMap[channel->mNodeName.data].id, channel));
		}

		m_BoneInfoMap = boneInfoMap;
	}

	void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src)
	{
		assert(src);

		dest.name = src->mName.data;
		dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
		dest.childrenCount = src->mNumChildren;

		for (int i = 0; i < src->mNumChildren; i++)
		{
			AssimpNodeData newData;
			ReadHierarchyData(newData, src->mChildren[i]);
			dest.children.push_back(newData);
		}
	}
	float m_Duration;
	int m_TicksPerSecond;
	std::vector<Bone> m_Bones;
	AssimpNodeData m_RootNode;
	std::map<std::string, BoneInfo> m_BoneInfoMap;
};
