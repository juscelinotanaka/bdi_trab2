//
//  Artigo.hpp
//  TP2
//
//  Created by Ivo Stinghen on 16/12/16.
//  Copyright © 2016 Ivo Stinghen. All rights reserved.
//

#ifndef Artigo_hpp
#define Artigo_hpp

#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

class Artigo {
    int id;
    char titulo[300];
    int ano;
    char autores[1024];
    int citacoes;
    char atualizacao[20];
    char snippet[1024];
    
public:
    Artigo ();
    Artigo (int id, string titulo, int ano, int citacoes, string atualizacao, string autores, string snippet);
    void Print ();
    int GetId();
    string GetTitulo();
    int GetAno();
    vector<string> GetAutores ();
    int GetCitacoes();
    string GetAtualizacao();
    string GetSnippet();
    
};

vector<string> split (string str, char delimiter);

#endif /* Artigo_hpp */
