#include <Magma/Math/Vector.hpp>
#include <cstdio>

using namespace Magma::Math;

void Init()
{
	Vector4f vec = { 1.0f, 1.0f, 1.0f };

	printf("%f %f %f %f\n", vec.x, vec.y, vec.z, vec.w);

	getchar();
}