#include "Button.h"

void Button::SetStateWait ()
{
	m_state = BUTTONSTATE::WAIT;
}

void Button::SetStateFocused ()
{
	m_state = BUTTONSTATE::FOCUSED;
}

void Button::SetStateClicked ()
{
	m_state = BUTTONSTATE::CLICKED;
}

BUTTONSTATE Button::GetCurrentState ()
{
	return m_state;
}
