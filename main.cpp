#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> 
#include "ControladorDeTransito.h"
#include "SistemaFicticio.h"

using namespace std;

void exibirMenu() {
    cout << "\n=========================================\n";
    cout << "       SISTEMA CONTROLADOR DE VIAGENS    \n";
    cout << "=========================================\n";
    cout << "1. Cadastrar Passageiro\n";
    cout << "2. Cadastrar Transporte\n";
    cout << "3. Iniciar Nova Viagem\n";
    cout << "4. Avancar Horas no Tempo\n";
    cout << "5. Mostrar Localizacao dos Passageiros\n";
    cout << "6. Mostrar Localizacao dos Transportes\n";
    cout << "7. Mostrar Viagens em Andamento\n";
    cout << "0. Sair\n"; 
    cout << "Escolha uma opcao: ";
}

int main() {
    int escolhaSistema = 0;

    cout << "==================================================\n";
    cout << "          INICIALIZACAO DO SISTEMA\n";
    cout << "==================================================\n";
    cout << "Escolha qual modo deseja operar nesta sessao:\n";
    cout << "1. Controlador de Viagens entre Capitais (Original)\n";
    cout << "2. Controlador de Viagens de Cidades a serem Cadastradas\n";
    cout << "--------------------------------------------------\n";
    cout << "Opcao: ";
    if (!(cin >> escolhaSistema)) {
        cin.clear(); cin.ignore(10000, '\n');
        cout << "Entrada invalida. Encerrando.\n";
        return 0;
    }
    cin.ignore(10000, '\n');

    if (escolhaSistema == 1) {
        ControladorDeTransito controlador;
        int opcao = -1;
        while (opcao != 0) {
            exibirMenu();
            if (!(cin >> opcao)) { cin.clear(); cin.ignore(10000, '\n'); continue; }
            cin.ignore();
            switch (opcao) {
                case 1: {
                    string nome, cidade;
                    cout << "Nome do Passageiro: "; getline(cin, nome);
                    cout << "Cidade de Localizacao Inicial: "; getline(cin, cidade);
                    controlador.cadastrarPassageiro(nome, cidade); break;
                }
                case 2: {
                    string nome, cidade; char tipo; int cap, vel, horasDesc, tempoDesc = 0;
                    cout << "Nome do Transporte: "; getline(cin, nome);
                    cout << "Tipo (T para Terrestre / A para Aereo): "; cin >> tipo;
                    cout << "Capacidade maxima: "; cin >> cap; cout << "Velocidade Media (km/h): "; cin >> vel;
                    cout << "Horas de uso ate pausar: "; cin >> horasDesc;
                    if (horasDesc > 0) { cout << "Tempo gasto parado (horas): "; cin >> tempoDesc; }
                    cin.ignore(); cout << "Cidade Inicial: "; getline(cin, cidade);
                    controlador.cadastrarTransporte(nome, tipo, cap, vel, horasDesc, tempoDesc, cidade); break;
                }
                case 3: {
                    string t, o, d, pNome; int qtd;
                    cout << "Nome do Transporte: "; getline(cin, t);
                    Transporte* veiculo = controlador.buscarTransporte(t);
                    if (!veiculo) { cout << "Veiculo nao encontrado.\n"; break; }
                    int maxCapacidade = veiculo->getCapacidade();
                    cout << "Cidade de Origem: "; getline(cin, o);
                    cout << "Cidade de Destino: "; getline(cin, d);
                    cout << "Quantidade de passageiros: "; cin >> qtd; cin.ignore();
                    if (qtd > maxCapacidade || qtd <= 0) { exit(EXIT_FAILURE); }
                    vector<string> pNomes;
                    for (int i = 0; i < qtd; ++i) {
                        cout << "Nome do Passageiro " << i + 1 << ": "; getline(cin, pNome);
                        pNomes.push_back(pNome);
                    }
                    controlador.iniciarViagem(t, pNomes, o, d); break;
                }
                case 4: { int h; cout << "Adiantar quantas horas? "; cin >> h; cin.ignore(); controlador.avancarTempo(h); break; }
                case 5: controlador.relatarLocalizacaoPassageiros(); break;
                case 6: controlador.relatarLocalizacaoTransportes(); break;
                case 7: controlador.mostrarViagensEmAndamento(); break;
                case 0: cout << "Encerrando...\n"; break;
            }
        }
    } else if (escolhaSistema == 2) {
        executarSistemaFicticio();
    }
    return 0;
}