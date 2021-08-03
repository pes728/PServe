//
//  Image.hpp
//  PRender
//
//  Created by Peter Sandquist on 9/28/20.
//

#ifndef Image_h
#define Image_h

#include "Color.h"
#include <string>


void saveImage(std::string dir, std::string filename, int width, int height, const void* data);

void setPixel(int x, int y, int width, int height, vec3 c, float* data, bool flip);

void init(int width, int height, float* data, vec3 c);

inline uint8_t toColor(float v);

void imageToColor(int width, int height, uint8_t* dst, float* src);

#endif /* Image_hpp */
