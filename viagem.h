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
    int kmDesdeUltimoDescanso;
    int horasRestantesDescanso;
    int tempoTotalEstimado;
    int horasPassadas;

public:
    Viagem(Transporte* v, vector<Passageiro*> p, Cidade* o, Cidade* d, int distTotal);
    
    bool avancarUmaHora(); // Retorna true se a viagem chegou ao fim nesta hora
    void exibirEstado() const;
    
    Transporte* getVeiculo() const { return veiculo; }
    vector<Passageiro*> getPassageiros() const { return passageiros; }
    Cidade* getOrigem() const { return origem; }
    Cidade* getDestino() const { return destino; }
};

#endif