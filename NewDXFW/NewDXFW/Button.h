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

	void SetPassed ();
	void SetReleased ();

	bool IsPassed ();
	bool HaveSingleClick ();

private:
	bool m_passed = false;
	bool m_clickSend = false;

	BUTTONSTATE m_state;
};