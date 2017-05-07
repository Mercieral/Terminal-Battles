#include "user.hpp"

using namespace std;

int main(int argc, char **argv) {

    cout << "\e[8;30;94t";

    if (argc == 2) {
        cout << "You are a client connecting to " << argv[1] << "\n";
        Client client = Client(argv[1]);
        client.connect();
    } else {
        cout << "You are a host\n";
        Host host = Host();
        host.connect();
    }

    return 0;
}