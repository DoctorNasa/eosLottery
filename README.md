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
 Redroll.io license by tech develop teams.

## Game smartcontract
``` let's review the current features in your NFT Lottery smart contract:

Single-mode, 2-player mode, and multiple-mode
Time-limited events
Lottery game jackpot method
Sending lost bets to the owner's address
Charging a fee per transaction
Staking CPU and RAM resources before playing
If you want to continue enhancing the smart contract, you can consider implementing some of the other features mentioned earlier:

Dynamic Jackpot: Instead of using a fixed percentage of lost bets for the jackpot pool, implement a dynamic system that changes the percentage based on factors like the total number of games played, the total amount of bets, or the number of consecutive games without a winner.

Tiered Lottery System: Allow players to select multiple numbers and win different levels of rewards based on the number of correct guesses. This will encourage users to participate more and make the game more interesting.

Referral Program: Introduce a referral program that rewards users for bringing in new participants. Users can earn a small percentage of the bets made by their referrals, incentivizing them to share the game with friends and followers.

NFT Prize Pool: Integrate a prize pool of NFTs that can be won alongside WAX token rewards. This adds an extra layer of excitement and variety to the game, as users will be motivated to participate to win exclusive NFTs.

Leaderboard: Implement a leaderboard to showcase top winners and participants, encouraging competition among users and increasing engagement.

NFT Staking: Allow users to stake their NFTs as an alternative form of entry to the lottery. This creates a new use case for NFTs and may attract users who own valuable NFTs but do not want to spend WAX tokens.

Social Features: Integrate social features such as chat rooms or in-game messaging, allowing users to interact with each other and discuss the game. This can foster a sense of community and increase user engagement.```


## Next Feature
1. Dynamic Jackpot: Create a dynamic jackpot that accumulates a percentage of the lost bets over time. This jackpot can be won at specific intervals or under certain conditions, which will increase user engagement and excitement.

2. Tiered Lottery System: Implement a tiered lottery system where users can select multiple numbers and win different levels of rewards based on the number of correct guesses. This will encourage users to participate more and make the game more interesting.

3. Referral Program: Introduce a referral program that rewards users for bringing in new participants. Users can earn a small percentage of the bets made by their referrals, thus incentivizing them to share the game with friends and followers.

4. Time-limited Events: Create time-limited events, like holiday-themed games, with unique rewards and increased chances of winning. This will attract more users during special occasions and keep the game fresh.

5. NFT Prize Pool: Integrate a prize pool of NFTs that can be won alongside WAX token rewards. This adds an extra layer of excitement and variety to the game, as users will be motivated to participate in order to win exclusive NFTs.

6. Leaderboard: Implement a leaderboard to showcase top winners and participants, encouraging competition among users and increasing engagement.

7. NFT Staking: Allow users to stake their NFTs as an alternative form of entry to the lottery. This creates a new use case for NFTs and may attract users who own valuable NFTs but do not want to spend WAX tokens.

8. Social Features: Integrate social features such as chat rooms or in-game messaging, allowing users to interact with each other and discuss the game. This can foster a sense of community and increase user engagement.