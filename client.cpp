#include <iostream>
#include <string>
#include <grpcpp/grpcpp.h>
#include "Greet.pb.h"
#include "Greet.grpc.pb.h"

int main() {
    auto channel{grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials())};
    auto stub{Greeter::NewStub(channel)};
    grpc::ClientContext context0{};
    HelloReply reply{};
    auto writer{stub->SendNames(&context0, &reply)};
    std::string name{};
    do {
        std::cout << "enter name: ";
        std::getline(std::cin, name);
        if (!name.empty()) {
            HelloRequest request{};
            request.set_name(name);
            writer->Write(request);
        }

    } while (!name.empty());
    writer->WritesDone();
    auto status = writer->Finish();
    if (status.ok()) {
        std::cout << "Server response: " << reply.message() << std::endl;
    } else {
        std::cout << "error" << std::endl;
        return 1;
    }
    grpc::ClientContext context1{};
    HelloRequest request{};
    request.set_name("Hello");
    auto reader{stub->GetResult(&context1, request)};
    while (reader->Read(&reply)) {
        std::cout << reply.message() << std::endl;
    }
    status = reader->Finish();
    if (status.ok()) {
        std::cout << "Completed" << std::endl;
    } else {
        std::cout << "error" << std::endl;
        return 2;
    }
    return 0;
}
