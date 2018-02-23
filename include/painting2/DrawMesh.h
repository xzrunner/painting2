#pragma once

#include "painting2/RenderReturn.h"
#include "painting2/Mesh.h"

#include <boost/noncopyable.hpp>

namespace cooking { class DisplayList; }

namespace pt2
{

template<typename T, typename Params>
class DrawMesh : boost::noncopyable
{
public:
	DrawMesh(const Mesh<T>& mesh);

	RenderReturn DrawInfoUV(cooking::DisplayList* dlist, const sm::Matrix2D* mt = nullptr);
	RenderReturn DrawInfoXY(cooking::DisplayList* dlist, const sm::Matrix2D* mt = nullptr);

	RenderReturn DrawTexture(cooking::DisplayList* dlist,
		const Params& params, const std::shared_ptr<const T>& base_sym = nullptr);

	RenderReturn DrawOnlyMesh(cooking::DisplayList* dlist, const sm::Matrix2D& mt, int tex_id);

protected:
	virtual RenderReturn DrawNode(const T& node, const Params& params) const = 0;

	virtual bool IsNodeImage(const T& node) const = 0;
	virtual pt2::RenderReturn PrepareDrawOnePass(cooking::DisplayList* dlist, 
		const T& node, const Params& params, float* texcoords, int* tex_id) const = 0;

private:
	RenderReturn DrawOnePass(cooking::DisplayList* dlist, const Params& params, const float* texcoords, int tex_id);

	RenderReturn DrawTwoPass(cooking::DisplayList* dlist, const Params& params, const T& node);
	RenderReturn DrawMesh2RT(cooking::DisplayList* dlist, RenderTarget* rt, const Params& params, const T& node);
	RenderReturn DrawRT2Screen(cooking::DisplayList* dlist, RenderTarget* rt, const sm::Matrix2D& mt);

protected:
	const Mesh<T>& m_mesh;

}; // DrawMesh

}

#include "painting2/DrawMesh.inl"