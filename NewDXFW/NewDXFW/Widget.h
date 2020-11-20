#pragma once
#include "NewsQueue.h"

class Widget
{
public:

	Widget ();
	virtual ~Widget ();

	virtual void Draw () = 0;
	virtual void Update () = 0;
	virtual void HandleNews (News news) = 0;
	
	int GetID ();

private:
	const int m_id;
	
	static int counterID;
};

