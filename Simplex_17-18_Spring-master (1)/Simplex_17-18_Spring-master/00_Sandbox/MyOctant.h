#ifndef _MYOCTANTCLASS_H
#define _MYOCTANTCLASS_H

#include "MyEntityManager.h"

namespace Simplex
{

	class MyOctant
	{
		static uint m_uOctantCount; //number of octants
		static uint m_uMaxLevel; //max level octant can go to
		static uint m_uIdealEntityCount; //ideal number of entities within object

		uint m_uID = 0; //current octant id
		uint m_uLevel = 0; //current octant level
		uint m_uChildren = 0; //number of children on the octant

		float m_fSize = 0.0f; //size of octant

		MeshManager* m_pMeshMngr = nullptr;
		MyEntityManager* m_pEntityMngr = nullptr;

		vector3 m_v3Center = vector3(0.0f); //center point of octant
		vector3 m_v3Min = vector3(0.0f); //minimum vector of octant
		vector3 m_v3Max = vector3(0.0f); //maximum vector of octant

		MyOctant* m_pParent = nullptr; //parent of octant
		MyOctant* m_pChild[8]; //children of octant

		std::vector<uint> m_EntityList; //list of entities under octant

		MyOctant* m_pRoot = nullptr; //root octant
		std::vector<MyOctant*> m_lChild; //list of nodes that contain objects
	public:
		//constructor with max level of subdivision and ideal number of objects per octant
		MyOctant(uint a_nMaxLevel = 2, uint a_nIdealEntityCount = 5);
		//constructor with center point in global space and size of sides
		MyOctant(vector3 a_v3Center, float a_fSize);

		//copy constructor
		MyOctant(MyOctant const& other);
		//copy assignment operator
		MyOctant& operator=(MyOctant const& other);
		//destructor
		~MyOctant(void);

		//swap in contents of another object into this one
		void Swap(MyOctant& other);

		//size and position getters
		float GetSize(void);
		vector3 GetCenterGlobal(void);
		vector3 GetMinGlobal(void);
		vector3 GetMaxGlobal(void);

		//check for collision of a specific entity using the index of the object in entity manager
		bool IsColliding(uint a_uRBIndex);

		//display the volume of the octant using a specified color
		void Display(uint a_nIndex, vector3 a_v3Color = C_YELLOW);
		void Display(vector3 a_v3Color = C_YELLOW);

		//display leaves that contain something using a specified color
		void DisplayLeafs(vector3 a_v3Color = C_YELLOW);

		//clears entity list for every node
		void ClearEntityList(void);

		//creates 8 smaller octants as children
		void Subdivide(void);

		//octant getters
		MyOctant* GetChild(uint a_nChild); //find specific child by index
		MyOctant* GetParent(void); //parent of octant

								   //checks if octant is a leaf or not (leaves have no children)
		bool IsLeaf(void);

		//checks if octant contains more than 'a_nEntities' worth of objects
		bool ContainsMoreThan(uint a_nEntities);

		//deletes all children all the way down to the leaves
		void KillBranches(void);

		//create tree with set maximum number of levels
		void ConstructTree(uint a_nMaxLevel = 3);

		//traverse tree and set objects in them to index
		void AssignIDtoEntity(void);

		//get number of octants in world
		uint GetOctantCount(void);
	private:
		void Init(void);
		void Release(void);

		//create list of all leaves that contain objects
		void ConstructList(void);
	};

}

#endif //_MYOCTANTCLASS_H