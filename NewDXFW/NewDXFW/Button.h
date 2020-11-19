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
	virtual void SetStateWait ();
	virtual void SetStateFocused ();
	virtual void SetStateClicked ();
	BUTTONSTATE GetCurrentState ();

private:
	BUTTONSTATE m_state;
};