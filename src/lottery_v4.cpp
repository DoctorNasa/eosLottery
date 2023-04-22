#include "lottery_v4.hpp"

void nft_lottery::start(name user, uint64_t selected_number, asset bet_amount, uint8_t mode) {
    require_auth(user);

    // Validate bet amount
    check(bet_amount.symbol == wax_symbol, "Only WAX tokens are accepted.");
    
    // Validate selected number
    check(selected_number >= min_selected_number && selected_number <= max_selected_number,
          "Selected number must be between 1 and 10.");

    // Validate game mode
    check(mode <= 2, "Invalid game mode. 0 = single-mode, 1 = 2-player mode, 2 = multiple-mode");

    // Validate resources (CPU, RAM)
    auto user_resources = get_account(user);
    check(user_resources.cpu_weight.amount > 0 && user_resources.net_weight.amount > 0, "User must stake CPU and RAM resources before playing.");

    // Store the game data
    games_index games(get_self(), get_self().value);
    auto ongoing_game = games.find(0);
    if (ongoing_game == games.end()) {
        // Create a new game
        games.emplace(user, [&](auto& new_game) {
            new_game.game_id = 0;
            new_game.players.push_back(user);
            new_game.selected_numbers.push_back(selected_number);
            new_game.bet_amount = bet_amount - fee_per_transaction;
            new_game.status = 0; // Ongoing
            new_game.start_time = current_time_point();
            new_game.mode = mode;
        });
    } else {
        // Join an existing game
        uint64_t max_players = (mode == 0) ? num_players_single : (mode == 1) ? num_players_two : num_players_multiple;
        check(ongoing_game->players.size() < max_players, "The game is full.");
        games.modify(ongoing_game, user, [&](auto& game_data) {
            game_data.players.push_back(user);
            game_data.selected_numbers.push_back(selected_number);
            game_data.bet_amount += bet_amount - fee_per_transaction;
        });
    }

    // Transfer fee to owner account
    action(permission_level{get_self(), "active"_n},
           "eosio.token"_n, "transfer"_n,
           std::make_tuple(get_self(), owner_account, fee_per_transaction, std::string("Transaction fee")))
        .send();
}

void nft_lottery::resolve(uint64_t game_id) {
    require_auth(get_self());

    games_index games(get_self(), get_self().value);
    auto ongoing_game = games.require_find(game_id, "Game not found");
    check(ongoing_game->status == 0, "Game is already resolved.");
    check(ongoing_game->players.size() >= num_players_single && ongoing_game->players.size() <= num_players_multiple, "Invalid number of players.");

    // Validate timeout
    time_point_sec now = current_time_point();
    check(now.sec_since_epoch() - ongoing_game->start_time.sec_since_epoch() >= random_number_timeout, "Not enough time passed to resolve the game.");

    // Generate a random winning number between 1 and 10
    eosio::random prng(current_time_point().time_since_epoch().count(), get_self().value);
    uint64_t winning_number = prng.range(min_selected_number, max_selected_number);

    // Distribute rewards
    asset total_bet = ongoing_game->bet_amount;
    bool winner_found = false;
for (size_t i = 0; i < ongoing_game->players.size(); ++i) {
    if (ongoing_game->selected_numbers[i] == winning_number) {
        winner_found = true;
        asset winner_reward = total_bet / ongoing_game->players.size();
        action(permission_level{get_self(), "active"_n},
               "eosio.token"_n, "transfer"_n,
               std::make_tuple(get_self(), ongoing_game->players[i], winner_reward, std::string("Winner reward")))
            .send();
    }
}

// If no winner, send the bet to the owner's address
if (!winner_found) {
    action(permission_level{get_self(), "active"_n},
           "eosio.token"_n, "transfer"_n,
           std::make_tuple(get_self(), owner_account, total_bet, std::string("No winner, transfer to owner")))
        .send();
}

// Update game status
games.modify(ongoing_game, get_self(), [&](auto& game_data) {
    game_data.status = 1; // Resolved
});

