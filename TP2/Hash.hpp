//
//  Hash.hpp
//  TP2
//
//  Created by Juscelino Tanaka on 12/01/17.
//  Copyright © 2017 Juscelino Tanaka. All rights reserved.
//

#ifndef Hash_hpp
#define Hash_hpp

#include <stdio.h>
#include "Artigo.hpp"

#define M 2
#define MM 4
#define NIL (-1L)
#define T 7

#define NUMERO_BUCKETS 3000
#define BLOCOS_POR_BUCKET 100
#define TAMANHO_BLOCO 4096
#define REGISTROS_POR_BLOCO 1

class Hash {
private:
    string fileName;
public:
    Hash(string fileName);
    void CloseHash ();
    long InserirArtigo (Artigo artigo);
    Artigo FindRec(int id);
    Artigo FindInBlock(long bloco, int id);
};

#endif /* Hash_hpp */
