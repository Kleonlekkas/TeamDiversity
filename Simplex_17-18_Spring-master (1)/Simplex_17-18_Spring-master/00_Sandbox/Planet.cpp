#include "Planet.h"

using namespace Simplex;

Planet::Planet(vector3 p_direction, vector3 p_force, vector3 p_center, float p_radius, String name)
{
	direction = p_direction;
	force = p_force;
	center = p_center;
	radius = p_radius;
	nameOfPlanet = name;

	//test values
	velocity = vector3(10, 0, 0);
	acceleration = vector3(0, 10, 0);

}

bool Planet::IsColliding(Planet one, Planet two)
{
	if (glm::distance(one.center, two.center) <= one.radius + two.radius)
		return true;
	else
		return false;
}

void Planet::CalculateGravity(float massOther, vector3 centerB) 
{

	float gravityC = 6.67408 * pow(10, -11);

	float distance = sqrt(pow(center.x - centerB.x, 2) + pow(center.y - centerB.y, 2) + pow(center.z - centerB.z, 2));

	float gravityforce = (gravityC * (mass * massOther)) / pow(distance, 2);

	vector3 dirGravity = vector3(centerB.x - center.x, centerB.y - center.y, centerB.z - center.z);
	float length = sqrt(pow(dirGravity.x, 2) + pow(dirGravity.y, 2) + pow(dirGravity.z, 2));
	vector3 normalized = dirGravity / length;

	vector3 foGravity = normalized * gravityforce;

	ApplyForce(foGravity);

}

void Planet::CalculateForce()
{

	force = mass * acceleration;

}

void Planet::ApplyForce(vector3 forceA) 
{

	acceleration += forceA;

}

/*
void Planet::CalculateAcceleration(float timeA) {

	acceleration = (velocity - previousVelocity) / timeA;

}
*/

void Planet::CalculateVelocity(float timeA) {

	float distance = sqrt(pow(center.x - previousPosition.x, 2) + pow(center.y - previousPosition.y, 2) + pow(center.z - previousPosition.z, 2));

	velocity = (center - previousPosition) / timeA;

	velocity += acceleration * timeA;

}


void Planet::Move() {

	/*
	float length = sqrt(pow(force.x, 2) + pow(force.y, 2) + pow(force.z, 2));
	vector3 normalized = force / length;

	center.x += normalized.x * velocity.x;
	center.y += normalized.y * velocity.y;
	center.z += normalized.z * velocity.z;
	*/

	float length = sqrt(pow(force.x, 2) + pow(force.y, 2) + pow(force.z, 2));
	vector3 normalized = force / length;

	previousPosition = center;

	CalculateForce();

	center += normalized * velocity;

	acceleration = vector3(0,10,0);

	//CalculateVelocity();

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