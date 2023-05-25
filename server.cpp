#include <iostream>
#include <vector>
#include <string>
#include <grpcpp/grpcpp.h>
#include "Greet.pb.h"
#include "Greet.grpc.pb.h"

class GreeterServiceImpl final : public Greeter::Service {
public:
    grpc::Status SendNames(grpc::ServerContext* context, grpc::ServerReader<HelloRequest>* reader, HelloReply* response) override {
        names.clear();
        std::cout << "New client connected with names" << std::endl;
        HelloRequest request{};
        while (reader->Read(&request)) {
            names.push_back(request.name());
        }
        response->set_message("Ok");
        std::cout << "Client disconnected" << std::endl << std::endl;
        return grpc::Status::OK;
    }

    grpc::Status GetResult(grpc::ServerContext* context, const HelloRequest* request, grpc::ServerWriter<HelloReply>* writer) override {
        std::cout << "New client connected with result" << std::endl;
        std::string message{request->name()};
        for (const auto &name: names) {
            HelloReply reply{};
            reply.set_message(message + ", " + name + "!");
            writer->Write(reply);
        }
        std::cout << "Client disconnected" << std::endl << std::endl;
        names.clear();
        return grpc::Status::OK;
    }

private:
    std::vector<std::string> names{};
};

int main() {
    GreeterServiceImpl service{};
    grpc::ServerBuilder builder{};
    builder.AddListeningPort("127.0.0.1:50051", grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    auto server{builder.BuildAndStart()};
    std::cout << "Server listening on port: 50051" << std::endl;
    server->Wait();
    return 0;
}
