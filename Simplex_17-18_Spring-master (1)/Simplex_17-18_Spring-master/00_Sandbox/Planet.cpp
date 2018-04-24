#include "Planet.h"

using namespace Simplex;

Planet::Planet(vector3 p_direction, vector3 p_force, vector3 p_center, float p_radius, String name)
{
	direction = p_direction;
	force = p_force;
	center = p_center;
	radius = p_radius;
	nameOfPlanet = name;
}

bool Planet::IsColliding(Planet one, Planet two)
{
	if (glm::distance(one.center, two.center) <= one.radius + two.radius)
		return true;
	else
		return false;
}

vector3 Planet::GetDirection(void) { return direction; }
float Planet::GetRadius(void) { return radius; }
vector3 Planet::GetCenter(void) { return center; }
vector3 Planet::GetForce(void) { return force; }
String Planet::GetName(void) { return nameOfPlanet; }