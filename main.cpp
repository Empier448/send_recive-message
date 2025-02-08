#include <iostream>
#include <thread>
#include <chrono>
#include "server.h"
#include "client.h"

int main() {
    // Start the server in a separate thread
    std::thread server_thread(server_function);

    // Allow more time for the server to start
    std::this_thread::sleep_for(std::chrono::seconds(2)); // เพิ่มเวลาให้มากขึ้น

    // Run the client
    bool connected = false;
    int attempts = 5;

    while (!connected && attempts > 0) {
        try {
            std::cout << "Attempting to connect to the server..." << std::endl;
            client_function();  // Try to connect to the server
            connected = true;    // If successful, set connected to true
            std::cout << "Connection successful!" << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Connection failed: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));  // Wait before retrying
            --attempts;
            if (attempts > 0) {
                std::cout << "Retrying... (" << attempts << " attempts left)" << std::endl;
            }
        }
    }

    if (!connected) {
        std::cerr << "Unable to connect to the server after multiple attempts." << std::endl;
    }

    // Wait for the server thread to complete
    if (server_thread.joinable()) {
        server_thread.join();  // Ensure the server thread completes
    }

    return 0;
}
