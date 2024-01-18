#include <filesystem>
#include <iostream>

using namespace std;
namespace fs = std::__fs::filesystem;

bool isScreenshot(const string& filename) {
  return filename.rfind("Screenshot", 0) == 0;
}

void moveFileToDirectory(const fs::path& file, const fs::path& targetDir) {
  try {
    fs::rename(file, targetDir / file.filename());
    cout << "Moved: " << file.filename() << endl;
  } catch (const fs::filesystem_error& e) {
    cerr << "Failed to move " << file.filename() << ": " << e.what() << endl;
  }
}

int main(int argc, char* argv[]) {
  fs::path appPath = (argc > 1) ? argv[1] : fs::current_path();
  fs::path targetDir = appPath / "images";

  // Create target directory if it doesn't exist
  if (!fs::exists(targetDir) && !fs::create_directory(targetDir)) {
    cerr << "Failed to create directory: " << targetDir << endl;
    return 1;
  }

  cout << "Processing path: " << appPath << endl;

  for (const auto& entry : fs::directory_iterator(appPath)) {
    if (entry.is_regular_file() &&
        isScreenshot(entry.path().filename().string())) {
      moveFileToDirectory(entry.path(), targetDir);
    }
  }

  cout << "Operation completed." << endl;

  return 0;
}
