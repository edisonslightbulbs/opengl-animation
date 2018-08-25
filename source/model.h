#pragma once

#include "mesh.h"

#include <map>
#include <vector>
#include <string>
using namespace std;


#ifdef __linux__
 #include <GL/glew.h>
 #include <assimp/scene.h>
 #include <assimp/Importer.hpp>
 #include <assimp/postprocess.h>
 #include <glm/gtc/quaternion.hpp>
#endif


#ifdef _WIN32 || _WIN64
 #include "GL\glew.h"
 #include "glm\gtc\quaternion.hpp"
 #include "assimp\Importer.hpp"
 #include "assimp\scene.h"
 #include "assimp\postprocess.h"
#endif


class Model
{
public:
	Model();
	~Model();
	static const uint MAX_BONES = 100000;


	void initShaders(GLuint shader_program);
	void loadModel(const string& path);
	void update();
	void draw(GLuint shaders_program);
	void showNodeName(aiNode* node);

	glm::mat4 aiToGlm(aiMatrix4x4 ai_matr);
	aiQuaternion nlerp(aiQuaternion a, aiQuaternion b, float blend); // lerp

private:
	Assimp::Importer import;
	const aiScene* scene;
	vector<Mesh> meshes; // one mesh in one object
	string directory;

	map<string, uint> m_bone_mapping; // maps a bone name and their index
	uint m_num_bones = 0;
	vector<BoneMatrix> m_bone_matrices;
	aiMatrix4x4 m_global_inverse_transform;

	GLuint m_bone_location[MAX_BONES];
	float ticks_per_second = 0.0f;

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> LoadMaterialTexture(aiMaterial* mat, aiTextureType type, string type_name);


	uint findPosition(float p_animation_time, const aiNodeAnim* p_node_anim);
	uint findRotation(float p_animation_time, const aiNodeAnim* p_node_anim);
	uint findScaling(float p_animation_time, const aiNodeAnim* p_node_anim);
	const aiNodeAnim* findNodeAnim(const aiAnimation* p_animation, const string p_node_name);

	// calculate transform matrix
	aiVector3D calcInterpolatedPosition(float p_animation_time, const aiNodeAnim* p_node_anim);
	aiQuaternion calcInterpolatedRotation(float p_animation_time, const aiNodeAnim* p_node_anim);
	aiVector3D calcInterpolatedScaling(float p_animation_time, const aiNodeAnim* p_node_anim);

	void readNodeHierarchy(float p_animation_time, const aiNode* p_node, const aiMatrix4x4 parent_transform);
	void boneTransform(double time_in_sec, vector<aiMatrix4x4>& transforms);

	// rotate Head
	glm::quat rotate_head_xz = glm::quat(cos(glm::radians(0.0f)), sin(glm::radians(0.0f)) * glm::vec3(1.0f, 0.0f, 0.0f));
};