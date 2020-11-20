#include "Button.h"

void Button::SetStateWait ()
{
	m_clickSend = false;

	m_state = BUTTONSTATE::WAIT;
}

void Button::SetStateFocused ()
{
	m_clickSend = false;

	if (m_state != BUTTONSTATE::CLICKED)
	{
		m_state = BUTTONSTATE::FOCUSED;
	}
}

void Button::SetStateClicked ()
{
	m_state = BUTTONSTATE::CLICKED;
}

BUTTONSTATE Button::GetCurrentState ()
{
	return m_state;
}

void Button::SetPassed ()
{
	m_passed = true;
}

void Button::SetReleased ()
{
	m_passed = false;
}

bool Button::IsPassed ()
{
	return m_passed;
}

bool Button::HaveSingleClick ()
{
	if (m_clickSend == false && m_state == BUTTONSTATE::CLICKED)
	{
		m_clickSend = true;
		return true;
	}
	
	return false;
}
