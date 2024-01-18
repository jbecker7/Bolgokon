#include <fstream>
#include <iostream>
#include <set>
#include <string>

using namespace std;


set<string> getCharacters(const string& filename) {
  ifstream file(filename, ios::binary);  // Open file in binary mode to read bytes instead of characters
  set<string> characters; // Look at me, I'm using a set!

  if (file.is_open()) {
    string character;
    char c;
    while (file.get(c)) { //while there's still characters to read
      if ((c & 0x80) == 0) {  //we have a single-byte character bruh bitwise is confusing
        if (!character.empty()) {
          characters.insert(character);
          character.clear();
        }
        character += c;
      } else if ((c & 0xC0) == 0xC0) {  // Start of new multi-byte character
        if (!character.empty()) {
          characters.insert(character);
        }
        character = c;
      } else {  // Continuation byte of a multi-byte character
        character += c;
      }
    }
    if (!character.empty()) {
      characters.insert(character);
    }
  }

  return characters;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Usage: cpp_program <filename>" << endl;
    return 1;
  }
  string filename = argv[1];

  set<string> characters = getCharacters(filename);

  ofstream myfile("characters.txt");
  for (const string& character : characters) {
    myfile << character << endl;
  }
  myfile.close();

  return 0;
}
