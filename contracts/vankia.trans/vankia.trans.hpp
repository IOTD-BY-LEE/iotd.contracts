#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/print.hpp>
#include <eosio/crypto.hpp>
#include <string>

namespace vankia
{

using namespace eosio;
using namespace std;

typedef uint8_t bool_positive;
typedef uint64_t time64;

#define BOOL_POSITIVE 1
#define BOOL_NAGETIVE 0
#define IPFS_HASH_LENGHT 46

enum ensure_right_class : uint8_t  // 确权类型
{
    EMPTY = 0,        // 空
    COLLECTION = 1,   // 采集确权
    INCOME = 2,       // 数据收益权确权
    USUFRUCT = 3,     // 数据使用权确权
    TRANSFER = 4,     // 数据转让权确权
    REWARD = 5,       // 平台任务奖励确权
    OTHER = 99        // 其他
};

struct account_record_content
{

    account_record_content() {}

    asset assets;
    name person_name;

    EOSLIB_SERIALIZE(account_record_content, (assets)(person_name))
};

struct account_record : public account_record_content
{

    account_record() {}

    account_record(name from_name, account_record_content content)
    {
        from = from_name;
        person_name = content.person_name;
        assets = content.assets;
    }
    name from;
    EOSLIB_SERIALIZE_DERIVED(account_record, account_record_content, (from))
};

struct withdraw_record_content
{

    withdraw_record_content() {}
    withdraw_record_content(asset con_assets, time64 con_record_time)
    {

        assets = con_assets;
        record_time = con_record_time;
    }
    asset assets;
    time64 record_time;
    EOSLIB_SERIALIZE(withdraw_record_content, (assets)(record_time))
};

class [[eosio::contract("vankia.trans")]] trans : public vankia::contract
{
  public:
    using contract::contract;

    // trans(name self) : contract(self) {}
    [[eosio::action]]
    void deposit(name from, vector<account_record_content> content);
    [[eosio::action]]
    void batchtransf(name from, vector<account_record_content> content, string trading_data_txid, string trading_class);
    [[eosio::action]]
    void withdraw(name from, asset assets);
    [[eosio::action]]
    inline asset getassets(name owner);
    [[eosio::action]]
    void listrecord(name from, uint64_t seq, string hash, vector<account_record_content> trans_list);
    [[eosio::action]]
    void authrightreg(const uint64_t id, const name agent, const string ipfsvalue, const string memo, const name producer);
    [[eosio::action]]
    void ensureright(name agent, name platform, asset quantity, string data_md5,string data_ipfs, name ensure_right_account, string ensure_right_class);

  private:

    // @abi table
    struct [[eosio::table]] accountlist
    {
        name to;
        asset assets;
        bool_positive flag;
        time64 account_time;

        uint64_t primary_key() const { return to.value; }
    };
    // @abi table
    struct [[eosio::table]] depositlist
    {
        uint64_t seq_no; //交易序号
        name from;
        string hash;
        vector<account_record_content> list;
        time64 deposit_time;
        uint64_t primary_key() const { return seq_no; }
    };
    // @abi table
    struct [[eosio::table]] withdrawlist
    {
        name withdraw_name;
        vector<withdraw_record_content> withdraw_infor;
        uint64_t primary_key() const { return withdraw_name.value; }
    };

    /// @abi table
    struct [[eosio::table]] authrightlst
    {
        uint64_t pk_id;        /// 流水ID，主键唯一的无符号64位整型
        name agent;    /// 代理商名称
        checksum256 ipfskey;   /// IPFS对应的sha256转换后的key
        string ipfsvalue;      /// IPFS对应的sha256转换后的value
        string memo;           /// 备注
        name producer; /// 生产数据的公司
        time64 regdate;        /// 登记时间

        auto primary_key() const { return pk_id; }
        fixed_bytes<32> get_ipfskey() const { return get_ipfskey(ipfskey); }

        static fixed_bytes<32> get_ipfskey(const checksum256 &ipfskey)
        {
            const uint64_t *p64 = reinterpret_cast<const uint64_t *>(&ipfskey);
            //print<32>(key256::make_from_word_sequence<uint64_t>(p64[0], p64[1], p64[2], p64[3]));
            return fixed_bytes<32>::make_from_word_sequence<uint64_t>(p64[0], p64[1], p64[2], p64[3]);
        }
    };

        /// @abi table
    struct [[eosio::table]] ensurergtlst
    {
        name                ensureaccount;        /// 确权用户，主键唯一的无符号8位整型
        uint64_t            totalrights;          /// 该用户总计确权数量
        time_point_sec      last_update_time;     /// 最后确权时间

        auto primary_key() const { return ensureaccount.value; }
    };

    typedef eosio::multi_index< "accountlist"_n, accountlist> account_list;
    typedef eosio::multi_index< "depositlist"_n, depositlist> deposit_list;
    typedef eosio::multi_index< "withdrawlist"_n, withdrawlist> withdraw_list;
    typedef eosio::multi_index< "authrightlst"_n, authrightlst, indexed_by<"byipfskey"_n, const_mem_fun<authrightlst, fixed_bytes<32>, &authrightlst::get_ipfskey>>> authright_list;
    typedef eosio::multi_index< "ensurergtlst"_n, ensurergtlst> ensurergt_list;
};

} // namespace vankia
