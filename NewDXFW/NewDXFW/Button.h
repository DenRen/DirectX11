#pragma once
#include "Widget.h"

enum class BUTTONSTATE
{
	WAIT, FOCUSED, CLICKED
};

class Button : public Widget
{
public:

protected:
	void SetStateWait ();
	void SetStateFocused ();
	void SetStateClicked ();
	BUTTONSTATE GetCurrentState ();

private:
	BUTTONSTATE m_state;
};