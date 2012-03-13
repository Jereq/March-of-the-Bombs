#include "BoundingBox.h"

BoundingBox::BoundingBox(glm::vec3 const& position, glm::vec3 const& halfSize)
	: position(position), halfSize(halfSize)
{
}

bool BoundingBox::rayIntersect(glm::vec3 origin, glm::vec3 direction, float& distance) const
{
	float tMin = -std::numeric_limits<float>::infinity();
	float tMax = std::numeric_limits<float>::infinity();

	glm::vec3 p = position - origin;

	for (int i = 0; i < 3; i++)
	{
		float e = p[i];
		float f = direction[i];

		float h = halfSize[i];

		if (glm::abs(f) > 1e-20)
		{
			float div = 1.f / f;
			float t1 = (e + h) * div;
			float t2 = (e - h) * div;

			if (t1 > t2)
			{
				if (t2 > tMin)
				{
					tMin = t2;
				}
				if (t1 < tMax)
				{
					tMax = t1;
				}
			}
			else
			{
				if (t1 > tMin)
				{
					tMin = t1;
				}
				if (t2 < tMax)
				{
					tMax = t2;
				}
			}

			if (tMin > tMax)
			{
				return false;
			}
			if (tMax < 0)
			{
				return false;
			}
		}
		else if (-e - h > 0 || -e + h < 0)
		{
			return false;
		}

		if (tMin >= distance)
		{
			return false;
		}
	}

	if (tMin > 0)
	{
		distance = tMin;
	}
	else
	{
		distance = tMax;
	}

	return true;
}

bool BoundingBox::frustumIntersect(Frustum& Frustum, BoundingBox& BoundingBox)
{
	glm::vec4 clippingPlanes[6];

	clippingPlanes[0] = Frustum.getLeftPlane();
	clippingPlanes[1] = Frustum.getRightPlane();
	clippingPlanes[2] = Frustum.getBottomPlane();
	clippingPlanes[3] = Frustum.getTopPlane();
	clippingPlanes[4] = Frustum.getBackPlane();
	clippingPlanes[5] = Frustum.getFrontPlane();

	for (int p = 0; p < 6; p++)
	{
		// leftplane
		if ((clippingPlanes[p].x * Frustum.TLN.x + clippingPlanes[p].y 
			* Frustum.TLN.y + clippingPlanes[p].z * Frustum.TLN.z + clippingPlanes[p].w) > 0) continue;
		// right plane
		if ((clippingPlanes[p].x * Frustum.TRN.x + clippingPlanes[p].y 
			* Frustum.TRN.y + clippingPlanes[p].z * Frustum.TRN.z + clippingPlanes[p].w) > 0) continue;
		// bottom plane
		if ((clippingPlanes[p].x * Frustum.BLN.x + clippingPlanes[p].y 
			* Frustum.BLN.y + clippingPlanes[p].z * Frustum.BLN.z + clippingPlanes[p].w) > 0) continue;
		// top plane
		if ((clippingPlanes[p].x * Frustum.TRF.x + clippingPlanes[p].y 
			* Frustum.TRF.y + clippingPlanes[p].z * Frustum.TRF.z + clippingPlanes[p].w) > 0) continue;
		// back plane
		if ((clippingPlanes[p].x * Frustum.TLF.x + clippingPlanes[p].y 
			* Frustum.TLF.y + clippingPlanes[p].z * Frustum.TLF.z + clippingPlanes[p].w) > 0) continue;
		// front plane
		if ((clippingPlanes[p].x * Frustum.TRN.x + clippingPlanes[p].y 
			* Frustum.TRN.y + clippingPlanes[p].z * Frustum.TRN.z + clippingPlanes[p].w) > 0) continue;
		
		//If we are still here, something was outside the frustum...
		
		return false;
	}
}