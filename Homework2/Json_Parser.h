/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Json_Parser.h
 * Author: Andrew
 *
 * Created on March 4, 2017, 8:11 PM
 */
#include "Json_Element.h"
#include <vector>
#include <memory>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <sstream>
#ifndef JSON_PARSER_H
#define JSON_PARSER_H

class Parser {
    //std::vector<std::unique_ptr<Value>> items; // Vector to accumulate items during parsing
    std::vector<std::unique_ptr<json_Base>> Parsed_Json;
    // std::vector<json_Base*> Parsed_Json;
public:
    void Parse_Json(std::string Json_Input);
    void Print_Json();

     std::unique_ptr<json_Base> Parse_Object(std::string::iterator& Start_Text, std::string::iterator& End_Text);
  std::unique_ptr<json_Base> Parse_Array(std::string::iterator& Start_Text, std::string::iterator& End_Text);
    std::unique_ptr<json_Base> Parse_String(std::string::iterator& Start_Text, std::string::iterator& End_Text);
    std::unique_ptr<json_Base> Parse_Number(std::string::iterator& Start_Text, std::string::iterator& End_Text);
    std::unique_ptr<json_Base> Parse_Bool(std::string::iterator& Start_Text, std::string::iterator& End_Text);
    //json_Base* Parse_Bool(std::string::iterator& Start_Text,std::string::iterator& End_Text);
    std::unique_ptr<json_Base> Parse_Null(std::string::iterator& Start_Text, std::string::iterator& End_Text);
};

void Parser::Parse_Json(std::string Json_Input) {
    std::string::iterator Start_Text = Json_Input.begin();
    std::string::iterator End_Text = Json_Input.end();
    int count = 1, supercount = 1;
    char json_Char_Val;
    while (Start_Text != End_Text) {
        json_Char_Val = *Start_Text;
        switch (json_Char_Val) {
            case '{':
                //return Parse_Object(f, l);       // Curly bracket starts object
                break;

            case '[':
                //return Parse_Array(f, l);        // Square bracket starts array
                break;

            case '"':
                Parsed_Json.push_back(Parse_String(Start_Text, End_Text)); 
                //return Parse_String(f, l);       // Quote starts string
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':     
            case '-':
                  Parsed_Json.push_back(Parse_Number(Start_Text, End_Text)); 
                // return Parse_Number(f, l);       // Digits and minus start number
                  count++;
                break;

            case 't':
                Parsed_Json.push_back(Parse_Bool(Start_Text, End_Text)); // 't' starts true
                // std::cout<<count<<"\n";
                count++;
                break;

            case 'f':
                Parsed_Json.push_back(Parse_Bool(Start_Text, End_Text)); // 'f' starts false
                // std::cout<<count<<"\n";
                count++;
                break;

            case 'n': Parsed_Json.push_back(Parse_Null(Start_Text, End_Text));         // 'n' starts null
            count++;
                break;
        }
        supercount++;
        Start_Text++;

        //Parsed_Json.push_back(Start_Text);

    }
    // std::cout<<"Total count is "<<supercount<<"\n";

};


//This may not take in to account "" in a string 
std::unique_ptr<json_Base> Parser::Parse_String(std::string::iterator& Start_Text, std::string::iterator& End_Text) {
  // Parsing a string
    std::string String_Val;
  bool First_Quote = false;
  Start_Text++;
  while(Start_Text != End_Text && *Start_Text != '"') {
      //If statement to skip the first quote
     
      
    if(*Start_Text == '\\') {
      Start_Text++; // Handle escape character (immediately consume next character, not handling specials)
     //after skip if it is the end of the file get out of loop.
      if(Start_Text == End_Text)
	break;
    }
      String_Val=String_Val+*Start_Text;
    ++Start_Text;
  }

  if(*Start_Text == '"' && Start_Text != End_Text)
  {
    ++Start_Text; // If the loop finished because it hit a quote, step over the end quote
  }
  
json_Base* New_String_Point = new json_String(String_Val);
 return std::unique_ptr<json_Base>(New_String_Point);
}




std::unique_ptr<json_Base> Parser::Parse_Number(std::string::iterator& Start_Text, std::string::iterator& End_Text) {
  // Parsing a number
    std::string Num_Val;
  bool exponet_flag = false, decimal_flag = false; // Flags to make sure we are not doubling on decimal or exponets 
  
  while(Start_Text!=End_Text && (isdigit(*Start_Text)||*Start_Text == '+'||*Start_Text == '-'||*Start_Text == '.'||*Start_Text == 'e'||*Start_Text == 'E')) 
  {
    if((exponet_flag && (*Start_Text == 'e' || *Start_Text == 'E')) || (decimal_flag && *Start_Text == '.')){
        std::cout<<"Numerical Error \n";
    }

    if(*Start_Text == 'e' || *Start_Text == 'E')
      exponet_flag = true;

    if(*Start_Text == '.')
      decimal_flag = true;
Num_Val=Num_Val+*Start_Text;
    ++Start_Text;
  }
  
  
json_Base* New_Numb_Point = new json_Number(Num_Val);
 return std::unique_ptr<json_Base>(New_Numb_Point);
  
}


std::unique_ptr<json_Base> Parser::Parse_Bool(std::string::iterator& Start_Text, std::string::iterator& End_Text) {
    //If it is false
    if (*Start_Text = 'a') {
        Start_Text++;
        if (*Start_Text = 'l') {
            Start_Text++;
            if (*Start_Text = 's') {
                Start_Text++;
                if (*Start_Text = 'e') {
                    Start_Text++;
                    json_Base* New_Bool_Point = new json_Bool(false);
                    return std::unique_ptr<json_Base>(New_Bool_Point);


                }
            }
        }
    }        //If it is true
    else if (*Start_Text = 'r') {
        Start_Text++;
        if (*Start_Text = 'u') {
            Start_Text++;
            if (*Start_Text = 'e') {
                Start_Text++;
                json_Base* New_Bool_Point = new json_Bool(true);
                return std::unique_ptr<json_Base>(New_Bool_Point);


            }
        }
    } else {
        std::cout << "Bool ERROR!!!! \n";
    }
}

std::unique_ptr<json_Base> Parser::Parse_Null(std::string::iterator& Start_Text, std::string::iterator& End_Text) {
    //If it is false
    if (*Start_Text = 'u') {
        Start_Text++;
        if (*Start_Text = 'l') {
            Start_Text++;
            if (*Start_Text = 'l') {
                Start_Text++;
                json_Base* New_Null_Point = new json_Null();
                return std::unique_ptr<json_Base>(New_Null_Point);



            }
        }
    } else {
        std::cout << "Null ERROR!!!! \n";
    }

}

void Parser::Print_Json() {
};
#endif /* JSON_PARSER_H */

