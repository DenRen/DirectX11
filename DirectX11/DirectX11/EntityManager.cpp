#include "EntityManager.h"

EntityManager *EntityManager::m_instance = nullptr;

EntityManager::EntityManager ()
{
	
}

EntityManager::~EntityManager ()
{
	RemoveAllEntityes ();
}

void EntityManager::Update ()
{
	const int quantityEntities = m_entities.size ();
	for (int i = 0; i < quantityEntities; i++)
	{
		m_entities[i]->Update ();
	}
}

void EntityManager::Render (ID3D11DeviceContext *deviceContext, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	const int quantityEntities = m_entities.size ();
	for (int i = 0; i < quantityEntities; i++)
	{
		m_entities[i]->Render (deviceContext, viewMatrix, projectionMatrix);
	}
	
}

Entity *EntityManager::AddEntity ()
{
	Entity *entity = new Entity ();
	m_entities.push_back (entity);

	return entity;
}

void EntityManager::RemoveEntity (Entity *entity)
{
	int index = -1;

	if (entity != nullptr)
	{
		if (ContainsEntity (entity, index))
		{
			m_entities.erase (m_entities.begin () + index);
		
			delete entity;
		}
	}
}

void EntityManager::RemoveAllEntityes ()
{
	const int quantityEntities = m_entities.size ();
	
	for (int i = 0; i < quantityEntities; i++)
	{
		delete m_entities[i];
	}

	m_entities.clear ();
}

bool EntityManager::ContainsEntity (Entity * entity)
{
	const int quantityEntities = m_entities.size ();
	for (int i = 0; i < quantityEntities; i++)
	{
		if (m_entities[i] == entity)
		{
			return true;
		}
	}

	return false;
}

bool EntityManager::ContainsEntity (Entity *entity, int &index)
{
	index = -1;

	const int quantityEntities = m_entities.size ();
	for (int i = 0; i < quantityEntities; i++)
	{
		if (m_entities[i] == entity)
		{
			index = i;
			return true;
		}
	}

	return false;
}

EntityManager *EntityManager::GetInstance ()
{
	if (m_instance == nullptr)
	{
		m_instance = new EntityManager ();
	}

	return m_instance;
}
