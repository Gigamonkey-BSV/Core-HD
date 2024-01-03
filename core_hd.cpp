
#include <argh.h>
#include <data/crypto/random.hpp>
#include <gigamonkey/wif.hpp>
#include <gigamonkey/schema/bip_39.hpp>
#include <gigamonkey/schema/bip_44.hpp>
#include <data/net/JSON.hpp>
#include <data/io/unimplemented.hpp>

using namespace data;

void generate (const argh::parser &) {
    throw method::unimplemented {"generate words"};
}

void restore (const argh::parser &) {
    throw method::unimplemented {"generate words"};
}

void run (const argh::parser &) {
    throw method::unimplemented {"generate words"};
}

int main (int arg_count, char **arg_values) {

    try {
        run (argh::parser {arg_count, arg_values});
    } catch (const std::exception &e) {
        std::cout << "Error: " << e.what () << std::endl;
        return 1;
    } catch (...) {
        std::cout << "Unknown error" << std::endl;
        return 1;
    }

    return 0;
}
