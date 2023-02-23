#pragma once

#include "config.h"

namespace AEC {

class Table
{

public:
    Table(Config &CFG, int64_t idx1, int largeur, int hauteur);
    ~Table();

    inline int64_t getIdx1() const { return _idx1; }
    inline int64_t getIdx2() const { return _idx2; }
    inline int getLargeur() const { return _largeur; }
    inline int getHauteur() const { return _hauteur; }

private:
    Config &_CFG;
    int64_t _idx1;
    int64_t _idx2;
    int _largeur;
    int _hauteur;
};
}
