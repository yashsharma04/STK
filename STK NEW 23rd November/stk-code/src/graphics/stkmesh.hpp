#ifndef STKMESH_H
#define STKMESH_H


#include <IMeshSceneNode.h>
#include <IMesh.h>
#include "../lib/irrlicht/source/Irrlicht/CMeshSceneNode.h"
#include "glwrap.hpp"
#include <vector>

struct GLMesh {
	GLuint vao_first_pass;
	GLuint vao_second_pass;
	GLuint vao_glow_pass;
	GLuint vao_displace_pass;
	GLuint vertex_buffer;
	GLuint index_buffer;
	GLuint textures[6];
	GLenum PrimitiveType;
	GLenum IndexType;
	size_t IndexCount;
	size_t Stride;
};

class STKMesh : public irr::scene::CMeshSceneNode
{
protected:
	std::vector<GLMesh> GLmeshes;
	core::matrix4 ModelViewProjectionMatrix, TransposeInverseModelView;
	core::vector3df windDir;
	void drawSolid(const GLMesh &mesh, video::E_MATERIAL_TYPE type);
	void drawTransparent(const GLMesh &mesh, video::E_MATERIAL_TYPE type);

	// Pass 1 shader (ie shaders that outputs normals and depth)
	void drawObjectPass1(const GLMesh &mesh);
	void drawNormalPass(const GLMesh &mesh);
	void drawObjectRefPass1(const GLMesh &mesh);
	void drawGrassPass1(const GLMesh &mesh);

	// Pass 2 shader (ie shaders that outputs final color)
	void drawSphereMap(const GLMesh &mesh);
	void drawSplatting(const GLMesh &mesh);
	void drawObjectPass2(const GLMesh &mesh);
	void drawObjectRefPass2(const GLMesh &mesh);
	void drawGrassPass2(const GLMesh &mesh);
	void drawUntexturedObject(const GLMesh &mesh);

	// Forward pass (for transparents meshes)
	void drawTransparentObject(const GLMesh &mesh);
	void drawBubble(const GLMesh &mesh);

	// Misc passes shaders (glow, displace...)
	void drawGlow(const GLMesh &mesh);
	void drawDisplace(const GLMesh &mesh);
public:
	STKMesh(irr::scene::IMesh* mesh, ISceneNode* parent, irr::scene::ISceneManager* mgr,	irr::s32 id,
		const irr::core::vector3df& position = irr::core::vector3df(0,0,0),
		const irr::core::vector3df& rotation = irr::core::vector3df(0,0,0),
		const irr::core::vector3df& scale = irr::core::vector3df(1.0f, 1.0f, 1.0f));
	virtual void render();
	~STKMesh();
};

#endif // STKMESH_H
