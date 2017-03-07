/* 
 * File:   Json_Parser.h
 * Author: Andrew
 *
 * Created on March 1, 2017, 8:11 PM
 */
#include "Json_Element.h"


#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <sstream>
#ifndef JSON_PARSER_H
#define JSON_PARSER_H

//#defines to help readability of "magic numbers"
#define Is_VALUE 1
#define Not_VALUE 0
#define Arr_Type 0
#define Obj_Type 1
#define Elem_Type 2

/*Base class of the Parser.
 * It has a vector that holds unique pointers of the base class for the json object.
 * I used unique pointers  to practice and to minimize leak of memory.
 * It has 11 functions total 9 are for parsing and 2 are for getting data out of the parsed functions.
 * Each of the functions has inputs of iterators which are passed by reference. The reason for this is to update the pointers as it goes through the string. 
 * The first function parse the json from a string input and then points to Parsing_Json_Componets until the whole input has been parsed.
 * From Parsing_Json_Componets we first ignore whitespace by using the Consume_Whitespace.
 * From there Parsing_Json_Componets will look at the first char of the string and based on the char will execute that objects parser from the 6  parse functions.
 * These will keep happening until then the whole string has been succefully parsed.
 *
 * From there we have a printing function to print the parsed json called Pretty_Print_Json.
 * We also have a Get_Json_Weight function which will return a static var of the amount of instances of a object we made.
 */
class Parser {
    std::vector<std::unique_ptr<json_Base>> Parsed_Json; //Vector that holds all the json elements
public:
    void Parse_Json(std::string Json_Input);
    void Consume_Whitespace(std::string::iterator& Start_Text, std::string::iterator& End_Text);
    std::unique_ptr<json_Base> Parsing_Json_Componets(std::string::iterator& Start_Text, std::string::iterator& End_Text, bool Is_A_Value);
    std::unique_ptr<json_Base> Parse_Object(std::string::iterator& Start_Text, std::string::iterator& End_Text);
    std::unique_ptr<json_Base> Parse_Array(std::string::iterator& Start_Text, std::string::iterator& End_Text);
    std::unique_ptr<json_Base> Parse_String(std::string::iterator& Start_Text, std::string::iterator& End_Text, bool Is_Val_Flag);
    std::unique_ptr<json_Base> Parse_Number(std::string::iterator& Start_Text, std::string::iterator& End_Text);
    std::unique_ptr<json_Base> Parse_Bool(std::string::iterator& Start_Text, std::string::iterator& End_Text);
    std::unique_ptr<json_Base> Parse_Null(std::string::iterator& Start_Text, std::string::iterator& End_Text);

    int Get_Json_Weight();
    std::string Pretty_Print_Json();
};

//This functions starts the process of parsing the json by making 2 iterator that point to the beginning and end of the string.
//The Is_Value function is to show that the input is a value and not a key/name. Which will dictate the amount of weight the json has.
//It finally pushes all of the text from the string it has to the Parsed_Json vector.

void Parser::Parse_Json(std::string Json_Input) {
    std::string::iterator Start_Text = Json_Input.begin();
    std::string::iterator End_Text = Json_Input.end();
    while (Start_Text != End_Text) {
        Parsed_Json.push_back(Parsing_Json_Componets(Start_Text, End_Text, Is_VALUE));
    }


};
// This function goes through the commmon whitespace values from ascii and just consumes it by stepping over it.
//The values include  a space, a comma, a new line, a vertical tab, a tab and so on.

void Parser::Consume_Whitespace(std::string::iterator& Start_Text, std::string::iterator& End_Text) {
    while (Start_Text != End_Text) {
        switch (*Start_Text) {
            case ' ':
            case '\t':
            case '\0':
            case '\v':
            case '\n':
            case ':':
            case '\r':
            case ',':
                Start_Text++;

                break;
            default: return;
        }
    }
}


// This function looks at the first char of the input from start_text and determines what object is next via a switch statement.
// It then goes to its respective function to parse properly.

std::unique_ptr<json_Base> Parser::Parsing_Json_Componets(std::string::iterator& Start_Text, std::string::iterator& End_Text, bool Is_A_Value) {
    Consume_Whitespace(Start_Text, End_Text);
    while (Start_Text != End_Text) {
        switch (*Start_Text) {

            case '{':
                Start_Text++;
                return Parse_Object(Start_Text, End_Text); // Curly bracket starts a json object
                break;

            case '[':
                Start_Text++;
                return Parse_Array(Start_Text, End_Text); // Square bracket starts a json array
                break;

            case '"':
                Start_Text++;
                return Parse_String(Start_Text, End_Text, Is_A_Value);
                break;
                //All numbers are fair game to be a valid number.
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
                return Parse_Number(Start_Text, End_Text);
                break;

            case 't':
                Start_Text++;
                return Parse_Bool(Start_Text, End_Text); // parse bool of true
                break;

            case 'f':
                Start_Text++;
                return Parse_Bool(Start_Text, End_Text); // parse bool of false
                break;

            case 'n':
                Start_Text++;
                return Parse_Null(Start_Text, End_Text); // 'n' starts null
                break;
                // Should never get here report error
            default: std::cout << "ERRORRRRRR Value is " << *Start_Text << "\n";
                break;
        }
    }

}


//This function creates a new object called Json_Object while creating 2 pointers to hold the name string and the value.
// From there the Parsing_Json_ Componets goes twice first for the name pointer which is not added to the weight.
// and then for the value pointer which is addede towards the weight variable.
// From there the name unique pointer is broken to a normal pointer exposing it Base_point which then can get the string which is sent to Str_val.
// After that the string and Value pointer go in a pair for the Json_object's vector. This is repeated until it sees the '}' char which completes the parse.

std::unique_ptr<json_Base> Parser::Parse_Object(std::string::iterator& Start_Text, std::string::iterator& End_Text) {
    json_Object* Obj = new json_Object();
    std::unique_ptr<json_Base> Name = nullptr;
    std::unique_ptr<json_Base> Value = nullptr;
    while (*Start_Text != '}' && Start_Text != End_Text) {
        Name = Parsing_Json_Componets(Start_Text, End_Text, Not_VALUE);
        Value = Parsing_Json_Componets(Start_Text, End_Text, Is_VALUE);
        if (Name && Value) {
            json_Base* Base_point;
            Base_point = Name.get();
            std::string str_val = Base_point->Str_Of_Val;
            Obj->Obj_vec.push_back(std::make_pair(str_val, std::move(Value)));
        }
        // if it is not equal to the '}' go to the next value and start the process again.
        if (*Start_Text != '}' && Start_Text != End_Text) {
            ++Start_Text;
        }
    }

    if (*Start_Text == '}' && Start_Text != End_Text) {
        Start_Text++; // Step over end bracket if not eof
    }
    return std::unique_ptr<json_Base>(Obj);
}

//The Parse_Array function creates a new json_Array and adds strings to the json_array vector 
//These are given by Parsing_Json_Componets from the next part of the string.
// From here it will skip the next value becuase of being a , and will conitnue until it sees the ']'.
//It then returns its combined string.

std::unique_ptr<json_Base> Parser::Parse_Array(std::string::iterator& Start_Text, std::string::iterator& End_Text) {
    json_Array* array = new json_Array();

    while (*Start_Text != ']' && Start_Text != End_Text) {
        array->Arr_vec.push_back(Parsing_Json_Componets(Start_Text, End_Text, Is_VALUE)); // Parse the next item and add it to the array

        if (Start_Text != End_Text && *Start_Text != ']')// For comma's
            ++Start_Text; // Only increment if within doc or array    
    }

    if (*Start_Text == ']' && Start_Text != End_Text) {
        ++Start_Text; // Step over end bracket if not eof
    }
    return std::unique_ptr<json_Base> (array);
}



// This makes a string called String_Val and will add characters to that string unless it sees a break character 
// which will consume the char or if it see the other " symbol.
// Once it makes the string it then creates a new json_string object and put that string object and Is_Val_Flag as a input.

std::unique_ptr<json_Base> Parser::Parse_String(std::string::iterator& Start_Text, std::string::iterator& End_Text, bool Is_Val_Flag) {
    // Parsing a string
    std::string String_Val;
    bool First_Quote = false;
    while (Start_Text != End_Text && *Start_Text != '"') {
        //If statement to skip the first quote
        if (*Start_Text == '\\') {
            Start_Text++;
            //after skip if it is the end of the file get out of loop.
            if (Start_Text == End_Text)
                break;
        }
        String_Val = String_Val + *Start_Text;
        ++Start_Text;
    }

    if (*Start_Text == '"' && Start_Text != End_Text) {
        ++Start_Text; // If the loop finished because it hit a quote, step over the end quote
    }

    json_Base* New_String_Point = new json_String(String_Val, Is_Val_Flag);
    return std::unique_ptr<json_Base>(New_String_Point);
}

// This function  makes a string called num_Val and will add numbers to that string until the number is finished. 
// after it creates the final numeric string it is then sent to a new json_Number object with the string input.
// Which will then convert the string into  a double.

std::unique_ptr<json_Base> Parser::Parse_Number(std::string::iterator& Start_Text, std::string::iterator& End_Text) {
    // Parsing a number
    std::string Num_Val;
    bool exponet_flag = false, decimal_flag = false; // Flags to make sure we are not doubling on decimal or exponets 

    while (Start_Text != End_Text && (isdigit(*Start_Text) || *Start_Text == '+' || *Start_Text == '-' || *Start_Text == '.' || *Start_Text == 'e' || *Start_Text == 'E')) {
        Num_Val = Num_Val + *Start_Text;
        ++Start_Text;
    }
    json_Base* New_Numb_Point = new json_Number(Num_Val);
    return std::unique_ptr<json_Base>(New_Numb_Point);

}
//This function checks if the following char's say true or false and if they successfully do, a new bool_object is made with that value.

std::unique_ptr<json_Base> Parser::Parse_Bool(std::string::iterator& Start_Text, std::string::iterator& End_Text) {
    //If it is false
    if (*Start_Text == 'a') {
        Start_Text++;
        if (*Start_Text == 'l') {
            Start_Text++;
            if (*Start_Text == 's') {
                Start_Text++;
                if (*Start_Text == 'e') {
                    Start_Text++;
                    json_Base* New_Bool_Point = new json_Bool(false);
                    return std::unique_ptr<json_Base>(New_Bool_Point);


                }
            }
        }
    }//If it is true
    else if (*Start_Text == 'r') {
        Start_Text++;
        if (*Start_Text == 'u') {
            Start_Text++;
            if (*Start_Text == 'e') {
                Start_Text++;
                json_Base* New_Bool_Point = new json_Bool(true);
                return std::unique_ptr<json_Base>(New_Bool_Point);
            }
        }
    } else {
        std::cout << "Bool ERROR!!!! \n";
    }
}

//This function checks if the following char's says null, if they successfully do, a new null_object is made.

std::unique_ptr<json_Base> Parser::Parse_Null(std::string::iterator& Start_Text, std::string::iterator& End_Text) {
    if (*Start_Text == 'u') {
        Start_Text++;
        if (*Start_Text == 'l') {
            Start_Text++;
            if (*Start_Text == 'l') {
                Start_Text++;
                json_Base* New_Null_Point = new json_Null();
                return std::unique_ptr<json_Base>(New_Null_Point);
            }
        }
    } else {
        std::cout << "Null ERROR!!!! \n";
    }

}

//This returns a static variable Weight_Of_Json from a base class.

int Parser::Get_Json_Weight() {
    int weight;
    json_Base* temp = new json_Base;
    weight = temp->Weight_Of_Json;
    delete temp;
    return weight;

}

//This starts the process of the pretty print for the json.
//It goes through each pointer from Parsed_Json based on a iterator and goes and 
// prints the nesscary print function based on the derived class.
//It combines all the strings up and then sends it to the output.

std::string Parser::Pretty_Print_Json() {
    std::string Final_Vector_String = "";
    std::string irr_str = "";
    std::unique_ptr<json_Base> x;

    std::vector<std::unique_ptr < json_Base>>::iterator it;
    for (it = Parsed_Json.begin(); it < Parsed_Json.end(); it++) {
        x = std::move(*it);
        json_Base* temp = x.get();
        irr_str = temp->Print();
        Final_Vector_String = Final_Vector_String + irr_str;

    }
    return Final_Vector_String;
};

#endif /* JSON_PARSER_H */

