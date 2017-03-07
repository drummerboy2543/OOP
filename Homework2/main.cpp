/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Andrew
 *
 * Created on March 1, 2017, 3:37 PM
 */
#include "Json_Operations.h"

/*
 * 
 */

// This main function waits for a input of a json file and will parse the json file
int main(int argc, char** argv) {
    Parser json_Parser;
 std::string input;  //This is a temp input to put in when reading each line to create the final input for the parser
 std::string final_input; //This is the final input for the parser
 std::string final_output; //This is the final output with pretty printing that is cout.
 
 //Combines the whole file into a big string.
        while (std::getline(std::cin,input, '\n')){
            if (input==""){
                std::cout<<"Error need a file \n";
            }
            else{
        final_input=final_input+input;}
        }
        
        json_Parser.Parse_Json(final_input);
        int Weight_Of_Json;
        //Give weight of json file
        Weight_Of_Json=json_Parser.Get_Json_Weight();
        std::cout<<"The weight of the json you inputed is "<<Weight_Of_Json<<" \n";
        //Printing json in pretty printer.
       final_output= json_Parser.Pretty_Print_Json();
       std::cout<<"\nThe final output is: \n "<<final_output;
    
    return 0;
}

