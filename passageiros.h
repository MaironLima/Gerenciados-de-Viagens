#ifndef PASSAGEIRO_H
#define PASSAGEIRO_H

#include <string>
#include "Cidade.h"

using namespace std;

class Passageiro {
private:
    string nome;
    Cidade* localAtual;
    bool emTransito;

public:
    Passageiro(string nome, Cidade* localInicial);

    string getNome() const;
    Cidade* getLocalAtual() const;
    void setLocalAtual(Cidade* local);
    bool isEmTransito() const;
    void setEmTransito(bool estado);
};

#endif