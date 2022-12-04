// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include "wordle.h"
#include "dict-eng.h"
#include <vector>
#include <list>

using namespace std;


// Add prototypes of helper functions here


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // organize the given information before running the recursion helper function
    int current =0;
    std::vector<char> yellow;
    std::map<char, std::string> green;
    std::vector<char> repeated;
    //std::cout<<" line 29: input: "<<in<<std::endl;
    for(int i = 0; i < (int)in.length() ; ++i)
    {
        if(in[i] != '-')
        {
            green[in[i]]+=(to_string(i));
            //std::cout<<"green:"<<green[in[i]]<<endl;
        }
    }

    //std::cout<<" line 41: repeated: ";
    for(char ch : floating)
    {
        yellow.push_back(ch);
        if (green.find(ch) != green.end()) //if the char is repeated more than twice
        {
            repeated.push_back(ch);
            //std::cout<<ch<<", ";
        }
        std::cout<<std::endl;
    }
    //if(!repeated.empty()){std::cout<<" line 63: repeated: "<<repeated<<std::endl;}
    //std::cout<<" line 50: yellow: "<<floating<<std::endl;
    //recursion helper function
    std::set<std::string>* candidate = new std::set<std::string>;
    static bool firstIteration = false;
    static int greenCounter = 0;
    candidate = wordle_helper(in, yellow, green, repeated, current, greenCounter, firstIteration, candidate);
    std::set<std::string>* answer = new std::set<std::string>;
    if((*candidate).empty()){return *answer;}
    for(string s : *candidate)
    {
        if(dict.find(s) != dict.end())
        {
            (*answer).insert(s);
        }
        
    }
    return *answer;
}

std::set<std::string>* wordle_helper(
    const std::string&input, 
    std::vector<char>&yellow, 
    std::map<char, std::string> green, 
    std::vector<char>& repeated,
    int current,
    int greenCounter,
    bool first, 
    std::set<std::string>* answer)
{
//1. create the word combination
    //if green is not at current
    //std::cout<<" line 64: curr: "<<current<<std::endl;
    if(input.empty() )
    {
        std::set<std::string>* empt = new std::set<std::string> {};
        return empt;
    }
    if(input[current] == '-')
    {
        //std::cout<<" line 67: processing input[curr]:"<<input[current]<<std::endl;
            //if current == 0
        if(first == false)
        {
            //std::cout<<" line 71: under if (curr ==0): "<<input<<std::endl;
            string temp = input;
            //std::cout<<" line 73: temp: "<<temp<<std::endl;
            //std::cout<<" line 74: modified temp =";
            for(int i = 97 ; i <= 122; ++i)
            {
                temp[current] = char(i);
                (*answer).insert(temp);
                //std::cout<<temp<<", ";
            }
            //std::cout<<std::endl;
            // std::cout<<"line 80: printing the rough answer set: ";
            // for(string s : *answer)
            // {
            //     std::cout<<s<<", ";
            // }
            // std::cout<<std::endl;
            first = true;
        }
            //if current != 0 
        else 
        {
            std::set<std::string>* tempAnswer = new std::set<std::string>;
            //if *answer != empty?
            for(string s: *answer)
            {
                string temp = s;
                
                for(int i = 97 ; i <= 122; ++i) //122
                {
                    temp[current] = char(i);
                    (*tempAnswer).insert(temp);
                }

            }
            answer = tempAnswer;
            // std::cout<<"line 106: rough answer set(2개 이상 알파벳이 추가되야 함.): ";
            // for(string s : *answer)
            // {
            //     std::cout<<s<<", ";
            // }
            // std::cout<<std::endl;
        }
    }
    else
    {
        ++greenCounter;
    } 

//2. checking the count of the letters in the combination
    int size = (int)input.length();
    // if(size - yellow <= current )
    // {

    // }
    if(greenCounter == size)
    {
        (*answer).insert(input);
    }

    //if the rough draft answer set is completed
    if(current == size -1)
    {
        //check if the repeated alphabets are being considered
        std::set<std::string> trashBin1;
        if(!repeated.empty())
        {
            for(char ch : repeated)
            {
                for(string s: *answer)
                {
                    int counter = 0;
                    for(char c : s)
                    {
                        if (c== ch)
                        {
                            ++counter;
                        }
                    }
                    if(counter < 2)
                    {
                        trashBin1.insert(s);
                    }
                }
            }
            // std::cout<<" line 137: this is trashBin1: ";
            for(string s : trashBin1)
            {
                (*answer).erase(s);
            //    std::cout << s<<", ";
            }
            // std::cout<<std::endl;
        }

        //check if the  floating words are all included
        //special case: yellow 에서 중복으로 나타나는 알파벳 캐치 안됨.
        //special case: yellow 에서 중복으로 나타나는 알파벳 캐치 안됨.
        std::set<std::string> trashBin2;
        if(!yellow.empty())
        {
            for(char ch : yellow)
            {
                for(string s: *answer)
                {
                    int counter = 0;
                    for(char c : s)
                    {
                        if (c== ch)
                        {
                            ++counter;
                        }
                    }
                    if(counter == 0)
                    {
                        trashBin2.insert(s);
                    }
                }
            }
            // std::cout<<" line 168: this is trashBin2: ";
            for(string s : trashBin2)
            {
                (*answer).erase(s);
            //     std::cout<<s<<", ";
            }
            // std::cout<<std::endl;
        }

        //std::cout<<"line 176: this is finalized answer: ";
        for(string s : *answer)
        {
            //std::cout<<s<<", ";
        }
        //std::cout<<std::endl;
        return answer;
    } 
//3. call the recursive function for the next curr
    else
    {
        return wordle_helper(input, yellow, green, repeated, ++current, greenCounter, first, answer); 
    }
}


// Define any helper functions here
