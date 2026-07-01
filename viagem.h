#ifndef VIAGEM_H
#define VIAGEM_H

#include <vector>
#include <string>
#include "Cidade.h"
#include "Transporte.h"
#include "Passageiros.h"

using namespace std;

class Viagem {
private:
    Transporte* veiculo;
    vector<Passageiro*> passageiros;
    Cidade* origem;
    Cidade* destino;
    
    int distanciaTotal;
    int kmPercorridos;
    int horasConsecutivasMovimento; 
    int horasRestantesDescanso;
    int tempoTotalEstimado;
    int horasPassadas;

    string formatarCapitalizar(string str) const;

public:
    Viagem(Transporte* v, vector<Passageiro*> p, Cidade* o, Cidade* d, int distTotal);
    
    bool avancarUmaHora(); 
    void exibirEstado() const;
    
    Transporte* getVeiculo() const { return veiculo; }
    vector<Passageiro*> getPassageiros() const { return passageiros; }
    Cidade* getOrigem() const { return origem; }
    Cidade* getDestino() const { return destino; }
    int getHorasPassadas() const { return horasPassadas; }
};

#endif