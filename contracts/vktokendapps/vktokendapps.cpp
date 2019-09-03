#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <eosio/crypto.hpp>
#include <eosio/time.hpp>
#include <vector>



#define TOKEN_SYMBOL symbol("VKT", 4)
#define GENESIS_TIME 1557849600
#define TYPE_POST 1
#define TYPE_COMMENT 2

using namespace eosio;
using std::vector;

/**
 * 随机数生成器
 */
class random_gen {
private:
    static random_gen instance;
    uint64_t seed = 0;

public:
    static random_gen &get_instance(name account) {
        instance.seed = current_time_point().sec_since_epoch() + account.value;
        return instance;
    }

    uint32_t range(uint32_t to) {
        checksum256 result = sha256((char *) &seed, sizeof(seed));
        auto arr = result.extract_as_byte_array();
        seed = arr[1];
        seed <<= 32;
        seed |= arr[0];
        return (uint32_t) (seed % to);
    }

    uint32_t range(uint32_t from, uint32_t to) {
        if (from == to) {
            return from;
        } else if (from < to) {
            return range(to - from + 1) + from;
        }
        return range(to);
    }
};

/**
 * 合约类
 */
CONTRACT vktokendapps : public eosio::contract {

public:
    vktokendapps(name self, name first_receiver, datastream<const char *> ds) :
            contract(self, first_receiver, ds), users(self, self.value) ,administer(self,self.value)
            {


            }

    /**
     * 每日签到领币
     */
    ACTION reward(name account) {
        require_auth(get_self());


        uint8_t reward_times = 1;                      //奖励倍数，默认为1倍
        auto adm_itr = administer.find(name("xinyue").value);
        if(adm_itr != administer.end())
        {
            reward_times = adm_itr->times;
        }

        auto itr = users.find(account.value);
        if (itr == users.end()) {
            itr = users.emplace(get_self(), [&](auto &user) {
                user.account = account;
                user.balance = asset(0, TOKEN_SYMBOL);
                user.last_reward_amount = asset(0, TOKEN_SYMBOL);

                user.sign_in_accumulate_days = 0;     //连续签到天数，这里初始给零，虽然走到这个逻辑点已经说明用户第一次签到了，但我们在后面的逻辑中给＋1
                for(int i = 0;i < 7;++i)
                {
                    user.recent_days.push_back(time_point_sec(0));
                    user.recent_rewards.push_back(asset(0, TOKEN_SYMBOL));
                }
                user.follow_num = 0;
                user.fans_num = 0;
                user.post_num = 0;
                user.like_num = 0;
                user.last_reward_time = time_point_sec(0);
                user.last_like_time = time_point_sec(0);
                user.like_times = 0;
            });

            auto tokens = get_reward(account, 1, reward_times);                   //第一次签到获得奖励
            users.modify(itr,get_self(), [&](auto &user) {
                user.sign_in_accumulate_days += 1;                                         //连续签到天数,这里应该是第一天
                user.balance = asset(tokens, TOKEN_SYMBOL);
                user.last_reward_amount = asset(tokens, TOKEN_SYMBOL);

                user.last_reward_time = time_point_sec(current_time_point());               //最用最后的签到时间，其实可以在签到数组中取
                user.recent_days[0] = user.last_reward_time;                                //存储用户最近七次的签到时间
                user.recent_rewards[0] = asset(tokens, TOKEN_SYMBOL);                       //用户最近七次的签到奖励

            });

        }

        //判断是否存在同一天多次签到
        if (!is_today(itr->last_reward_time.sec_since_epoch()))
        {
            //判断签到累积是否有中断
            if(discontinue_sign_in(itr->last_reward_time.sec_since_epoch()))
            {
                //如果有中断的话
                users.modify(itr,get_self(),[&](auto& user) {
                    user.last_reward_time = time_point_sec(current_time_point());        //将其中签到时间更新
                    user.sign_in_accumulate_days = 1;                                    //用户连续签到天数改为一天
                });

                auto tokens = get_reward(account,itr->sign_in_accumulate_days,reward_times); //根据签到时间生成奖励

                users.modify(itr,get_self(),[&](auto& user){
                   user.last_reward_amount = asset(tokens,TOKEN_SYMBOL);                 //最后一次签到所得
                   user.balance += asset(tokens,TOKEN_SYMBOL);                           //代币总金额
                   user.recent_rewards[0] = asset(tokens,TOKEN_SYMBOL);                         //由于中断签到，所以清除其签到奖励记录，并将今日奖励放入首位
                   user.recent_days[0] = user.last_reward_time;                           //由于中断签到，所以清除其签到日期记录，并将今日日期放入首位
                   for(int i = 1;i < 7; ++i)
                   {
                       user.recent_rewards[i] = asset(0,TOKEN_SYMBOL);                          //清除其签到奖励记录
                       user.recent_days[i] = time_point_sec(0);                                           //清除其签到日期记录
                   }
                });
            }
            else        //如果没有中断
            {
                //没有中断的话先更新其累计签到天数
                users.modify(itr,get_self(),[&](auto &user){
                    user.sign_in_accumulate_days += 1;
                    user.last_reward_time = time_point_sec(current_time_point());        //更新签到时间
                });
                //计算累计应得奖励
                auto tokens = get_reward(account,itr->sign_in_accumulate_days,reward_times);

                users.modify(itr,get_self(),[&](auto &user){
                    user.last_reward_amount = asset(tokens,TOKEN_SYMBOL);
                    user.balance += asset(tokens,TOKEN_SYMBOL);
                });

                if( itr->sign_in_accumulate_days <= 7) //如果签到时间不足七天
                {
                    users.modify(itr,get_self(),[&](auto& user){
                        user.recent_rewards[ user.sign_in_accumulate_days - 1] = asset(tokens,TOKEN_SYMBOL);
                        user.recent_days[ user.sign_in_accumulate_days - 1] = user.last_reward_time;
                    });
                }
                else           //累计签到时间超过7天
                {
                    users.modify(itr,get_self(),[&](auto &user){
                        for(int i = 0;i < 6;++i)
                        {
                            user.recent_rewards[i] = user.recent_rewards[i + 1];
                            user.recent_days[i] = user.recent_days[i + 1];
                        }
                        user.recent_rewards[6] = asset(tokens,TOKEN_SYMBOL);
                        user.recent_days[6] = user.last_reward_time;

                    });
                }


            }







        }
    }

    //传进来一个倍数
    ACTION startactive(uint8_t times)        //开启活动
    {
        require_auth("xinyue"_n);
        auto adm_itr = administer.find(name("xinyue").value);
        if(adm_itr == administer.end())
        {
            adm_itr = administer.emplace(get_self(),[&](auto& admin){
                admin.account = "xinyue"_n;
                admin.times = 1;
            });
        }
        administer.modify(adm_itr,get_self(),[&](auto& admin){
            admin.times = times;
        });
    }
    /**
     * 发微文
     */
    ACTION post(name author, const std::string &content, uint32_t attachtype, const std::string &attachment) {
        require_auth(author);

        check_user(author);
        if (attachtype != 0) {
            check(attachment != "", "attachment can not be empty");
        }

        post_t posts(get_self(), get_self().value);
        posts.emplace(author, [&](auto &post) {
            post.id = posts.available_primary_key();
            post.author = author;
            post.content = content;
            post.attachtype = attachtype;
            post.attachment = attachment;
            post.time = time_point_sec(current_time_point());
            post.balance = asset(0, TOKEN_SYMBOL);
            post.like_num = 0;
            post.comment_num = 0;
        });

        auto itr = users.find(author.value);
        users.modify(itr, same_payer, [&](auto &user) {
            user.post_num++;
        });
    }

    /**
     * 评论
     */
    ACTION comment(name author, const std::string &content, uint64_t post_id,
                   bool has_parent, uint64_t pid, name reply_to) {

        require_auth(author);

        check_user(author);

        post_t posts(get_self(), get_self().value);
        auto itr = posts.find(post_id);
        check(itr != posts.end(), "post does not exist");

        comment_t comments(get_self(), get_self().value);
        if (has_parent) {
            check(comments.find(pid) != comments.end(), "parent comment does not exist");
        }

        comments.emplace(author, [&](auto &comment) {
            comment.id = comments.available_primary_key();
            comment.post_id = post_id;
            comment.author = author;
            comment.time = time_point_sec(current_time_point());
            comment.content = content;
            comment.balance = asset(0, TOKEN_SYMBOL);
            comment.like_num = 0;
            comment.has_parent = has_parent;
            comment.pid = pid;
            comment.reply_to = reply_to;
        });

        posts.modify(itr, same_payer, [&](auto &post) {
            post.comment_num++;
        });
    }

    /**
     * 点赞
     */
    ACTION like(name author, uint32_t type, uint64_t type_id) {
        require_auth(author);

        check_user(author);
        check(type == TYPE_POST || type == TYPE_COMMENT, "invalid like type");
        check_liked(author, type, type_id);

        if (type == TYPE_POST) {
            post_t posts(get_self(), get_self().value);
            check(posts.find(type_id) != posts.end(), "post does not exist");

            update_like_data<post_t>(author, type_id);

        } else if (type == TYPE_COMMENT) {
            comment_t comments(get_self(), get_self().value);
            check(comments.find(type_id) != comments.end(), "comment does not exist");

            update_like_data<comment_t>(author, type_id);
        }

        like_t likes(get_self(), get_self().value);
        likes.emplace(author, [&](auto &like) {
            like.id = likes.available_primary_key();
            like.type = type;
            like.type_id = type_id;
            like.author = author;
        });
    }

    /**
     * 关注
     */
    ACTION follow(name from, name to) {
        require_auth(from);

        check_user(from);
        check_user(to);

        check(from != to, "can not follow yourself");

        follow_t follows(get_self(), get_self().value);
        auto secondary = follows.get_index<"byfrom"_n>();

        for (auto itr = secondary.lower_bound(from.value); itr != secondary.upper_bound(from.value); itr++) {
            if (itr->to == to) {
                check(false, "already followed");
            }
        }

        follows.emplace(from, [&](auto &follow) {
            follow.id = follows.available_primary_key();
            follow.from = from;
            follow.to = to;
        });
    }

    /**
     * 取消关注
     */
    ACTION unfollow(name from, uint64_t id) {
        require_auth(from);

        follow_t follows(get_self(), get_self().value);
        auto itr = follows.find(id);

        check(itr != follows.end(), "follow does not exist");
        check(itr->from == from, "invalid follow");

        follows.erase(itr);
    }

    /**
     * 提现
     */
    ACTION withdraw(name account, asset quantity) {
        require_auth(account);

        auto itr = users.find(account.value);

        check(quantity.amount > 0, "withdraw amount must be positive");
        check(itr->balance >= quantity, "overdrawn balance");

        transfer_token(account, quantity);
        users.modify(itr, account, [&](auto &user) {
            user.balance -= quantity;
        });
    }

    /**
     * 充值
     */
    [[eosio::on_notify("eosio.token::transfer")]]
    void deposit(name from, name to, asset quantity, std::string memo) {
        if (to != get_self()) return;

        if (quantity.symbol == TOKEN_SYMBOL) {
            auto itr = users.find(from.value);

            if (itr != users.end()) {
                users.modify(itr, get_self(), [&](auto &user) {
                    user.balance += quantity;
                });
            }
        }
    }

private:

    /**
     * 判断指定时间是否是今天
     */
    bool is_today(uint32_t last_time) {
        return current_time_point().sec_since_epoch() / 86400 == last_time / 86400;
    }

    //判断连续签到的状态是否中断
    bool discontinue_sign_in(uint32_t last_time) {
        return (current_time_point().sec_since_epoch() / 86400 - last_time / 86400) >= 2;
    }

    /**
     * 检查用户是否存在
     */
    void check_user(name account) {
        auto itr = users.find(account.value);
        check(itr != users.end(), "user does not exist");
    }

    /**
     * 计算每日签到领币的数量
     */
    uint32_t get_reward(name account, uint32_t sum_days, uint8_t multiple = 1) {
        //account: 用户账户
        //sum_days: 用户连续签到天数，需要根据这个值酌定奖励
        //multiple: 是否开启倍率，即活动的时候会增加签到奖励

        // uint32_t seconds = current_time_point().sec_since_epoch() - GENESIS_TIME;
//    if(seconds <= 30*86400){
//      return random_gen().get_instance(account).range(10, 50) * 10000;
//    }else if(seconds > 30*86400 && seconds <= 120*86400){
//      return random_gen().get_instance(account).range(5, 20) * 10000;
//    }else if(seconds > 120*86400 && seconds <= 360*86400){
//      return random_gen().get_instance(account).range(2, 10) * 10000;
//    }
//    return random_gen().get_instance(account).range(1, 5) * 10000;



        //下面根据连续签到的天数来生成用户每天签到得到的奖励,最多只有七天，如果超过七天还是按七天算
        if (sum_days > 7)
            sum_days = 7;
        return sum_days * multiple * 10000;



    }

    /**
     * 计算每次点赞领币的数量
     */
    uint32_t get_like_reward(name account) {
        auto itr = users.find(account.value);
        auto rnum = random_gen().get_instance(account).range(100, 500);
        return itr->balance.amount * rnum * 0.00001;
    }

    /**
     * 检查是否已经点赞
     */
    void check_liked(name author, uint32_t type, uint64_t type_id) {
        bool liked = false;

        like_t likes(get_self(), get_self().value);
        auto secondary = likes.get_index<"byauthor"_n>();

        for (auto itr = secondary.lower_bound(author.value); itr != secondary.upper_bound(author.value); itr++) {
            if (itr->type == type && itr->type_id == type_id) {
                liked = true;
            }
        }

        if (liked) check(false, "already liked");
    }

    /**
     * 更新点赞相关的数据
     */
    template<typename T>
    void update_like_data(name liker, uint64_t type_id) {

        T datas(get_self(), get_self().value);
        auto itr = datas.find(type_id);
        auto liker_itr = users.find(liker.value);

        datas.modify(itr, same_payer, [&](auto &row) {
            row.like_num++;
        });
        users.modify(users.find(itr->author.value), same_payer, [&](auto &user) {
            user.like_num++;
        });

        if (liker != itr->author) {

            bool istoday = is_today(liker_itr->last_like_time.sec_since_epoch());

            if (!istoday || (istoday && liker_itr->like_times < 10)) {

                auto tokens = asset(get_like_reward(liker), TOKEN_SYMBOL);

                issue_token(tokens);
                users.modify(liker_itr, same_payer, [&](auto &user) {
                    user.balance += tokens;
                    user.last_like_time = time_point_sec(current_time_point());
                    user.like_times = istoday ? (user.like_times + 1) : 1;
                });

                issue_token(tokens);
                users.modify(users.find(itr->author.value), same_payer, [&](auto &user) {
                    user.balance += tokens;
                });
                datas.modify(itr, same_payer, [&](auto &row) {
                    row.balance += tokens;
                });
            }
        }
    }

    /**
     * 发行代币
     */
    void issue_token(asset quantity) {
        action(
                permission_level{get_self(), "active"_n},
                "eosio.token"_n,
                "issue"_n,
                std::make_tuple(get_self(), quantity, std::string("reward issue token"))
        ).send();
    }

    /**
     * 转账代币
     */
    void transfer_token(name to, asset quantity) {
        action(
                permission_level{get_self(), "active"_n},
                "eosio.token"_n,
                "transfer"_n,
                std::make_tuple(get_self(), to, quantity, std::string("reward transfer token"))
        ).send();
    }



    //用户表
    TABLE usertable {
        name account;                             //VKT账户
        asset balance;                            //代币余额
        asset last_reward_amount;                 //最后签到所得
        uint32_t sign_in_accumulate_days;         //新增：连续签到天数
        vector<asset> recent_rewards;             //新增：记录近七天内每天签到获得的奖励
        vector<time_point_sec> recent_days;       //新增：记录用户近七天内签到的时间


        uint32_t follow_num;            //关注数
        uint32_t fans_num;              //粉丝数
        uint32_t post_num;              //微文数
        uint32_t like_num;              //获赞数
        time_point_sec last_reward_time;//上次领币时间
        time_point_sec last_like_time;  //上次点赞时间
        uint32_t like_times;            //当天已点赞次数


        uint64_t primary_key() const { return account.value; }
    };

    typedef multi_index<"usertable"_n, usertable> user_t;
    user_t users;

    TABLE admintable
    {
        name account;
        uint8_t times;    //活动倍率
                          //其他活动再添加
        uint64_t primary_key() const{ return account.value; }
    };
    typedef multi_index<"admintable"_n,admintable> admin_t;
    admin_t administer;


    //微文表
    TABLE posttable {
        uint64_t id;              //自增id
        name author;              //作者
        std::string content;      //内容
        uint32_t attachtype;      //附件类型 0=无 1=url 2=ipfshash 3=pic 4=video 5=file
        std::string attachment;   //附件
        time_point_sec time;      //创建时间
        asset balance;            //获得代币数
        uint32_t like_num;        //获得赞数
        uint32_t comment_num;     //评论数

        uint64_t primary_key() const { return id; }

        uint64_t get_secondary_1() const { return author.value; }
    };

    typedef multi_index<"posttable"_n, posttable,
            indexed_by<"byauthor"_n, const_mem_fun<posttable, uint64_t, &posttable::get_secondary_1>>
    > post_t;

    //评论表
    TABLE commenttable {
        uint64_t id;              //自增id
        uint64_t post_id;         //微文id
        name author;              //评论者
        time_point_sec time;      //创建时间
        std::string content;      //评论内容
        asset balance;            //获得代币数
        uint32_t like_num;        //获得赞数
        bool has_parent;          //是否有父级评论
        uint64_t pid;             //父级评论id
        name reply_to;            //回复 @账户名：xxx

        uint64_t primary_key() const { return id; }

        uint64_t get_secondary_1() const { return post_id; }

        uint64_t get_secondary_2() const { return author.value; }
    };

    typedef multi_index<"commenttable"_n, commenttable,
            indexed_by<"bypost"_n, const_mem_fun<commenttable, uint64_t, &commenttable::get_secondary_1>>,
            indexed_by<"byauthor"_n, const_mem_fun<commenttable, uint64_t, &commenttable::get_secondary_2>>
    > comment_t;



    //点赞表
    TABLE liketable {
        uint64_t id;              //自增id
        uint32_t type;            //点赞类型 1=微文点赞 2=评论点赞
        uint64_t type_id;         //微文或评论的id
        name author;              //点赞者

        uint64_t primary_key() const { return id; }

        uint64_t get_secondary_1() const { return type_id; }

        uint64_t get_secondary_2() const { return author.value; }
    };

    typedef multi_index<"liketable"_n, liketable,
            indexed_by<"bytypeid"_n, const_mem_fun<liketable, uint64_t, &liketable::get_secondary_1>>,
            indexed_by<"byauthor"_n, const_mem_fun<liketable, uint64_t, &liketable::get_secondary_2>>
    > like_t;

    //关注表
    TABLE followtable {
        uint64_t id;              //自增id
        name from;                //关注者
        name to;                  //被关注者

        uint64_t primary_key() const { return id; }

        uint64_t get_secondary_1() const { return from.value; }

        uint64_t get_secondary_2() const { return to.value; }
    };

    typedef multi_index<"followtable"_n, followtable,
            indexed_by<"byfrom"_n, const_mem_fun<followtable, uint64_t, &followtable::get_secondary_1>>,
            indexed_by<"byto"_n, const_mem_fun<followtable, uint64_t, &followtable::get_secondary_2>>
    > follow_t;

};

EOSIO_DISPATCH(vktokendapps, (reward)(startactive))

