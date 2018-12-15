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
// #include "eosio.token.hpp"
 
#define EOS_SYMBOL eosio::symbol("EOS", 4)
#define TOKEN_CONTRACT "eosio.token"_n

typedef double real_type;
typedef uint8_t card ;

using namespace std;
using namespace eosio;

CONTRACT eoscrazytown : public eosio::contract {
    public:    
        eoscrazytown( name receiver, name code, datastream<const char*> ds ) :
        contract(receiver, code, ds),
        _global(receiver, receiver.value),
        _players(receiver, receiver.value) {}

    ACTION init(const capi_checksum256& hash);
    ACTION clear();     
    // ACTION test();
    // ACTION verify( const capi_checksum256& seed, const capi_checksum256& hash );                        
    ACTION reveal(const capi_checksum256& seed, const capi_checksum256& hash);
    
    // rec
    ACTION receipt(const rec_reveal& reveal) {
        require_auth(get_self());
    }

    struct [[eosio::table("global")]] st_global {       
        uint64_t defer_id = 0;
        capi_checksum256 hash;
        uint8_t dragon ;
        uint8_t tiger ;
    };   

    TABLE player {
        uint64_t  account; //account_name to uint64
        vector<int64_t> vbets ;
        auto primary_key() const { return account; }
        EOSLIB_SERIALIZE(player, (account)(vbets)) 
    };

    typedef singleton<"global"_n, st_global> singleton_global;
    typedef eosio::multi_index<"player"_n, player> player_t;
    singleton_global _global ;
    player_t _players;  

    
    void apply(uint64_t receiver, uint64_t code, uint64_t action);

private:
    void onTransfer(name   &from,
                    name   &to,
                    asset          &quantity,
                    string         &memo);

    const vector<int64_t> getBets(const string &s, const char &c) ;
    auto getBeton( const vector<int64_t> &v );
    const int64_t getTotalBets(const vector<int64_t> &v);

    const auto checkBets( const asset &eos, const string &memo,
                vector<int64_t> &vbets, int64_t &totalBets  );                

    auto getResult( const card &a,  const card &b ) ;
};


void eoscrazytown::apply(uint64_t receiver, uint64_t code, uint64_t action) {   
    auto &thiscontract = *this;
    if (action == ( "transfer"_n ).value && code == ( "eosio.token"_n ).value ) {
        auto transfer_data = unpack_action_data<st_transfer>();
        onTransfer(transfer_data.from, transfer_data.to, transfer_data.quantity, transfer_data.memo);
        return;
    }

    if (code != get_self().value) return;
    switch (action) {
        EOSIO_DISPATCH_HELPER(eoscrazytown,
                             (init)
                             (clear)
                             (reveal)
                             (receipt)
        );
    };
}

extern "C" {
    [[noreturn]] void apply(uint64_t receiver, uint64_t code, uint64_t action) {
        eoscrazytown p( name(receiver), name(code), datastream<const char*>(nullptr, 0) );
        p.apply(receiver, code, action);
        eosio_exit(0);
    }
}