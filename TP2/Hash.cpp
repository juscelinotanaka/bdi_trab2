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

//TODO: FAZER RETORNAR O NUMERO EXATO DO BUCKET

// Função para inserção dos artigos no arquivo de hash
long inserirArtigo(int chave, Artigo artigo){
    
    int i, j, achouEspacoLivre = 0;
    
    long pos = -1L;
    
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
        
        // Lê o bloco do bucket.
        fread(bloco, 1, TAMANHO_BLOCO, arquivoHash);
        
        // Copia os registros do bloco lido para um vetor de artigos.
        memcpy(artigos, bloco, sizeof(Artigo) * REGISTROS_POR_BLOCO);
        
        // Loop para caminhar nos registros de um bloco e achar um bloco vazio
        for (j = 0; j < REGISTROS_POR_BLOCO; j++) {
            if (artigos[j].GetId() == artigo.GetId()) {
                return -2L;
            }
            else if (artigos[j].GetId() == 0) {
                //se id == 0 entao espaco ta vazio
                achouEspacoLivre = 1;
                
                artigos[j] = artigo;
                memcpy(bloco, artigos, sizeof(Artigo) * REGISTROS_POR_BLOCO);
                
                fseek(arquivoHash, -TAMANHO_BLOCO, SEEK_CUR);
                
                pos = ftell(arquivoHash);
                
                fwrite(bloco, 1, TAMANHO_BLOCO, arquivoHash);
                
                return pos;
            }
        }
        
        // Se não achou nenhum espaço livre no bloco j, pula para o próximo bloco do bucket.
        fseek(arquivoHash, TAMANHO_BLOCO, SEEK_CUR);
    }
    
    // Se não achou nenhum espaço livre no bucket i, pula para o próximo bucket.
    if (achouEspacoLivre == 0) {
        // tratar o que fazer quando o bucket esta cheio.
        cout << "NAO FOI POSSIVEL INSERIR O REGISTRO: " << artigo.GetId() << ". O BUCKET JA ESTA CHEIO." << endl;
        exit(10);
    }
    
    return pos;
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
        
        // Loop para caminhar nos registros de um bloco e achar o elemento dentro do bloco
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

Artigo Hash::FindInBlock(long block, int id) {
    Artigo art;
    
    void *bloco = NULL;
    bloco = malloc(TAMANHO_BLOCO);
    memset(bloco, 0, TAMANHO_BLOCO);
    
    // posiciona no bloco desejado
    fseek(arquivoHash, block, SEEK_SET);
    
    // Lê o primeiro bloco do bucket.
    fread(bloco, 1, TAMANHO_BLOCO, arquivoHash);
    
    // Vetor de artigos para armazenar os registros lidos em 1 bloco.
    Artigo artigos[REGISTROS_POR_BLOCO];
    
    // Copia os registros do bloco lido para um vetor de artigos.
    memcpy(artigos, bloco, sizeof(Artigo) * REGISTROS_POR_BLOCO);
    
    // Loop para caminhar nos registros de um bloco e achar um bloco vazio
    for (int j = 0; j < REGISTROS_POR_BLOCO; j++) {
        if (artigos[j].GetId() == id && artigos[j].GetId() != 0) {
            return artigos[j];
        }
    }
    
    return art;
}

long Hash::InserirArtigo (Artigo artigo) {
    int chave = calculaChave(artigo.GetId());
    
    return inserirArtigo(chave, artigo);
    
    return chave;
}
