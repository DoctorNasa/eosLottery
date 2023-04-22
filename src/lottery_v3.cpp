#include "lottery_v3.hpp"

void nft_lottery::start(name user, uint64_t selected_number, asset bet_amount) {
    require_auth(user);

    // Validate bet amount
    check(bet_amount.symbol == wax_symbol, "Only WAX tokens are accepted.");
    check(bet_amount.amount > fee_per_transaction.amount, "Bet amount must be greater than the fee.");

    // Validate selected number
    check(selected_number >= min_selected_number && selected_number <= max_selected_number,
          "Selected number must be between 1 and 10.");

    // Check if the player has staked enough CPU and RAM resources
    auto user_account = get_account(user);
    check(user_account.cpu_limit.used < user_account.cpu_limit.available, "Not enough staked CPU resources.");
    check(user_account.ram_usage < user_account.ram_quota, "Not enough staked RAM resources.");

    // Store the game data
    games_index games(get_self(), get_self().value);
    auto ongoing_game = games.find(0);
    if (ongoing_game == games.end()) {
        // Create a new game
        games.emplace(user, [&](auto& new_game) {
            new_game.game_id = 0;
            new_game.user1 = user;
            new_game.selected_number1 = selected_number;
            new_game.user2 = name("");
            new_game.selected_number2 = 0;
            new_game.bet_amount = bet_amount - fee_per_transaction;
            new_game.status = 0; // Ongoing
        });
    } else {
        // Join an existing game
        games.modify(ongoing_game, user, [&](auto& game_data) {
            game_data.user2 = user;
            game_data.selected_number2 = selected_number;
        });
    }

    // Send fee to fee_receiver
    action(permission_level{get_self(), "active"_n},
           "eosio.token"_n, "transfer"_n,
           std::make_tuple(get_self(), fee_receiver, fee_per_transaction, std::string("Fee for starting a game")))
        .send();
}

void nft_lottery::resolve(uint64_t game_id) {
    require_auth(contract_owner);

    games_index games(get_self(), get_self().value);
    auto game_itr = games.find(game_id);
    check(game_itr != games.end(), "Game not found.");
    check(game_itr->status == 0, "Game is already resolved.");
    check(game_itr->user2 != name(""), "Game must have two players to resolve.");

    // Randomize the winning number
    eosio::random random_gen;
    uint64_t winning_number = random_gen.range(min_selected_number, max_selected_number);

    // Update game status
    asset winner_reward = game_itr->bet_amount * 2;
    asset loser_reward = asset(0, wax_symbol);
    name winner = name("");
    name loser = name("");
    uint64_t game_status = 3; // Draw

    if (game_itr->selected_number1 == winning_number && game_itr->selected_number2 != winning_number) {
        winner = game_itr->user1;
        loser = game_itr->user2;
        game_status = 1; // User1 wins
    } else if (game_itr->selected_number1 != winning_number && game_itr->selected_number2 == winning_number) {
        winner = game_itr->user2;
        loser = game_itr->user1;
        game_status = 2; // User2 wins
    }

    games.modify(game_itr, contract_owner, [&](auto& game_data) {
        game_data.status = game_status;
    });

    if (game_status != 3) {
        // Send WAX tokens to the winner
        action(permission_level{get_self(), "active"_n},
               "eosio.token"_n, "transfer"_n,
               std::make_tuple(get_self(), winner, winner_reward, std::string("Lottery winnings")))
            .send();

        // Send WAX tokens to the contract owner if there is a loser
        action(permission_level{get_self(), "active"_n},
               "eosio.token"_n, "transfer"_n,
               std::make_tuple(get_self(), contract_owner, loser_reward, std::string("Lottery lost bet")))
            .send();
    }
}

void nft_lottery::transfer_handler(name from, name to, asset quantity, std::string memo) {
    // Ensure the contract is the receiver
    if (to != get_self()) {
        return;
    }

    // Parse memo for the start action
    if (memo.substr(0, 5) == "start") {
        // Parse selected number from the memo
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
