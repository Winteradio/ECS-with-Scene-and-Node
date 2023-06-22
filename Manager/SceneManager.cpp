#include "SceneManager.h"

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::Init()
{
    Log::Info(" Initialize Scene Manager ");
}

void SceneManager::Destroy()
{
    Log::Info(" Destroy Scene Manager ");
}

SceneManager& SceneManager::GetHandle() { return m_SceneManager; }
SceneManager SceneManager::m_SceneManager;