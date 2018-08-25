#include "model.h"
#include "animation.h"
#include "display_input.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Model::Model()
{
	scene = nullptr;
}


Model::~Model()
{
	import.FreeScene();
}

void Model::initShaders(GLuint shader_program)
{	// get location all matrices of bones
	for(uint i = 0; i < MAX_BONES; i++){
		string name = "bones[" + to_string(i) + "]";// name like in shader
		m_bone_location[i] = glGetUniformLocation(shader_program, name.c_str());
	}
}

void Model::update(){
	// quaternion rotation
	if(Control::Instance()->keyPressed(SDL_SCANCODE_1)){
		rotate_head_xz *= glm::quat(cos(glm::radians(1.0f / 2)), sin(glm::radians(1.0f / 2)) * glm::vec3(1.0f, 0.0f, 0.0f));
	}

	if(Control::Instance()->keyPressed(SDL_SCANCODE_2)){
		rotate_head_xz *= glm::quat(cos(glm::radians(-1.0f / 2)), sin(glm::radians(-1.0f / 2)) * glm::vec3(1.0f, 0.0f, 0.0f));
	}

	if(Control::Instance()->keyPressed(SDL_SCANCODE_3)){
		rotate_head_xz *= glm::quat(cos(glm::radians(1.0f / 2)), sin(glm::radians(1.0f / 2)) * glm::vec3(0.0f, 0.0f, 1.0f));
	}

	if(Control::Instance()->keyPressed(SDL_SCANCODE_4)){
		rotate_head_xz *= glm::quat(cos(glm::radians(-1.0f / 2)), sin(glm::radians(-1.0f / 2)) * glm::vec3(0.0f, 0.0f, 1.0f));
	}
}

void Model::draw(GLuint shaders_program)
{
	vector<aiMatrix4x4> transforms;
	boneTransform((double) SDL_GetTicks() / 1000.0f, transforms);

	for (uint i = 0; i < transforms.size(); i++) // move all matrices for actual model position to shader
	{
		glUniformMatrix4fv(m_bone_location[i], 1, GL_TRUE, (const GLfloat*)&transforms[i]);
	}

	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shaders_program);
	}
}


void Model::loadModel(const string& path)
{
	/**
	 *   Model::loadModel
	 *   each node represent parts of the loaded model
	 *   each bone is represented by a node (the mesh forms and deforms about each bone)
	 *   each bone contains arrays of vertices it affects (weight distribution from 0 to 1)
	 *   each mChannels is one aiNodeAnim
	 *   aiNodeAnim represents bone transformations (scaling, rotations and linea translations)
	 *   aiNodeAnim transformations affect the mesh vertices associated with the bone (based on weight distrubution)
	 */

	scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "error assimp : " << import.GetErrorString() << endl;
		return;
	}
	m_global_inverse_transform = scene->mRootNode->mTransformation;
	m_global_inverse_transform.Inverse();

	if (scene->mAnimations[0]->mTicksPerSecond != 0.0)
	{
		ticks_per_second = scene->mAnimations[0]->mTicksPerSecond;
	}
	else
	{
		ticks_per_second = 25.0f;
	}

	// directory for collada.dae and textures
	directory = path.substr(0, path.find_last_of('/'));

	cout << " \n------------------------------- Scene Details -------------------------------------------- \n "<< endl;
	cout << "     Animations: " << scene->HasAnimations() << endl;
	cout << "         Meshes: " << scene->mNumMeshes << endl;
	cout << "       Channels: " << scene->mAnimations[0]->mNumChannels << endl;
	cout << "       Duration: " << scene->mAnimations[0]->mDuration << endl;
	cout << " TicksPerSecond: " << scene->mAnimations[0]->mTicksPerSecond << endl << endl;
	cout << " 				  "<< endl;

	showNodeName(scene->mRootNode); 		// node names
	processNode(scene->mRootNode, scene);	// nodes in procession
}

void Model::showNodeName(aiNode* node)
{
	cout << node->mName.data << endl;
	for(uint i = 0; i < node->mNumChildren; i++){
		showNodeName(node->mChildren[i]);
	}
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	Mesh mesh;
	for(uint i = 0; i < scene->mNumMeshes; i++){
		aiMesh* ai_mesh = scene->mMeshes[i];
		mesh = processMesh(ai_mesh, scene);
		meshes.push_back(mesh); //accumulate all meshes in one vector
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	cout << " ------------------------ Animated Model Details ------------------------------------------ \n "<< endl;
	cout << "   number of bones: "<< mesh->mNumBones << endl;
	cout << "number of vertices: "<< mesh->mNumVertices << endl;
	cout << " 				  "<< endl;

	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;
	vector<VertexBoneData> bones_id_weights_for_each_vertex;

	vertices.reserve(mesh->mNumVertices);
	indices.reserve(mesh->mNumVertices);

	bones_id_weights_for_each_vertex.resize(mesh->mNumVertices);

	//vertices
	for (uint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		if (mesh->mNormals != NULL)
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
		}
		else
		{
			vertex.normal = glm::vec3();
		}

		/* ASSIMP :: for an animated model, each model can have up to 8 different sets of
		 *			  texture coordinates this application is only limited to the first
		 *			  set of of texture coordinates.
		 *
		 *            this is a potential bug source
		 */

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.text_coords = vec;
		}
		else
		{
			vertex.text_coords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	//indices
	for (uint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	//material
	if (mesh->mMaterialIndex >= 0)
	{
		//all pointers created in assimp will be deleted automaticaly when we call import.FreeScene();
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture> diffuse_maps = LoadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
		bool exist = false;
		for (int i = 0; (i < textures.size()) && (diffuse_maps.size() != 0); i++)
		{
			if (textures[i].path ==  diffuse_maps[0].path)
			{
				exist = true;
			}
		}
		if(!exist && diffuse_maps.size() != 0) textures.push_back(diffuse_maps[0]);

		//textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
		vector<Texture> specular_maps = LoadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
		exist = false;
		for (int i = 0; (i < textures.size()) && (specular_maps.size() != 0); i++){
			if (textures[i].path == specular_maps[0].path){
				exist = true;
			}
		}
		if (!exist  && specular_maps.size() != 0) textures.push_back(specular_maps[0]);
	}

	// load bones
	for (uint i = 0; i < mesh->mNumBones; i++)
	{
		uint bone_index = 0;
		string bone_name(mesh->mBones[i]->mName.data);

		cout << mesh->mBones[i]->mName.data << endl;

		if (m_bone_mapping.find(bone_name) == m_bone_mapping.end())
		{
			// Allocate an index for a new bone
			bone_index = m_num_bones;
			m_num_bones++;
			BoneMatrix bi;
			m_bone_matrices.push_back(bi);
			m_bone_matrices[bone_index].offset_matrix = mesh->mBones[i]->mOffsetMatrix;
			m_bone_mapping[bone_name] = bone_index;
		}
		else
		{
			bone_index = m_bone_mapping[bone_name];
		}

		for (uint j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			uint vertex_id = mesh->mBones[i]->mWeights[j].mVertexId;
			float weight = mesh->mBones[i]->mWeights[j].mWeight;
			bones_id_weights_for_each_vertex[vertex_id].addBoneData(bone_index, weight);
		}
	}

	return Mesh(vertices, indices, textures, bones_id_weights_for_each_vertex);
}

vector<Texture> Model::LoadMaterialTexture(aiMaterial* mat, aiTextureType type, string type_name)
{
	vector<Texture> textures;
	for (uint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString ai_str;
		mat->GetTexture(type, i, &ai_str);

		string filename = string(ai_str.C_Str());
		filename = directory + '/' + filename;

		Texture texture;
		texture.id = Animation::loadImageToTexture(filename.c_str()); // return prepaired openGL texture
		texture.type = type_name;
		texture.path = ai_str;
		textures.push_back(texture);
	}
	return textures;
}

uint Model::findPosition(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	for (uint i = 0; i < p_node_anim->mNumPositionKeys - 1; i++){
		if (p_animation_time < (float)p_node_anim->mPositionKeys[i + 1].mTime)
		{
			return i;
		}
	}
	assert(0);
	return 0;
}

uint Model::findRotation(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	for (uint i = 0; i < p_node_anim->mNumRotationKeys - 1; i++) {
		if (p_animation_time < (float)p_node_anim->mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}
	assert(0);
	return 0;
}

uint Model::findScaling(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	for (uint i = 0; i < p_node_anim->mNumScalingKeys - 1; i++){
 		if (p_animation_time < (float)p_node_anim->mScalingKeys[i + 1].mTime){
			return i;
		}
	}
	assert(0);
	return 0;
}

aiVector3D Model::calcInterpolatedPosition(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	if (p_node_anim->mNumPositionKeys == 1){
		return p_node_anim->mPositionKeys[0].mValue;
	}

	uint position_index = findPosition(p_animation_time, p_node_anim);
	uint next_position_index = position_index + 1;
	assert(next_position_index < p_node_anim->mNumPositionKeys);

	float delta_time = (float)(p_node_anim->mPositionKeys[next_position_index].mTime - p_node_anim->mPositionKeys[position_index].mTime);

	float factor = (p_animation_time - (float)p_node_anim->mPositionKeys[position_index].mTime) / delta_time;
	assert(factor >= 0.0f && factor <= 1.0f);
	aiVector3D start = p_node_anim->mPositionKeys[position_index].mValue;
	aiVector3D end = p_node_anim->mPositionKeys[next_position_index].mValue;
	aiVector3D delta = end - start;

	return start + factor * delta;
}

aiQuaternion Model::calcInterpolatedRotation(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	if (p_node_anim->mNumRotationKeys == 1){
		return p_node_anim->mRotationKeys[0].mValue;
	}

	uint rotation_index = findRotation(p_animation_time, p_node_anim);
	uint next_rotation_index = rotation_index + 1;
	assert(next_rotation_index < p_node_anim->mNumRotationKeys);

	float delta_time = (float)(p_node_anim->mRotationKeys[next_rotation_index].mTime - p_node_anim->mRotationKeys[rotation_index].mTime);

	float factor = (p_animation_time - (float)p_node_anim->mRotationKeys[rotation_index].mTime) / delta_time;

	assert(factor >= 0.0f && factor <= 1.0f);
	aiQuaternion start_quat = p_node_anim->mRotationKeys[rotation_index].mValue;
	aiQuaternion end_quat = p_node_anim->mRotationKeys[next_rotation_index].mValue;

	return nlerp(start_quat, end_quat, factor);
}

aiVector3D Model::calcInterpolatedScaling(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	if (p_node_anim->mNumScalingKeys == 1){
		return p_node_anim->mScalingKeys[0].mValue;
	}

	uint scaling_index = findScaling(p_animation_time, p_node_anim);
	uint next_scaling_index = scaling_index + 1;
	assert(next_scaling_index < p_node_anim->mNumScalingKeys);

	float delta_time = (float)(p_node_anim->mScalingKeys[next_scaling_index].mTime - p_node_anim->mScalingKeys[scaling_index].mTime);

	float  factor = (p_animation_time - (float)p_node_anim->mScalingKeys[scaling_index].mTime) / delta_time;
	assert(factor >= 0.0f && factor <= 1.0f);
	aiVector3D start = p_node_anim->mScalingKeys[scaling_index].mValue;
	aiVector3D end = p_node_anim->mScalingKeys[next_scaling_index].mValue;
	aiVector3D delta = end - start;

	return start + factor * delta;
}

const aiNodeAnim * Model::findNodeAnim(const aiAnimation * p_animation, const string p_node_name)
{
	// channel in animation contains aiNodeAnim
	// numChannels == numBones
	for (uint i = 0; i < p_animation->mNumChannels; i++)
	{
		const aiNodeAnim* node_anim = p_animation->mChannels[i];
		if (string(node_anim->mNodeName.data) == p_node_name)
		{
			return node_anim;
		}
	}
	return nullptr;
}

// start from RootNode
void Model::readNodeHierarchy(float p_animation_time, const aiNode* p_node, const aiMatrix4x4 parent_transform)
{
	string node_name(p_node->mName.data);
	const aiAnimation* animation = scene->mAnimations[0];
	aiMatrix4x4 node_transform = p_node->mTransformation;
	const aiNodeAnim* node_anim = findNodeAnim(animation, node_name);

	if (node_anim)
	{
		//scaling
		//aiVector3D scaling_vector = node_anim->mScalingKeys[2].mValue;
		aiVector3D scaling_vector = calcInterpolatedScaling(p_animation_time, node_anim);
		aiMatrix4x4 scaling_matr;
		aiMatrix4x4::Scaling(scaling_vector, scaling_matr);

		//rotation
		//aiQuaternion rotate_quat = node_anim->mRotationKeys[2].mValue;
		aiQuaternion rotate_quat = calcInterpolatedRotation(p_animation_time, node_anim);
		aiMatrix4x4 rotate_matr = aiMatrix4x4(rotate_quat.GetMatrix());

		//translation
		//aiVector3D translate_vector = node_anim->mPositionKeys[2].mValue;
		aiVector3D translate_vector = calcInterpolatedPosition(p_animation_time, node_anim);
		aiMatrix4x4 translate_matr;
		aiMatrix4x4::Translation(translate_vector, translate_matr);

		if ( string(node_anim->mNodeName.data) == string("Head")){
			aiQuaternion rotate_head = aiQuaternion(rotate_head_xz.w, rotate_head_xz.x, rotate_head_xz.y, rotate_head_xz.z);

			node_transform = translate_matr * (rotate_matr * aiMatrix4x4(rotate_head.GetMatrix())) * scaling_matr;
		}
		else
		{
			node_transform = translate_matr * rotate_matr * scaling_matr;
		}

	}

	aiMatrix4x4 global_transform = parent_transform * node_transform;


	if (m_bone_mapping.find(node_name) != m_bone_mapping.end()) // true if node_name exist in bone_mapping
	{
		uint bone_index = m_bone_mapping[node_name];
		m_bone_matrices[bone_index].final_world_transform = m_global_inverse_transform * global_transform * m_bone_matrices[bone_index].offset_matrix;
	}

	for (uint i = 0; i < p_node->mNumChildren; i++)
	{
		readNodeHierarchy(p_animation_time, p_node->mChildren[i], global_transform);
	}

}

void Model::boneTransform(double time_in_sec, vector<aiMatrix4x4>& transforms)
{
	aiMatrix4x4 identity_matrix; // = mat4(1.0f);

	double time_in_ticks = time_in_sec * ticks_per_second;
	float animation_time = fmod(time_in_ticks, scene->mAnimations[0]->mDuration);

	readNodeHierarchy(animation_time, scene->mRootNode, identity_matrix);

	transforms.resize(m_num_bones);

	for (uint i = 0; i < m_num_bones; i++)
	{
		transforms[i] = m_bone_matrices[i].final_world_transform;
	}
}

glm::mat4 Model::aiToGlm(aiMatrix4x4 ai_matr)
{
	glm::mat4 result;
	result[0].x = ai_matr.a1; result[0].y = ai_matr.b1; result[0].z = ai_matr.c1; result[0].w = ai_matr.d1;
	result[1].x = ai_matr.a2; result[1].y = ai_matr.b2; result[1].z = ai_matr.c2; result[1].w = ai_matr.d2;
	result[2].x = ai_matr.a3; result[2].y = ai_matr.b3; result[2].z = ai_matr.c3; result[2].w = ai_matr.d3;
	result[3].x = ai_matr.a4; result[3].y = ai_matr.b4; result[3].z = ai_matr.c4; result[3].w = ai_matr.d4;
	return result;
}

aiQuaternion Model::nlerp(aiQuaternion a, aiQuaternion b, float blend)
{
	a.Normalize();
	b.Normalize();

	aiQuaternion result;
	float dot_product = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	float one_minus_blend = 1.0f - blend;

	if (dot_product < 0.0f)
	{
		result.x = a.x * one_minus_blend + blend * -b.x;
		result.y = a.y * one_minus_blend + blend * -b.y;
		result.z = a.z * one_minus_blend + blend * -b.z;
		result.w = a.w * one_minus_blend + blend * -b.w;
	}
	else
	{
		result.x = a.x * one_minus_blend + blend * b.x;
		result.y = a.y * one_minus_blend + blend * b.y;
		result.z = a.z * one_minus_blend + blend * b.z;
		result.w = a.w * one_minus_blend + blend * b.w;
	}

	return result.Normalize();
}