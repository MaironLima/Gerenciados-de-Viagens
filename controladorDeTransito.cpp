#include "controladorDeTransito.h"
#include "distancias.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <cmath>

using namespace std;

ControladorDeTransito::~ControladorDeTransito()
{
    for (auto par : cidades)
        delete par.second;
    for (auto t : transportes)
        delete t;
    for (auto p : passageiros)
        delete p;
    for (auto v : viagensAtivas)
        delete v;
}

string ControladorDeTransito::tornarMinusculo(string str)
{
    transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
        { return tolower(c); });
    return str;
}

Cidade *ControladorDeTransito::obterOuCriarCidade(const string &nome)
{
    string nF = tornarMinusculo(nome);
    if (distancias.find(nF) == distancias.end())
        return nullptr;
    if (cidades.find(nF) == cidades.end())
    {
        cidades[nF] = new Cidade(nF);
    }
    return cidades[nF];
}

Transporte *ControladorDeTransito::buscarTransporte(const string &nome)
{
    for (auto t : transportes)
    {
        if (t->getNome() == nome)
            return t;
    }
    return nullptr;
}

Passageiro *ControladorDeTransito::buscarPassageiro(const string &nome)
{
    for (auto p : passageiros)
    {
        if (p->getNome() == nome)
            return p;
    }
    return nullptr;
}

void ControladorDeTransito::cadastrarTransporte(const string &nome, char tipo, int cap, int vel, int distDesc, int tempoDesc, const string &cidadeInicial)
{
    if (cap <= 0)
    {
        cout << "Erro especifico: A capacidade do transporte nao pode ser de 0 ou menos passageiros.\n";
        return;
    }
    Cidade *cid = obterOuCriarCidade(cidadeInicial);
    if (!cid)
    {
        cout << "Erro especifico: A cidade inicial '" << cidadeInicial << "' nao existe na malha de capitais.\n";
        return;
    }

    transportes.push_back(new Transporte(nome, toupper(tipo), cap, vel, distDesc, tempoDesc, cid));
    cout << "Transporte '" << nome << "' registrado com sucesso em '" << cid->getNome() << "'.\n";
}

void ControladorDeTransito::cadastrarPassageiro(const string &nome, const string &cidadeInicial)
{
    Cidade *cid = obterOuCriarCidade(cidadeInicial);
    if (!cid)
    {
        cout << "Erro especifico: A cidade inicial '" << cidadeInicial << "' nao existe na malha de capitais.\n";
        return;
    }
    passageiros.push_back(new Passageiro(nome, cid));
    cout << "Passageiro '" << nome << "' registrado com sucesso em '" << cid->getNome() << "'.\n";
}

void ControladorDeTransito::iniciarViagem(const string &nomeTransporte, const vector<string> &nomesPassageiros, const string &origem, const string &destino)
{
    string oF = tornarMinusculo(origem);
    string dF = tornarMinusculo(destino);

    if (oF == dF)
    {
        cout << "Erro especifico: A cidade de destino '" << destino << "' e igual a de origem. Nao havera deslocamento.\n";
        return;
    }

    Transporte *veiculo = buscarTransporte(nomeTransporte);
    if (!veiculo)
    {
        cout << "Erro especifico: O veiculo '" << nomeTransporte << "' nao foi encontrado no sistema.\n";
        return;
    }

    if (veiculo->isEmTransito())
    {
        cout << "Erro especifico: O veiculo '" << nomeTransporte << "' ja esta em transito em outra viagem.\n";
        return;
    }

    Cidade *cidOrigem = obterOuCriarCidade(oF);
    if (!cidOrigem || veiculo->getLocalAtual() != cidOrigem)
    {
        cout << "Erro especifico: O veiculo '" << nomeTransporte << "' nao esta localizado na origem '" << origem << "'.\n";
        return;
    }

    Cidade *cidDestino = obterOuCriarCidade(dF);
    if (!cidDestino)
    {
        cout << "Erro especifico: A cidade de destino '" << destino << "' nao existe na malha de capitais.\n";
        return;
    }

    vector<Passageiro *> alocados;
    for (const auto &n : nomesPassageiros)
    {
        Passageiro *pass = buscarPassageiro(n);
        if (!pass)
        {
            cout << "Erro especifico: O passageiro '" << n << "' nao esta cadastrado no sistema.\n";
            return;
        }
        if (pass->isEmTransito() || pass->getLocalAtual() != cidOrigem)
        {
            cout << "Erro especifico: O passageiro '" << n << "' nao esta disponivel na cidade de origem '" << origem << "'.\n";
            return;
        }
        alocados.push_back(pass);
    }

    int km = getDistancia(oF, dF);
    if (km < 0)
    {
        cout << "Erro especifico: Nao existe rota direta conectando '" << origem << "' a '" << destino << "'.\n";
        return;
    }

    char tipoNecessario = getTipoTrajetoNecessario(oF, dF);
    if (tipoNecessario == 'A' && veiculo->getTipo() != 'A')
    {
        cout << "Erro especifico: O trecho ate '" << destino << "' requer um veiculo Aereo devido ao bloqueio terrestre.\n";
        return;
    }

    // Criar e iniciar a jornada de tempo real
    Viagem *novaViagem = new Viagem(veiculo, alocados, cidOrigem, cidDestino, km);
    viagensAtivas.push_back(novaViagem);

    cout << "\n>>> VIAGEM INICIADA COM SUCESSO! <<<\n";
}

void ControladorDeTransito::avancarTempo(int horas)
{
    if (horas <= 0)
        return;
    cout << "\n--- AVANCANDO " << horas << " HORA(S) NO SISTEMA ---\n";

    for (int h = 0; h < horas; ++h)
    {
        for (auto it = viagensAtivas.begin(); it != viagensAtivas.end();)
        {
            Viagem *v = *it;
            bool concluida = v->avancarUmaHora();

            if (concluida)
            {
                cout << "\n=========================================\n";
                cout << "     [VIAGEM CONCLUIDA COM SUCESSO]       \n";
                cout << "=========================================\n";
                cout << "Origem: " << v->getOrigem()->getNome() << "\n";
                cout << "Destino: " << v->getDestino()->getNome() << "\n";
                cout << "Veiculo: " << v->getVeiculo()->getNome() << "\n";
                cout << "Velocidade Media: " << v->getVeiculo()->getVelocidade() << " km/h\n";
                cout << "Regra de Pausa: A cada " << v->getVeiculo()->getDistEntreDescansos()
                    << " km para por " << v->getVeiculo()->getTempoDescanso() << " hora(s).\n";
                cout << "Passageiros de desembarque:\n";
                for (auto p : v->getPassageiros())
                {
                    cout << " - " << p->getNome() << "\n";
                }
                cout << "=========================================\n";

                delete v;
                it = viagensAtivas.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

void ControladorDeTransito::mostrarViagensEmAndamento() const
{
    cout << "\n--- VIAGENS EM ANDAMENTO (" << viagensAtivas.size() << ") ---\n";
    if (viagensAtivas.empty())
    {
        cout << "Nenhuma viagem sendo executada no momento.\n";
        return;
    }
    for (auto v : viagensAtivas)
    {
        v->exibirEstado();
        cout << "-----------------------------------------\n";
    }
}