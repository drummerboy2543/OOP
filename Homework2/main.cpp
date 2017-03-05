/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Andrew
 *
 * Created on March 3, 2017, 3:37 PM
 */
#include "Json_Parser.h"

/*
 * 
 */
int main(int argc, char** argv) {
    
    Parser x;
 std::ifstream inputfile;
 std::string input;
    inputfile.open("front.json");
    if (inputfile.is_open()) {
        std::getline(inputfile,input, '\n');
        
        x.Parse_Json(input);
        std::cout<<"Its here the string is: ";
      //  std::cout<<input;
    }
    return 0;
}

