#ifndef GERENCIADORARQUIVOS_H
#define GERENCIADORARQUIVOS_H

#include "ModelosFicticios.h"
#include <vector>

void salvarDados(const vector<CidadeFicticia>& cidades, 
                 const vector<TrajetoFicticio>& trajetos, 
                 const vector<TransporteFicticio>& transportes, 
                 const vector<PassageiroFicticio>& passageiros,
                 const vector<ViagemFicticia>& viagens);

void carregarDados(vector<CidadeFicticia>& cidades, 
                   vector<TrajetoFicticio>& trajetos, 
                   vector<TransporteFicticio>& transportes, 
                   vector<PassageiroFicticio>& passageiros,
                   vector<ViagemFicticia>& viagens);

#endif