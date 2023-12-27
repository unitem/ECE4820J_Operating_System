# lab 6

1. Plugin Architecture: add a method like "void openFile(const std::string& filePath) override { ... }" and "void processFile() override { ... }" to the interface.
2. Refactor: Add methods like "bool canHandleFileType(const char *fileType)" and those in plugin interface as virtual methods to handle different file types. Then move the text files processing function to a new struct called "TextFileHandler". 
3. First, it can scan the directory to load plugin files. Second, it can initialize the plugins and call the plugin functions. Third, it can unload the plugins. Some structures are shown below:
```C++
// Plugin Interface
class FilePluginInterface {
public:
    virtual ~FilePluginInterface() {}
    virtual bool canHandleFileType(const std::string& fileType) = 0;
    virtual void openFile(const std::string& filePath) = 0;
    virtual void processFile() = 0;
};

// Text Plugin
class TextFilePlugin : public FilePluginInterface {
public:
    bool canHandleFileType(const std::string& fileType) override {
        return fileType == "txt";
    }

    void openFile(const std::string& filePath) override {
        // Open and read the text file
    }

    void processFile() override {
        // Process the file content
    }
};

// Main Application
class Application {
private:
    std::vector<std::unique_ptr<FilePluginInterface>> plugins;

public:
    void loadPlugins() {
        // Load plugins (e.g., TextFilePlugin)
        plugins.push_back(std::make_unique<TextFilePlugin>());
    }

    void openAndProcessFile(const std::string& filePath) {
        std::string fileType = determineFileType(filePath); // Implement this

        for (auto& plugin : plugins) {
            if (plugin->canHandleFileType(fileType)) {
                plugin->openFile(filePath);
                plugin->processFile();
                return;
            }
        }
        // Handle the case where no plugin can handle the file type
    }
};
```

4. CSV File plugin skeleton:
```C++
#include <string>
#include <fstream>
#include <iostream>
// Additional includes may be required for CSV parsing

class CSVFilePlugin : public FilePluginInterface {
public:
    // Check if the file type is CSV
    bool canHandleFileType(const std::string& fileType) override {
        return fileType == "csv";
    }

    // Open the CSV file
    void openFile(const std::string& filePath) override {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filePath << std::endl;
            return;
        }

        // Read the content here

        file.close();
    }

    // Process the CSV file
    void processFile() override {
        // handling each type of data in each column
    }
};

```
