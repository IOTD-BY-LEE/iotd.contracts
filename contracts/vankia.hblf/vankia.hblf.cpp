#include <eosio.token/eosio.token.hpp>
#include <eosio.system/native.hpp>
#include "vankia.hblf.hpp"
namespace vankia
{
using namespace eosio;
using namespace std;
using namespace eosiosystem;
/// @abi action 
void hblf::addstudent(string studentCode, string userId, string studentname, string status, vector<keypair> contentmap)
{

}

/// @abi action 
void hblf::modstudent(string studentCode, string userId, string studentname, string status, vector<keypair> contentmap)
{

}

/// @abi action 
void hblf::delstudent(string studentCode, string userId, string studentname, string status, vector<keypair> contentmap)
{

}

/// @abi action 
void hblf::addteachbase(string teachCode, string userId, string teachname, string status, vector<keypair> contentmap)
{

}

/// @abi action 
void hblf::modteachbase(string teachCode, string userId, string teachname, string status, vector<keypair> contentmap)
{

}

/// @abi action 
void hblf::delteachbase(string teachCode, string userId, string teachname, string status, vector<keypair> contentmap)
{

}

} // namespace vankia

EOSIO_DISPATCH(vankia::hblf, (addstudent)(modstudent)(delstudent)(addteachbase)(modteachbase)(delteachbase))
