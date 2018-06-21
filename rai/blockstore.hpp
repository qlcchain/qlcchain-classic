#pragma once

#include <rai/common.hpp>

namespace rai
{
/**
 * The value produced when iterating with \ref store_iterator
 */
class store_entry
{
public:
	store_entry ();
	void clear ();
	store_entry * operator-> ();
	rai::mdb_val first;
	rai::mdb_val second;
};

/**
 * Iterates the key/value pairs of a transaction
 */
class store_iterator
{
public:
	store_iterator (MDB_txn *, MDB_dbi);
	store_iterator (std::nullptr_t);
	store_iterator (MDB_txn *, MDB_dbi, MDB_val const &);
	store_iterator (rai::store_iterator &&);
	store_iterator (rai::store_iterator const &) = delete;
	~store_iterator ();
	rai::store_iterator & operator++ ();
	void next_dup ();
	rai::store_iterator & operator= (rai::store_iterator &&);
	rai::store_iterator & operator= (rai::store_iterator const &) = delete;
	rai::store_entry & operator-> ();
	bool operator== (rai::store_iterator const &) const;
	bool operator!= (rai::store_iterator const &) const;
	MDB_cursor * cursor;
	rai::store_entry current;
};

/**
 * Manages block storage and iteration
 */
class block_store
{
public:
	block_store (bool &, boost::filesystem::path const &, int lmdb_max_dbs = 128);

	MDB_dbi block_database (rai::block_type);
	void block_put_raw (MDB_txn *, MDB_dbi, rai::block_hash const &, MDB_val);
	void block_put (MDB_txn *, rai::block_hash const &, rai::block const &, rai::block_hash const & = rai::block_hash (0));
	MDB_val block_get_raw (MDB_txn *, rai::block_hash const &, rai::block_type &);
	rai::block_hash block_successor (MDB_txn *, rai::block_hash const &);
	void block_successor_clear (MDB_txn *, rai::block_hash const &);
	std::unique_ptr<rai::block> block_get (MDB_txn *, rai::block_hash const &);
	std::unique_ptr<rai::block> block_random (MDB_txn *);
	std::unique_ptr<rai::block> block_random (MDB_txn *, MDB_dbi);
	void block_del (MDB_txn *, rai::block_hash const &);
	bool block_exists (MDB_txn *, rai::block_hash const &);
	rai::block_counts block_count (MDB_txn *);
	bool root_exists (MDB_txn *, rai::uint256_union const &);

	void frontier_put (MDB_txn *, rai::block_hash const &, rai::account const &);
	rai::account frontier_get (MDB_txn *, rai::block_hash const &);
	void frontier_del (MDB_txn *, rai::block_hash const &);

	/**
	 * 检查 abi 是否存在
	 */
	bool abi_exists (MDB_txn *, rai::block_hash const &);
	/**
	 * 写 abi block_hash 到数据库
	 */
	bool abi_put (MDB_txn *, rai::block_hash const &);

	/**
	 * 删除指定的 abi hash
	 */
	void abi_del (MDB_txn *, rai::block_hash const &);

	/*** ACCOUNT ****/
	/**
	 * 从数据库中删除指定的 account 的所有 account_info
	 */
	void accounts_del (MDB_txn *, rai::account const &);
	/**
	 * 从数据库中查询指定的 account 的 account_info 列表
	 */
	bool accounts_get (MDB_txn *, rai::account const &, std::vector<rai::account_info> &);
	/**
	 * 从数据库中查询指定的 account 的第一个 account_info
	 */
	bool accounts_get_first (MDB_txn *, rai::account const &, rai::account_info &);
	/**
	 * 从数据库中查询指定的 account 的 指定 token 的 account_info
	 */
	bool accounts_get (MDB_txn *, rai::account const &, rai::block_hash const &, rai::account_info &);
	/**
	 * 向数据库中更新指定的 account 的 account_info 列表
	 */
	bool accounts_put (MDB_txn *, rai::account const &, std::vector<rai::account_info> const &);

	/*** TOKEN ACCOUNT **/
	/**
	 * 更新 account_info 信息到数据库中
	 * 
	 * 有则更新，无则创建
	 * 
	 */
	void account_put (MDB_txn *, rai::account const &, rai::account_info const &);
	/**
	 * 从数据库中查询指定的 token 的 account_info
	 * TODO: 检查所有调用逻辑
	 */
	bool account_get (MDB_txn *, rai::account const &, rai::account_info &);
	/**
	 * 从数据库中删除指定的 token 的 account_info
	 */
	void account_del (MDB_txn *, rai::account const &);
	/**
	 * 检查指定 token 账号是否存在
	 */
	bool account_exists (MDB_txn *, rai::account const &);
	size_t account_count (MDB_txn *);
	rai::store_iterator account_latest_begin (MDB_txn *, rai::account const &);
	rai::store_iterator account_latest_begin (MDB_txn *);
	rai::store_iterator latest_begin (MDB_txn *, rai::account const &);
	rai::store_iterator latest_begin (MDB_txn *);
	rai::store_iterator latest_end ();

	void pending_put (MDB_txn *, rai::pending_key const &, rai::pending_info const &);
	void pending_del (MDB_txn *, rai::pending_key const &);
	bool pending_get (MDB_txn *, rai::pending_key const &, rai::pending_info &);
	bool pending_exists (MDB_txn *, rai::pending_key const &);
	rai::store_iterator pending_begin (MDB_txn *, rai::pending_key const &);
	rai::store_iterator pending_begin (MDB_txn *);
	rai::store_iterator pending_end ();

	void block_info_put (MDB_txn *, rai::block_hash const &, rai::block_info const &);
	void block_info_del (MDB_txn *, rai::block_hash const &);
	bool block_info_get (MDB_txn *, rai::block_hash const &, rai::block_info &);
	bool block_info_exists (MDB_txn *, rai::block_hash const &);
	rai::store_iterator block_info_begin (MDB_txn *, rai::block_hash const &);
	rai::store_iterator block_info_begin (MDB_txn *);
	rai::store_iterator block_info_end ();
	rai::uint128_t block_balance (MDB_txn *, rai::block_hash const &);
	static size_t const block_info_max = 32;

	rai::uint128_t representation_get (MDB_txn *, rai::account const &);
	void representation_put (MDB_txn *, rai::account const &, rai::uint128_t const &);
	void representation_add (MDB_txn *, rai::account const &, rai::uint128_t const &);
	rai::store_iterator representation_begin (MDB_txn *);
	rai::store_iterator representation_end ();

	void unchecked_clear (MDB_txn *);
	void unchecked_put (MDB_txn *, rai::block_hash const &, std::shared_ptr<rai::block> const &);
	std::vector<std::shared_ptr<rai::block>> unchecked_get (MDB_txn *, rai::block_hash const &);
	void unchecked_del (MDB_txn *, rai::block_hash const &, rai::block const &);
	rai::store_iterator unchecked_begin (MDB_txn *);
	rai::store_iterator unchecked_begin (MDB_txn *, rai::block_hash const &);
	rai::store_iterator unchecked_end ();
	size_t unchecked_count (MDB_txn *);
	std::unordered_multimap<rai::block_hash, std::shared_ptr<rai::block>> unchecked_cache;

	void checksum_put (MDB_txn *, uint64_t, uint8_t, rai::checksum const &);
	bool checksum_get (MDB_txn *, uint64_t, uint8_t, rai::checksum &);
	void checksum_del (MDB_txn *, uint64_t, uint8_t);
	// 资产 CURD
	void assets_put (MDB_txn *, rai::asset_key const &, rai::asset_value const &);
	bool assets_get (MDB_txn *, rai::asset_key const &, rai::asset_value &);
	void assets_delete (MDB_txn *, rai::asset_key const &);

	// 智能合约 CURD
	// bool smart_contract_put (MDB_txn *, rai::block_hash const &, rai::smart_contract_block const &);
	// bool smart_contract_put (MDB_txn * , rai::smart_contract_block const & );
	// bool smart_contract_get (MDB_txn *, rai::block_hash const &, rai::smart_contract_block const &);
	// void smart_contract_delete (MDB_txn *, rai::block_hash const &);

	// Return latest vote for an account from store
	std::shared_ptr<rai::vote> vote_get (MDB_txn *, rai::account const &);
	// Populate vote with the next sequence number
	std::shared_ptr<rai::vote> vote_generate (MDB_txn *, rai::account const &, rai::raw_key const &, std::shared_ptr<rai::block>);
	// Return either vote or the stored vote with a higher sequence number
	std::shared_ptr<rai::vote> vote_max (MDB_txn *, std::shared_ptr<rai::vote>);
	// Return latest vote for an account considering the vote cache
	std::shared_ptr<rai::vote> vote_current (MDB_txn *, rai::account const &);
	void flush (MDB_txn *);
	rai::store_iterator vote_begin (MDB_txn *);
	rai::store_iterator vote_end ();
	std::mutex cache_mutex;
	std::unordered_map<rai::account, std::shared_ptr<rai::vote>> vote_cache;

	void version_put (MDB_txn *, int);
	int version_get (MDB_txn *);
	void do_upgrades (MDB_txn *);
	void upgrade_v1_to_v2 (MDB_txn *);
	void upgrade_v2_to_v3 (MDB_txn *);
	void upgrade_v3_to_v4 (MDB_txn *);
	void upgrade_v4_to_v5 (MDB_txn *);
	void upgrade_v5_to_v6 (MDB_txn *);
	void upgrade_v6_to_v7 (MDB_txn *);
	void upgrade_v7_to_v8 (MDB_txn *);
	void upgrade_v8_to_v9 (MDB_txn *);
	void upgrade_v9_to_v10 (MDB_txn *);
	void upgrade_v10_to_v11 (MDB_txn *);

	void clear (MDB_dbi);

	rai::mdb_env environment;

	/**
	 * Maps head block to owning account
	 * rai::block_hash -> rai::account
	 */
	MDB_dbi frontiers;

	/**
	 * Maps account to account information, head, rep, open, balance, timestamp and block count.
	 * rai::account -> rai::block_hash, rai::block_hash, rai::block_hash, rai::amount, uint64_t, uint64_t
	 */
	MDB_dbi accounts;

	/**
	 * Maps block hash to send block.
	 * rai::block_hash -> rai::send_block
	 */
	MDB_dbi send_blocks;

	/**
	 * Maps block hash to receive block.
	 * rai::block_hash -> rai::receive_block
	 */
	MDB_dbi receive_blocks;

	/**
	 * Maps block hash to open block.
	 * rai::block_hash -> rai::open_block
	 */
	MDB_dbi open_blocks;

	/**
	 * Maps block hash to change block.
	 * rai::block_hash -> rai::change_block
	 */
	MDB_dbi change_blocks;

	/**
	 * Maps block hash to state block.
	 * rai::block_hash -> rai::state_block
	 */
	MDB_dbi state_blocks;

	/**
	 * Maps (destination account, pending block) to (source account, amount).
	 * rai::account, rai::block_hash -> rai::account, rai::amount
	 */
	MDB_dbi pending;

	/**
	 * Maps block hash to account and balance.
	 * block_hash -> rai::account, rai::amount
	 */
	MDB_dbi blocks_info;

	/**
	 * Representative weights.
	 * rai::account -> rai::uint128_t
	 */
	MDB_dbi representation;

	/**
	 * Unchecked bootstrap blocks.
	 * rai::block_hash -> rai::block
	 */
	MDB_dbi unchecked;

	/**
	 * Mapping of region to checksum.
	 * (uint56_t, uint8_t) -> rai::block_hash
	 */
	MDB_dbi checksum;

	/**
	 * Highest vote observed for account.
	 * rai::account -> uint64_t
	 */
	MDB_dbi vote;

	/**
	 * Meta information about block store, such as versions.
	 * rai::uint256_union (arbitrary key) -> blob
	 */
	MDB_dbi meta;

	/**
	 * 账号与 Token open_block 的映射
	 * account->token_open_blocks
	 */
	MDB_dbi token_accounts;

	/*
	 * 资产的 id 与 值的映射
	 * asset_key->asset_value
	 */
	MDB_dbi assets;

	/*
	 * 智能合约 hash 与 智能合约 block 的映射
	 *  smart_contract_hash->smart_contract_block
	 */
	MDB_dbi smart_contract;

	/*
	 * abi hash 列表，用于校验
	 *  abi_hash ->
	 */
	MDB_dbi abi;
};
}
