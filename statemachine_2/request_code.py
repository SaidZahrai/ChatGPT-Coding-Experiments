import os
import sys
import openai
import importlib.util
import re
import time

waiting_time = 1

#read description file and make 1st dictionary
def make_descriptions(filename):
    # Create a dictionary to store the file content in a structured way
    file_dict = {}

    # Read the lines from the file and populate the dictionary
    with open(filename, 'r') as f:
        lines = [line.strip() for line in f.readlines() if line.strip()]
        i = 0
        while i < len(lines):
            line = lines[i]
            if line.startswith('file:'):
                fname = line.split('file:')[1].strip()
                i += 1
                description = ''
                while i < len(lines) and not lines[i].startswith('file:'):
                    description += lines[i] + '\n'
                    i += 1
                file_dict[fname] = description.strip()
            else:
                i += 1
    return file_dict

# Create directories and files from the dictionary
def write_files(file_dict, directory):
    for fname, codeblock in file_dict.items():
        filepath = os.path.join(directory, fname)
        dirname = os.path.dirname(filepath)
        if not os.path.exists(dirname):
            os.makedirs(dirname)
        with open(filepath, 'w') as f:
            f.write(codeblock)

# Send message to ChatGPT and return the response
def send_message(myRequest):
    response = openai.ChatCompletion.create(
        model="gpt-3.5-turbo",
        messages=[
                {"role": "system", "content": "SW engineer"},
                {"role": "user", "content": myRequest},
        ],
        temperature=0.0
    )
    result = ''
    for choice in response.choices:
        result += choice.message.content
    return result

# Extract code from the response
def extract_code_from_markdown(markdown_string):
    code_blocks = re.findall(r'```(.+?)\n(.+?)```', markdown_string, re.DOTALL)
    code_parts = []
    for language, code in code_blocks:
        print(code)
        code_parts.append({
            'language': language.strip(),
            'code': code.strip()
        })
    return code_parts

# Create directories and files from the dictionary
def request_code(file_dict):
    generated_code = {}
    for fname, description in file_dict.items():
        print(fname, '\n', description)
        markdown = send_message(description)
        print('\n\n\n',markdown,'\n\n\n')
        generated_code[fname] = markdown #extract_code_from_markdown(markdown)
        time.sleep(waiting_time)
    return generated_code

api_key = "your-api-key"

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
print("Accessing via ", api_key)

if (len(sys.argv) < 3):
    print("python request_code.py instruction_file code_root_directory")

# Get the filename and directory from the command line arguments
filename = sys.argv[1]
directory = sys.argv[2]

description_dict = make_descriptions(filename)
code_dict = request_code(description_dict)
write_files(code_dict, directory)

