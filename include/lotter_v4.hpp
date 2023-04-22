
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <eosio/random.hpp>
#include <atomicassets.hpp>
#include <atomicdata.hpp>
#include <base58.hpp>
#include <checkformat.hpp>
#include <vector>

using namespace eosio;

CONTRACT nft_lottery : public contract {
public:
    using contract::contract;

    ACTION start(name user, uint64_t selected_number, asset bet_amount);
    ACTION resolve(uint64_t game_id);

private:
    TABLE game {
        uint64_t game_id;
        std::vector<name> players;
        std::vector<uint64_t> selected_numbers;
        asset bet_amount;
        uint64_t status; // 0 = ongoing, 1 = resolved
        time_point_sec start_time;
        uint8_t mode; // 0 = single-mode, 1 = 2-player mode, 2 = multiple-mode

        uint64_t primary_key() const { return game_id; }
    };

    typedef multi_index<"games"_n, game> games_index;

    // Constants
    const symbol wax_symbol = symbol("WAX", 8);
    const uint64_t min_selected_number = 1;
    const uint64_t max_selected_number = 10;
    const uint64_t num_players_single = 1;
    const uint64_t num_players_two = 2;
    const uint64_t num_players_multiple = 4;
    const uint64_t random_number_timeout = 60; // 60 seconds
    const asset fee_per_transaction = asset(30000, wax_symbol); // 0.3 WAX
    const name owner_account = "dogandcatnf"_n;
};
