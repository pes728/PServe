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

void setPixel(int x, int y, int width, int height, vec3 c, float* data, bool flip){
    int index = flip ? (x + (height - 1 - y) * width): (x + y * width);
    index *= 3;
    data[index + 0] = c[0];
    data[index + 1] = c[1];
    data[index + 2] = c[2];
}

void init(int width, int height, float* data, vec3 c){
    for(int i = 0; i < width * height; i++){
        int index = i * 3;
        data[index + 0] = c[0];
        data[index + 1] = c[1];
        data[index + 2] = c[2];
    }
}

inline uint8_t toColor(float a) {
    return (uint8_t)(256 * clamp(a, 0.0, 0.999));
}

void imageToColor(int width, int height, uint8_t* dst, float* src)
{
    for (int i = 0; i < width * height * 3; i++) {
        dst[i] = toColor(src[i]);
    }
}
