#pragma once
#include "Scroller.h"

class ScrollerVertical : public Scroller
{
public:

	ScrollerVertical (float coorX, float coorY, float width, float height,
					  float relHeightScroll);

	static void InitDefTex_FirstButtonArrow (const char *wait, const char *focused, const char *clicked);
	static void InitDefTex_SecondButtonArrow (const char *wait, const char *focused, const char *clicked);
	static void InitDefTex_ScrollSlider (const char *wait, const char *focused, const char *clicked);
	static void InitDefTex_SurfaceButton (const char *wait, const char *focused, const char *clicked);

	void SetSlider (float state);
	void MoveSlider (float deltaState);	// In coord
	float GetStateSlider ();

	void Draw ();
	void Update ();
	void HandleNews (News news);

private:


	static TextureButton def_texFirstButtonArrow;
	static TextureButton def_texSecondButtonArrow;
	static TextureButton def_texScrollSlider;
	static TextureButton def_texSurfaceButton;

	bool m_prevScrollClicked = false;
	bool m_mousePosUpdated = false;
	MousePosition m_prevMousePosition = { 0, 0 };
	MousePosition m_deltaMousePosition = { 0, 0 };
};

