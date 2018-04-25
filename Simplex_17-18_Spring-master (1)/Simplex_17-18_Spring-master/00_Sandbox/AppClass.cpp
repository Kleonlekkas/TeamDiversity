#include "AppClass.h"

using namespace Simplex;

void Application::InitVariables(void)
{

	m_pCameraMngr->MoveForward(m_fMovementSpeed * -2000);		// starts camera farther back to see the entire system
	m_pCameraMngr->MoveSideways(m_fMovementSpeed * 2000);		// starts camera to the right to see all the planets
	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Music
	String sRoute = m_pSystem->m_pFolder->GetFolderData();
	sRoute += m_pSystem->m_pFolder->GetFolderAudio();

	//Entity Manager
	m_pEntityMngr = MyEntityManager::GetInstance();

	String planet[11] = { "Sun", "Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune", "Pluto", "Moon" };
	float scale[11] = { 109.0f, 2.298f, 5.694f, 6.0f, 3.192f, 11.21f, 9.45f, 6.01f, 5.82f, 2.0f };
	m_uOctantLevels = 1;
	m_pRoot = new MyOctant(m_uOctantLevels, 5);

#pragma region Make some meshes, add them to a group and add the group to a model
	/*
	Mesh* pMesh1 = new Mesh();
	Mesh* pMesh2 = new Mesh();
	Mesh* pMesh3 = new Mesh();
	pMesh1->GenerateCone(1.0f, 1.0f, 10, C_RED);
	pMesh2->GenerateTorus(1.0f, 0.7f, 10, 10, C_MAGENTA);
	pMesh3->GenerateTube(1.0f, 0.7f, 1.0f, 10, C_BLUE);
	int nMesh1 = m_pMngr->AddMesh(pMesh1);
	int nMesh2 = m_pMngr->AddMesh(pMesh2);

	Group* pGroup1 = new Group();
	pGroup1->AddMesh(nMesh1, MeshOptions(glm::translate(AXIS_Y), WIRE));
	pGroup1->AddMesh(nMesh2, MeshOptions(glm::translate(-AXIS_Y), SOLID));
	Group* pGroup2 = new Group();
	pGroup2->AddMesh(pMesh3, MeshOptions(IDENTITY_M4, SOLID | WIRE));

	m_pModel1 = new Simplex::Model();
	m_pModel1->AddGroup(pGroup1);
	m_pModel1->AddGroup(pGroup2);
	*/
#pragma endregion
#pragma region Load Model and change its rendering properties
	/*
	m_pModel1 = new Simplex::Model();
	m_pModel1->Load("Macross\\YF19.BTO");
	m_pModel1->ChangeMeshOptions(MeshOptions(IDENTITY_M4, SOLID | WIRE), -1, -1);
	*/
#pragma endregion
#pragma region Load Model and change its materials
	/*
	m_pModel1 = new Simplex::Model();
	m_pModel1->Load("Macross\\YF19.BTO");
	//m_pModel1->ChangeMeshOptions(MeshOptions(IDENTITY_M4, SOLID | WIRE), -1, -1);
	m_pMatMngr->AddMaterial("Cloud.png", "Cloud.png");
	m_pModel1->ChangeMaterialOfGroup("Cloud.png", "ALL");
	*/
#pragma endregion
#pragma region Load Model disconect and change its materials
	/*
	m_pModel1 = new Simplex::Model();
	m_pModel1->Load("Macross\\YF19.BTO");
	//m_pModel1->ChangeMeshOptions(MeshOptions(IDENTITY_M4, SOLID | WIRE), -1, -1);
	m_pMatMngr->AddMaterial("Cloud.png", "Cloud.png");
	m_pModel1->DuplicateMaterialsUnique(-1, "_copy"); //dupliocate existing materials
	m_pModel1->CreateMaterials(); //duplicate one per mesh
	m_pModel1->ChangeMaterialOfGroup("Cloud.png", "ALL");
	*/
#pragma endregion
#pragma region Load Model and duplicate onto another one
	/*
	m_pModel3 = new Simplex::Model();
	m_pModel3->Load("Macross\\YF19.BTO");
	m_pModel1 = new Simplex::Model();
	m_pModel1->Instance(m_pModel3, "_copy_");
	*/
#pragma endregion
#pragma region Load Model
	/*
	m_pModel1 = new Simplex::Model();
	m_pModel1->Load("Macross\\YF19.BTO");
	*/
#pragma endregion
#pragma region Sandbox
	//Background music
	m_soundBGM.openFromFile(sRoute + "elementary-wave-11.ogg");
	m_soundBGM.play();
	m_soundBGM.setLoop(true);

	//sound effect
	m_soundBuffer.loadFromFile(sRoute + "12C.wav");
	m_sound.setBuffer(m_soundBuffer);

	//load model   --- Planets ---         --- Set their initial positions ---

	for (int i = 0; i < 11; i++)
	{
		String first("Planets\\0");
		String second("_");
		String third(".OBJ");
		String together;

		together = first + std::to_string(i) + second + planet[i] + third;

		if (i == 10)
			together = "Planets\\03A_Moon.OBJ";

		m_pEntityMngr->AddEntity(together, planet[i]);
		m_pEntityMngr->SetAxisVisibility(true, planet[i]);

		if (i != 0 && i < 5) 
			m_pEntityMngr->SetModelMatrix(glm::translate(vector3(110 + (i * 15), 0.0f, 0.0f)) * glm::scale(vector3(scale[i])));
		else if (i != 10 && i >= 5)
		{
			if (i == 5)
				m_pEntityMngr->SetModelMatrix(glm::translate(vector3(100 + (i * 20), 0.0f, 0.0f)) * glm::scale(vector3(scale[i])));
			else
				m_pEntityMngr->SetModelMatrix(glm::translate(vector3(115 + (i * 20), 0.0f, 0.0f)) * glm::scale(vector3(scale[i])));
		}

		if (i == 10)
			m_pEntityMngr->SetModelMatrix(glm::translate(vector3(123012491240.0f, 213182398.0f, 9032384923.0f)));		// hehe we hide the moon - we did this to load the moon first

		planets.push_back(Planet(vector3(0.0f), vector3(0.0f), vector3(15 + (i * 3), 0.0f, 0.0f), 0.0f, planet[i]));	// no direction, force, tried to figure out center, no radius, name of planet
	}

	//Sun
	//m_pEntityMngr->AddEntity("Planets\\00_Sun.OBJ", "Sun");
	//m_pEntityMngr->SetAxisVisibility(true, "Sun"); //set visibility of the entity's axis
	//m_pEntityMngr->GetEntity
		

#pragma endregion
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Move light... just for fun...
	///static double dTimer = 0.0f; //create a variable to store time
	//static uint uClock = m_pSystem->GenClock(); //generate a clock to track time
	//dTimer += m_pSystem->GetDeltaTime(uClock); //get the time difference since last time called
	//double dAngle = MapValue(dTimer, 0.0, 5.0, 0.0, 360.0);//map the value so we do not need to wait 360 seconds, only 5
	
	//static vector3 v3Color(C_WHITE); //color of the light
	//vector3 v3Position(glm::sin(glm::radians(dAngle)) * 5.0f, 2.5f, glm::cos(glm::radians(dAngle)) * 5.0f);//holds position of light
	//m_pLightMngr->SetPosition(v3Position, 1); //set the position of first light(0 is reserved for global light)
	//m_pLightMngr->SetIntensity(5.0f, 1); //set the intensity of first light
	//m_pLightMngr->SetColor(v3Color, 1); //set the color of first light
	//m_pMeshMngr->AddSphereToRenderList(glm::translate(v3Position) * glm::scale(vector3(0.15f)), v3Color, RENDER_SOLID); //add a sphere to "see" it

	//Update Entity Manager
	m_pEntityMngr->Update();

	//Not working as intended
	//for (int i = 0; i < m_pEntityMngr->m_uEntityCount; i++) {
		//Move the entity of the index
	//	matrix4 currentMatrix = m_pEntityMngr->GetEntity(i)->GetModelMatrix();
	//	currentMatrix *= glm::translate(IDENTITY_M4, listOfPlanetProps[i].direction); //translate it
	//	m_pEntityMngr->SetModelMatrix(currentMatrix); //return it to its owner
	//}

	//Move the entity of the index
	//matrix4 currentMatrix = m_pEntityMngr->GetEntity(9)->GetModelMatrix();
	//currentMatrix *= glm::translate(IDENTITY_M4, listOfPlanetProps[9].direction); //translate it
	//m_pEntityMngr->SetModelMatrix(currentMatrix); //return it to its owner


	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);

	matrix4 sunMatrix = m_pEntityMngr->GetEntity(0)->GetModelMatrix();
	sunMatrix = glm::scale(vector3(109.0f));
	m_pEntityMngr->GetEntity(0)->SetModelMatrix(sunMatrix);

	//matrix4 venusMatrix = m_pEntityMngr->GetEntity(2)->GetModelMatrix();
	//glm::vec3 scale = glm::vec3(4, 4, 4);
	//venusMatrix = glm::scale(venusMatrix,scale);
	//m_pEntityMngr->GetEntity(2)->SetModelMatrix(venusMatrix);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

		//display octree
		if (m_uOctantID == -1)
			m_pRoot->Display();
		else
			m_pRoot->Display(m_uOctantID);
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	// set the model matrix of the model
	//m_pModel->SetModelMatrix(ToMatrix4(m_qArcBall));
	//play the default sequence of the model
	//m_pModel->PlaySequence();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();
	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}

void Application::Release(void)
{
	//release the entity manager
	m_pEntityMngr->ReleaseInstance();

	//release GUI
	ShutdownGUI();
}

void Application::MoveComets(void)
{
	for (int i = 11; i < m_pEntityMngr->m_uEntityCount; i++)
	{
		m_pEntityMngr->GetEntity(i)->SetModelMatrix(glm::translate(vector3()));
	}
}