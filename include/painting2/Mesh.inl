#pragma once

#include <rigging.h>
#include <polymesh/TrianglesMesh.h>
#include <polymesh/Skin2Mesh.h>

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
Mesh<T>::Mesh(const std::shared_ptr<const T>& base)
	: m_base(base)
{
	sm::vec2 sz = m_base->GetBounding().Size();
	m_width = sz.x;
	m_height = sz.y;

	m_node_radius = std::min(m_width * 0.1f, 5.0f);
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
void Mesh<T>::LoadFromTransform(const pm::MeshTransform& transform)
{
	if (m_mesh) {
		m_mesh->LoadFromTransform(transform);
	}
}

template<typename T>
void Mesh<T>::StoreToTransforom(pm::MeshTransform& transform) const
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
	case pm::MESH_SKIN2:
		static_cast<pm::Skin2Mesh*>(m_mesh.get())->Update(query_joint_world_mt, sk_pose);
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
	case pm::MESH_TRIANGLES:
		static_cast<pm::TrianglesMesh*>(m_mesh.get())->Update(
			deform_state->offset0, deform_state->count0, deform_state->offset1, deform_state->count1, vertices);
		break;
	case pm::MESH_SKIN2:
		static_cast<pm::Skin2Mesh*>(m_mesh.get())->Update(
			deform_state->offset0, deform_state->count0, deform_state->offset1, deform_state->count1, vertices);
		break;
	}
}

}