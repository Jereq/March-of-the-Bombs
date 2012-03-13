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

bool BoundingBox::frustumIntersect(Frustum const& frustum)
{
	glm::vec4 clippingPlanes[6];

	clippingPlanes[0] = frustum.getLeftPlane();
	clippingPlanes[1] = frustum.getRightPlane();
	clippingPlanes[2] = frustum.getBottomPlane();
	clippingPlanes[3] = frustum.getTopPlane();
	clippingPlanes[4] = frustum.getBackPlane();
	clippingPlanes[5] = frustum.getFrontPlane();

	for (int p = 0; p < 6; p++)
	{
		// leftplane
		if ((clippingPlanes[p].x * frustum.TLN.x + clippingPlanes[p].y 
			* frustum.TLN.y + clippingPlanes[p].z * frustum.TLN.z + clippingPlanes[p].w) > 0) continue;
		// right plane
		if ((clippingPlanes[p].x * frustum.TRN.x + clippingPlanes[p].y 
			* frustum.TRN.y + clippingPlanes[p].z * frustum.TRN.z + clippingPlanes[p].w) > 0) continue;
		// bottom plane
		if ((clippingPlanes[p].x * frustum.BLN.x + clippingPlanes[p].y 
			* frustum.BLN.y + clippingPlanes[p].z * frustum.BLN.z + clippingPlanes[p].w) > 0) continue;
		// top plane
		if ((clippingPlanes[p].x * frustum.TRF.x + clippingPlanes[p].y 
			* frustum.TRF.y + clippingPlanes[p].z * frustum.TRF.z + clippingPlanes[p].w) > 0) continue;
		// back plane
		if ((clippingPlanes[p].x * frustum.TLF.x + clippingPlanes[p].y 
			* frustum.TLF.y + clippingPlanes[p].z * frustum.TLF.z + clippingPlanes[p].w) > 0) continue;
		// front plane
		if ((clippingPlanes[p].x * frustum.TRN.x + clippingPlanes[p].y 
			* frustum.TRN.y + clippingPlanes[p].z * frustum.TRN.z + clippingPlanes[p].w) > 0) continue;
		
		//If we are still here, something was outside the frustum...
		
		return false;
	}

	return true;
}