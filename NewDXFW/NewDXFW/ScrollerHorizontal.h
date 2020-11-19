#pragma once
#include "Scroller.h"

class ScrollerHorizontal : public Scroller
{
public:

	ScrollerHorizontal (float coorX, float coorY, float width, float height, float relWidthScroll);

	static void InitDefTex_FirstButtonArrow  (const char *wait, const char *focused, const char *clicked);
	static void InitDefTex_SecondButtonArrow (const char *wait, const char *focused, const char *clicked);
	static void InitDefTex_ScrollSlider		 (const char *wait, const char *focused, const char *clicked);
	static void InitDefTex_SurfaceButton	 (const char *wait, const char *focused, const char *clicked);

	void SetSlider (float state);
	void MoveSlider (float deltaState);	// In coord

	void Draw ();
	void Update ();
	void HandleNews (News news);

private:

	BUTTONSTATE m_firstArrow;
	BUTTONSTATE m_secondArrow;
	BUTTONSTATE m_firstSurface;
	BUTTONSTATE m_secondSurface;

	static TextureButton def_texFirstButtonArrow;
	static TextureButton def_texSecondButtonArrow;
	static TextureButton def_texScrollSlider;
	static TextureButton def_texSurfaceButton;
};

