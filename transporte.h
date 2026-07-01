#ifndef TRANSPORTE_H
#define TRANSPORTE_H

#include <string>
#include "Cidade.h"

using namespace std;

class Transporte {
private:
    string nome;
    char tipo; 
    int capacidade;
    int velocidade;
    int horasEntreDescansos; 
    int tempoDescanso;
    Cidade* localAtual;
    bool emTransito;

public:
    Transporte(string nome, char tipo, int capacidade, int velocidade, int horasEntreDescansos, int tempoDescanso, Cidade* localInicial);
    
    string getNome() const;
    char getTipo() const;
    int getCapacidade() const;
    int getVelocidade() const;
    int getHorasEntreDescansos() const;
    int getTempoDescanso() const;
    
    Cidade* getLocalAtual() const;
    void setLocalAtual(Cidade* local);
    bool isEmTransito() const;
    void setEmTransito(bool estado);
};

#endif