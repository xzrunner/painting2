#pragma once

#include <rigging.h>
#include <polymesh2/TrianglesMesh.h>
#include <polymesh2/Skin2Mesh.h>
#include <polymesh2/Mesh.h>

namespace
{

static const float*
query_joint_world_mt(int joint_id, const void* ud)
{
	const rg_skeleton_pose* sk_pose = static_cast<const rg_skeleton_pose*>(ud);
	return sk_pose->poses[joint_id].world.m;
}

}

namespace pt2
{

template<typename T>
Mesh<T>::Mesh()
	: m_width(0)
	, m_height(0)
	, m_node_radius(0)
{
}

template<typename T>
Mesh<T>::Mesh(const T& base, float width, float height)
	: m_base(base)
	, m_width(width)
	, m_height(height)
	, m_node_radius(std::min(m_width * 0.1f, 5.0f))
{
}

template<typename T>
Mesh<T>::~Mesh() = default;

template<typename T>
void Mesh<T>::DumpToTriangles(CU_VEC<sm::vec2>& vertices,
						   CU_VEC<sm::vec2>& texcoords,
						   CU_VEC<int>& triangles) const
{
	if (m_mesh) {
		m_mesh->Dump(vertices, texcoords, triangles);
	}
}

template<typename T>
void Mesh<T>::LoadFromTransform(const pm2::MeshTransform& transform)
{
	if (m_mesh) {
		m_mesh->LoadFromTransform(transform);
	}
}

template<typename T>
void Mesh<T>::StoreToTransforom(pm2::MeshTransform& transform) const
{
	if (m_mesh) {
		m_mesh->StoreToTransform(transform);
	}
}

template<typename T>
sm::rect Mesh<T>::GetRegion() const
{
	if (!m_mesh) {
		return sm::rect(); // empty
	}

	sm::rect ret;
	CU_VEC<sm::vec2> vertices, texcoords;
	CU_VEC<int> triangles;
	m_mesh->Dump(vertices, texcoords, triangles);
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		ret.Combine(vertices[i]);
	}
	return ret;
}

template<typename T>
void Mesh<T>::Update(const rg_skeleton_pose* sk_pose)
{
	if (!m_mesh) {
		return;
	}

	switch (m_mesh->Type())
	{
	case pm2::MESH_SKIN2:
		static_cast<pm2::Skin2Mesh*>(m_mesh.get())->Update(query_joint_world_mt, sk_pose);
		break;
	default:
		break;
	}
}

template<typename T>
void Mesh<T>::Update(const rg_tl_deform_state* deform_state, const float* vertices)
{
	if (!m_mesh) {
		return;
	}

	switch (m_mesh->Type())
	{
	case pm2::MESH_TRIANGLES:
		static_cast<pm2::TrianglesMesh*>(m_mesh.get())->Update(
			deform_state->offset0, deform_state->count0, deform_state->offset1, deform_state->count1, vertices);
		break;
	case pm2::MESH_SKIN2:
		static_cast<pm2::Skin2Mesh*>(m_mesh.get())->Update(
			deform_state->offset0, deform_state->count0, deform_state->offset1, deform_state->count1, vertices);
		break;
	}
}

}