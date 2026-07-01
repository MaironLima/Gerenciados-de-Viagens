#ifndef CIDADE_H
#define CIDADE_H

#include <string>

using namespace std;

class Cidade {
private:
    string nome;

public:
    Cidade(string nome);
    string getNome() const;
};

#endif