#include "server.h"
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <vector>

using boost::asio::ip::tcp;

void server_function() {
    try {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(),800));

        std::cout << "Server is listening on port 65432" << std::endl;

        tcp::socket socket(io_context);
        acceptor.accept(socket);

        while (true) {
            std::vector<char> buffer(1024);  // ลดขนาด buffer ลงเป็น 1024
            boost::system::error_code error;
            size_t length = socket.read_some(boost::asio::buffer(buffer), error);

            if (error == boost::asio::error::eof) {
                std::cout << "Client disconnected" << std::endl;
                break;
            }
            else if (error) {
                throw boost::system::system_error(error);
            }

            std::string message(buffer.data(), length);
            std::cout << "Received message: " << message << std::endl;

            if (message == "exit") {
                std::cout << "Client requested to close the connection" << std::endl;
                break;
            }

            // ส่งการตอบกลับไปยังไคลเอนต์
            std::string response = "Hello from server";
            boost::asio::write(socket, boost::asio::buffer(response));
        }

        socket.shutdown(tcp::socket::shutdown_both);
        socket.close();

    }
    catch (std::exception& e) {
        std::cerr << "Exception in server: " << e.what() << std::endl;
    }
}
