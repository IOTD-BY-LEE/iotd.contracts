#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/print.hpp>
#include <eosio/crypto.hpp>
#include <string>

namespace vankia
{

using namespace eosio;
using namespace std;

struct keypair
{

    keypair() {}
    keypair(string str_key, string str_value)
    {
        key = str_key;
        value = str_value;
    }
    string key;
    string value;

    EOSLIB_SERIALIZE(keypair, (key)(value))
};

class [[eosio::contract("vankia.hblf")]] hblf : public vankia::contract
{
  public:
    using contract::contract;

    [[eosio::action]]
    void addstudent(string studentCode, string userId, string studentname, string status, vector<keypair> contentmap);
    [[eosio::action]]
    void modstudent(string studentCode, string userId, string studentname, string status, vector<keypair> contentmap);
    [[eosio::action]]
    void delstudent(string studentCode, string userId, string studentname, string status, vector<keypair> contentmap);
    [[eosio::action]]
    void addteachbase(string teachCode, string userId, string teachname, string status, vector<keypair> contentmap);
    [[eosio::action]]
    void modteachbase(string teachCode, string userId, string teachname, string status, vector<keypair> contentmap);
    [[eosio::action]]
    void delteachbase(string teachCode, string userId, string teachname, string status, vector<keypair> contentmap);

  private:

};

} // namespace vankia
