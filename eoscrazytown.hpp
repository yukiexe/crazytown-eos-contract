/**
 *  @dev minakokojima, yukiexe
 *  @copyright Andoromeda
 */
#pragma once
#include <eosiolib/eosio.hpp>
#include <eosiolib/singleton.hpp>
#include <eosiolib/asset.hpp>
#include <cmath>
#include <string>
#include <algorithm>

#include "config.hpp"
#include "utils.hpp"

#define EOS_SYMBOL S(4, EOS)
#define TOKEN_CONTRACT N(eosio.token)

typedef double real_type;

using std::string;
using eosio::symbol_name;
using eosio::asset;
using eosio::symbol_type;
using eosio::contract;
using eosio::permission_level;
using eosio::action;

// todo: reveal()
// todo: getResult()
// todo: reveal() => send bonus

class eoscrazytown : public eosio::contract {
    public:    
        eoscrazytown(account_name self) :
        contract(self),
        _global(_self, _self), //  
        players(_self, _self) {}

    // @abi action    
    void transfer(account_name   from,
                  account_name   to,
                  asset          quantity,
                  string         memo);
    
    void onTransfer(account_name   from,
                    account_name   to,
                    asset          quantity,
                    string         memo);  

     // @abi action
     void reveal(const uint64_t& id, const checksum256& seed);


    typedef uint8_t card ;
 
    // @abi table global i64
    struct st_global {       
        uint64_t defer_id ;
        card a ;
        card b ;       
        time roundTimeStamp; 
        st_global() { // todo
            vector<uint8_t> desk;
            for (auto i=0;i<52;++i) desk.push_back(i);
            std::random_shuffle( desk.begin(), desk.end(), gen);
             /*
                uint32_t r = cur + seed.hash[cur] % (52 - cur);
                 std::swap(desk[cur], desk[r]);*/  
        }
    };
    typedef singleton<N(global), st_global> singleton_global ;
    singleton_global _global ;
                     

    // @abi table players account_name
    struct player {
        account_name  account;
        vector<int64_t> vbets ;
        asset quantity ;
        auto primary_key() const { return account; }
        // EOSLIB_SERIALIZE(player, (account)(bets)(vbets)(quantity)) 
    };
    typedef eosio::multi_index<N(player), player> player_index;
    player_index players;     
  
private:
    auto getResult( const card a,  const card b ) ;
    const vector<int64_t> getBets(const string& s, const char& c) ;
    auto getBeton( const vector<int64_t> v ) ;
    const int64_t getTotalBets(const vector<int64_t> v) ;
    
   
    auto checkBets( const asset eos, const string memo,
                vector<int64_t> &vbets, int64_t &totalBets  ) ;

/*
    uint8_t compute_random_roll(const checksum256& seed1, const checksum160& seed2) {
        string mixed_seed = sha256_to_hex(seed1);
        mixed_seed += sha1_to_hex(seed2);
        return uint64_hash(mixed_seed) % 100 + 1;
    }*/
                
};

extern "C" {
    void apply(uint64_t receiver, uint64_t code, uint64_t action) {
        auto self = receiver;
        eoscrazytown thiscontract(self);
        if ((code == N(eosio.token)) && (action == N(transfer))) {
            execute_action(&thiscontract, &eoscrazytown::onTransfer);
            return;
        }
        
        if (code != receiver) return;
        switch (action) { EOSIO_API(eoscrazytown, (transfer)(reveal)) };
        // eosio_exit(0);
    }
}