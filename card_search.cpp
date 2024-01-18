#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include "json.hpp"

using json = nlohmann::json;
using namespace std;

// We take in a vector of card names from the JSON file and the user's input
vector<string> autocomplete(const vector<string>& cardNames,
                            const string& input) {
  vector<string> matches;
  if (input.empty()) {
    return matches;  // No input provided, no matches, obviously
  }
  for (const auto& name : cardNames) {
    // npos means "no position" so the input is nowhere in the name
    if (name.find(input) != std::string::npos) {
      matches.push_back(name);
    }
  }
  return matches;
}

void printCardDetails(const json& j, const string& cardName) {
  for (const auto& item : j["data"]) {
    if (item.contains("name") && item["name"].get<string>() == cardName) {
      cout << "Card Details:\n";
      cout << "Name: " << item["name"] << "\n";
      cout << "Type: " << item["type"] << "\n";
      cout << "Frame Type: " << item["frameType"] << "\n";
      cout << "Description: " << item["desc"] << "\n";
      cout << "ATK: " << item["atk"] << "\n";
      cout << "DEF: " << item["def"] << "\n";
      cout << "Level: " << item["level"] << "\n";
      cout << "Race: " << item["race"] << "\n";
      cout << "Attribute: " << item["attribute"] << "\n";

      // Extract and print the image URL so the GUI can grab it
      if (item.contains("card_images") && item["card_images"].is_array()) {
        auto& images = item["card_images"];
        if (!images.empty() && images[0].contains("image_url")) {
          cout << "Image URL: " << images[0]["image_url"].get<string>() << "\n";
        }
      }

      break;  // Once found, no need to continue the loop (since we found it)
    }
  }
}

int main(int argc, char* argv[]) {
  ifstream inFile("cards.json");
  json j;
  inFile >> j;

  vector<string> cardNames;
  if (j.contains("data") && j["data"].is_array()) {
    for (const auto& item : j["data"]) {
      if (item.contains("name")) {
        cardNames.push_back(item["name"].get<string>());
      }
    }
  }

  if (argc > 1) {
    string searchTerm = argv[1];
    vector<string> suggestions = autocomplete(cardNames, searchTerm);

    if (!suggestions.empty()) {
      // Check if the search term exactly matches a card name
      // Type inference because I'm a Gen Z baby who started with Python :(
      auto it = find(cardNames.begin(), cardNames.end(), searchTerm);
      if (it != cardNames.end()) {
        // If an exact match is found, print card details
        printCardDetails(j, searchTerm);
      } else {
        // Otherwise, print the list of suggestions so the user can choose
        for (const auto& suggestion : suggestions) {
          cout << suggestion << endl;
        }
      }
    } else {
      cout << "No suggestions found." << endl;
    }
  } else {
    cout << "No search term provided." << endl;
  }

  return 0;
}
