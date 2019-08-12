#pragma once

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/action.hpp>
#include <eosio/transaction.hpp>

#include <string>

namespace eosiosystem {
   class system_contract;
}

namespace eosio {

   using std::string;

   /**
    * @defgroup eosiotoken eosio.token
    * @ingroup eosiocontracts
    *
    * eosio.token contract
    *
    * @details eosio.token contract defines the structures and actions that allow users to create, issue, and manage
    * tokens on eosio based blockchains.
    * @{
    */
   class [[eosio::contract("eosio.token")]] token : public contract {
      public:
         using contract::contract;

         /**
          * Create action.
          *
          * @details Allows `issuer` account to create a token in supply of `maximum_supply`.
          * @param issuer - the account that creates the token,
          * @param maximum_supply - the maximum supply set for the token created.
          *
          * @pre Token symbol has to be valid,
          * @pre Token symbol must not be already created,
          * @pre maximum_supply has to be smaller than the maximum supply allowed by the system: 1^62 - 1.
          * @pre Maximum supply must be positive;
          *
          * If validation is successful a new entry in statstable for token symbol scope gets created.
          */
         [[eosio::action]]
         void create( const name&   issuer,
                      const asset&  maximum_supply);
         /**
          * Issue action.
          *
          * @details This action issues to `to` account a `quantity` of tokens.
          *
          * @param to - the account to issue tokens to, it must be the same as the issuer,
          * @param quntity - the amount of tokens to be issued,
          * @param memo - the memo string that accompanies the token issue transaction.
          */
         [[eosio::action]]
         void issue( const name& to, const asset& quantity, const string& memo );
         /**
          * Issuelock action.
          *
          * @details This action issues to `to` account a `quantity` of tokens.
		  * And the ability to lock a 'lockquantity' of tokens and unlock a period 'unlock_delay_sec'.
          *
          * @param to - the account to issue tokens to, it must be the same as the issuer,
          * @param quntity - the amount of tokens to be issued,
          * @param memo - the memo string that accompanies the token issue transaction.
		  * @param lockquantity - the amount of tokens to be locked,
		  * @param unlock_delay_sec - the POSIX time of seconds to be locked,
          */
         [[eosio::action]]
         void issuelock( const name& to, 
                         const asset& quantity, 
                         const string& memo, 
                         const asset& lockquantity, 
                         const uint32_t& unlock_delay_sec );
         /**
          * Retire action.
          *
          * @details The opposite for create action, if all validations succeed,
          * it debits the statstable.supply amount.
          *
          * @param quantity - the quantity of tokens to retire,
          * @param memo - the memo string to accompany the transaction.
          */
         [[eosio::action]]
         void retire( const asset& quantity, const string& memo );

         /**
          * Transfer action.
          *
          * @details Allows `from` account to transfer to `to` account the `quantity` tokens.
          * One account is debited and the other is credited with quantity tokens.
          *
          * @param from - the account to transfer from,
          * @param to - the account to be transferred to,
          * @param quantity - the quantity of tokens to be transferred,
          * @param memo - the memo string to accompany the transaction.
          */
         [[eosio::action]]
         void transfer( const name&    from,
                        const name&    to,
                        const asset&   quantity,
                        const string&  memo );
         /**
          * Open action.
          *
          * @details Allows `ram_payer` to create an account `owner` with zero balance for
          * token `symbol` at the expense of `ram_payer`.
          *
          * @param owner - the account to be created,
          * @param symbol - the token to be payed with by `ram_payer`,
          * @param ram_payer - the account that supports the cost of this action.
          *
          * More information can be read [here](https://github.com/EOSIO/eosio.contracts/issues/62)
          * and [here](https://github.com/EOSIO/eosio.contracts/issues/61).
          */
         [[eosio::action]]
         void open( const name& owner, const symbol& symbol, const name& ram_payer );

         /**
          * Close action.
          *
          * @details This action is the opposite for open, it closes the account `owner`
          * for token `symbol`.
          *
          * @param owner - the owner account to execute the close action for,
          * @param symbol - the symbol of the token to execute the close action for.
          *
          * @pre The pair of owner plus symbol has to exist otherwise no action is executed,
          * @pre If the pair of owner plus symbol exists, the balance has to be zero.
          */
         [[eosio::action]]
         void close( const name& owner, const symbol& symbol );

         /**
          * lock action.
          *
          * @details the ability to lock a 'quantity' of tokens and unlock a period 'unlock_delay_sec'.
          *
          * @param owner - the owner account to execute the close action for,
          * @param quantity - the symbol of the token to execute the lock action for.
		  * @param unlock_delay_sec - the POSIX time of seconds to be locked,
          *
          * @pre The pair of owner plus symbol has to exist otherwise no action is executed,
          * @pre If the pair of owner plus symbol exists, the balance has to be zero.
          */
         [[eosio::action]]
         void lock( const name& owner, const asset& quantity, const uint32_t& unlock_delay_sec );
         /**
          * unlock action.
          *
          * @details This action is the opposite for lock, it unlock the account `owner`
          * for token `symbol`.
          *
          * @param owner - the owner account to execute the close action for,
          * @param sym_code - the sym_code of the token to execute the unlock action for.
          *
          * @pre The pair of owner plus symbol has to exist otherwise no action is executed,
          * @pre If the pair of owner plus symbol exists, the balance has to be zero.
          */
         [[eosio::action]]      
         void unlock( const name& owner, const symbol_code& sym_code );
         /**
          * dounlock action.
          *
          * @details This action is the opposite for lock, it unlock the account `owner`
          * for token `symbol`.
          *
          * @param owner - the owner account to execute the close action for,
          * @param sym_code - the sym_code of the token to execute the close action for.
          *
          * @pre The pair of owner plus symbol has to exist otherwise no action is executed,
          * @pre If the pair of owner plus symbol exists, the balance has to be zero.
          */
         [[eosio::action]]
         void dounlock( const name& owner, const symbol_code& sym_code );
         /**
          * blacklistadd action.
          *
          * @details This action is the blacklist for the account `owner`
          * this account can only tranfer amount of limit 'token_min' quantity tokens.
          *
          * @param owner - the owner account to execute the blacklistadd action for,
          * @param token_min - the amount of limit tokens to be ability to transfer,
          *
          * @pre The pair of owner plus symbol has to exist otherwise no action is executed,
          * @pre If the pair of owner plus symbol exists, the balance has to be zero.
          */
         [[eosio::action]]
         void blacklistadd( const name& account, const asset& token_min );
         /**
          * blacklistrm action.
          *
          * @details This action is the opposite for blacklistadd, it blacklistrm the account `owner`
          * for transfer token.
          *
          * @param owner - the owner account to execute the blacklistrm action for,
          *
          * @pre The pair of owner plus symbol has to exist otherwise no action is executed,
          * @pre If the pair of owner plus symbol exists, the balance has to be zero.
          */
         [[eosio::action]]
         void blacklistrm( const name& account );
		 
         /**
          * Get supply method.
          *
          * @details Gets the supply for token `sym_code`, created by `token_contract_account` account.
          *
          * @param token_contract_account - the account to get the supply for,
          * @param sym_code - the symbol to get the supply for.
          */
         static asset get_supply( const name& token_contract_account, const symbol_code& sym_code )
         {
            stats statstable( token_contract_account, sym_code.raw() );
            const auto& st = statstable.get( sym_code.raw() );
            return st.supply;
         }

         /**
          * Get balance method.
          *
          * @details Get the balance for a token `sym_code` created by `token_contract_account` account,
          * for account `owner`.
          *
          * @param token_contract_account - the token creator account,
          * @param owner - the account for which the token balance is returned,
          * @param sym_code - the token for which the balance is returned.
          */
         static asset get_balance( const name& token_contract_account, const name& owner, const symbol_code& sym_code )
         {
            accounts accountstable( token_contract_account, owner.value );
            const auto& ac = accountstable.get( sym_code.raw() );
            return ac.balance;
         }

         using create_action = eosio::action_wrapper<"create"_n, &token::create>;
         using issue_action = eosio::action_wrapper<"issue"_n, &token::issue>;
         using retire_action = eosio::action_wrapper<"retire"_n, &token::retire>;
         using transfer_action = eosio::action_wrapper<"transfer"_n, &token::transfer>;
         using open_action = eosio::action_wrapper<"open"_n, &token::open>;
         using issuelock_action = eosio::action_wrapper<"issuelock"_n, &token::issuelock>;
         using lock_action = eosio::action_wrapper<"lock"_n, &token::lock>;
         using unlock_action = eosio::action_wrapper<"unlock"_n, &token::unlock>;
         using dounlock_action = eosio::action_wrapper<"dounlock"_n, &token::dounlock>;
         using blacklistadd_action = eosio::action_wrapper<"blacklistadd"_n, &token::blacklistadd>;
         using blacklistrm_action = eosio::action_wrapper<"blacklistrm"_n, &token::blacklistrm>;
      private:
         struct [[eosio::table]] account {
            asset    balance;

            uint64_t primary_key()const { return balance.symbol.code().raw(); }
         };

         struct [[eosio::table]] currency_stats {
            asset    supply;
            asset    max_supply;
            name     issuer;

            uint64_t primary_key()const { return supply.symbol.code().raw(); }
         };

         struct frozen_balance {
            asset balance;
            uint32_t unlock_delay_sec;
            time_point_sec unlock_request_time;
            time_point_sec unlock_execute_time;
         };

         struct [[eosio::table]] locked_account {
            asset total_balance;
            std::vector<frozen_balance> balances;

            uint64_t primary_key()const { return total_balance.symbol.code().raw(); }
         };

         /// @abi table blacklist i64
         struct [[eosio::table]] blacklist_row {
            name account;
            asset tokens_blocked;

            uint64_t primary_key() const { return account.value; }
         };


         typedef eosio::multi_index< "accounts"_n, account > accounts;
         typedef eosio::multi_index< "stat"_n, currency_stats > stats;
         typedef eosio::multi_index< "locked"_n, locked_account > locked_accounts;
         typedef eosio::multi_index< "blacklist"_n, blacklist_row> blacklists;

         void sub_balance( const name& owner, const asset& value );
         void add_balance( const name& owner, const asset& value, const name& ram_payer );
         static bool sort_by_execute_time (frozen_balance i,frozen_balance j) {
            return (i.unlock_execute_time < j.unlock_execute_time);
         }
   };
   /** @}*/ // end of @defgroup eosiotoken eosio.token
} /// namespace eosio
