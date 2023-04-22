#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <eosio/random.hpp>
#include <atomicassets.hpp>
#include <atomicdata.hpp>
#include <base58.hpp>
#include <checkformat.hpp>

using namespace eosio;

CONTRACT nft_lottery : public contract {
public:
    using contract::contract;

    ACTION start(name user, uint64_t selected_number, asset bet_amount);
    ACTION resolve(uint64_t game_id);

private:
    void transfer_handler(name from, name to, asset quantity, std::string memo);

    TABLE game {
        uint64_t game_id;
        name user1;
        uint64_t selected_number1;
        name user2;
        uint64_t selected_number2;
        asset bet_amount;
        uint64_t status; // 0 = ongoing, 1 = user1 wins, 2 = user2 wins, 3 = draw

        uint64_t primary_key() const { return game_id; }
    };

    typedef multi_index<"games"_n, game> games_index;

    // Constants
    const symbol wax_symbol = symbol("WAX", 8);
    const uint64_t min_selected_number = 1;
    const uint64_t max_selected_number = 10;
    const name fee_receiver = "dogandcatnf"_n; // Replace with the actual WAX account name
    const name contract_owner = "dogandcatnf"_n; // Replace with the actual WAX account name
    const asset fee_per_transaction = asset(30000, wax_symbol); // 0.3 WAX
};