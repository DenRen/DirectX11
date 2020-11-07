#include "de_text.h"

//create font outlines
void deText::initFont()
{
  HDC hDC = wglGetCurrentDC();
  HFONT hFont;
  LOGFONT logfont;
  GLYPHMETRICSFLOAT *agmf = new GLYPHMETRICSFLOAT[256];

  //memset(&logfont, 0, sizeof(logfont));
  //set font properties
  logfont.lfHeight = 12;
  logfont.lfWidth = 0;
  logfont.lfEscapement = 0;
  logfont.lfOrientation = 0;
  logfont.lfWeight = FW_NORMAL;
  logfont.lfItalic = FALSE;
  logfont.lfUnderline = FALSE;
  logfont.lfStrikeOut = FALSE;
  logfont.lfCharSet = ANSI_CHARSET;
  logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
  logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
  logfont.lfQuality = PROOF_QUALITY;
  logfont.lfPitchAndFamily = DEFAULT_PITCH || FF_ROMAN;
  strcpy_s(logfont.lfFaceName, LF_FACESIZE, "Tahoma");
  //create font
  hFont = CreateFontIndirect(&logfont);
  SelectObject(hDC, hFont);
  //generate 256 gl lists
  nFontList = glGenLists(256);
  if (!nFontList)
  {
    //TODO: errors handling
	DeleteObject(hFont);
	delete[] agmf;
    return;
  }
  //create outlines
  wglUseFontOutlines(hDC, 0, 256, nFontList, 0.0, 0.5, WGL_FONT_POLYGONS, agmf);
  //delete font
  DeleteObject(hFont);
}

//display text
void deText::printText(const double x, const double y, const char *text)
{
  //backup view
  glPushMatrix();
  //move view to text position (left bottom corner of the text)
  glTranslatef((GLfloat)x, (GLfloat)y, 0.0);
  //scale view
  glScalef(30.0, 30.0, 0.0);
  //rotate view
  glRotatef(180, 1.0, 0.0, 0.0);
  //set text color black
  glColor3f(0.0, 0.0, 0.0);
  //set list base
  glListBase(nFontList);
  //call lists, drawing every symbol
  glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
  //restore view
  glPopMatrix();
}