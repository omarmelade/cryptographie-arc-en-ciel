#pragma once

#include "config.h"

#include <iostream>
#include <sstream>

namespace AEC {

typedef unsigned char byte;

class Utility
{
public:
    static std::string empreinteToString(byte *empreinte);
    static std::string i2c(Config &CFG, int64_t index);
    static int64_t h2i(Config &CFG, byte* empreinte, int t);
    static int64_t i2i(Config &CFG, int64_t index, int t);
    static int64_t nouvelle_chaine(Config &CFG, int64_t idx1, int largeur);
    static int64_t index_aleatoire();
    static void hash_MD5(const char *s, byte *empreinte);
    static void hash_SHA1(const char *s, byte *empreinte);

};

}

