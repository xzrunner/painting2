#pragma once

#include "painting2/RenderReturn.h"
#include "painting2/Mesh.h"

#include <boost/noncopyable.hpp>

namespace cooking { class DisplayList; }
namespace ur2 { class Framebuffer; }

namespace pt2
{

template<typename Type, typename Params>
class DrawMesh : boost::noncopyable
{
public:
	DrawMesh(const Mesh<Type>& mesh);

	RenderReturn DrawInfoUV(cooking::DisplayList* dlist, const sm::Matrix2D* mt = nullptr);
	RenderReturn DrawInfoXY(cooking::DisplayList* dlist, const sm::Matrix2D* mt = nullptr);

	RenderReturn DrawTexture(const ur2::Device& dev, ur2::Context& ctx,
        cooking::DisplayList* dlist, const Params& params, const Type& base_sym = nullptr);

	RenderReturn DrawOnlyMesh(cooking::DisplayList* dlist, const sm::Matrix2D& mt, int tex_id);

protected:
	virtual RenderReturn DrawNode(const ur2::Device& dev, ur2::Context& ctx, ur2::RenderState& rs,
        const Type& node, const Params& params) const = 0;

	virtual bool IsNodeImage(const Type& node) const = 0;
	virtual RenderReturn PrepareDrawOnePass(cooking::DisplayList* dlist,
		const Type& node, const Params& params, float* texcoords, int* tex_id) const = 0;

	virtual const sm::Matrix2D& GetMat(const Params& params) const = 0;

private:
	RenderReturn DrawOnePass(const ur2::Device& dev, ur2::Context& ctx,
        cooking::DisplayList* dlist, const Params& params, const float* texcoords, int tex_id);

	RenderReturn DrawTwoPass(cooking::DisplayList* dlist, const Params& params, const Type& node);
	RenderReturn DrawMesh2RT(cooking::DisplayList* dlist, const std::shared_ptr<ur2::Framebuffer>& rt, const Params& params, const Type& node);
	RenderReturn DrawRT2Screen(cooking::DisplayList* dlist, const std::shared_ptr<ur2::Framebuffer>& rt, const sm::Matrix2D& mt);

protected:
	const Mesh<Type>& m_mesh;

}; // DrawMesh

}

#include "painting2/DrawMesh.inl"