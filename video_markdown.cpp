#include <algorithm>   // we need this to use std::find and std::sort
#include <filesystem>  // this is how we get the directory contents
#include <fstream>     // this lets us write to a file
#include <iostream>    // lol not writing a comment for this one kiddo
#include <string>      // strings are a class 哈哈哈
#include <vector>      // dynamic arrays <3

using namespace std;
namespace fs = std::__fs::filesystem;

const vector<string> videoExtensions = {".mp4", ".avi", ".mov",
                                        ".mkv", ".flv", ".wmv"};

bool isVideoFile(const fs::path& path) {
  string extension = path.extension().string();
  // syntax here is find(start, end, value) and returns an iterator
  // .end() is the same as the last element + 1
  // so the != videoExtensions.end() is checking if the iterator does not exist
  return find(videoExtensions.begin(), videoExtensions.end(), extension) !=
         videoExtensions.end();
}

void collectVideoFiles(const fs::path& dirPath, ofstream& mdFile) {
  for (const auto& entry : fs::directory_iterator(dirPath)) {
    if (entry.is_directory()) {
      // This vector hols all our video file names
      // We　make a new one for each directory
      vector<string> videoFiles;

      for (const auto& fileEntry :
           // Search recursively for video files
           fs::recursive_directory_iterator(entry.path())) {
        if (fileEntry.is_regular_file() && isVideoFile(fileEntry.path())) {
          // We only want the file name without the extension
          videoFiles.push_back(fileEntry.path().filename().stem().string());
        }
      }

      if (!videoFiles.empty()) {
        // Sort the video file names alphabetically
        sort(videoFiles.begin(), videoFiles.end());
        // Add the markdown header for the directory
        mdFile << "# " << entry.path().filename().string() << endl;
        for (const auto& fileName : videoFiles) {
          // Add a nice little markdown checkbox for each video file
          mdFile << "- [ ] " << fileName << endl;
        }
      }
    }
  }
}

int main(int argc, char* argv[]) {
  // Default path is the current directory
  string path = ".";

  // Check if a path argument is provided
  if (argc > 1) {
    path = argv[1];
  }

  ofstream mdFile("/Users/jcb/Desktop/SuperApp/video_queue.md");
  if (!mdFile.is_open()) {
    cerr << "Failed to open file for writing." << endl;
    return 1;
  }

  collectVideoFiles(path, mdFile);
  mdFile.close();

  cout << "Video file names with directory headers have been written to "
          "video_queue.md"
       << endl;
  return 0;
}