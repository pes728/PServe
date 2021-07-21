//
//  Image.cpp
//  PRender
//
//  Created by Peter Sandquist on 9/28/20.
//

#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_write.h"

void saveImage(std::string dir, std::string filename, int width, int height, const void* data){
    stbi_write_bmp((dir + filename + ".bmp").c_str(), width, height, 3, data);
}

void setPixel(int x, int y, int width, Color c, uint8_t* data){
    int index = (x + y * width) * 3;
    data[index + 0] = c.data[0];
    data[index + 1] = c.data[1];
    data[index + 2] = c.data[2];
}

void init(int width, int height, uint8_t* data, Color c){
    for(int i = 0; i < width * height; i++){
        int index = i * 3;
        data[index + 0] = c.data[0];
        data[index + 1] = c.data[1];
        data[index + 2] = c.data[2];
    }
}
