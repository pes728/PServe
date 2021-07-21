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

void setPixel(int x, int y, int width, Color c, uint8_t* data);

void init(int width, int height, uint8_t* data, Color c);

#endif /* Image_hpp */
