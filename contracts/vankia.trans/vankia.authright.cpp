namespace vankia
{

//1.采集确权：MD5 HASH 采集确权
//2.数据收益权确权：MD5 HASH 数据源帐号 确权类型
//3.数据收益权确权：MD5 HASH 购买帐号 确权类型
//4.数据转让权确权：MD5 HASH 购买帐号 确权类型
// using ensurerightreg::authright_table;
/// @abi action
void trans::ensureright(name agent, name platform, asset quantity, string data_md5,string data_ipfs, name ensure_right_account, string ensure_right_class)
{
  //获取授权，如果没有授权，Action调用会中止，事务会回滚
  require_auth(agent);

  //实例化address数据表（multi_index），参数用于建立对表的访问权限
  //如果访问自己的合约则具有读写权限，访问其他人的合约则具有只读权限
  ensurergt_list ensurergtlst(get_self(), get_self().value);

  //multi_index的find函数通过主键（primary_key）查询数据，返回迭代器itr
  //auto关键字会自动匹配类型
  auto iterator = ensurergtlst.find(agent.value);
  if (iterator == ensurergtlst.end())
  {
      iterator = ensurergtlst.emplace(get_self(), [&](auto &tmp_record) {
          tmp_record.ensureaccount = agent;
          tmp_record.totalrights = 1;
          tmp_record.last_update_time = time_point_sec();
      });
  }
  else
  {
      ensurergtlst.modify(iterator, get_self(), [&](auto &tmp_record) {
          tmp_record.totalrights += 1;
          tmp_record.last_update_time = time_point_sec();
      });
  }
  
  // it can tranfer from agent to platform,but agent cannot equal platform
  if(agent != platform){
    action(
      permission_level{ agent, "active"_n },
      "eosio.token"_n, "transfer"_n,
      std::make_tuple(agent, platform, quantity, std::string("vankia.trans ensureright"))
    ).send();
  }
}


// using authright::authright_table;
/// @abi action
void trans::authrightreg(const uint64_t id, const name agent, const string ipfsvalue, const string memo, const name producer)
{
  checksum256 cksum256ipfskey;
  //获取授权，如果没有授权，Action调用会中止，事务会回滚
  require_auth(agent);

  //实例化address数据表（multi_index），参数用于建立对表的访问权限
  //如果访问自己的合约则具有读写权限，访问其他人的合约则具有只读权限
  authright_list authrighttbl(get_self(), get_self().value);

  //multi_index的find函数通过主键（primary_key）查询数据，返回迭代器itr
  //auto关键字会自动匹配类型
  auto itr = authrighttbl.find(id);
  //如果判断条件不成立，则终止执行并打印错误信息
  check(itr == authrighttbl.end(), "primary key id for account already exists");

  //由于IPFS生成的HASH值为46bytes，为了增加索引便于查找，调用sha256转换。
  cksum256ipfskey = sha256(ipfsvalue.c_str(), IPFS_HASH_LENGHT);

  auto ipfskeyidx = authrighttbl.get_index<"byipfskey"_n>();
  auto itr2 = ipfskeyidx.find(authrightlst::get_ipfskey(cksum256ipfskey));
  //如果判断条件不成立，则终止执行并打印错误信息
  check(itr2 == ipfskeyidx.end(), "ipfs sha256 for account already exists");

  authrighttbl.emplace(get_self(), [&](auto &ar) {
    ar.pk_id = id;
    ar.agent = agent;
    ar.ipfskey = cksum256ipfskey;
    ar.ipfsvalue = ipfsvalue;
    ar.memo = memo;
    ar.producer = producer;
    //获取当前时间
    ar.regdate = current_time_point().sec_since_epoch();
  });
}

} // namespace vankia

// EOSIO_ABI(vankia::authright, (reg))
