/* Licence: https://github.com/eos-referendum/master/LICENCE.md */

/*
 * File:   referendum.hpp
 * Author: Michael Fletcher | EOS42
 *
 * Created on 20 June 2018, 17:26
 */

#include <eosiolib/eosio.hpp>
#include <eosiolib/singleton.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/transaction.hpp>
#include <eosio.system/eosio.system.hpp>
#include "referendum_declarations.hpp"

namespace referendum {

typedef eosio::multi_index<N(voters), eosiosystem::voter_info> voter_info_table;

/* store ref configurations for querying */
//@abi table
struct refconfig{
  //TODO -> Ref Name
  //TODO -> Ref Description
  uint64_t min_part_p; //min vote percent
  uint8_t vote_period_d; //total vote period
  uint8_t sust_vote_d; // total daily votes passed
  uint8_t yes_vote_w; // yes weight

   EOSLIB_SERIALIZE(refconfig, (min_part_p)(vote_period_d)(sust_vote_d)(yes_vote_w))
};
typedef eosio::singleton<N(refconfig), refconfig> referendum_config_table;

/* keep track of the referendum */
//@abi table
struct refinfo{
   uint64_t total_days; // total days passed
   uint8_t total_c_days; // total consecutive days vote has passed
   bool	    vote_active = true; // false when the vote has finished

   EOSLIB_SERIALIZE(refinfo, (total_days)(total_c_days))
};
typedef eosio::singleton<N(refinfo), refinfo> referendum_results_table;


/*TODO Ensure no one can alter referendum contract tables */ 
/* keep track of voters */
//@abi table
struct regvoters {

    account_name name;
    uint8_t	 vote_side;
    
    uint64_t primary_key() const {
        return name;
    }

    EOSLIB_SERIALIZE(regvoters, (name));
};
typedef eosio::multi_index<N(refvoters), regvoters>  registered_voters_table;


class referendum : public eosio::contract {
public:
    referendum(account_name self):contract(self), registered_voters(self, self), voter_info(N(eosio), N(eosio)), referendum_results(self, self), referendum_config(self, self){}
 
    void init(account_name publisher); 
    void vote(account_name voter, uint8_t vote_side);
    void unvote(account_name voter);
    void countvotes(account_name publisher);

private:
    registered_voters_table    registered_voters;
    voter_info_table	       voter_info;
    referendum_results_table   referendum_results;
    referendum_config_table    referendum_config;
 
    bool validate_side(uint8_t vote_side);
};

EOSIO_ABI(referendum, (vote)(unvote)(countvotes))

}