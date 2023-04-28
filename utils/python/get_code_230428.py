# April 28, 2023 - Said Zahrai
# 
# Attempt to ask ChatGPT to generate code with Python
# 
# If ask ChatGPT about mmethods to access its API, it will certainly gine an answer, but
# the answer will certainly be based on old information. You can easily find up-to-date
# information on internet.
# I used this link: https://blog.devgenius.io/chatgpt-how-to-use-it-with-python-5d729ac34c0d 
# and made the below small program:

import openai

api_key = "your-api-key"

# I wanted a simple way to set my api key without needing to make changes in the file
# and allowing a possible user to just adh his/her own API key. My solution is to have
# a module
# def my_api_key():
#     return "sk-xxx" # This must be valid API key
# This should be executed only if the module exists.
# 
# I asked ChatGPT 3.5 March 23for help with:
# Python - How can I import a local file only if it exists?  
# Here is the suggestion I got, which I adapted for my use where it is noted:

import importlib.util
import sys
import os

module_name = "my_api_key"                      # The module name set to the name I wanted.
module_file = module_name + ".py"

if os.path.isfile(module_file):
    try:
        spec = importlib.util.spec_from_file_location(module_name, module_file)
        my_module = importlib.util.module_from_spec(spec)
        sys.modules[module_name] = my_module
        spec.loader.exec_module(my_module)
        api_key = my_module.my_api_key()        # Added to use the function and set my API key
    except ImportError:
        print(f"Failed to import {module_name}")
else:
    print(f"{module_name} does not exist")

openai.api_key = api_key

print(f"Accessing ChatGPT with API key {api_key}\n")

myRequest = \
'''give me an example of state machine code in c++.
   Separate in to cpp and hpp files.
   First give the cpp file and second hpp file, and then write a CMakeLists.txt to build the application.
'''

response = openai.ChatCompletion.create(
    model="gpt-3.5-turbo",
    messages=[
            {"role": "system", "content": "SW engineer"},
            {"role": "user", "content": myRequest},
        ]
)

result = ''
for choice in response.choices:
    result += choice.message.content

# Next, I need to write a function to separate the code from the explanations. I asked ChatGPT for help.
# Here is my request to 'ChatGPT Mar 23 Version':
# write a python function that takes a markdown string and extracts the code contents and puts them in an array of
# dictionary objects. The first element in the dictionary will be the language that is given in the block and 
# second the respective code. 

# And here is the generated code: 

import re

def extract_code_from_markdown(markdown_string):
    code_blocks = re.findall(r'```(.+?)\n(.+?)```', markdown_string, re.DOTALL)
    code_parts = []
    for language, code in code_blocks:
        code_parts.append({
            'language': language.strip(),
            'code': code.strip()
        })
    return code_parts

# Added myself to check the correctness
codeArray = extract_code_from_markdown(result)

for ca in codeArray:
    print('-------------------------------------------------------\n')
    print('Code snippet in ', ca['language'],':\n')
    print('-------------------------------------------------------\n')
    print(ca['code'],':\n\n')

# And here is the output of the program

#API key will be: sk-xxx
# -------------------------------------------------------

# Code snippet in  cpp :

# -------------------------------------------------------

# #ifndef STATEMACHINE_HPP
# #define STATEMACHINE_HPP

# class StateMachine {
# public:
#     StateMachine();
#     void processEvent(int event);

# private:
#     enum State {
#         STATE_IDLE,
#         STATE_READY,
#         STATE_RUNNING
#     };

#     State currentState;
#     static const int TRANSITION_TABLE[][3];
# };

# #endif //STATEMACHINE_HPP :


# -------------------------------------------------------

# Code snippet in  cpp :

# -------------------------------------------------------

# #include "StateMachine.hpp"
# #include <iostream>

# const int StateMachine::TRANSITION_TABLE[][3] = {
#     {STATE_READY, STATE_IDLE, STATE_RUNNING},
#     {STATE_RUNNING, STATE_READY, STATE_IDLE},
#     {-1, -1, -1}
# };

# StateMachine::StateMachine() {
#     currentState = STATE_IDLE;
# }

# void StateMachine::processEvent(int event) {
#     int row = currentState;
#     int col = event - 1;
#     int nextState = TRANSITION_TABLE[row][col];

#     if (nextState == -1) {
#         std::cerr << "Invalid event: " << event << " in state: " << currentState << std::endl;
#         return;
#     }

#     std::cout << "Event: " << event << " in state: " << currentState << " => next state: " << nextState << std::endl;
#     currentState = static_cast<State>(nextState);
# } :


# -------------------------------------------------------

# Code snippet in  cmake :

# -------------------------------------------------------

# cmake_minimum_required(VERSION 3.0)

# project(StateMachine)

# set(CMAKE_CXX_STANDARD 11)

# file(GLOB_RECURSE SOURCES "src/*.cpp")

# add_executable(StateMachine ${SOURCES})

# target_include_directories(StateMachine PRIVATE include) :


# -------------------------------------------------------

# Code snippet in  . :

# -------------------------------------------------------

# |-- include
# |   `-- StateMachine.hpp
# |-- src
# |   `-- StateMachine.cpp
# `-- CMakeLists.txt :


# -------------------------------------------------------

# Code snippet in  mkdir build :

# -------------------------------------------------------

# cd build
# cmake ..
# make
# ./StateMachine :