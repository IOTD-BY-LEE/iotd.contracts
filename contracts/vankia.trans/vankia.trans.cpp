#include <eosio.token/eosio.token.hpp>
#include <eosio.system/native.hpp>
#include "vankia.trans.hpp"
#include "vankia.authright.cpp"
namespace vankia
{
using namespace eosio;
using namespace std;
using namespace eosiosystem;
void trans::deposit(name from, vector<account_record_content> content)
{
    require_auth(from);

    vector<account_record> acc_record;

    for (auto content_itr = content.begin(); content_itr != content.end(); ++content_itr)
    {
        account_record i_proposal = account_record(from, *content_itr);
        acc_record.push_back(i_proposal);
    }
    
    // check the asset of every accout,check the asset veroflow
    asset temp_asset(0,symbol("VKT",4));
    for (auto record_itr = acc_record.begin(); record_itr != acc_record.end(); ++record_itr)
    {
       check(symbol("VKT",4).code().raw() == record_itr->assets.symbol.code().raw(), "Deposit asset symbol error");
       temp_asset += record_itr->assets;
       check(record_itr->assets.is_valid(),"Deposit asset error");
       check(temp_asset.is_valid(),"Deposit asset error");
       check(from!=record_itr->person_name,"Deposit cannot to self");
    }

    // check the asset of from is enough,check the asset veroflow
    asset from_banlance  = token::get_balance("eosio.token"_n,from,symbol("VKT", 4).code());
    check(from_banlance.is_valid(),"Deposit asset error");
    check(from_banlance>=temp_asset,"Deposit balance of from account is not enough");

    for (auto record_itr = acc_record.begin(); record_itr != acc_record.end(); ++record_itr)
    {
        //根据record_itr更新table
        account_list list(get_self(), record_itr->person_name.value);
        auto itr = list.find(record_itr->person_name.value);
        if (itr == list.end())
        {
            itr = list.emplace(from, [&](auto &tmp_record) {
                tmp_record.to = record_itr->person_name;
                tmp_record.assets = record_itr->assets;
                tmp_record.flag = BOOL_POSITIVE;
                tmp_record.account_time = current_time_point().sec_since_epoch();
            });
        }
        else
        {
            list.modify(itr, same_payer, [&](auto &tmp_record) {
                check(BOOL_POSITIVE == itr->flag || BOOL_NAGETIVE == itr->flag, "invalid bool_positive");
                check(symbol("VKT",4).code().raw() == itr->assets.symbol.code().raw(), "Deposit asset symbol error");
                check(record_itr->assets.symbol.code().raw() == itr->assets.symbol.code().raw(), "invalid bool_positive");
                tmp_record.account_time = current_time_point().sec_since_epoch();
                if (BOOL_POSITIVE == itr->flag)
                {
                    check(tmp_record.assets < (tmp_record.assets + record_itr->assets), "Deposit asset overflow");
                    tmp_record.assets += record_itr->assets;
                }
                else
                {
                    tmp_record.flag = tmp_record.assets < record_itr->assets ? BOOL_POSITIVE : BOOL_NAGETIVE;
                    tmp_record.assets = tmp_record.assets < record_itr->assets ? (record_itr->assets - tmp_record.assets) : (tmp_record.assets - record_itr->assets);
                }
            });
        }
        std::string strMemo = "vankia.trans deposit";
        INLINE_ACTION_SENDER(eosio::token, transfer)( "eosio.token"_n, {from,"active"_n},
         { from, record_itr->person_name, record_itr->assets, strMemo } );
    }
    return;
}


void trans::batchtransf(name from, vector<account_record_content> content, string trading_data_txid, string trading_class)
{
    require_auth(from);

    vector<account_record> acc_record;

    for (auto content_itr = content.begin(); content_itr != content.end(); ++content_itr)
    {
        account_record i_proposal = account_record(from, *content_itr);
        acc_record.push_back(i_proposal);
    }
    
    // check the asset of every accout,check the asset veroflow
    asset temp_asset(0,symbol("VKT",4));
    for (auto record_itr = acc_record.begin(); record_itr != acc_record.end(); ++record_itr)
    {
       check(symbol("VKT",4).code().raw() == record_itr->assets.symbol.code().raw(), "Deposit asset symbol error");
       temp_asset += record_itr->assets;
       check(record_itr->assets.is_valid(),"Deposit asset error");
       check(temp_asset.is_valid(),"Deposit asset error");
       check(from!=record_itr->person_name,"Deposit cannot to self");
    }

    // check the asset of from is enough,check the asset veroflow
    asset from_banlance  = token::get_balance("eosio.token"_n,from,symbol("VKT", 4).code());
    check(from_banlance.is_valid(),"Deposit asset error");
    check(from_banlance>=temp_asset,"Deposit balance of from account is not enough");

    for (auto record_itr = acc_record.begin(); record_itr != acc_record.end(); ++record_itr)
    {
        //根据record_itr更新table
        account_list list(get_self(), record_itr->person_name.value);
        auto itr = list.find(record_itr->person_name.value);
        if (itr == list.end())
        {
            itr = list.emplace(from, [&](auto &tmp_record) {
                tmp_record.to = record_itr->person_name;
                tmp_record.assets = record_itr->assets;
                tmp_record.flag = BOOL_POSITIVE;
                tmp_record.account_time = current_time_point().sec_since_epoch();
            });
        }
        else
        {
            list.modify(itr, same_payer, [&](auto &tmp_record) {
                check(BOOL_POSITIVE == itr->flag || BOOL_NAGETIVE == itr->flag, "invalid bool_positive");
                check(symbol("VKT",4).code().raw() == itr->assets.symbol.code().raw(), "Deposit asset symbol error");
                check(record_itr->assets.symbol.code().raw() == itr->assets.symbol.code().raw(), "invalid bool_positive");
                tmp_record.account_time = current_time_point().sec_since_epoch();
                if (BOOL_POSITIVE == itr->flag)
                {
                    check(tmp_record.assets < (tmp_record.assets + record_itr->assets), "Deposit asset overflow");
                    tmp_record.assets += record_itr->assets;
                }
                else
                {
                    tmp_record.flag = tmp_record.assets < record_itr->assets ? BOOL_POSITIVE : BOOL_NAGETIVE;
                    tmp_record.assets = tmp_record.assets < record_itr->assets ? (record_itr->assets - tmp_record.assets) : (tmp_record.assets - record_itr->assets);
                }
            });
        }
        // it can tranfer from from to person_name,but from cannot equal person_name
        std::string strMemo = "vankia.trans deposit";
        if(from != record_itr->person_name){
            INLINE_ACTION_SENDER(eosio::token, transfer)( "eosio.token"_n, {from,"active"_n},
            { from, record_itr->person_name, record_itr->assets, strMemo } );
        }
    }
    return;
}

void trans::withdraw(name from, asset assets)
{
    require_auth(from);
    account_list list(get_self(), from.value);
    withdraw_list list_withdraw(get_self(), from.value);
    auto itr = list.find(from.value);
    check(itr != list.end(), "withdraw account doesn't exist");
    check(assets < itr->assets, "withdraw balance  doesn't enough");
    check(BOOL_POSITIVE == itr->flag, "withdraw balance  doesn't enough");
    check(assets.symbol.code().raw() == itr->assets.symbol.code().raw(), "withdraw currency not match");
    list.modify(itr, same_payer, [&](auto &tmp_record) {
        tmp_record.assets -= assets;
        tmp_record.account_time = current_time_point().sec_since_epoch();
    });
    auto iterator = list_withdraw.find(from.value);
    if (iterator == list_withdraw.end())
    {
        iterator = list_withdraw.emplace(from, [&](auto &tmp_record) {
            tmp_record.withdraw_name = from;
            tmp_record.withdraw_infor.push_back(withdraw_record_content(assets, current_time_point().sec_since_epoch()));
        });
    }
    else
    {
        list_withdraw.modify(iterator, from, [&](auto &tmp_record) {
            tmp_record.withdraw_infor.push_back(withdraw_record_content(assets, current_time_point().sec_since_epoch()));
        });
    }
    return;
}

#if 0
void accounting::get_assets(name owner)
{
  ;
}
#else
asset trans::getassets(name owner)
{
    account_list list(get_self(), owner.value);
    return list.get(owner.value).assets;
}
#endif

#if 1
void trans::listrecord(name from, uint64_t seq, string hash, vector<account_record_content> trans_list)
{
    require_auth(from);
    deposit_list list(get_self(), get_self().value);

    //查找该序列号对应的交易报是否已经存在无需保存
    auto itr = list.find(seq);
    check(itr == list.end(), "transaction has exist");

    itr = list.emplace(from, [&](auto &translist) {
        translist.from = from;
        translist.seq_no = seq;
        translist.hash = hash;
        translist.list = move(trans_list);
        translist.deposit_time = current_time_point().sec_since_epoch();
    });
    return;
}
#else
void accounting::listrecord(name from, uint64_t seq, vector<account_record_content> transList)
{
    require_auth(from);
    deposit_list trans_records(get_self(), get_self());

    //查找该序列号对应的交易报是否已经存在无需保存
    auto itr = trans_records.find(seq);
    check(itr == trans_records.end(), "transaction has exist");

    itr = trans_records.emplace(from, [&](auto &translist) {
        translist.from = from;
        translist.seq_no = seq;
        // translist.hash         = hash;
        translist.list = move(transList);
        translist.records_time = current_time();
    });
    return;
}
#endif
} // namespace vankia

EOSIO_DISPATCH(vankia::trans, (ensureright)(batchtransf)(deposit)(withdraw)(listrecord)(authrightreg))
