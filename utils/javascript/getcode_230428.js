// April 28, 2023 - Said Zahrai
//
// Attempt to access ChatGPT using node and Javascript
//

/*
There are a number of modules on internet to make access to ChatGPT simple. Here,
I have used https://github.com/transitive-bullshit/chatgpt-api

I will also ask ChatGPT to complete the program so that I can extract the code
from the complete response.

Using a new chat with 'ChatGPT Mar 23 Version'
*/

/*
First request to ChatGPT:
NodeJS ES6 - A function reading a string from a file called my_api_key.txt if it exists. 
Otherwise returing 'your-api-key.

You can choose to set your API key eother here in the code or in a file called my_api_key.txt
*/

// The output is:

import { promises as fs } from 'fs';

async function getApiKey() {
  let apiKey = 'your-api-key';
  try {
    apiKey = (await fs.readFile('my_api_key.txt', 'utf8')).trim();
    console.log('API key is read: ', apiKey)                             // Added manually
  } catch (err) {
    // If the file doesn't exist or there's an error reading it, 
    // just use the default 'your-api-key'
    console.log('Default API key is used: ', apiKey)                     // Added manually
  }
  return apiKey;
}

export default getApiKey;

/*
Second request to ChatGPT:
NodeJS ES6 - A function that takes a markdown string and extracts the code contents and puts them in an array of
json objects. The first element in object will be the language that is given in the block, if it exists, and 
the second the respective code. 

*/

// The output is:
function extractCodeBlocks(markdown) {
  const codeBlocks = [];

  const regex = /```(?:(\S+)\n)?([\s\S]+?)```/g;
  let match;
  while ((match = regex.exec(markdown)) !== null) {
    const [fullMatch, language, code] = match;
    codeBlocks.push({ language, code });
  }

  return codeBlocks;
}

// taken and modified from https://github.com/transitive-bullshit/chatgpt-api
import { ChatGPTAPI } from 'chatgpt'
let apiKey = "";
await getApiKey().then(data => apiKey = data);

async function example(message) {
  const api = new ChatGPTAPI({
    apiKey: apiKey
  });

  const res = await api.sendMessage(message);
  return res.text.trim();
}

var GPT_request = `
give me an example of state machine code in c++.
Separate in to cpp and hpp files. 
First give the cpp file and second hpp file, 
and then write a CMakeLists.txt to build the application.`

var d = "";
example(GPT_request
).then(data=> {
      d=data;
      const generatedCode = extractCodeBlocks(d);
      console.log('Generated code:', generatedCode);
    }
      );
