#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <eosio/crypto.hpp>
#include <atomicassets.hpp>
#include <atomicdata.hpp>
#include <base58.hpp>
#include <checkformat.hpp>

using namespace eosio;

class [[eosio::contract("lottery")]] lottery : public contract {
public:
    using contract::contract;

    // Action to allow user to place a bet
    [[eosio::action]]
    void bet(name user, uint8_t number, asset quantity);

    // Action to allow the contract owner to draw the winning number
    [[eosio::action]]
    void draw();

private:
    // Struct to represent a single bet
    struct [[eosio::table]] bet {
        uint64_t id;
        name user;
        uint8_t number;
        asset quantity;

        uint64_t primary_key() const { return id; }
    };

    // Table to store the list of bets
    typedef eosio::multi_index<"bets"_n, bet> bet_index;

    bet_index bets;

    // Internal function to subtract an amount from a user's account balance
    void sub_balance(name user, asset quantity);

    // Internal function to add an amount to a user's account balance
    void add_balance(name user, asset quantity);
};
