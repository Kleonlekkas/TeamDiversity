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

void Planet::ApplyForce(vector3 forceA) 
{

	force += forceA;

}

void Planet::CalculateAcceleration(float timeA) {

	acceleration = (velocity - previousVelocity) / timeA;

}

void Planet::CalculateVelocity(float timeA) {

	velocity = (center - previousPosition) / timeA;

	velocity = velocity + acceleration * timeA;

}

void Planet::Move() {

	vector3 newVelocity;

	force = mass * acceleration; 

	newVelocity = velocity + ((force / mass) * CLOCKS_PER_SEC);

	center += force;

	/*
	previousPosition = center;

	force = mass * acceleration;

	float length = sqrt(pow(force.x, 2) + pow(force.y, 2) + pow(force.z, 2));
	vector3 normalized = force / length;

	center.x += normalized.x * velocity.x;
	center.y += normalized.y * velocity.y;
	center.z += normalized.z * velocity.z;

	velocity.x = (center.x - previousPosition.x) / CLOCKS_PER_SEC;
	velocity.y = (center.y - previousPosition.y) / CLOCKS_PER_SEC;
	velocity.z = (center.z - previousPosition.z) / CLOCKS_PER_SEC;
	*/

	//float length = sqrt(pow(force.x, 2) + pow(force.y, 2) + pow(force.z, 2));

	//vector3 normalized = force / length;

	//position += normalized * velocity;

}

vector3 Planet::GetDirection(void) { return direction; }
float Planet::GetRadius(void) { return radius; }
vector3 Planet::GetCenter(void) { return center; }
vector3 Planet::GetForce(void) { return force; }
String Planet::GetName(void) { return nameOfPlanet; }