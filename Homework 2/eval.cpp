//
//  eval.cpp
//  Homework 2(q5)
//
//  Created by Jahan Cherian on 4/25/15.
//  Copyright (c) 2015 Jahan Cherian. All rights reserved.
//

#include <iostream>
#include <string>
#include <cctype>
#include <stack>
#include <cassert>

using namespace std;

//////////////////////////////-----------EXTRA FUNCTION PROTOTYPES----------///////////////////////////////

string noSpaces(string infix);  // Removes all spaces from the infix
bool isValid(string infix); // Checks if infix is valid
int precedence(char ch); // Assumes valid, and evaluates the precedence of !, & and |
string convertInfixToPostfix(string infix, string& postfix); // Assumes valid and converts from infix to postfix

//////////////////////////////-----------EVALUATE FUNCTION----------///////////////////////////////

int evaluate(string infix, string& postfix, bool& result)
{
    if (isValid(infix)) // Execute only if we have a valid infix
    {
        postfix = convertInfixToPostfix(infix, postfix);    // Convert infix to postfix and store in postfix
        if (postfix.empty())        // If after conversion we have an empty postfix, then we return 1
            return 1;
        stack<bool> operandStack;   // Create a stack of bools to take in the operands
        for (int i = 0; i < postfix.length(); i++)
        {
            char ch = postfix[i];
            if (ch == 'T')      // T is boolean for true, so push true
                operandStack.push(true);
            else if (ch == 'F')   // F is boolean for false, so push false
                operandStack.push(false);
            else if(ch == '!')
            {
                bool temp = operandStack.top(); // Store the top in a temp, pop it and push back the opposite of it
                operandStack.pop();
                operandStack.push(!temp);
            }
            else
            {
                bool operand2 = operandStack.top();
                operandStack.pop();
                if (operandStack.empty())   // Checks for closing paren, and if it exists without anything else return 1
                    return 1;
                bool operand1 = operandStack.top();
                operandStack.pop();
                if (postfix[i] == '|')  // If we encounter the | operator
                {
                    if (operand1 || operand2)   //Evaluate the top two operators with the || operator which is the same as |
                        operandStack.push(true);
                    else
                        operandStack.push(false);
                }
                if (postfix[i] == '&')  //If we encounter the & operator
                {
                    if (operand1 && operand2)   //Evaluate the top two operators with the && operator which is the same as &
                        operandStack.push(true);
                    else
                        operandStack.push(false);
                }
            }
        }
        result = operandStack.top();    // The last and only item in our stack should be our result, and we return 0
        return 0;
    }
    return 1;
}

//////////////////////////////-----------MAIN----------///////////////////////////////

int main()  //Test cases
{
    string pf;
    bool answer;
    assert(evaluate("T| F", pf, answer) == 0  &&  pf == "TF|" && answer);
    assert(evaluate("T|", pf, answer) == 1);
    assert(evaluate("F F", pf, answer) == 1);
    assert(evaluate("TF", pf, answer) == 1);
    assert(evaluate("()", pf, answer) == 1);
    assert(evaluate("T(F|T)", pf, answer) == 1);
    assert(evaluate("T(&T)", pf, answer) == 1);
    assert(evaluate("(T&(F|F)", pf, answer) == 1);
    assert(evaluate("", pf, answer) == 1);
    assert(evaluate("F  |  !F & (T&F) ", pf, answer) == 0
           &&  pf == "FF!TF&&|"  &&  !answer);
    assert(evaluate(" F  ", pf, answer) == 0 &&  pf == "F"  &&  !answer);
    assert(evaluate("((T))", pf, answer) == 0 &&  pf == "T"  &&  answer);
    assert(evaluate("T", pf, answer) == 0 && answer);
    assert(evaluate("(F)", pf, answer) == 0 && !answer);
    assert(evaluate("T&(F)", pf, answer) == 0 && !answer);
    assert(evaluate("T & !F", pf, answer) == 0 && answer);
    assert(evaluate("!(F|T)", pf, answer) == 0 && !answer);
    assert(evaluate("!F|T", pf, answer) == 0 && answer);
    assert(evaluate("T|F&F", pf, answer) == 0 && answer);
    assert(evaluate("T&!(F|T&T|F)|!!!(F&T&F)", pf, answer) == 0 && answer);
    assert(evaluate("F!", pf, answer) == 1);
    assert(evaluate("!F", pf, answer) == 0 && answer);
    assert(evaluate("dsvfwerg", pf, answer) == 1 && answer);
    cout << "Passed all tests" << endl;
}

//////////////////////////////-----------EXTRA FUNCTION IMPLEMENTATIONS----------//////////////////////////////////

string noSpaces(string infix)   // Returns the temporary infix string with no spaces and all the characters of infix
{
    string infix_nospaces = "";
    for (int i = 0; i < infix.size(); i++)
        if (infix[i] != ' ')
            infix_nospaces += infix[i];
    return infix_nospaces;
}

bool isValid(string infix)
{
    string temp_infix = noSpaces(infix);    // Make sure there are no spaces
    for (int i = 0; i < temp_infix.size(); i++) // Loop through the infix without spaces
    {
        if (temp_infix[temp_infix.size() - 1] == '|' || temp_infix[temp_infix.size() - 1] == '&')   // Special case for the ends, so as to avoid
                                                                                                    // undefined behaviour
                return false;
        if (temp_infix[i] == '|' || temp_infix[i] == '&')   // If we have |, &, we are allowed to have T F and ) before the operators
                                                            // and allowed to have T F ! and ( after the operators, else invalid
        {
            if ((temp_infix[i-1] != 'T' && temp_infix[i-1] != 'F' && temp_infix[i-1] != ')') || ((temp_infix[i+1]) !='T' && temp_infix[i+1] !='F' && temp_infix[i+1] !='!'
                && temp_infix[i+1] !='('))
                return false;
        }
        else if (temp_infix[i] == 'T' || temp_infix[i] == 'F')  // If we have T, F, then we can't have T,F or ! after the operand
        {
            if (temp_infix[i+1] == 'T' || temp_infix[i+1] == 'F' || temp_infix[i+1] == '(' || temp_infix[i+1] == '!')
                return false;
        }
    }
    return true;
}

int precedence(char ch)
{
    switch (ch)
    {
        case '!':       // ! has highest precedence
            return 2;
        case '&':       // & has higher precedence than |
            return 1;
        case '|':
            return 0;   // | has lowest precedence
        default:
            return -1;
    }
}


string convertInfixToPostfix(string infix, string& postfix)
{
    postfix = "";   // Initialize to empty string
    stack<char> operatorStack;  // Create an operator stack that takes in characters
    for (int i = 0; i < infix.size(); i++)
    {
        char ch = infix[i];
        switch (ch)
        {
            case 'T':       // If an operand (T,F) then append to postfix
            case 'F':
                postfix += ch;
                break;
            case '(':       // If open paren '(' then push onto our stack
                operatorStack.push(ch);
                break;
            case ')':
                while (operatorStack.top() != '(')  // Until we hit the matching opening paren for our closing paren, append the
                                                    // characters from the stack onto postfix
                {
                    postfix += operatorStack.top();
                    operatorStack.pop();
                }
                operatorStack.pop();    // Pop the open paren
                break;
            case '!':   // If we have an operator (!,&,|), then append to postfix, while the stack is not empty, we dont hit an open paren
                        // and our precedence of the current character is less than the precedence of whats on the stack
            case '&':
            case '|':
                while (!operatorStack.empty() && operatorStack.top() != '(' &&
                       precedence(ch) <= precedence(operatorStack.top()))
                {
                    postfix += operatorStack.top();
                    operatorStack.pop();
                }
                operatorStack.push(ch); // Push the operator on the stack
                break;
            default:
                break;
        }
    }
    while (!operatorStack.empty())  // After everything append whats left in the operator stack to postfix and return postfix
    {
        postfix += operatorStack.top();
        operatorStack.pop();
    }
    return postfix;
}

