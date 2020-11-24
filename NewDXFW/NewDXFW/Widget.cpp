#include "Widget.h"
#include "NewsQueue.h"

int Widget::counterID = (int) SENDER_NEWS::SIZE;

Widget::Widget () :
	m_id (Widget::counterID++)
{}

Widget::~Widget ()
{

}

int Widget::GetID ()
{
	return m_id;
}