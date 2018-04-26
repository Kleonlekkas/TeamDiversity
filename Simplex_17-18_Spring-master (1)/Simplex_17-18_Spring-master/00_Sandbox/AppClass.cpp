#include "AppClass.h"

using namespace Simplex;
using namespace std;

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
		//m_pEntityMngr->SetAxisVisibility(true, planet[i]);

		if (i != 0 && i < 5) 
			m_pEntityMngr->SetModelMatrix(glm::translate(vector3(210 + (i * 15), 50.0f, 0.0f)) * glm::scale(vector3(scale[i])));
		else if (i != 10 && i >= 5)
		{
			if (i == 5)
				m_pEntityMngr->SetModelMatrix(glm::translate(vector3(200 + (i * 20), 50.0f, 0.0f)) * glm::scale(vector3(scale[i])));
			else
				m_pEntityMngr->SetModelMatrix(glm::translate(vector3(215 + (i * 20), 50.0f, 0.0f)) * glm::scale(vector3(scale[i])));
		}

		if (i == 10)
			m_pEntityMngr->SetModelMatrix(glm::translate(vector3(300.0f)));		// hehe we hide the moon - we did this to load the moon first

		planets.push_back(Planet(vector3(0.0f), vector3(0.0f), vector3(15 + (i * 3), 0.0f, 0.0f), 0.0f, planet[i]));	// no direction, force, tried to figure out center, no radius, name of planet
	}

	for (int i = 0; i < 500; i++)
	{
		m_pEntityMngr->AddEntity("Planets\\03A_Moon.OBJ", "Moon");
		vector3 v3Position = vector3(glm::sphericalRand(315.0f));
		v3Position.x += 200.0f;
		matrix4 m4Position = glm::translate(v3Position);
		vector3 scale = glm::sphericalRand(3.0f);
		scale.x = scale.y = scale.z;
		m_pEntityMngr->SetModelMatrix(m4Position * glm::scale(scale));
		planets.push_back(Planet(vector3(0.0f), vector3(0.0f), v3Position, 0.0f, "Moon"));
	}

	m_uOctantLevels = 1;
	m_pRoot = new MyOctant(m_uOctantLevels, 5);
	m_pEntityMngr->Update();

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


	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);

	matrix4 sunMatrix = m_pEntityMngr->GetEntity(0)->GetModelMatrix();
	sunMatrix = glm::translate(vector3(100,50,0));
	m_pEntityMngr->GetEntity(0)->SetModelMatrix(sunMatrix * glm::scale(vector3(100)));
	
	m_pRoot = new MyOctant(m_uOctantLevels, 5);

	m_pEntityMngr->Update();

	//for (int i = 0; i < m_pEntityMngr->m_uEntityCount; i++)
	//{
	//	if (m_pEntityMngr[i].Collides())
	//		m_pEntityMngr->RemoveEntity(i);
	//}
	
	for (int i = 0; i < asteroids.size(); i++)
	{
		matrix4 tempMatrix = m_pEntityMngr->GetEntity(i + 9)->GetModelMatrix();
		tempMatrix = glm::translate(vector3(1.0f, 1.0f, 1.0f));
		m_pEntityMngr->GetEntity(i + 9)->SetModelMatrix(tempMatrix);
		//asteroids[i].SetCenter(vector3(m_pCameraMngr->GetPosition().x + glm::translate(vector3(1.0f)), m_pCameraMngr->GetPosition().y + glm::translate(vector3(1.0f)), m_pCameraMngr->GetPosition().z + glm::translate(vector3(1.0f))));
	}
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
	SafeDelete(m_pRoot);

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