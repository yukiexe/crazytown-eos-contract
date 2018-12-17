#include "eoscrazytown.hpp"

void eoscrazytown::init(const capi_checksum256& hash) {
    require_auth(get_self());
    auto g = _global.get_or_create( get_self(), st_global{.hash = hash});    
    g.hash = hash;
    _global.set(g, get_self());

    clear() ;
}

void eoscrazytown::clear() {
    require_auth(get_self());
    while (_players.begin() != _players.end()) {
        _players.erase(_players.begin());
    }
    /*
    // multi_index can't erase when the format changed
        auto it = db_lowerbound_i64(_self, _self, N(global), 0);
        while (it >= 0) {
            auto del = it;
            uint64_t dummy;
            it = db_next_i64(it, &dummy);
            db_remove_i64(del);
        }
    */
}

const auto eoscrazytown::checkBets( const asset &eos, const string &memo,
                              vector<int64_t> &vbets, int64_t &totalBets  ) {  // check eos.amount == total bets
    vbets = getBets( memo, ',' ) ;
    totalBets = getTotalBets( vbets ) ;    
    return eos.amount == totalBets ;
}

// input
ACTION eoscrazytown::onTransfer(name &from, name &to, asset &eos, string &memo) {        
    if (to != get_self()) return ;

    require_auth(from);
    eosio_assert(eos.is_valid(), "Invalid token transfer...");
    eosio_assert(eos.symbol == EOS_SYMBOL, "only EOS token is allowed");
    eosio_assert(eos.amount > 0, "must bet a positive amount");
    eosio_assert(memo != "" , "must have bets in memo") ;
    eosio_assert(memo.size() >= 21  , "bets wrong...") ;

                
    // todo: input check non-num
    vector<int64_t> vbets ;
    int64_t totalBets = 0 ;
    eosio_assert( eoscrazytown::checkBets( eos, memo, vbets, totalBets ), "Bets not equal to amount.");
    eosio_assert( totalBets >= 1, "Bets should >= 1");
    eosio_assert( totalBets <= 100, "Bets should not > 100");

    // const auto& sym = eosio::symbol(EOS_SYMBOL);
    // accounts eos_account( "eosio.token"_n, get_self().value );
    // auto old_balance = eos_account.get(sym).balance;

    // eosio_assert( totalBets <= old_balance.amount / 20, "Bets too big");
    auto itr = _players.find(from.value);
    if (itr == _players.end()) {
        _players.emplace(get_self(), [&](auto& p) {
            p.account = from.value;
            p.vbets = vbets ;
        });
    } else eosio_assert( false, "Already bet.");
    
}


auto eoscrazytown::getResult( const card &a,  const card &b ) {
    string result = "XXXXXXXXXXX" ;
    const char o = 'O' ;
    auto aS = a / 13 ;
    auto bS = b / 13 ;
    auto aP = a % 13 ;
    auto bP = b % 13 ;
    aP ++;
    bP ++;

    if ( aP > bP ) result[0] = o ; // (1)
    else if ( aP < bP ) result[1] = o ; // (2)
    else result[2] = o ; // (3)

    if ( aS == 1 || aS == 2 ) result[3] = o ; // (4) red
    else result[5] = o ; // (6)

    if ( bS == 1 || bS == 2 ) result[4] = o ; // (5) red
    else result[6] = o  ; // (7)

    if ( ( aP & 1 ) == 1 ) result[7] = o  ; // (8) odd
    else result[8] = o  ; // (9)

    if ( ( bP & 1 ) == 1 ) result[9] = o  ; // (10) odd
    else result[10] = o  ; // (11)

    return result ;
}

const vector<int64_t> eoscrazytown::getBets(const string& s, const char& c) { // need protect
    vector<int64_t> vbets;
    auto vs = explode( s, c ) ;
    for ( auto &n:vs ) {
        vbets.push_back( (int64_t)string_to_price( n ) ) ;
    }

    // change format
    vector<int64_t> v(vbets) ;
    v[0] = vbets[1] ; // (1)
    v[1] = vbets[6] ; // (2)
    v[2] = vbets[0] ; // (3)
    v[3] = vbets[4] ; // (4)
    v[4] = vbets[9] ; // (5)
    v[5] = vbets[5] ; // (6)
    v[6] = vbets[10] ; // (7)
    v[7] = vbets[2] ; // (8)
    v[8] = vbets[3] ; // (9)
    v[9] = vbets[7] ; // (10)
    v[10] = vbets[8] ; // (11)

    return v ;
}

auto eoscrazytown::getBeton( const vector<int64_t> &v ) {
    string beton = "" ;
    for(auto &n:v) beton+= ( n != 0 ) ? 'O' : 'n' ;
    
    return beton;
}

const int64_t eoscrazytown::getTotalBets(const vector<int64_t> &v) {
    int64_t totalBets = 0 ;
    for (auto &n:v) totalBets += n ;

    return totalBets ;
}

// Output
void eoscrazytown::reveal(const capi_checksum256& seed, const capi_checksum256& hash){ // hash for next round
    require_auth(get_self());

    card dragon = seed.hash[ 0 ] % 52 ;
    card tiger = seed.hash[ 1 ] % 52 ;
    const rec_reveal _reveal{
        .dragon = dragon,
        .tiger = tiger,
        .server_hash = _global.get().hash,
        .client_seed = seed,
    }; 
    /*
    // singleton -> _global 
    auto g = _global.get();    
    g.dragon = dragon ;
    g.tiger = tiger ;
    _global.set(g, _self);
    */
    action( // give result to client
            permission_level{get_self(), "active"_n},
            get_self(), "receipt"_n, _reveal
    ).send();

    auto result = getResult( _reveal.dragon, _reveal.tiger ) ;         

    string beton ;
    string presult = "" ;
    int64_t bonus ;
    const char o = 'O' ;
    for (const auto& p : _players ) {
        auto& bets = p.vbets ;
        beton = getBeton( bets ) ;
    
        bonus = 0 ;
        presult = "" ;
        // exp:
        // r 2.0: O X X X X X X X X X X // no space !
        // beton: O X X O X X O X O O O // no space !
        if ( result[2] == o ) { // (3) draw
            if ( result[2] == beton[2] ) bonus += bets[2] + bets[2] * DRAW ; // (3)
            bonus += bets[0] / 2 ;
            bonus += bets[1] / 2 ;
            /*
            if ( result[3] == beton[3] ) bonus += bets[3] + bets[3] * COLOR ; // (4)
            if ( result[4] == beton[4] ) bonus += bets[4] + bets[4] * COLOR ; // (5)

            if ( result[5] == beton[5] ) bonus += bets[5] + bets[5] * COLOR ; // (6)
            if ( result[6] == beton[6] ) bonus += bets[6] + bets[6] * COLOR ; // (7)
            */
        }
        else { 
            if ( result[0] == o ) {
                if ( result[0] == beton[0] ) {
                    bonus += bets[0] + bets[0] * SIDE ; // (1)
                    presult += '1' ;
                }  
            }
            else { // result[1] == o
                if ( result[1] == beton[1] ) {
                    bonus += bets[1] + bets[1] * SIDE ; // (2)
                    presult += '2' ;
                }
            }
    
            if ( result[3] == beton[3] ) {
                bonus += bets[3] + bets[3] * COLOR ; // (4)
                presult += '4' ;
            }
            
            if ( result[4] == beton[4] ) {
                bonus += bets[4] + bets[4] * COLOR ; // (5)
                presult += '5' ;
            } 

            if ( result[5] == beton[5] ) {
                bonus += bets[5] + bets[5] * COLOR ; // (6)
                presult += '6' ;
            } 
            if ( result[6] == beton[6] ) {
                bonus += bets[6] + bets[6] * COLOR ; // (7)
                presult += '7' ;
            }
            

            if ( result[7] == beton[7] ) {  
                bonus += bets[7] + bets[7] * ODD ; // (8)
                presult += '8' ;
            }
            if ( result[8] == beton[8] ) {
                bonus += bets[8] + bets[8] * EVEN ; // (9)
                presult += '9' ;
            }
            if ( result[9] == beton[9] ) {
                bonus += bets[9] + bets[9] * ODD ; // (10)
                presult += 'A' ;
            }
            if ( result[10] == beton[10] ) {
                bonus += bets[10] + bets[10] * EVEN ; // (11)
                presult += 'B' ;
            }
        }

        
        action( // winner winner chicken dinner
            permission_level{get_self(), "active"_n},
            get_self(), "transfer"_n,
            make_tuple(get_self(), p.account, asset(bonus, EOS_SYMBOL),
                std::string("winner winner chicken dinner") )
        ).send();

    }

    auto g = _global.get_or_create( _self, st_global{.hash = hash});    
    g.hash = hash;
    g.dragon = dragon ;
    g.tiger = tiger ;
    _global.set(g, _self);

    while (_players.begin() != _players.end()) {
        _players.erase(_players.begin());
    }

    
    // init( hash ) ;
} 