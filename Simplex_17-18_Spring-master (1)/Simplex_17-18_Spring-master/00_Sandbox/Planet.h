#pragma once
#include "Definitions.h"

namespace Simplex
{
	class Planet
	{
		private:
			vector3 direction;
			vector3 force;
			vector3 center;
			float radius;
			String nameOfPlanet;

		public:
			Planet(vector3 p_direction, vector3 p_force, vector3 p_center, float p_radius, String name);

			bool IsColliding(Planet one, Planet two);

			vector3 GetDirection(void);
			float GetRadius(void);
			vector3 GetCenter(void);
			vector3 GetForce(void);
			String GetName(void);
	};
}