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

class eoscrazytown : public contract {
    public:    
        eoscrazytown(account_name self) : token(self) {}

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
        EOSLIB_SERIALIZE(global, (id)(defer_id)(reserve)(supply)(claim_time)) 
    };
    typedef eosio::multi_index<N(global), global> global_index;
    global_index global;                 

    // @abi table order i64    
    struct player {
        account_name  account;
        asset         quantity;
        auto primary_key() const { return account; }
    
        EOSLIB_SERIALIZE(player, (account)(quantity)) 
    };
    typedef eosio::multi_index<N(player), player> player_index;
    player_index players;     
  

    auto getResult( const card a,  const card b ) ;
    void reveal() ;

};