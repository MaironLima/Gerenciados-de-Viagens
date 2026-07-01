#include "GerenciadorArquivos.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

void salvarDados(const vector<CidadeFicticia>& cidades, const vector<TrajetoFicticio>& trajetos, const vector<TransporteFicticio>& transportes, const vector<PassageiroFicticio>& passageiros, const vector<ViagemFicticia>& viagens) {
    ofstream fCid("cidades_ficticias.txt"), fTraj("trajetos_ficticios.txt"), fTrans("transportes_ficticios.txt"), fPass("passageiros_ficticios.txt"), fViag("viagens_ficticias.txt");

    for (const auto& c : cidades) fCid << c.getNome() << "\n";
    for (const auto& t : trajetos) fTraj << t.getOrigem() << ";" << t.getDestino() << ";" << t.getTipo() << ";" << t.getDistancia() << "\n";
    for (const auto& t : transportes) fTrans << t.getNome() << ";" << t.getTipo() << ";" << t.getCapacidade() << ";" << t.getVelocidade() << ";" << t.getDistEntreDescansos() << ";" << t.getTempoDescanso() << ";" << t.getLocalAtual() << ";" << t.isEmTransito() << "\n";
    for (const auto& p : passageiros) fPass << p.getNome() << ";" << p.getLocalAtual() << ";" << p.isEmTransito() << "\n";
    
    for (const auto& v : viagens) {
        fViag << v.veiculo->getNome() << ";" << v.origem << ";" << v.destino << ";" << v.distanciaTotal << ";" << v.kmPercorridos << ";" << v.horasPassadas << ";";
        for (size_t i = 0; i < v.rotaCompleta.size(); ++i) {
            fViag << v.rotaCompleta[i] << (i == v.rotaCompleta.size() - 1 ? "" : ",");
        }
        fViag << ";";
        for (size_t i = 0; i < v.passageiros.size(); ++i) {
            fViag << v.passageiros[i]->getNome() << (i == v.passageiros.size() - 1 ? "" : ",");
        }
        fViag << "\n";
    }
}

void carregarDados(vector<CidadeFicticia>& cidades, vector<TrajetoFicticio>& trajetos, vector<TransporteFicticio>& transportes, vector<PassageiroFicticio>& passageiros, vector<ViagemFicticia>& viagens) {
    string linha, item;
    
    ifstream fCid("cidades_ficticias.txt");
    while (getline(fCid, linha)) { if(!linha.empty()) cidades.push_back(CidadeFicticia(linha)); }

    ifstream fTraj("trajetos_ficticios.txt");
    while (getline(fTraj, linha)) {
        if(linha.empty()) continue;
        stringstream ss(linha); string o, d, t; int dist;
        getline(ss, o, ';'); getline(ss, d, ';'); getline(ss, t, ';'); ss >> dist;
        trajetos.push_back(TrajetoFicticio(o, d, t, dist));
    }

    ifstream fTrans("transportes_ficticios.txt");
    while (getline(fTrans, linha)) {
        if(linha.empty()) continue;
        stringstream ss(linha); string n, t, loc; int cap, vel, dd, td; bool emTr;
        getline(ss, n, ';'); getline(ss, t, ';'); ss >> cap; ss.ignore(); ss >> vel; ss.ignore(); ss >> dd; ss.ignore(); ss >> td; ss.ignore(); getline(ss, loc, ';'); ss >> emTr;
        transportes.push_back(TransporteFicticio(n, t, cap, vel, dd, td, loc, emTr));
    }

    ifstream fPass("passageiros_ficticios.txt");
    while (getline(fPass, linha)) {
        if(linha.empty()) continue;
        stringstream ss(linha); string n, loc; bool emTr;
        getline(ss, n, ';'); getline(ss, loc, ';'); ss >> emTr;
        passageiros.push_back(PassageiroFicticio(n, loc, emTr));
    }

    ifstream fViag("viagens_ficticias.txt");
    while (getline(fViag, linha)) {
        if(linha.empty()) continue;
        stringstream ss(linha); string vNome, orig, dest, rotaStr, passStr; int distT, kmP, hP;
        getline(ss, vNome, ';'); getline(ss, orig, ';'); getline(ss, dest, ';'); ss >> distT; ss.ignore(); ss >> kmP; ss.ignore(); ss >> hP; ss.ignore();
        getline(ss, rotaStr, ';'); getline(ss, passStr, ';');

        TransporteFicticio* veic = nullptr;
        for (auto& tr : transportes) { if (tr.getNome() == vNome) veic = &tr; }
        if (!veic) continue;

        vector<string> rota; string sub; stringstream ssR(rotaStr);
        while (getline(ssR, sub, ',')) rota.push_back(sub);

        vector<PassageiroFicticio*> passs; stringstream ssP(passStr);
        while (getline(ssP, sub, ',')) {
            for (auto& p : passageiros) { if (p.getNome() == sub) passs.push_back(&p); }
        }
        viagens.push_back(ViagemFicticia(veic, passs, orig, dest, rota, distT, kmP, hP));
    }
}