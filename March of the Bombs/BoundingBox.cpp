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

//void BoundingBox::selectionBox(glm::vec3 TLN, glm::vec3 BRN, glm::vec3 TLF, glm::vec3 BRF, BoundingBox& BoundingBox)
//{
//	// TLN = TopLeftNear, BRN = BottomRightNear
//	// TLF = TopLeftFar, BRF = BottomRightFar
//	glm::vec3 vTLN, vBRN, vTLF, vBRF;
//	glm::vec3 v1, v2, v3, v4;
//	vTLN = TLN;
//	vBRN = BRN;
//	vTLF = TLF;
//	vBRF = BRF;
//
//}