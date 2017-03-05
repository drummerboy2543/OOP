/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Json_Element.h
 * Author: Andrew
 *
 * Created on March 4, 2017, 7:32 PM
 */

#ifndef JSON_ELEMENT_H
#define JSON_ELEMENT_H
#include <string>
#include <iostream>
class json_Base{
    
protected:
    enum json_Typing {Arr_Type,Obj_Type,Elem_Type};
public:
    json_Base(){;}
    virtual ~json_Base() = default;
    virtual int Weight() =0;
    json_Typing Element_Type;
};

class json_Array : public json_Base {

    json_Array() {
        Element_Type = Arr_Type;
    }
    int Weight(){;}
};


class json_Object : public json_Base {

    json_Object() {
        Element_Type = Obj_Type;
    }
    int Weight(){return 1;}
};


class json_Element : public json_Base {
public:
    json_Element() {
        Element_Type = Elem_Type;
    }
    int Weight(){return 1;}
};


class json_String : public json_Element {
   std::string Value;
public:
    json_String(std::string str):Value(str) {
        std::cout<<"The string value is "<<Value<<"\n";
    }
    ;
};

class json_Number : public json_Element {
   std::string Value;
public:
    json_Number(std::string num):Value(num) {
        std::cout<<"The int value is "<<Value<<"\n";
    }
    ;
};

class json_Bool : public json_Element {
    bool Value;
public:
    json_Bool(bool v): Value(v){
        std::cout<<"The bool value is "<<Value<<"\n";
    };
    
};

class json_Null: public json_Element {
public:
    json_Null() {
        std::cout<<"NULL!! "<<"\n";
    }
    
};



#endif /* JSON_ELEMENT_H */

