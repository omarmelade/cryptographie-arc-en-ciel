#include "table.h"

namespace AEC {

Table::Table(Config &CFG, int64_t idx1, int largeur, int hauteur)
    : _CFG(CFG),
      _idx1(idx1),
      _largeur(largeur),
      _hauteur(hauteur)
{
}

}