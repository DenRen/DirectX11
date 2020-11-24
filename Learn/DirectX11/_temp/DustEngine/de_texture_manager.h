#ifndef __DE_TEXTURE_MANAGER_H
#define __DE_TEXTURE_MANAGER_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>

//textures loading/unloading class
class deTextureManager
{
private:
  //texture names
  std::vector<GLuint> textures;
public:
  //clear all textures
  void clear()
  {
    for (std::vector<GLuint>::iterator idxTex = textures.begin(); idxTex != textures.end(); ++idxTex)
    {
      glDeleteTextures(1, &(*idxTex));
    }
    textures.clear();
  }
  //get a texture name by its index
  GLuint getName(const int index)
  {
    if ((index < 0) || (index > textures.size() - 1))
      return -1;
    return textures[index];
  }
  //load a texture (256x256 RAW)
  void loadTexture(const char *filename)
  {
    GLuint texture;
    int width, height;
    void *data;
    FILE *file;

    //open texture data
    //!!!
    file = fopen(filename, "rb");
      if (file == NULL) return;

    //allocate buffer
    width = 256;
    height = 256;
    data = malloc( width * height * 3 );

    //read texture data
    fread(data, width * height * 3, 1, file);
    fclose(file);

    //allocate a texture name
    glGenTextures(1, &texture);

    //select our current texture
    glBindTexture(GL_TEXTURE_2D, texture);

    //select modulate to mix texture with color for shading
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    //when texture area is small, bilinear filter the closest mipmap
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_LINEAR_MIPMAP_NEAREST);
    //when texture area is large, bilinear filter the first mipmap
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //build our texture mipmaps
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

    //free buffer
    free(data);

    textures.resize(textures.size() + 1);
    textures.push_back(texture);
  }
};

#endif