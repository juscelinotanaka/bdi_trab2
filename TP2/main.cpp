//
//  main.cpp
//  TP2
//
//  Created by Juscelino Tanaka & Ivo Stinghen on 14/12/16.
//  Copyright © 2016 Juscelino Tanaka & Ivo Stinghen. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include "Artigo.hpp"
#include "Hash.hpp"
#include <limits>

using namespace std;

Hash h("hash_file.txt");


void WaitForEnter () {
    cout << endl << "PRESSIONE ENTER PARA CONTINUAR...";
    cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
    cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
}

void ExecFunction (string func, string meta) {
    if (func.compare("upload") == 0) {
        cout << "CARGA DE DADOS" << endl;
        
        string path = meta;
        
        ifstream iFile(path);        // input.txt has integers, one per line
        
        if (!iFile.is_open()) {
            cout << "O CAMINHO INFORMADO NAO EH INVALIDO OU O ARQUIVO NAO EXISTE" << endl << path << endl;
            exit(1);
        }
        
        cout << "CARREGANDO ARQUIVO: " << path << endl;
        
        string line;
        
        int count = 0;
        
        int cLess = 0, cMore = 0, repetido = 0;
        
        while ( getline (iFile,line) )
        {
            
            vector<string> parts = split(line, ';');
            
            if (parts.size() < 7) {
                cLess++;
            }
            else if (parts.size() > 7) {
                cMore++;
            } else {
                count++;
                
                int id = stoi(parts[0]);
                string titulo = parts[1];
                int ano = stoi(parts[2]);
                string autores = parts[3];
                int citacoes = stoi(parts[4]);
                string atualizacao = parts[5];
                string snippet = parts[6];
                
                Artigo art(id, titulo, ano, citacoes, atualizacao, autores, snippet);
                
                long bloco = h.InserirArtigo(art);
                
                //TODO: o par <id, bloco> deve ser inserido na b-tree.
                //TODO: No caso do seek2 seria o par <titulo, bloco>.
                
                // A busca será feita no Hash pelo Bloco + chave conforme exemplo mais embaixo.
                
                //USADO PARA DEBUG E TESTAR A BUSCA POR BLOCO - REMOVER DEPOIS
                if (id == 1 || id == 1200)
                    cout << "bloco de " << id << " : " << bloco << endl << endl;
                
                if (bloco == -2L) {
                    cout << "JA EXISTE UM ARTIGO COM ESTE ID: " << art.GetId() << endl;
                    count--;
                    repetido++;
                }
            }
        }
        
        cout << "Mais: " << cMore << endl
        << "Menos: " << cLess << endl
        << "Inseridos: " << count << endl
        << "Repetidos: " << repetido << endl;
        
    } else if (func.compare("findrec") == 0) {
        int id = atoi(meta.c_str());
        
        Artigo art = h.FindRec(id);
        if (art.GetId() == id) {
            cout << "ARTIGO ENCONTRADO: " << endl;
            art.Print();
        } else {
            cout << "NAO FOI POSSIVEL ENCONTRAR O REGISTRO: " << id << "." << endl;
        }
    } else if (func.compare("seek1") == 0) {
        Artigo art;
        int id = atoi(meta.c_str());
        
        // SUBSTITUIR A PARTIR DAQUI
        //TODO: buscar na B-tree o id informado e pegar de la o numero do bloco (que foi salvo na b-tree durante a insercao)
        long hardCodedBlock = 1;
        switch (id) {
            case 1:
                hardCodedBlock = 409600;
                break;
            case 1200:
                hardCodedBlock = 491520000;
                break;
            default:
                break;
        }
        
        // DEVE SER SUBSTITUIDO ATE AQUI - e trocar o hardcodedblock a seguir pelo valor do bloco que a b-tree retornou
        
        art = h.FindInBlock(hardCodedBlock, id); // bloco do 1200
        
        if (art.GetId() == id) {
            art.Print();
        } else {
            // isso nunca deve ser exibido para o usuário
            cout << "O REGISTRO NAO FOI ENCONTRADO OU NAO EXISTE NO BLOCO INDICADO" << endl;
        }
    } else if (func.compare("seek2") == 0) {
        
    } else {
        cout << "funcao invalida" << endl;
    }
    
    WaitForEnter();

}

void LoopMenu () {
    int opcao;
    string func, meta;
    
    do {
        cout << endl
        << "-----------------------" << endl
        << "MENU PRINCIPAL" << endl
        << "-----------------------" << endl
        << "1 - UPLOAD" << endl
        << "2 - FINDREC" << endl
        << "3 - SEEK 1" << endl
        << "4 - SEEK 2" << endl
        << "-----------------------" << endl
        << "0 - SAIR" << endl
        << "-----------------------" << endl
        << "OPCAO: "
        ;
        
        cin >> opcao;
        switch (opcao) {
            case 1:
                func = "upload";
                cout << "PATH DO ARQUIVO: ";
                break;
            case 2:
                func = "findrec";
                cout << "ID DO ARTIGO: ";
                break;
            case 3:
                func = "seek1";
                cout << "ID DO ARTIGO (BUSCA POR INDEX): ";
                break;
            case 4:
                func = "seek2";
                cout << "TITULO DO ARTIGO: ";
                break;
            case 0:
                continue;
                break;
                
            default:
                cout << "OPCAO INVALIDA." << endl;
                usleep(1 * 1000 * 1000);
                continue;
                break;
        }
        
        cin >> meta;
        
        ExecFunction(func, meta);
    } while (opcao != 0);
}

int main(int argc, const char * argv[]) {
    if (argc == 3) {
        ExecFunction(argv[1], argv[2]);
    } else {
        
        cout << "Uso do programa:" << endl
        << "./exec funcao parametro" << endl
        << "upload <file>: \tPrograma que fará a carga inicial da massa de testes" << endl
        << "findrec <ID>: \tBusca no arquivo de dados por um registro com o ID informado" << endl
        << "seek1 <ID>: \tDevolve o registro com ID igual ao informado" << endl
        << "seek2 <Titulo>: Mostra os dados do registro que possua o Titulo igual ao informado" << endl
        << endl;
        
        LoopMenu();
    }
    
    return 0;
}
