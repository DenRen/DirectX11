#include "Widget.h"
#include "NewsQueue.h"

int Widget::counterID = (int) SENDER_NEWS::SIZE;

Widget::Widget () :
	m_id (Widget::counterID++)
{}

int Widget::GetID ()
{
	return m_id;
}