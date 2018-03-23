#include "painting2/GradientColor.h"

namespace pt2
{

bool GradientColor::operator == (const GradientColor& col) const
{
	if (items.size() != col.items.size() ||
		angle != col.angle) {
		return false;
	}

	for (int i = 0, n = items.size(); i < n; ++i) {
		if (!(items[i] == col.items[i])) {
			return false;
		}
	}

	return true;
}

}