#include "client.h"
#include <boost/asio.hpp>
#include <iostream>
#include <vector>
#include <string>

using boost::asio::ip::tcp;

void client_function() {
    try {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);

        // กำหนด IP และพอร์ตของเซิร์ฟเวอร์
        tcp::resolver::results_type endpoints = resolver.resolve("192.168.0.104", "800");

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        std::cout << "Connected to server." << std::endl;

        while (true) {
            std::string message;
            std::cout << "Enter message to send (type 'exit' to quit): ";
            std::getline(std::cin, message);

            if (message == "exit") {
                // แจ้งเซิร์ฟเวอร์ก่อนปิดการเชื่อมต่อ
                boost::asio::write(socket, boost::asio::buffer(message + "\n"));
                break;
            }

            // ส่งข้อความไปยังเซิร์ฟเวอร์
            boost::asio::write(socket, boost::asio::buffer(message + "\n"));

            // รับการตอบกลับจากเซิร์ฟเวอร์
            std::vector<char> buffer(4096);
            boost::system::error_code error;
            size_t length = socket.read_some(boost::asio::buffer(buffer), error);

            if (error == boost::asio::error::eof) {
                std::cout << "Connection closed by server." << std::endl;
                break;
            }
            else if (error) {
                throw boost::system::system_error(error);
            }

            std::string response(buffer.data(), length);
            std::cout << "Received response: " << response << std::endl;
        }

        // ปิดการเชื่อมต่อ
        socket.shutdown(tcp::socket::shutdown_both);
        socket.close();
        std::cout << "Connection closed." << std::endl;
    }
    catch (std::exception& e) {
        std::cerr << "Exception in client: " << e.what() << std::endl;
    }
}
