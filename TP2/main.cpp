//
//  main.cpp
//  TP2
//
//  Created by Juscelino Tanaka on 14/12/16.
//  Copyright © 2016 Juscelino Tanaka. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    
    if (argc != 3) {
        cout << "Uso do programa: \t./exec funcao parametro" << endl
        << "upload <file>: \tPrograma que fará a carga inicial da massa de testes" << endl
        << "findrec <ID>: \tBusca no arquivo de dados por um registro com o ID informado" << endl
        << "seek1 <ID>: \tDevolve o registro com ID igual ao informado" << endl
        << "seek2 <Titulo>: Mostra os dados do registro que possua o Titulo igual ao informado" << endl
//        << "" << endl
        << endl;
        return 1;
    }
    
    string func = argv[1];
    
    
    
    if (func.compare("upload") == 0) {
        cout << "CARGA DE DADOS" << endl;
        
        string path = argv[2];
        
        ifstream iFile(path);        // input.txt has integers, one per line
        
        
        if (!iFile.is_open()) {
            cout << "O CAMINHO INFORMADO NAO EH INVALIDO OU O ARQUIVO NAO EXISTE" << endl << path << endl;
            exit(1);
        }
        
        
        cout << "CARREGANDO ARQUIVO: " << argv[2] << endl;
        
        string line;
        
        int count = 0;
        
        int count7 = 0;
        
        while ( getline (iFile,line) )
        {
            int currentItem = 0;
            
            // remove aspas das pontas
            if (line.length() > 1) {
                line = line.substr(1, line.length() - 2);
            } else {
                exit (2);
            }
            
            int step = 0;
            
            string delimiter = "\";";
            
            size_t pos = 0;
            string token;
            while ((pos = line.find(delimiter)) != string::npos) {
                token = line.substr(0, pos);
//                cout << step << ": " << token << endl;
                line.erase(0, pos + delimiter.length());
                
                step++;
                
            }
//            cout << step << ": " << line << endl;
            
            if (step != 6) {
                count7++;
                cout << "error 6: " << line << endl;
            }
            
//            cout << endl;
        }
        
        cout << "ttotal 7: " << count7 << endl;
        
        
        
    } else if (func.compare("findrec") == 0) {
        
    } else if (func.compare("seek1") == 0) {
        
    } else if (func.compare("seek2") == 0) {
        
    } else {
        cout << "funcao invalida" << endl;
    }
    
    return 0;
}
