#include "Widget.h"

int Widget::counterID = 0;

Widget::Widget () :
	m_id (Widget::counterID++)
{}

int Widget::GetID ()
{
	return m_id;
}
	
