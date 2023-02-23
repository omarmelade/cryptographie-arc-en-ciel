#pragma once

#include "config.h"

#include <map>

namespace AEC {

class Table
{

public:
    Table(Config &CFG, int largeur, int hauteur, bool test = false);
    Table(Config &CFG);

    inline int getLargeur() const { return _largeur; }
    inline int getHauteur() const { return _hauteur; }
    inline std::map<int64_t, int64_t> &getTable() { return _table; }

    bool sauve_table(std::string filename);
    bool ouvre_table(std::string filename);

private:
    Config &_CFG;
    int _largeur;
    int _hauteur;

    std::map<int64_t, int64_t> _table;

    void create_table_test();
    void create_table();

};
}
