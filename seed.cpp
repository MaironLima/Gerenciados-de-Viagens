#include "Seed.h"
#include <vector>
#include <string>

using namespace std;

void semearSistema(ControladorDeTransito& controlador) {
    // 1. Semear Cidades Base (Registradas na tabela oficial)
    controlador.cadastrarCidade("natal");
    controlador.cadastrarCidade("recife");
    controlador.cadastrarCidade("joao pessoa");
    controlador.cadastrarCidade("maceio");
    controlador.cadastrarCidade("salvador");
    controlador.cadastrarCidade("fortaleza");
    controlador.cadastrarCidade("sao paulo");
    controlador.cadastrarCidade("rio de janeiro");
    controlador.cadastrarCidade("belo horizonte");

    // 2. Semear Trajetos (Conexões permitidas de trânsito)
    // Rota Costeira Terrestre do Nordeste
    controlador.cadastrarTrajeto("fortaleza", "natal", 'T');
    controlador.cadastrarTrajeto("natal", "joao pessoa", 'T');
    controlador.cadastrarTrajeto("joao pessoa", "recife", 'T');
    controlador.cadastrarTrajeto("recife", "maceio", 'T');
    controlador.cadastrarTrajeto("maceio", "aracaju", 'T'); // Auto-cadastra aracaju se existir na tabela
    controlador.cadastrarTrajeto("aracaju", "salvador", 'T');
    
    // Rota Principal para o Sudeste
    controlador.cadastrarTrajeto("salvador", "belo horizonte", 'T');
    controlador.cadastrarTrajeto("belo horizonte", "rio de janeiro", 'T');
    controlador.cadastrarTrajeto("rio de janeiro", "sao paulo", 'T');

    // Rota Aquática de Teste (Exemplo: Recife <-> Fernando de Noronha ou Salvador de barco)
    // Como a tabela não tem Noronha, usaremos conexões litorâneas existentes para o teste aquático
    controlador.cadastrarTrajeto("natal", "recife", 'A');
    controlador.cadastrarTrajeto("recife", "salvador", 'A');

    // 3. Semear Transportes
    // Nome, Tipo (T/A), Capacidade, Velocidade (km/h), Distancia Pro Descanso, Tempo Descanso, Cidade Inicial
    controlador.cadastrarTransporte("onibus_expresso", 'T', 40, 80, 300, 2, "natal");
    controlador.cadastrarTransporte("carreta_sula", 'T', 2, 90, 400, 4, "salvador");
    controlador.cadastrarTransporte("navio_costeiro", 'A', 150, 35, 1000, 0, "natal"); // sem descanso obrigatório
    controlador.cadastrarTransporte("carro_particular", 'T', 4, 110, 200, 1, "rio de janeiro");

    // 4. Semear Passageiros
    controlador.cadastrarPassageiro("mairon", "natal");
    controlador.cadastrarPassageiro("lucas", "natal");
    controlador.cadastrarPassageiro("alice", "salvador");
    controlador.cadastrarPassageiro("pedro", "rio de janeiro");
    controlador.cadastrarPassageiro("carla", "fortaleza");
}