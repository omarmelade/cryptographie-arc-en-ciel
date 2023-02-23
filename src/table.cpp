#include "table.h"
#include "utility.h"

namespace AEC {

Table::Table(Config &CFG, int largeur, int hauteur, bool test /*= false*/)
    : _CFG(CFG),
      _largeur(largeur),
      _hauteur(hauteur)
{
    if (test) {
        create_table_test();
    }
    else {
        create_table();
    }
}

Table::Table(Config &CFG)
    : _CFG(CFG),
      _largeur(0),
      _hauteur(0)
{
}

void Table::create_table_test()
{
    _table.clear();
    for (int i = 0; i < _hauteur; ++i) {
        int64_t chaineX = Utility::nouvelle_chaine(_CFG, i, _largeur);
        _table[chaineX] = i;
    }
}

void Table::create_table()
{
    _table.clear();
    for (int i = 0; i < _hauteur; ++i) {
        int64_t idx = Utility::index_aleatoire() % _CFG.getN();
        int64_t chaineX = Utility::nouvelle_chaine(_CFG, idx, _largeur);
        _table[chaineX] = idx;
    }
}

// TODO: use ifstream filename instead of FILE *f
bool Table::sauve_table(std::string filename)
{
    // write header in file (largeur, hauteur, alphabet, taille_min, taille_max)
    FILE *f = fopen(filename.c_str(), "wb");
    fprintf(f, "%d %d %s %d %d\n", _largeur, _hauteur, _CFG.getCodeAlphabet().c_str(), _CFG.getTailleMin(), _CFG.getTailleMax());
    // write all map elements in bytes
    for (auto it = _table.begin(); it != _table.end(); ++it) {
        fwrite(&it->first, sizeof(int64_t), 1, f);
        fwrite(&it->second, sizeof(int64_t), 1, f);
    }
    fclose(f);

    return true;
}

// TODO: use ifstream filename instead of FILE *f
bool Table::ouvre_table(std::string filename)
{
    // read header in file (largeur, hauteur, alphabet, taille_min, taille_max)
    FILE *f = fopen(filename.c_str(), "rb");
    int largeur, hauteur, taille_min, taille_max;
    char *alphabet = new char[100];
    size_t res = fscanf(f, "%d %d %s %d %d\n", &largeur, &hauteur, alphabet, &taille_min, &taille_max);
    std::cout << "genial" << std::endl;
    if (!res) {
        fclose(f);
        return false;
    }
    _CFG.setTailleMin(taille_min);
    _CFG.setTailleMax(taille_max);
    std::string alphabet_str(alphabet);
    _CFG.setCodeAlphabet(alphabet_str);
    _largeur = largeur;
    _hauteur = hauteur;

    // read all map elements in bytes
    _table.clear();
    for (int i = 0; i < _hauteur; ++i) {
        int64_t chaineX, idx;
        if (!fread(&chaineX, sizeof(int64_t), 1, f)) {
            fclose(f);
            return false;
        }
        if (!fread(&idx, sizeof(int64_t), 1, f)) {
            fclose(f);
            return false;
        }
        _table[chaineX] = idx;
    }
    fclose(f);

    return true;
}


}