
#include <argh.h>
#include <data/crypto/random.hpp>
#include <gigamonkey/wif.hpp>
#include <gigamonkey/schema/bip_39.hpp>
#include <gigamonkey/schema/bip_44.hpp>
#include <data/net/JSON.hpp>
#include <data/io/unimplemented.hpp>

using namespace data;

void run (const argh::parser &);

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

void version ();
void help ();

void generate (const argh::parser &);
void restore (const argh::parser &);

struct command {
    enum name {
        unset,
        help,
        version,
        generate,
        restore
    };

    name Method;

    static name read (const argh::parser &, uint32 position = 1);
};

void run (const argh::parser & p) {

    if (p["version"]) version ();

    else if (p["help"]) help ();

    else {

        command::name cmd = command::read (p);

        switch (cmd) {
            case command::version: {
                version ();
                break;
            }

            case command::help: {
                help ();
                break;
            }

            case command::generate: {
                generate (p);
                break;
            }

            case command::restore: {
                restore (p);
                break;
            }

            default: {
                std::cout << "Error: could not read user's command." << std::endl;
                help ();
            }
        }
    }
}

template <typename X>
void inline read_option (const argh::parser &p, uint32 index, const std::string &option, maybe<X> &m) {
    X x;
    if (auto positional = p (index); positional) positional >> x;
    else if (auto opt = p (option); opt) opt >> x;
    else return;
    m = x;
}

template <typename X>
void inline read_option (const argh::parser &p, uint32 index, maybe<X> &m) {
    X x;
    if (auto positional = p (index); positional) positional >> x;
    else return;
    m = x;
}

template <typename X>
void inline read_option (const argh::parser &p, const std::string &option, maybe<X> &m) {
    X x;
    if (auto opt = p (option); opt) opt >> x;
    else return;
    m = x;
}

command::name command::read (const argh::parser &p, uint32 position) {
    maybe<std::string> m;
    read_option (p, position, m);
    if (!bool (m)) return unset;
    std::transform (m->begin (), m->end (), m->begin (),
        [] (unsigned char c) { return std::tolower (c); });
    if (*m == "help") return help;
    if (*m == "version") return version;
    if (*m == "generate") return generate;
    if (*m == "restore") return restore;
    return unset;
}

void version () {
    std::cout << "Welcome to Core HD version 0.0" << std::endl;
}

void help () {
    version ();
    std::cout <<
        "\n\tCoreHD generate                   -- create a new HD wallet." <<
        "\n\tCoreHD restore \"12 or 24 words\"   -- restore a wallet from words." << std::endl;
}

void generate (const argh::parser &) {
    throw method::unimplemented {"generate words"};
}

void restore (const argh::parser &p) {
    using namespace Gigamonkey::HD;
    std::cout << "attempting to restore wallet \n" << std::endl;

    list<string> words;
    int i = 2;
    while (true) {
        maybe<string> word;
        read_option (p, i, word);
        if (!word || word == "") break;
        words <<= *word;
        i++;
    }

    if (data::size (words) == 0) throw exception {} << "No words provided";
    std::cout << "reading words " << words << std::endl;

    std::stringstream ss;
    while (true) {
        ss << words.first ();
        words = words.rest ();
        if (data::size (words) == 0) break;
        ss << " ";
    }

    string phrase = ss.str ();

    seed x;

    if (BIP_39::valid (phrase)) {
        std::cout << "words have been read as BIP 39" << std::endl;
        x = BIP_39::read (phrase);/*
    } else if (Electrum_SV::valid (*words)) {
        std::cout << "words have been read as Electrum SV" << std::endl;
        x = Eletrum_SV::read (*words);*/
    } else {
        throw exception {} << "invalid words";
    }

    BIP_32::secret master_sk = BIP_32::secret::from_seed (x);
    BIP_32::pubkey master_pk = master_sk.to_public ();

    std::cout << "read master key: " << "\n\tsecret: " << master_sk << "\n\tpubkey: " << master_pk << "\n" << std::endl;

    std::cout << "if your wallet uses BIP 44, there is a derivation path to the true master key. " << std::endl;

    BIP_44::root bip44_root {master_sk};

    BIP_44::master_secret bitcoin = bip44_root.master (BIP_44::coin_type_Bitcoin);
    BIP_44::master_secret bitcoin_cash = bip44_root.master (BIP_44::coin_type_Bitcoin_Cash);
    BIP_44::master_secret bitcoin_sv = bip44_root.master (BIP_44::coin_type_Bitcoin_SV);

    std::cout << "bip 44 masters: " <<
        "\n\tcoin type Bitcoin:      " << bitcoin.Secret <<
        "\n\tcoin type Bitcoin Cash: " << bitcoin_cash.Secret <<
        "\n\tcoin type Bitcoin SV:   " << bitcoin_sv.Secret << std::endl;

}
