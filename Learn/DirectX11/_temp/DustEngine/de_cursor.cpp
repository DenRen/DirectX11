#include "de_cursor.h"

//draw cursor on the screen
void deCursor::draw()
{
  //backup view
  glPushMatrix();
  //move view to cursor position
  glTranslatef((GLfloat)x, (GLfloat)y, 0.0);
  //rotate view
  glRotatef((GLfloat)angle, 0.0, 0.0, 1.0);
  //set color and draw outer square
  glColor3f((GLfloat)1.0, (GLfloat)0.0, (GLfloat)1.0);
  glRectf((GLfloat)(2*collRadius), (GLfloat)(2*collRadius), (GLfloat)(-2*collRadius), (GLfloat)(-2*collRadius));
  //set color and draw inner square
  glColor3f((GLfloat)0.0, (GLfloat)0.0, (GLfloat)0.0);
  glRectf((GLfloat)collRadius, (GLfloat)collRadius, (GLfloat)-collRadius, (GLfloat)-collRadius);
  //restore original view
  glPopMatrix();
}