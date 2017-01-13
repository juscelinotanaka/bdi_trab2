//
//  Hash.cpp
//  TP2
//
//  Created by Juscelino Tanaka on 12/01/17.
//  Copyright © 2017 Juscelino Tanaka. All rights reserved.
//

#include "Hash.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

FILE *arquivoHash;

// Função para inicializar o arquivo de hash vazio.
// "hash_file.txt"

Hash::Hash(string fileName) {
    this->fileName = fileName;
    
    int i;
    
    arquivoHash = fopen(fileName.c_str(), "r+b");
    
    if (arquivoHash == NULL) {
        cout << "CRIANDO HASH. ESSE PROCESSO PODE LEVAR ALGUM TEMPO." << endl;
        
        arquivoHash = fopen(fileName.c_str(), "w+b");
        
        void *bloco = NULL;
        bloco = malloc(TAMANHO_BLOCO);
        memset(bloco, 0, TAMANHO_BLOCO);
        
        for (i = 0; i < NUMERO_BUCKETS; i++) {
            // cada bucket tera n blocos de tamanho 4096
            fwrite(bloco, TAMANHO_BLOCO, BLOCOS_POR_BUCKET, arquivoHash);
        }
    }
    
    cout << "ARQUIVO HASH PRONTO." << endl;
}

void Hash::CloseHash () {
    fclose(arquivoHash);
    cout << "ARQUIVO HASH FECHADO" << endl;
}

// Função para calcular a chave do hash no qual o artigo será inserido.
int calculaChave (int id){
    return (id % (NUMERO_BUCKETS));
}

// Função para inserção dos artigos no arquivo de hash
void inserirArtigo(int chave, Artigo artigo){
    
    int i, j, achouEspacoLivre = 0;
    
//    arquivoHash = fopen("hash_file.txt", "rb+");
    
    // Ponteiro para leitura do bloco.
    void *bloco = NULL;
    bloco = malloc(TAMANHO_BLOCO);
    memset(bloco, 0, TAMANHO_BLOCO);
    
    // Vetor de artigos para armazenar os registros lidos em 1 bloco.
    Artigo artigos[REGISTROS_POR_BLOCO];
    
    // Posiciona o arquivo no bucket correspondente ao valor da chave.
    fseek(arquivoHash, chave * BLOCOS_POR_BUCKET * TAMANHO_BLOCO, SEEK_SET);
    
    // Loop para caminhar nos blocos de um bucket.
    for (i = 0; i < BLOCOS_POR_BUCKET; i++) {
        
        // Lê o primeiro bloco do bucket.
        fread(bloco, 1, TAMANHO_BLOCO, arquivoHash);
        
        // Copia os registros do bloco lido para um vetor de artigos.
        memcpy(artigos, bloco, sizeof(Artigo) * REGISTROS_POR_BLOCO);
        
        // Loop para caminhar nos registros de um bloco e achar um bloco vazio
        for (j = 0; j < REGISTROS_POR_BLOCO; j++) {
            
            if (artigos[j].GetId() == artigo.GetId()) {
                cout << "JA EXISTE UM ARTIGO COM ESTE ID: " << artigo.GetId() << endl;
                return;
            }
            //se id == 0 entao espaco ta vazio
            if (artigos[j].GetId() == 0) {
                achouEspacoLivre = 1;
                
                artigos[j] = artigo;
                memcpy(bloco, artigos, sizeof(Artigo) * REGISTROS_POR_BLOCO);
                
                fseek(arquivoHash, -TAMANHO_BLOCO, SEEK_CUR);
                fwrite(bloco, 1, TAMANHO_BLOCO, arquivoHash);
                
                break;
            }
        }
        
        // Se não achou nenhum espaço livre no bloco j, pula para o próximo bloco do bucket.
        if (achouEspacoLivre == 0) {
            fseek(arquivoHash, TAMANHO_BLOCO, SEEK_CUR);
        }
        else{
            break;
        }
    }
    
    // Se não achou nenhum espaço livre no bucket i, pula para o próximo bucket.
    if (achouEspacoLivre == 0) {
//        if (chave == aux-1) {
//            printf("Disco cheio, nao foi possivel inserir artigo!");
//        }
//        else{
//            inserirArtigo((chave+1) % NUMERO_BUCKETS, artigo);
//        }
    
        // tratar o que fazer quando o bucket esta cheio.
        cout << "NAO FOI POSSIVEL INSERIR O REGISTRO: " << artigo.GetId() << ". O BUCKET JA ESTA CHEIO." << endl;
        exit(10);
    }
}

Artigo Hash::FindRec(int id) {
    Artigo art;
    int chave = calculaChave(id);
    
    int i, j, achouArtigo = 0;
    
    // Ponteiro para leitura do bloco.
    void *bloco = NULL;
    bloco = malloc(TAMANHO_BLOCO);
    memset(bloco, 0, TAMANHO_BLOCO);
    
    // Vetor de artigos para armazenar os registros lidos em 1 bloco.
    Artigo artigos[REGISTROS_POR_BLOCO];
    
    // Posiciona o arquivo no bucket correspondente ao valor da chave.
    fseek(arquivoHash, chave * BLOCOS_POR_BUCKET * TAMANHO_BLOCO, SEEK_SET);
    
    // Loop para caminhar nos blocos de um bucket.
    for (i = 0; i < BLOCOS_POR_BUCKET; i++) {
        
        // Lê o primeiro bloco do bucket.
        fread(bloco, 1, TAMANHO_BLOCO, arquivoHash);
        
        // Copia os registros do bloco lido para um vetor de artigos.
        memcpy(artigos, bloco, sizeof(Artigo) * REGISTROS_POR_BLOCO);
        
        // Loop para caminhar nos registros de um bloco e achar um bloco vazio
        for (j = 0; j < REGISTROS_POR_BLOCO; j++) {
            
            if (artigos[j].GetId() == chave && artigos[j].GetId() != 0) {
                
                achouArtigo = 1;
                
                
                art = artigos[j];
                
                break;
            }
        }
        
        // Se não achou nenhum espaço livre no bloco j, pula para o próximo bloco do bucket.
        if (achouArtigo == 0) {
            fseek(arquivoHash, TAMANHO_BLOCO, SEEK_CUR);
        }
        else{
            break;
        }
    }
    
    // Se não achou nenhum espaço livre no bucket i, pula para o próximo bucket.
    if (achouArtigo == 0) {
        // tratar o que fazer quando o bucket esta cheio.
        
    }
    
    return art;
}

int Hash::InserirArtigo (Artigo artigo) {
    int chave = calculaChave(artigo.GetId());
    inserirArtigo(chave, artigo);
    
    return chave;
}
