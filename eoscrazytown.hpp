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
        _global(receiver, receiver.value), //  
        players(receiver, receiver.value) {}


<<<<<<< Updated upstream
class eoscrazytown : public eosio::contract {
    public:    
        eoscrazytown(account_name self) :
        contract(self),
        _global(_self, _self),
        players(_self, _self){}


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

    // @abi action
    void transfer(account_name   from,
                  account_name   to,
=======
    ACTION init(const capi_checksum256& hash);

    ACTION clear();     

    ACTION test();

    ACTION verify( const capi_checksum256& seed, const capi_checksum256& hash );                        

    ACTION reveal(const capi_checksum256& seed, const capi_checksum256& hash);

    ACTION transfer(name   from,
                  name   to,
>>>>>>> Stashed changes
                  asset          quantity,
                  string         memo);
    
    void onTransfer(name   &from,
                    name   &to,
                    asset          &quantity,
                    string         &memo);

    ACTION receipt(const rec_reveal& reveal) {
        require_auth(get_self());
    }

    TABLE st_global {       
        uint64_t defer_id = 0;
<<<<<<< Updated upstream
        checksum256 hash;
        uint8_t dragon ;
        uint8_t tiger ;
        EOSLIB_SERIALIZE( st_global, (defer_id)(hash)(dragon)(tiger)) ;
=======
        capi_checksum256 hash;
>>>>>>> Stashed changes
    };
    typedef singleton<"global"_n, st_global> singleton_global;
    singleton_global _global;         

    TABLE player {
        uint64_t  account; //account_name to uint64
        vector<int64_t> vbets ;
        uint64_t     primary_key() const { return account; }
        // EOSLIB_SERIALIZE(player, (account)(vbets)) 
    };
    typedef eosio::multi_index<"player"_n, player> player_index;
    player_index players;  

    
<<<<<<< Updated upstream
    void apply(account_name code, action_name action);
=======
    void apply(uint64_t receiver, uint64_t code, uint64_t action) ;   
>>>>>>> Stashed changes
  
private:
    const vector<int64_t> getBets(const string &s, const char &c) ;
    auto getBeton( const vector<int64_t> &v );
    const int64_t getTotalBets(const vector<int64_t> &v);

    auto checkBets( const asset &eos, const string &memo,
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
        EOSIO_DISPATCH_HELPER(eoscrazytown, (init)(test)(clear)(reveal));
    };
}

extern "C" {
    [[noreturn]] void apply(uint64_t receiver, uint64_t code, uint64_t action) 
    {
        eoscrazytown p( name(receiver), name(code), datastream<const char*>(nullptr, 0) );
        p.apply(receiver, code, action);
        eosio_exit(0);
    }
}