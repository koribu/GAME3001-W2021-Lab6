#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	
	
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();	
	}
	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::m_CheckShipLOS(DisplayObject* targetObject)
{
	auto ShipToTargetDistance = Util::distance(m_pShip->getTransform()->position, targetObject->getTransform()->position);
	if(ShipToTargetDistance<=m_pShip->getLOSDistance())
	{
		std::vector<DisplayObject*> contactList;
		for (auto object : getDisplayList())
		{
			auto ShipToObjectDistance = Util::distance(m_pShip->getTransform()->position, object->getTransform()->position);
			if(ShipToObjectDistance<=ShipToTargetDistance )
			{
				if(object->getType() != m_pShip->getType() && object->getType() != targetObject->getType())
				{
					contactList.push_back(object);
				}
			}
		}
		contactList.push_back(targetObject);
		auto hasLOS = CollisionManager::LOSCheck(m_pShip->getTransform()->position, 
		                                         m_pShip->getTransform()->position + m_pShip->getCurrentDirection() * m_pShip->getLOSDistance(), contactList, targetObject);

		m_pShip->setHasLOS(hasLOS);
	}
}

void PlayScene::update()
{
	updateDisplayList();
	m_CheckShipLOS(m_pTarget);

}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}

	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_F))
	{

	}

	
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_M))
	{
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_G))
	{
	}
	
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";


	
	// add the ship to the scene as a start point
	m_pShip = new Ship();
	m_pShip->getTransform()->position = glm::vec2(200.0f, 300.0f);
	addChild(m_pShip,2);
	

	m_pObstacle = new Obstacle();
	m_pObstacle->getTransform()->position = glm::vec2(400.0f, 300.0f);
	addChild(m_pObstacle);

	m_pObstacle1 = new Obstacle();
	m_pObstacle1->getTransform()->position = glm::vec2(200, 300.0f);
	addChild(m_pObstacle1);

	m_pObstacle2 = new Obstacle();
	m_pObstacle2->getTransform()->position = glm::vec2(400.0f, 100);
	addChild(m_pObstacle2);



	m_pTarget = new Target();
	m_pTarget->getTransform()->position = glm::vec2(600.0f, 300.0f);
	addChild(m_pTarget);

}

void PlayScene::GUI_Function() 
{
	//TODO: We need to deal with this
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001 - Lab 6", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	//allow ship rotation
	static int angle;
	if(ImGui::SliderInt("Ship Direction", &angle, -360,360))
	{
		m_pShip->setCurrentHeading(angle);
	}
	
	ImGui::Separator();

	static int shipRotation[] = { m_pShip->getTransform()->position.x, m_pShip->getTransform()->position.y };
	if (ImGui::SliderInt2("Ship Position", shipRotation, 0, 800))
	{
		m_pShip->getTransform()->position.x = shipRotation[0];
		m_pShip->getTransform()->position.y = shipRotation[1];
	}
	
	static int targetPosition[] = { m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y };
	if(ImGui::SliderInt2("Target Position", targetPosition, 0, 800))
	{
		m_pTarget->getTransform()->position.x = targetPosition[0];
		m_pTarget->getTransform()->position.y = targetPosition[1];
	}
	
	ImGui::Separator();
	
	if(ImGui::Button("Find Shortest Path"))
	{

	}

	if (ImGui::Button("Start"))
	{
		// make ship move
	}

	ImGui::SameLine();
	
	if (ImGui::Button("Reset"))
	{
		// reset everything back to initial values
		
	}

	ImGui::Separator();

	
	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

