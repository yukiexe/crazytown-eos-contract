/**
 *  @dev minakokojima, yukiexe
 *  @copyright Andoromeda
 */
#pragma once
#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/singleton.hpp>
// #include <cmath>
// #include <string>

#include "config.hpp"
#include "utils.hpp"
 
#define EOS_SYMBOL S(4, EOS)
#define TOKEN_CONTRACT N(eosio.token)

typedef double real_type;

using std::string;
using eosio::symbol_name;
using eosio::asset;
using eosio::symbol_type;
using eosio::permission_level;
using eosio::action;

class eoscrazytown : public eosio::contract {
    public:    
        eoscrazytown(account_name self) :
        contract(self),
        _global(_self, _self), //  
        players(_self, _self) {}


    // @abi action
    void init(const checksum256& hash);
    // @abi action
    void clear();     
    // @abi action
    void test();
    // @abi action
    void verify( const checksum256& seed, const checksum256& hash );                        
    // @abi action
    void reveal(const checksum256& seed, const checksum256& hash);

    /*    
    void transfer(account_name   from,
                  account_name   to,
                  asset          quantity,
                  string         memo);
    */
    void onTransfer(account_name   &from,
                    account_name   &to,
                    asset          &quantity,
                    string         &memo);



    typedef uint8_t card ;

    struct rec_reveal {
        card dragon ;
        card tiger ;
        checksum256 server_hash;
        checksum256 client_seed;
    };

    // @abi table global
    struct st_global {       
        uint64_t defer_id = 0;
        checksum256 hash;
    };
    typedef singleton<N(global), st_global> singleton_global;
    singleton_global _global;         

    // @abi table player
    struct player {
        account_name  account;
        vector<int64_t> vbets ;
        uint64_t     primary_key() const { return account; }
        // EOSLIB_SERIALIZE(player, (account)(vbets)) 
    };
    typedef eosio::multi_index<N(player), player> player_index;
    player_index players;  

    
    void apply(account_name code, action_name action);       
  
private:
    const vector<int64_t> getBets(const string &s, const char &c) ;
    auto getBeton( const vector<int64_t> &v );
    const int64_t getTotalBets(const vector<int64_t> &v);

    auto checkBets( const asset &eos, const string &memo,
                vector<int64_t> &vbets, int64_t &totalBets  );                

    auto getResult( const card &a,  const card &b ) ;
};


void eoscrazytown::apply(account_name code, action_name action) {   
    auto &thiscontract = *this;

    if (action == N(transfer)) {
        if (code == N(eosio.token)) {
            execute_action(&thiscontract, &eoscrazytown::onTransfer);
        }
        return;
    }

    if (code != _self) return;
    switch (action) {
        EOSIO_API(eoscrazytown, (init)(test)(clear)(reveal));
    };
}

extern "C" {
    [[noreturn]] void apply(uint64_t receiver, uint64_t code, uint64_t action) 
    {
        eoscrazytown p(receiver);
        p.apply(code, action);
        eosio_exit(0);
    }
}