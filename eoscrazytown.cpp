#include "eoscrazytown.hpp"

// input
void eoscrazytown::onTransfer(account_name from, account_name to, asset eos, string memo) {        
    if (to != _self) return ;

    require_auth(from);
    eosio_assert(eos.is_valid(), "Invalid token transfer");
    eosio_assert(eos.symbol == EOS_SYMBOL, "only EOS token is allowed");
    eosio_assert(eos.amount > 0, "must bet a positive amount");
    eosio_assert(memo != "" , "must have bets in memo") ;
    eosio_assert(memo.size() >= 21  , "bets wrong") ;
    
    // todo: input check non-num
    vector<int64_t> vbets ;
    int64_t totalBets = 0 ;
    eosio_assert( checkBets( eos, memo, vbets, totalBets ), "Bets not equal to amount.");
    auto itr = players.find(from);
    if (itr == players.end()) {
        players.emplace(_self, [&](auto& p) {
            p.account = from;
            p.vbets = vbets ;
            p.quantity = eos;
        });
    } else {
        eosio_assert( false, "Already bet.");
        return ;
        /*
        player.modify(itr, 0, [&](auto &p) {
                
                p.quantity += eos;
            });*/
    }
}

auto eoscrazytown::checkBets( const asset eos, const string memo,
                              vector<int64_t> &vbets, int64_t &totalBets  ) {  // check eos.amount == total bets
    vbets = getBets( memo, ' ' ) ;
    totalBets = getTotalBets( vbets ) ;    
    return eos.amount == totalBets ;
}

auto eoscrazytown::getResult( const card a,  const card b ) { // need rewrite
    string result = "XXXXXXXXXXX" ;
    const char o = 'O' ;
    if ( a.points > b.points ) result[0] = o ; // (1)
    else if ( b.points > a.points ) result[1] = o ; // (2)
    else result[2] = o ; // (3)

    if ( a.suit ==  HEART || a.suit == DIAMOND ) result[3] = o ; // (4) red
    else result[5] = o ; // (6)

    if ( b.suit ==  HEART || b.suit == DIAMOND ) result[4] = o ; // (5) red
    else result[6] = o  ; // (7)

    if ( ( a.points & 1 ) == 1 ) result[7] = o  ; // (8) odd
    else result[8] = o  ; // (9)

    if ( ( b.points & 1 ) == 1 ) result[9] = o  ; // (10) odd
    else result[10] = o  ; // (11)

    return result ;
}

const vector<int64_t> eoscrazytown::getBets(const string& s, const char& c) { // need protect
    vector<int64_t> vbets;
    auto vs = explode( s, c ) ;
    for ( auto n:vs ) {
        vbets.push_back( (int64_t)string_to_price( n ) ) ;
    }

    return vbets ;
}

auto eoscrazytown::getBeton( const vector<int64_t> v ) {
    string beton = "" ;

    for(auto n:v) {
        if ( n != 0 ) beton+='O';
        else beton+='X' ;
    }
    
    return beton;
}

const int64_t eoscrazytown::getTotalBets(const vector<int64_t> v) {
    int64_t totalBets = 0 ;
    for (auto n:v) totalBets += n ;

    return totalBets ;
}

// Output
void eoscrazytown::reveal(const uint64_t& id, const checksum256& seed) {
    require_auth(_self);

    // todo
    auto result = getResult(
        _global.get_or_create( _self, st_global{.a = card() }),
        _global.get_or_create( _self, st_global{.b = card() })
    );

    string beton ;
    // string presult ;
    vector<int64_t> bets ;
    asset payout = asset(0, EOS_SYMBOL);
    int64_t bonus ;
    const char o = 'O' ;
    for ( auto p = begin(players) ; p != end(players) ; ++p ) {
        beton = getBeton( p->vbets ) ;
        bets = p->vbets ;
        bonus = 0 ;
        // exp:
        // r 2.0: O X X X X X X X X X X // no space !
        // beton: O X X O X X O X O O O // no space !
        if ( result[2] == o ) { // (3) draw
            // presult += 'x' ; 

            return ;
        }
        else { 
            if ( result[0] == beton[0] )
                ;// presult += 'w' ; // win
            else
                ;// presult += 'l' ; // lose
        }

        if ( result[3] == beton[3] ) bonus += bets[3] + bets[3] * COLOR ; // (4)
        if ( result[4] == beton[4] ) bonus += bets[4] + bets[4] * COLOR ; // (5)

        if ( result[5] == beton[5] ) bonus += bets[5] + bets[5] * COLOR ; // (6)
        if ( result[6] == beton[6] ) bonus += bets[6] + bets[6] * COLOR ; // (7)

        if ( result[7] == beton[7] )  bonus += bets[7] + bets[7] * ODD ; // (8)
        if ( result[8] == beton[8] )  bonus += bets[8] + bets[8] * EVEN ; // (9)

        if ( result[9] == beton[9] )  bonus += bets[9] + bets[9] * ODD ; // (10)
        if ( result[10] == beton[10] )  bonus += bets[10] + bets[10] * EVEN ; // (11)

        // 發錢
    }

}