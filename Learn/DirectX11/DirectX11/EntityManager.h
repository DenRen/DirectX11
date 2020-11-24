#pragma once

#include "Entity.h"
#include <vector>

class EntityManager
{
public:
	~EntityManager ();

	void Update ();
	void Render (ID3D11DeviceContext *deviceContext, 
				 D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

	Entity *AddEntity ();
	void RemoveEntity (Entity *entity);
	void RemoveAllEntityes ();

	bool ContainsEntity (Entity *entity);
	bool ContainsEntity (Entity *entity, int &index);

	static EntityManager *GetInstance ();

private:
	EntityManager ();

	std::vector <Entity *> m_entities;
	static EntityManager *m_instance;
};

