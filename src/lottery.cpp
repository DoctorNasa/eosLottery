#include <lottery.hpp>

// Empty implementation - the lottery contract has no state to initialize
void lottery::init() {}

// Action to allow user to place a bet
void lottery::bet(name user, uint8_t number, asset quantity) {
    // Check that user is authorized to make this transaction
    require_auth(user);

    // Check that bet amount is positive
    check(quantity.amount > 0, "Invalid bet amount");

    // Check that the number is within the valid range
    check(number >= 1 && number <= 10, "Invalid number");

    // Deduct the bet amount from the user's account balance
    sub_balance(user, quantity);

    // Add the user's bet to the list of bets
    bets.emplace(user, [&](auto& bet) {
        bet.id = bets.available_primary_key();
        bet.user = user;
        bet.number = number;
        bet.quantity = quantity;
    });
}

// Action to allow the contract owner to draw the winning number
void lottery::draw() {
    // Check that the caller is authorized to draw the winning number
    require_auth(get_self());

    // Generate a random number between 1 and 10 using the SHA256 hash of the current time
    checksum256 hash = sha256((char *)&now(), sizeof(uint64_t));
    uint64_t random_number = hash._hash[1] % 10 + 1;

    // Check each bet to see if the user has won
    for (auto& bet : bets) {
        if (bet.number == random_number) {
            // Calculate the user's winnings based on the odds
            asset winnings = bet.quantity * 9;

            // Add the winnings to the user's account balance
            add_balance(bet.user, winnings);

            // Emit an event to notify the user of their winnings
            atomicdata::emit_notification(
                atomicdata::Notification{
                    .recipient = bet.user,
                    .content = "Congratulations! You've won the lottery!"
                }
            );
        }
    }

    // Clear the list of bets
    bets.clear();
}

// Internal function to subtract an amount from a user's account balance
void lottery::sub_balance(name user, asset quantity) {
    action(
        permission_level{ user, "active"_n },
        "wax.token"_n, "transfer"_n,
        std::make_tuple(user, get_self(), quantity, std::string("Lottery bet"))
    ).send();
}

// Internal function to add an amount to a user's account balance
void lottery::add_balance(name user, asset quantity) {
    action(
        permission_level{ get_self(), "active"_n },
        "wax.token"_n, "transfer"_n,
        std::make_tuple(get_self(), user, quantity, std::string("Lottery winnings"))
    ).send();
}
