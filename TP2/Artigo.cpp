//
//  Artigo.cpp
//  TP2
//
//  Created by Juscelino Tanaka on 16/12/16.
//  Copyright © 2016 Juscelino Tanaka. All rights reserved.
//

#include "Artigo.hpp"
#include <sstream>

Artigo::Artigo () {
    
};

Artigo::Artigo (int id, string titulo, int ano, int citacoes, string atualizacao, string autores, string snippet) {
    this->id = id;
    strcpy(this->titulo, titulo.c_str());
    this->ano = ano;
    strcpy(this->autores, autores.c_str());
    this->citacoes = citacoes;
    strcpy(this->atualizacao, atualizacao.c_str());
    strcpy(this->snippet, snippet.c_str());
}

int Artigo::GetId() {
    return this->id;
}

string Artigo::GetTitulo() {
    return this->titulo;
}

int Artigo::GetAno() {
    return this->ano;
}

vector<string> Artigo::GetAutores () {
    vector<string> temp;
    
    return split(autores, '|');
}

int Artigo::GetCitacoes() {
    return this->citacoes;
}

string Artigo::GetAtualizacao() {
    return this->atualizacao;
}

string Artigo::GetSnippet() {
    return this->snippet;
}

void Artigo::Print () {
    cout
    << "ID: " << id << endl
    << "TITULO:\n\t" << titulo << endl
    << "ANO:\n\t" << ano << endl
    << "CITACOES:\n\t" << citacoes  << endl
    << "ATUALIZACAO:\n\t" << atualizacao  << endl
//    << "RESUMO:\n\t" << snippet << endl
    ;
    cout << "AUTORES:" << endl;
    
    vector<string> vAutores = GetAutores();
    
    for (int i = 0; i < vAutores.size(); i++) {
        cout << "\t" << vAutores[i] << endl;
    }
}

vector<string> split (string str, char delimiter) {
    vector<string> result;
    stringstream ss(str);
    string token;
    
    while (getline(ss, token, delimiter)) {
        unsigned long first = 0, length;
        if (token[0] == '"')
            first = 1;
        
        if (token[token.length() - 1] == '"')
            length = token.length() - 1 - first;
        else
            length = token.length();
        
        
        if (token.length() > 0)
            token = token.substr(first, length);
        
        result.push_back(token);
    }
    
    return result;
}
