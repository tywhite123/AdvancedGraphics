#include "Plane.h"



Plane::Plane()
{
}

Plane::Plane(const Vector3 & normal, float distance, bool normalise)
{
	if (normalise) {
		float length = sqrt(Vector3::Dot(normal, normal));

		this->normal = normal / length;
		this->distance = distance / length;
	}
	else {
		this->normal = normal;
		this->distance = distance;
	}
}


Plane::~Plane()
{
}

bool Plane::SphereInPlane(const Vector3 & pos, float radius) const
{
	if (Vector3::Dot(pos, normal) + distance <= -radius) {
		return false;
	}
	return true;
}
