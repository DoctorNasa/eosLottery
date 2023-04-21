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
    ACTION resolve(name user, uint64_t game_id);

private:
    void transfer_handler(name from, name to, asset quantity, std::string memo);

    TABLE game {
        uint64_t game_id;
        name user;
        uint64_t selected_number;
        asset bet_amount;
        uint64_t status; // 0 = ongoing, 1 = won, 2 = lost

        uint64_t primary_key() const { return game_id; }
    };

    typedef multi_index<"games"_n, game> games_index;

    // Constants
    const symbol wax_symbol = symbol("WAX", 8);
    const uint64_t min_selected_number = 1;
    const uint64_t max_selected_number = 10;
};
