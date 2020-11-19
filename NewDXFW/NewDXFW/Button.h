#pragma once
#include "Widget.h"

enum class BUTTONSTATE
{
	WAIT, FOCUSED, CLICKED
};

class Button : public Widget
{
public:

	void SetStateWait ();
	void SetStateFocused ();
	void SetStateClicked ();
	BUTTONSTATE GetCurrentState ();

	bool HaveSingleClick ();

private:
	bool m_clickSend = false;

	BUTTONSTATE m_state;
};