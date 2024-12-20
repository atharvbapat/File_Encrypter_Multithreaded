// #include <iostream>
// #include <filesystem>
// #include "./process/ProcessManager.hpp"
// #include "./process/Task.hpp"
// #include "./Input/IO.hpp"

// namespace fs = std::filesystem;

// int main(int argc, char* argv[]) {
//     std::string directory;
//     std::string action;

//     std::cout << "Enter the directory path: ";
//     std::getline(std::cin, directory);

//     std::cout << "Enter the action (encrypt/decrypt): ";
//     std::getline(std::cin, action);

//     try {
//         if (fs::exists(directory) && fs::is_directory(directory)) {
//             ProcessManagement processManagement;

//             for (const auto& entry : fs::recursive_directory_iterator(directory)) {
//                 if (entry.is_regular_file()) {
//                     std::string filePath = entry.path().string();
//                     IO io(filePath);
//                     std::fstream f_stream = io.getFileStream(); // Removed std::move

//                     if (f_stream.is_open()) {
//                         Action taskAction = (action == "encrypt") ? Action::ENCRYPT : Action::DECRYPT;
//                         auto task = std::make_unique<Task>(std::move(f_stream), taskAction, filePath);
//                         processManagement.submitToQueue(std::move(task));
//                     } else {
//                         std::cout << "Unable to open file: " << filePath << std::endl;
//                     }
//                 }
//             }

//             processManagement.executeTasks();
//         } else {
//             std::cout << "Invalid directory path!" << std::endl;
//         }
//     } catch (const fs::filesystem_error& ex) {
//         std::cout << "Filesystem error: " << ex.what() << std::endl;
//     }

//     return 0;
// }





#include <iostream>
#include <filesystem>
#include <iomanip>
#include <ctime>
#include "./process/ProcessManager.hpp"
#include "./process/Task.hpp"
#include "./Input/IO.hpp"

namespace fs = std::filesystem;

std::string getCurrentTimeString() {
    // Get current time as time_t
    std::time_t now = std::time(nullptr);
    // Convert to tm structure
    std::tm* tm_now = std::localtime(&now);
    // Format time as a string
    std::ostringstream timeStream;
    timeStream << std::put_time(tm_now, "%Y-%m-%d %H:%M:%S");
    return timeStream.str();
}

int main(int argc, char* argv[]) {
    std::string directory;
    std::string action;

    std::cout << "Enter the directory path: ";
    std::getline(std::cin, directory);

    std::cout << "Enter the action (encrypt/decrypt): ";
    std::getline(std::cin, action);

    try {
        if (fs::exists(directory) && fs::is_directory(directory)) {
            ProcessManagement processManagement;

            for (const auto& entry : fs::recursive_directory_iterator(directory)) {
                if (entry.is_regular_file()) {
                    std::string filePath = entry.path().string();
                    IO io(filePath);
                    std::fstream f_stream = io.getFileStream(); // Removed std::move

                    if (f_stream.is_open()) {
                        Action taskAction = (action == "encrypt") ? Action::ENCRYPT : Action::DECRYPT;

                        // Get the current time and format it
                        std::string currentTime = getCurrentTimeString();

                        // Log the current time, action, and file path
                        std::cout << currentTime << " - " << action << " action started on file: " << filePath << std::endl;

                        // Create and submit the task
                        auto task = std::make_unique<Task>(std::move(f_stream), taskAction, filePath);
                        processManagement.submitToQueue(std::move(task));
                    } else {
                        std::cout << "Unable to open file: " << filePath << std::endl;
                    }
                }
            }

            processManagement.executeTasks();
        } else {
            std::cout << "Invalid directory path!" << std::endl;
        }
    } catch (const fs::filesystem_error& ex) {
        std::cout << "Filesystem error: " << ex.what() << std::endl;
    }

    return 0;
}
