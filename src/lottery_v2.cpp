#include "lottery.hpp"

void nft_lottery::start(name user, uint64_t selected_number, asset bet_amount) {
    require_auth(user);

    // Validate bet amount
    check(bet_amount.symbol == wax_symbol, "Only WAX tokens are accepted.");
    check(bet_amount.amount > 0, "Bet amount must be greater than 0.");

    // Validate selected number
    check(selected_number >= min_selected_number && selected_number <= max_selected_number,
          "Selected number must be between 1 and 10.");

    // Store the game data
    games_index games(get_self(), get_self().value);
    games.emplace(user, [&](auto& new_game) {
        new_game.game_id = games.available_primary_key();
        new_game.user = user;
        new_game.selected_number = selected_number;
        new_game.bet_amount = bet_amount;
        new_game.status = 0; // Ongoing
    });
}

void nft_lottery::resolve(name user, uint64_t game_id) {
    require_auth(user);

    games_index games(get_self(), get_self().value);
    auto game_itr = games.find(game_id);
    check(game_itr != games.end(), "Game not found.");
    check(game_itr->status == 0, "Game is already resolved.");

    // Randomize the winning number
    eosio::random random_gen;
    uint64_t winning_number = random_gen.range(min_selected_number, max_selected_number);

    // Update game status
    games.modify(game_itr, user, [&](auto& game_data) {
        if (game_data.selected_number == winning_number) {
            // User wins
            game_data.status = 1;
            // TODO: Implement NFT reward distribution
        } else {
            // User loses
            game_data.status = 2;
        }
    });
}

void nft_lottery::transfer_handler(name from, name to, asset quantity, std::string memo) {
    // Ensure the contract is the receiver
    if (to != get_self()) {
        return;
    }

    // Parse memo for the start action
    if (memo.substr(0, 5) == "start") {
        // TODO: Parse selected number from the memo
        uint64_t selected_number = std::stoi(memo.substr(6));
        start(from, selected_number, quantity);
    }
}

extern "C" void apply(uint64_t receiver, uint64_t code, uint64_t action) {
    if (code == "eosio.token"_n.value && action == "transfer"_n.value) {
        execute_action(name(receiver), name(code), &nft_lottery::transfer_handler);
    } else if (code == receiver) {
        switch (action) {
            EOSIO_DISPATCH_HELPER(nft_lottery, (start)(resolve))
        }
    }
    eosio_exit(0);
}
