/* 
 * File:   utils.cpp
 * Author: juanzaragoza
 * 
 * Created on 13 de septiembre de 2014, 14:54
 */

#include "utils.h"

void validateCommandLineArguments(int argc, char** argv, string &input, string &output, string &function){
    
    string aux;
    string pgmExtension = ".pgm";
    
    if(argc<=1){
        
        input = "cin";
        output = "cout";
        function = "z";
        
    } else{
        
        for(int i=1; i<argc; i++){
            
            if(strcmp(argv[i],"-i")==0){
                
                if(i+1 < argc){
                    
                    aux = argv[i+1];
                    
                    if(aux.length() >= pgmExtension.length() && aux.compare(aux.length() - pgmExtension.length(), pgmExtension.length(), pgmExtension) == 0){
                        input = aux;
                    } else{
                        input = "cin";
                    }
                    
                } else{
                    input = "cin";
                }
                
            } else if(strcmp(argv[i],"-o")==0){
                
                if(i+1 < argc){
                    
                    aux = argv[i+1];
                    
                    if(aux.length() >= pgmExtension.length() && aux.compare(aux.length() - pgmExtension.length(), pgmExtension.length(), pgmExtension) == 0){
                        output = aux;
                    } else{
                        output = "cout";
                    }
                    
                } else{
                    output = "cout";
                }
                
            } else if(strcmp(argv[i],"-f")==0){
                
                if(i+1 < argc){
                    
                    aux = argv[i+1];
                    
                    if(aux == "exp" || aux == "z"){
                        function = aux;
                    } else{
                        function = "z";
                    }
                    
                } else{
                    
                    function = "z";
                }
                
            }
            
        }
        
    }
    
    if(input == "") input = "cin";
    if(output == "") output = "cout";
    if(function == "") function = "z";
}

