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
  
};

// ===
auto eoscrazytown::getResult ( const card a,  const card b ) {
    string result = "" ;
    if ( a.points > b.points ) result += "a" ; // (1)
    else if ( b.points > a.points ) result += "b" ; // (2)
    else result += "x" ; // (3)

    if ( a.suit ==  HEART || a.suit == DIAMOND ) result += "aR" ; // (4) red
    else result += "aB" ; // (6)

    if ( b.suit ==  HEART || b.suit == DIAMOND ) result += "bR" ; // (5) red
    else result += "bB" ; // (7)

    if ( a.points & 1 == 1 ) result += "aO" ; // (8) odd
    else result += "aE" ; // (9)

    if ( b.points & 1 == 1 ) result += "bO" ; // (10) odd
    else result += "bE" ; // (11)

    return result ;
}



void eoscrazytown::give_out_bonus ( const string result ) {
 

}


void eoscrazytown::reveal(const checksum256 &seed, const checksum256 &hash) {
    require_auth(_self);

    auto result = getResult ( a, b ) ;
    string presult = "" ;
    for ( auto p = begin(players) ; p != end(players) ; ++p ) {
        presult = "" ;
        // exp:
        // r:  xaRbBaObE
        // pr: annaRaBnnbBnnaEbOnn
        if ( result[0] == 'x' ) { // draw
            presult += 'x' ; 
        }
        else { 
            if ( p->beton[0] == result[0] )
                presult += 'w' ; // win
            else
                presult += 'l' ; // lose
        }

        if ( p->beton.substr(1,2) == result.substr(1,2) ) // 
        if ( p->beton.substr(3,2) == result.substr(3,2) )
        if ( p->beton.substr(5,2) == result.substr(5,2) )
        if ( p->beton.substr(7,2) == result.substr(7,2) )
        

    }



    // assert_sha256((char *)&seed, sizeof(seed), (const checksum256 *)&global.begin()->hash);
    // auto n = offers.available_primary_key();
    /*
    for (int i = 0; i < n; ++i) {
        auto itr = offers.find(i);
        deal_with(itr, seed);
    }
    auto itr = global.find(0);
    global.modify(itr, 0, [&](auto &g) {
        g.hash = hash;
        g.offerBalance = 0;
    });*/
}