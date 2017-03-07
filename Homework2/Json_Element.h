/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Json_Element.h
 * Author: Andrew
 *
 * Created on March 1, 2017, 7:32 PM
 */

#ifndef JSON_ELEMENT_H
#define JSON_ELEMENT_H
//Headers to call certain functions.
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include<sstream>

//Function that gives a certain amount of indentation based on the integer input
std::string indent(int i) {
    std::string indent = "";
    int count = 0;
    while (count < i) {
        indent = indent + "  ";
        count++;
    }
    return indent;

}
/*Base class of the json file.
//It has enum values that represent the type 
 * Arr_Type=0,
 * Obj_Type=1,
 * Elem_Type=2
 * it then holds 2 static variables one is two keep track how much weight there is the json file.
 * Also keeps track of how many indentations need for printing.
 * We also have a value to hold a string for printing
 * The main functions are 
 * Weight- gets weight
 * Print- A virtual function that is used to print out the values.
*/
class json_Base {
protected:

    enum json_Typing {
        Arr_Type, Obj_Type, Elem_Type
    };
public:

    json_Base() {}
    virtual ~json_Base() = default;
    std::string Str_Of_Val;
    int static Weight_Of_Json;
    int static indentnum;
//Returns the weight_OF_Json var
    int Get_Weight() {
        return Weight_Of_Json;
    };

    virtual std::string Print() {};
    json_Typing Element_Type;
};

//Initalizing static variables.
int json_Base::indentnum = 0;
int json_Base::Weight_Of_Json=0;



/*The json_Object class has a constructor and a function.
  * 
  * Constuctor- it sets the type to a object and then increments the weight by one.
  *
  * Print- The print function goes through a vector of unique_pointers that point to json_Base objects and prints the contents from each pointer. 
 *         I  used unique pointers  to practice and to minimize leak of memory.
 *         The way we printed each vector is through a iterator from the vector class.Which iterates from begin to the end of the vector.
 *         From there formating is put on to make the result "pretty"  like adding , in between arrays.
 *         After that we the activate the print function on the  json_base object  which will return a string. This can call any type from number to a array object.
 *         From there it combines to a major string and then returns the combined string.
  */

class json_Array : public json_Base {
public:
    std::vector<std::unique_ptr<json_Base>> Arr_vec;
//Select the element type, increment the weight by one.
    json_Array() {
        Element_Type = Arr_Type;
        Weight_Of_Json = Weight_Of_Json + 1;
    }

    std::string Print() {
        std::string Final_ARR_String = "";
        std::unique_ptr<json_Base> x;
        std::vector<std::unique_ptr < json_Base>>::iterator it;
        Final_ARR_String = "[ ";
        //Iterate through each vector and combine all the strings into one string and return.
        for (it = Arr_vec.begin(); it < Arr_vec.end(); it++) {
            x = std::move(*it);
            json_Base* temp = x.get();
            if (it == Arr_vec.end() || it == Arr_vec.begin()) {
                Final_ARR_String = Final_ARR_String + temp->Print();
            } else {
                Final_ARR_String = Final_ARR_String + ", " + temp->Print();
            }

        }
        Final_ARR_String = Final_ARR_String + "] ";
        return Final_ARR_String;
    }

};



/*The json_Object class has a constructor and a function.
  * 
  * Constuctor- it sets the type to a object and then increments the weight by one.
  *
  * Print- The print function goes through a vector of a pair between the string name (.first) 
 *         and then the object of the value the name is assigned to (.second).
 *         The way this is done is through a iterator from the vector class. which iterates from begin to the end of the vector.
 *         From there formating is put on to make the result "pretty"  like adding "" around the name string.
 *          Another thing in praticular to note is the incrementation of the indentnum which is used to add spaces to the string when the function indent is called.
 *         After that we the activate the print function on the  json_base value which will return a string. This happens recursivly.
 *         From there it combines to a major string and then returns the combined string.
  */
class json_Object : public json_Base {
public:
    std::vector<std::pair<std::string, std::unique_ptr<json_Base>>> Obj_vec;

    json_Object() {
        Element_Type = Obj_Type;
        Weight_Of_Json = Weight_Of_Json + 1;
    }
    std::string Print() {
        std::string name_str = "";
        std::string irr_str = "";
        std::string Final_OBJ_String = "";
        std::pair<std::string, std::unique_ptr < json_Base>> x;
        std::vector<std::pair<std::string, std::unique_ptr < json_Base>>>::iterator it;
        //Format the objects
        Final_OBJ_String = indent(indentnum) + "{ \n";
        indentnum++;
         //Iterate through each vector and combine all the strings into one string and return.
        for (it = Obj_vec.begin(); it < Obj_vec.end(); it++) {
            x = std::move(*it);
            name_str = x.first;
            name_str = indent(indentnum) + "\"" + name_str + "\": ";
            json_Base* temp = x.second.get();
            irr_str = temp->Print();
            irr_str = irr_str + "\n";
            Final_OBJ_String = Final_OBJ_String + name_str + irr_str;

        }
        indentnum--;
        Final_OBJ_String = Final_OBJ_String + indent(indentnum) + "}";
        return Final_OBJ_String;
        ;
    }
};

//Another base class for all the elements below.
//Sets all types to Elementype that inherate from this class.
//This includes the json_String class, the json_Number class, the json_Bool class, and the json_Null class
class json_Element : public json_Base {
public:

    json_Element() {
        Element_Type = Elem_Type;
    }

};

 /*The json_String class has a constructor and a function.
  * 
  * Constuctor-  it sets the Str_Of_Val string to the input str and then increments the weight counter based on if it is a value or a key input.
  * 
  * Print- returns out the string called Str_Of_Val with quotes around it for pretty printing.
  */
class json_String : public json_Element {
    std::string Value;
public:

    json_String(std::string str, bool Is_A_Value) : Value(str) {
        Str_Of_Val = Value;
        if (Is_A_Value == true) {
            Weight_Of_Json = Weight_Of_Json + 1;
        }
    }

    std::string Print() {
        return '\"' + Str_Of_Val + '\"';
    }
};


 /*The json_Number class has a constructor and a function.
  * 
  * Constuctor- the input it sets it equal to a string called str_of_val and then converts the string into a double and stores it.
  *             it also increments the weight by one.
  * Print- returns out the string called Str_Of_Val.
  */
class json_Number : public json_Element {
    double Value;
public:

    json_Number(std::string num_str) {

        std::istringstream str(num_str);
        str >> Value;
        Str_Of_Val = num_str;
        Weight_Of_Json = Weight_Of_Json + 1;
    }

    std::string Print() {
        return Str_Of_Val;
        ;
    }
};


 /*The json_Bool class has a constructor and a function.
  * 
  * Constuctor-  it sets the Str_Of_Val string to the value true or false and then increments the weight counter
  *              it also increments the weight by one.
  * Print- returns out the string called Str_Of_Val.
  */
class json_Bool : public json_Element {
    bool Value;
public:

    json_Bool(bool v) : Value(v) {
        if (v == true) {
            Str_Of_Val = "true";
        } else {
            Str_Of_Val = "false";
        }
        Weight_Of_Json = Weight_Of_Json + 1;
    };

    std::string Print() {
        return Str_Of_Val;

        ;
    }

};

 /*The json_Null class has a constructor and a function.
  * 
  * Constuctor-  it increments the weight counter
  * 
  * Print- returns a string that is spelled "Null"
  */
class json_Null : public json_Element {
public:

    json_Null() {
        Weight_Of_Json = Weight_Of_Json + 1;
    }

    std::string Print() {
        return "Null";
    }
};



#endif /* JSON_ELEMENT_H */

