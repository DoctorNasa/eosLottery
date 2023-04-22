# NFT Lottery Smart Contract

This repository contains a basic NFT lottery smart contract for the EOSIO-based WAX blockchain. Players can bet WAX tokens on a number between 1 and 10, and if their chosen number matches the randomly generated winning number, they win an NFT reward.

## Features

1. Two-player mode.
2. Players can select a number between 1 and 10.
3. Bet with WAX tokens.
4. The smart contract randomizes the winning number.
5. When a user wins, they receive a reward.
6. When a user loses, the lost bet amount is sent to the contract owner's WAX address.
7. A fee of 0.3 WAX is charged per transaction.
8. Players must have staked enough CPU and RAM resources before participating.

## Dependencies

- EOSIO.CDT v1.6.x or later
- eosio.cdt v1.8.x (for eosio::random)

## Files

- `lottery.hpp`: Header file containing the contract class and action declarations
- `lottery.cpp`: Implementation of the contract's actions and handling token transfers

## Building the Smart Contract

To build the smart contract, run the following command:

#eosio-cpp -abigen -I include -R resource -contract nft_lottery -o nft_lottery.wasm src/lottery.cpp

creaet file as yaml.

## Deployment

To deploy the smart contract, use the following commands:

1. Set the contract:

#cleos set contract <your_account_name> /path/to/contract/directory -p <your_account_name>@active


2. Give the contract permission to use the `eosio.token::transfer` action:

#cleos set account permission <your_account_name> active '{"threshold": 1, "keys": [{"key": "<your_public_key>", "weight": 1}], "accounts": [{"permission":{"actor":"<your_account_name>","permission":"eosio.code"},"weight":1}], "waits":[]}' owner -p <your_account_name>@active


## Usage

1. Send WAX tokens to the contract with a memo indicating the selected number:

#cleos transfer <your_account_name> <contract_account_name> "1.00000000 WAX" "start 5"

Replace `<your_account_name>` with your account name, `<contract_account_name>` with the contract's account name, and `5` with the selected number between 1 and 10.

2. Resolve the lottery game:

cleos push action <contract_account_name> resolve '["<your_account_name>", <game_id>]' -p <your_account_name>@active

vbnet
Copy code

Replace `<your_account_name>` with your account name, `<contract_account_name>` with the contract's account name, and `<game_id>` with the game ID that needs to be resolved.

## Testing

Thoroughly test the contract before deploying it to a live environment.

## License

Q: Create a Lottery Contract for game NFT Lottery.
Feature: 
1. User can selected  Number, 1-10 only one number from 1-10.
2. User are available bet with WAX Token.
3. Smart-contract randomize number 1-10.
4. When user win, they will get reward. 
#make sure this contract use WAX Token?
#please wire the code complete? 
Make sure you are include as list file.
1.atomicassets.hpp
2.atomicdata.hpp
3.base58.hpp
4.checkformat.hpp
Make sure you have complete coding a lottery.hpp file.
testing

The previos smartcontract
please make sure you have update the contract as required.
update feature:
1. create a 2-player mode.
2. if the player selected the wrong number, make sure to send the wax token bet to the owner's address [ wax address].
3. For every betting make sure to get a fee of 0.3 wax/transaction, create {rescive fee} [wax address].
4. Make sure you have to create a correct smart contract.
5. wax address for receiving lost bets and the fee is {dogandcatnf}.make sure have update
6. Make sure the player has stake CPU, ram resources before playing.