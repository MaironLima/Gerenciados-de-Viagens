#ifndef CONTROLADOR_DE_TRANSITO_H
#define CONTROLADOR_DE_TRANSITO_H

#include <vector>
#include <string>
#include <unordered_map>
#include "Cidade.h"
#include "Transporte.h"
#include "Passageiros.h"
#include "Viagem.h"

using namespace std;

class ControladorDeTransito {
private:
    unordered_map<string, Cidade*> cidades;
    vector<Transporte*> transportes;
    vector<Passageiro*> passageiros;
    vector<Viagem*> viagensAtivas;

    string tornarMinusculo(string str);
    string formatarCapitalizar(string str) const;

public:
    ~ControladorDeTransito();

    Cidade* obterOuCriarCidade(const string& nome);
    Transporte* buscarTransporte(const string& nome);
    Passageiro* buscarPassageiro(const string& nome);
    
    void cadastrarTransporte(const string& nome, char tipo, int cap, int vel, int horasDesc, int tempoDesc, const string& cidadeInicial);
    void cadastrarPassageiro(const string& nome, const string& cidadeInicial);
    void iniciarViagem(const string& nomeTransporte, const vector<string>& nomesPassageiros, const string& origem, const string& destino);
    void avancarTempo(int horas);
    void mostrarViagensEmAndamento() const;
    void relatarLocalizacaoPassageiros() const;
    void relatarLocalizacaoTransportes() const;
};

#endif