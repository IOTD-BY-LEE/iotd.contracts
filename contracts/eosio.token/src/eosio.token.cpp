#include <eosio.token/eosio.token.hpp>

namespace eosio {

void token::create( const name&   issuer,
                    const asset&  maximum_supply )
{
    require_auth( get_self() );

    auto sym = maximum_supply.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( maximum_supply.is_valid(), "invalid supply");
    check( maximum_supply.amount > 0, "max-supply must be positive");

    stats statstable( get_self(), sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    check( existing == statstable.end(), "token with symbol already exists" );

    statstable.emplace( get_self(), [&]( auto& s ) {
       s.supply.symbol = maximum_supply.symbol;
       s.max_supply    = maximum_supply;
       s.issuer        = issuer;
    });
}


void token::issue( const name& to, const asset& quantity, const string& memo )
{
    auto sym = quantity.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    stats statstable( get_self(), sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    check( existing != statstable.end(), "token with symbol does not exist, create token before issue" );
    const auto& st = *existing;
    check( to == st.issuer, "tokens can only be issued to issuer account" );

    require_auth( st.issuer );
    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must issue positive quantity" );

    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    check( quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

    statstable.modify( st, same_payer, [&]( auto& s ) {
       s.supply += quantity;
    });

    add_balance( st.issuer, quantity, st.issuer );
}

void token::issuelock( const name& to, 
                       const asset& quantity, 
                       const string& memo, 
                       const asset& lockquantity,
                       const uint32_t& unlock_delay_sec )
{
    auto sym = quantity.symbol;
    const symbol_code sym_code = sym.code();
    check( sym.is_valid(), "invalid symbol name" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    stats statstable( get_self(), sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    check( existing != statstable.end(), "token with symbol does not exist, create token before issue" );
    const auto& st = *existing;

    require_auth( st.issuer );
    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must issue positive quantity" );

    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    check( quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

    statstable.modify( st, same_payer, [&]( auto& s ) {
       s.supply += quantity;
    });

    add_balance( st.issuer, quantity, st.issuer );

    if( to != st.issuer ) {
      SEND_INLINE_ACTION( *this, transfer, { {st.issuer, "active"_n} },
                          { st.issuer, to, quantity, memo }
      );
      SEND_INLINE_ACTION( *this, lock, { {st.issuer, "active"_n} },
                          { to, lockquantity, unlock_delay_sec }
      );
      SEND_INLINE_ACTION( *this, unlock, { {st.issuer, "active"_n} },
                          { to, sym_code }
      );
    }
}

void token::retire( const asset& quantity, const string& memo )
{
    auto sym = quantity.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    stats statstable( get_self(), sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    check( existing != statstable.end(), "token with symbol does not exist" );
    const auto& st = *existing;

    require_auth( st.issuer );
    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must retire positive quantity" );

    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );

    statstable.modify( st, same_payer, [&]( auto& s ) {
       s.supply -= quantity;
    });

    sub_balance( st.issuer, quantity );
}

void token::transfer( const name&    from,
                      const name&    to,
                      const asset&   quantity,
                      const string&  memo )
{
    check( from != to, "cannot transfer to self" );
    require_auth( from );
    check( is_account( to ), "to account does not exist");
    auto sym = quantity.symbol.code();
    stats statstable( get_self(), sym.raw() );
    const auto& st = statstable.get( sym.raw() );

    require_recipient( from );
    require_recipient( to );

    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must transfer positive quantity" );
    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    locked_accounts locked_acnts( get_self(), from.value );
    auto target = locked_acnts.find( quantity.symbol.code().raw() );

    blacklists blacklist( get_self(), from.value );
    auto blacklist_itr = blacklist.find( from.value );
    if ( blacklist_itr != blacklist.end() ) {
       accounts from_acnts( get_self(), from.value );
       const auto& from = from_acnts.get( quantity.symbol.code().raw(), "no balance object found" );

       // blacklist if tranfer exceeds black amount
       if ( target != locked_acnts.end() ) {
         //  printf("--------------------------");
         //  print(from.balance.amount);
         //  printf("--------------------------");
         //  print(target->total_balance.amount);
         //  printf("--------------------------");
         //  print(quantity.amount);
         //  printf("--------------------------");
         //  print(blacklist_itr->tokens_blocked.amount);
          check( from.balance.amount - target->total_balance.amount - quantity.amount >= blacklist_itr->tokens_blocked.amount, 
          "quantity exceeds available amount allowed by the blacklisted user");
       } else {
          check( from.balance.amount - quantity.amount >= blacklist_itr->tokens_blocked.amount, 
          "quantity exceeds the amount allowed by the blacklisted user");
       }
    }

    if ( target != locked_acnts.end() ) {
       // check if sufficient amount is not locked for this transfer
       accounts from_acnts( get_self(), from.value );
       const auto& from = from_acnts.get( quantity.symbol.code().raw(), "no balance object found" );
       check( from.balance.amount - target->total_balance.amount >= quantity.amount, "locked balance" );
    }
    auto payer = has_auth( to ) ? to : from;

    sub_balance( from, quantity );
    add_balance( to, quantity, payer );
}

void token::lock( const name& owner, const asset& quantity, const uint32_t& unlock_delay_sec )
{
    auto sym = quantity.symbol.code();
    stats statstable( get_self(), sym.raw() );
    const auto& st = statstable.get( sym.raw() );

    require_auth( st.issuer );

    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must lock positive quantity" );
    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );

    check( unlock_delay_sec > 0, "must set unlock delay");
    //check( unlock_delay_sec > 3*24*3600, "must set unlock delay less than refund delay");

    accounts acnts( get_self(), owner.value );
    const auto& acnt = acnts.get( quantity.symbol.code().raw(), "no balance object found" );
    check( quantity.amount <= acnt.balance.amount, "quantity to lock is larger than current balance" );

    locked_accounts locked_acnts( get_self(), owner.value );
    auto target = locked_acnts.find( quantity.symbol.code().raw() );

    if( target == locked_acnts.end() ) {
       locked_acnts.emplace( st.issuer, [&]( auto& a ) {
          a.total_balance = quantity;
          a.balances.reserve( 1 );
          a.balances.push_back( frozen_balance{quantity, unlock_delay_sec, time_point_sec::min(), time_point_sec::min()} );
         //  a.unlock_delay_sec = unlock_delay_sec;
         //  a.unlock_request_time = time_point_sec::min();
         //  a.unlock_execute_time = time_point_sec::min();
       });
    } else {
       check( quantity.amount + target->total_balance.amount <= acnt.balance.amount, "increased quantity to lock is larger than current balance" );
       locked_acnts.modify( target, same_payer, [&]( auto& a ) {
          a.total_balance.amount += quantity.amount;
          a.balances.reserve( a.balances.size() + 1 );
          a.balances.push_back( frozen_balance{quantity, unlock_delay_sec, time_point_sec::min(), time_point_sec::min()} );
       });
    }
}

void token::unlock( const name& owner, const symbol_code& sym_code )
{
   auto sym = sym_code;
   stats statstable( get_self(), sym.raw() );
   const auto& st = statstable.get( sym.raw() );

   require_auth( st.issuer );

   // if( quantity.amount <= 0){
   //    cancel_deferred( owner.value ); // check if needed
   //    return;
   // }

   // check( quantity.is_valid(), "invalid quantity" );
   // check( quantity.amount > 0, "must unlock positive quantity" );
   // check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
   locked_accounts locked_acnts( get_self(), owner.value );
   auto target = locked_acnts.find( sym_code.raw() );
   check( target != locked_acnts.end(), "no locked balance object found" );
   check( 0 < target->total_balance.amount && 0 < target->balances.size(), "no locked balances item found" );

   locked_acnts.modify( target, same_payer, [&]( auto& a ) {
      for(auto itr = a.balances.begin(); itr != a.balances.end();itr++) {
         if(itr->unlock_request_time <= time_point_sec::min() + seconds(1)){
            itr->unlock_request_time = time_point_sec(current_time_point());
         }
         if(itr->unlock_execute_time <= time_point_sec::min() + seconds(1)){
            itr->unlock_execute_time = time_point_sec(current_time_point()) + itr->unlock_delay_sec;
         }
      }
      if(a.balances.size() > 1) {
         std::sort (a.balances.begin(), a.balances.end(), sort_by_execute_time);
      }
   });

   eosio::transaction out;
   out.actions.emplace_back( permission_level{owner, "active"_n},
                             get_self(), "dounlock"_n,
                             std::make_tuple(owner, sym)
   );
   int32_t ndelay_sec = target->balances.begin()->unlock_execute_time.sec_since_epoch() - time_point_sec(current_time_point()).sec_since_epoch() ;
   if(ndelay_sec <= 0) {
      out.delay_sec = 3;
   } else {
      out.delay_sec = ndelay_sec;
   }
   printf("unlock-----out.delay_sec=%d",out.delay_sec);
   cancel_deferred( owner.value ); // check if needed
   out.send( owner.value, st.issuer, true );

}

void token::dounlock( const name& owner, const symbol_code& sym_code )
{
   auto sym = sym_code;
   stats statstable( get_self(), sym.raw() );
   const auto& st = statstable.get( sym.raw() );

   locked_accounts locked_acnts( get_self(), owner.value );
   auto target = locked_acnts.find( sym_code.raw() );
   check( target != locked_acnts.end(), "no locked balance object found" );
   check( target->balances.begin()->unlock_request_time > time_point_sec::min(), "unlock is not requested" );
   check( time_point_sec(target->balances.begin()->unlock_request_time + seconds(target->balances.begin()->unlock_delay_sec)) <= time_point_sec(current_time_point()),
                 "unlock is not avalialbe yet");
   check( 0 < target->total_balance.amount && 0 < target->balances.size(), "no locked balances item found" );

   locked_acnts.modify( target, same_payer, [&]( auto& a ) {
      if (a.total_balance.amount >= a.balances.begin()->balance.amount) {
         a.total_balance.amount -= a.balances.begin()->balance.amount;
      }else {
         a.total_balance.amount = 0;
      }
      a.balances.erase(a.balances.begin());
   });

   if(target->balances.size() > 0 && target->total_balance.amount > 0)
   {
      SEND_INLINE_ACTION( *this, unlock, { {st.issuer, "active"_n} },
                     { owner, sym }
      );
   } else {
      locked_acnts.erase( target );
   }
}

void token::sub_balance( const name& owner, const asset& value ) {
   accounts from_acnts( get_self(), owner.value );

   const auto& from = from_acnts.get( value.symbol.code().raw(), "no balance object found" );
   check( from.balance.amount >= value.amount, "overdrawn balance" );

   from_acnts.modify( from, owner, [&]( auto& a ) {
         a.balance -= value;
      });
}

void token::add_balance( const name& owner, const asset& value, const name& ram_payer )
{
   accounts to_acnts( get_self(), owner.value );
   auto to = to_acnts.find( value.symbol.code().raw() );
   if( to == to_acnts.end() ) {
      to_acnts.emplace( ram_payer, [&]( auto& a ){
        a.balance = value;
      });
   } else {
      to_acnts.modify( to, same_payer, [&]( auto& a ) {
        a.balance += value;
      });
   }
}

void token::open( const name& owner, const symbol& symbol, const name& ram_payer )
{
   require_auth( ram_payer );

   check( is_account( owner ), "owner account does not exist" );

   auto sym_code_raw = symbol.code().raw();
   stats statstable( get_self(), sym_code_raw );
   const auto& st = statstable.get( sym_code_raw, "symbol does not exist" );
   check( st.supply.symbol == symbol, "symbol precision mismatch" );

   accounts acnts( get_self(), owner.value );
   auto it = acnts.find( sym_code_raw );
   if( it == acnts.end() ) {
      acnts.emplace( ram_payer, [&]( auto& a ){
        a.balance = asset{0, symbol};
      });
   }
}

void token::close( const name& owner, const symbol& symbol )
{
   require_auth( owner );
   accounts acnts( get_self(), owner.value );
   auto it = acnts.find( symbol.code().raw() );
   check( it != acnts.end(), "Balance row already deleted or never existed. Action won't have any effect." );
   check( it->balance.amount == 0, "Cannot close because the balance is not zero." );
   acnts.erase( it );
}

void token::blacklistadd( const name&  account, const asset& token_min ) {
   require_auth( get_self() );

   print(token_min, "\n");
   blacklists blacklist( get_self(), account.value );

   check( blacklist.find( account.value ) == blacklist.end(),
                  " account alreaded blacklisted" );

   blacklist.emplace( get_self(), [&](auto& bl) {
      bl.account = account;
      bl.tokens_blocked = token_min;
   });
}


void token::blacklistrm( const name&  account ) {
   require_auth( get_self() );

   blacklists blacklist( get_self(), account.value );

   auto blacklist_itr = blacklist.find( account.value );

   check( blacklist_itr != blacklist.end(),
                  " account not on blacklist");

   const auto& blacklisted_account = *blacklist_itr;

   blacklist.erase( blacklisted_account );
}
} /// namespace eosio
