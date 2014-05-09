#include "texture_atlas.h"
#include <iostream>
#include <cstdlib>

using namespace std;

namespace
{
Image loadImage()
{
    try
    {
        return Image(L"textures.png");
    }
    catch(exception &e)
    {
        cerr << "error : " << e.what() << endl;
        exit(1);
    }
}
}

const Image TextureAtlas::texture = loadImage();

const TextureAtlas TextureAtlas::Font8x8(0, 0, 128, 128),
TextureAtlas::Line(128, 0, 1, 1),
TextureAtlas::Point(128, 1, 1, 1);
