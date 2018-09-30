/**
 *  @dev minakokojima, yukiexe
 *  @copyright Andoromeda
 */
#pragma once
#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/contract.hpp>
#include <cmath>
#include <string>

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

// todo: bank bets
// todo: reveal()
// todo: getResult()
// todo: reveal() => send bonus

class eoscrazytown : public contract {
    public:    
        eoscrazytown(account_name self) : token(self) {}

    // @abi action    
    void transfer(account_name   from,
                  account_name   to,
                  asset          quantity,
                  string         memo);
    
    void onTransfer(account_name   from,
                    account_name   to,
                    asset          quantity,
                    string         memo);  



    enum suitType {
        SPADE, HEART, DIAMOND, CLUD
    };

    struct card {
        suitType suit ;
        uint8_t points ;
    } ;
 
    // @abi table global i64
    struct global {
        uint64_t id = 0;        
        uint64_t defer_id = 0;
        card a ;
        card b ;       
        time roundTimeStamp;
        uint64_t primary_key() const { return id; }
        EOSLIB_SERIALIZE(global, (id)(defer_id)(a)(b)(roundTimeStamp)) 
    };
    typedef eosio::multi_index<N(global), global> global_index;
    global_index global;                 

    // @abi table order i64    
    struct player {
        account_name  account;
        string bets ;
        vector<int64_t> vbets ;
        asset quantity ;
        auto primary_key() const { return account; }
    
        EOSLIB_SERIALIZE(player, (account)(bets)(vbets)(quantity)) 
    };
    typedef eosio::multi_index<N(player), player> player_index;
    player_index players;     
  

    auto getResult( const card a,  const card b ) ;
    const vector<int64_t> getBets(const string& s) ;
    auto getBeton( const vector<int64_t> v ) ;
    void reveal() ;

    auto checkBets( const asset eos, const string memo,
                vector<int64_t> &vbets, int64_t &totalBets  ) ;

};