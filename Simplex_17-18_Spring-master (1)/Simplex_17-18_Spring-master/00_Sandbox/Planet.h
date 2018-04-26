#pragma once
#include "Definitions.h"
#include "Math.h"

namespace Simplex
{
	class Planet
	{
		private:
			vector3 acceleration;
			vector3 velocity;
			vector3 previousVelocity;
			//vector3 position;
			vector3 direction;
			vector3 force;
			vector3 center;
			vector3 previousPosition;
			float mass;
			float radius;
			String nameOfPlanet;

			float time;

		public:

			//vector3 acceleration;

			Planet(vector3 p_direction, vector3 p_force, vector3 p_center, float p_radius, String name);

			bool IsColliding(Planet one, Planet two);

			void ApplyForce(vector3 forceA);

			void CalculateAcceleration(float timeA);

			void CalculateVelocity(float timeA);

			void Move();

			vector3 GetDirection(void);
			float GetRadius(void);
			vector3 GetCenter(void);
			vector3 GetForce(void);
			String GetName(void);
			void SetCenter(vector3 vec);
	};
}