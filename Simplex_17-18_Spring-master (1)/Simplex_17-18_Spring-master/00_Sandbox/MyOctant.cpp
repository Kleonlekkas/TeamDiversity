#include "MyOctant.h"
using namespace Simplex;

//set default values
uint MyOctant::m_uOctantCount = 0;
uint MyOctant::m_uMaxLevel = 3;
uint MyOctant::m_uIdealEntityCount = 5;


uint MyOctant::GetOctantCount(void) { return m_uOctantCount; }

//initialize octant to default empty values
void MyOctant::Init(void)
{
	m_uChildren = 0;
	m_fSize = 0.0f;
	m_uID = m_uOctantCount;
	m_uLevel = 0;
	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();
	for (uint i = 0; i < 8; i++)
	{
		m_pChild[i] = nullptr;
	}

}

void MyOctant::Swap(MyOctant& other)
{
	//swap octant values with those of 'other' octant
	std::swap(m_uChildren, other.m_uChildren);
	std::swap(m_fSize, other.m_fSize);
	std::swap(m_uID, other.m_uID);
	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_lChild, other.m_lChild);
	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	std::swap(m_uLevel, other.m_uLevel);
	std::swap(m_pParent, other.m_pParent);
	for (uint i = 0; i < 8; i++)
	{
		std::swap(m_pChild[i], other.m_pChild[i]);
	}
}

MyOctant* MyOctant::GetParent(void) { return m_pParent; }

//destroy all branches of octant, clear entity list
void MyOctant::Release(void)
{
	if (m_uLevel == 0) KillBranches();
	m_uChildren = 0;
	m_fSize = 0.0f;
	m_EntityList.clear();
	m_lChild.clear();
}

//create octant with predetermined max level and ideal entity count
MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	Init();

	m_uOctantCount = 0;
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
	m_uID = m_uOctantCount;

	m_pRoot = this;
	m_lChild.clear();

	std::vector<vector3> lMinMax;

	//find min and max values among all entities to create octant size
	uint nObjects = m_pEntityMngr->GetEntityCount();
	for (uint i = 0; i < nObjects; i++)
	{
		MyEntity* pEntity = m_pEntityMngr->GetEntity(i);
		RigidBody* pRigidBody = pEntity->GetRigidBody();
		lMinMax.push_back(pRigidBody->GetMinGlobal());
		lMinMax.push_back(pRigidBody->GetMaxGlobal());
	}

	RigidBody* pRigidBody = new RigidBody(lMinMax);

	//find halfwidth of octant
	vector3 vHalfWidth = pRigidBody->GetHalfWidth();
	float fMax = vHalfWidth.x;
	for (int i = 0; i < 3; i++)
	{
		if (fMax < vHalfWidth[i]) fMax = vHalfWidth[i];
	}

	//set center of octree
	vector3 v3Center = pRigidBody->GetCenterLocal();
	lMinMax.clear();
	SafeDelete(pRigidBody);

	//set dimensions of octant (size, center, min, and max)
	m_fSize = fMax * 2.0f;
	m_v3Center = v3Center;
	m_v3Min = m_v3Center - (vector3(fMax)); //center - halfwidth
	m_v3Max = m_v3Center + (vector3(fMax)); //center + halfwidth
	m_uOctantCount++; //increment octant count
	ConstructTree(m_uMaxLevel); //construct tree from with set max level
}


//create octant with predetermined center and size
MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	Init();
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;
	m_v3Min = m_v3Center - (vector3(m_fSize) / 2.0f);
	m_v3Max = m_v3Center + (vector3(m_fSize) / 2.0f);
	m_uOctantCount++;
}

MyOctant::MyOctant(MyOctant const& other)
{
	m_uChildren = other.m_uChildren;
	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;
	m_fSize = other.m_fSize;
	m_uID = other.m_uID;
	m_uLevel = other.m_uLevel;
	m_pParent = other.m_pParent;
	m_pRoot = other.m_pRoot;
	m_lChild = other.m_lChild;

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	for (uint i = 0; i < 8; i++)
	{
		m_pChild[i] = other.m_pChild[i];
	}
}

MyOctant& MyOctant::operator=(MyOctant const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}
	return *this;
}

MyOctant::~MyOctant() { Release(); }

float MyOctant::GetSize(void) { return m_fSize; }

vector3 MyOctant::GetCenterGlobal(void) { return m_v3Center; }

vector3 MyOctant::GetMinGlobal(void) { return m_v3Min; }

vector3 MyOctant::GetMaxGlobal(void) { return m_v3Max; }

void MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{
	//display octant with specific index: 'a_nIndex'
	if (m_uID == a_nIndex)
	{
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
			glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
		return;
	}

	//display all children of octant
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(a_nIndex);
	}
}

void MyOctant::Display(vector3 a_v3Color)
{
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(a_v3Color);
	}

	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
		glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
}

void MyOctant::Subdivide(void)
{
	//if already at max level
	if (m_uLevel >= m_uMaxLevel) return;

	//if octant has children already
	if (m_uChildren != 0) return;

	m_uChildren = 8;
	float fSize = m_fSize / 4.0f;
	float fSizeD = fSize * 2.0f;
	vector3 v3Center;

	//set positions and create children octants
	v3Center = m_v3Center;
	v3Center.x -= fSize;
	v3Center.y -= fSize;
	v3Center.z -= fSize;
	m_pChild[0] = new MyOctant(v3Center, fSizeD);

	v3Center.x += fSizeD;
	m_pChild[1] = new MyOctant(v3Center, fSizeD);

	v3Center.z += fSizeD;
	m_pChild[2] = new MyOctant(v3Center, fSizeD);

	v3Center.x -= fSizeD;
	m_pChild[3] = new MyOctant(v3Center, fSizeD);

	v3Center.y += fSizeD;
	m_pChild[4] = new MyOctant(v3Center, fSizeD);

	v3Center.z -= fSizeD;
	m_pChild[5] = new MyOctant(v3Center, fSizeD);

	v3Center.x += fSizeD;
	m_pChild[6] = new MyOctant(v3Center, fSizeD);

	v3Center.z += fSizeD;
	m_pChild[7] = new MyOctant(v3Center, fSizeD);

	for (uint i = 0; i < 8; i++)
	{
		//set childrens' root, parent, level, and subdivide if necessary (contains too many entities)
		m_pChild[i]->m_pRoot = m_pRoot;
		m_pChild[i]->m_pParent = this;
		m_pChild[i]->m_uLevel = m_uLevel + 1; //set to one more level than current
		if (m_pChild[i]->ContainsMoreThan(m_uIdealEntityCount))
		{
			m_pChild[i]->Subdivide();
		}
	}
}

MyOctant* MyOctant::GetChild(uint a_nChild)
{
	//if invalid index
	if (a_nChild > 7) return nullptr;
	return m_pChild[a_nChild];
}

bool MyOctant::IsColliding(uint a_uRBIndex)
{
	uint nObjectCount = m_pEntityMngr->GetEntityCount();
	//if invalid entity index
	if (a_uRBIndex >= nObjectCount) return false;

	//get position of entity at index: 'a_uRBIndex'
	MyEntity* pEntity = m_pEntityMngr->GetEntity(a_uRBIndex);
	RigidBody* pRigidBody = pEntity->GetRigidBody();
	vector3 v3MinO = pRigidBody->GetMinGlobal();
	vector3 v3MaxO = pRigidBody->GetMaxGlobal();

	//check if colliding (by seeing if entity is outside bounds of octant)
	if (m_v3Max.x < v3MinO.x) return false;
	if (m_v3Min.x > v3MaxO.x) return false;

	if (m_v3Max.y < v3MinO.y) return false;
	if (m_v3Min.y > v3MaxO.y) return false;

	if (m_v3Max.z < v3MinO.z) return false;
	if (m_v3Min.z > v3MaxO.z) return false;

	return true;
}

bool MyOctant::IsLeaf(void) { return m_uChildren == 0; }

bool MyOctant::ContainsMoreThan(uint a_nEntities)
{
	uint nCount = 0;
	uint nObjectCount = m_pEntityMngr->GetEntityCount();

	for (uint i = 0; i < nObjectCount; i++)
	{
		//add to nCount for every colliding entity
		if (IsColliding(i)) nCount++;
		if (nCount > a_nEntities) return true;
	}

	return false;
}

void MyOctant::KillBranches(void)
{
	//kill all children down branches until leaves are reached (octant has no children)
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->KillBranches();
		delete m_pChild[i];
		m_pChild[i] = nullptr;
	}

	m_uChildren = 0;
}

void MyOctant::DisplayLeafs(vector3 a_v3Color)
{
	uint nLeafs = m_lChild.size();

	for (uint i = 0; i < nLeafs; i++)
	{
		m_lChild[i]->DisplayLeafs(a_v3Color);
	}

	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
		glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
}

void MyOctant::ClearEntityList(void)
{
	//clear all entities down every branch
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->ClearEntityList();
	}

	m_EntityList.clear();
}

void MyOctant::ConstructTree(uint a_nMaxLevel)
{
	//if tree is alreayd created
	if (m_uLevel != 0) return;

	m_uMaxLevel = a_nMaxLevel;
	m_uOctantCount = 1;

	//destroy, then rebuild
	m_EntityList.clear();
	KillBranches();
	m_lChild.clear();

	if (ContainsMoreThan(m_uIdealEntityCount)) Subdivide();

	AssignIDtoEntity();
	ConstructList();
}

void MyOctant::AssignIDtoEntity(void)
{
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->AssignIDtoEntity();
	}

	if (m_uChildren == 0)
	{
		uint nEntities = m_pEntityMngr->GetEntityCount();

		for (int i = 0; i < nEntities; i++)
		{
			if (IsColliding(i))
			{
				m_EntityList.push_back(i);
				m_pEntityMngr->AddDimension(i, m_uID);
			}
		}
	}
}

void MyOctant::ConstructList(void)
{
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->ConstructList();
	}

	if (m_EntityList.size() == 0)
	{
		m_pRoot->m_lChild.push_back(this);
	}
}