#include <rai/common.hpp>

#include <rai/blockstore.hpp>
#include <rai/lib/interface.h>
#include <rai/node/common.hpp>
#include <rai/versioning.hpp>

#include <boost/property_tree/json_parser.hpp>

#include <queue>

#include <boost/phoenix/core/value.hpp>
#include <ed25519-donna/ed25519.h>

// Genesis keys for network variants
namespace
{
char const * test_private_key_data = "69AF95E5AD27F7DCEBBECFBFF7C05194F4E6678C8A6915FA87AD6DC46F45EDD9";
char const * test_public_key_data = "B3DEFADF674A687DC0D4EF3EEF7B5A0E177BAE0D9887B562E2F234148D3682B3"; // xrb_3e3j5tkog48pnny9dmfzj1r16pg8t1e76dz5tmac6iq689wyjfpiij4txtdo
char const * beta_public_key_data = "0311B25E0D1E1D7724BBA5BD523954F1DBCFC01CB8671D55ED2D32C7549FB252"; // xrb_11rjpbh1t9ixgwkdqbfxcawobwgusz13sg595ocytdbkrxcbzekkcqkc3dn1
//char const * live_public_key_data = "E89208DD038FBB269987689621D52292AE9C35941A7484756ECCED92A65093BA"; // xrb_3t6k35gi95xu6tergt6p69ck76ogmitsa8mnijtpxm9fkcm736xtoncuohr3
char const * live_public_key_data = "1327950B79C7A18C61C26965E274F38E0D8C618CDD72BA17E00E2F3655CE0B78";
char const * live_public_key_data_QN1 = "6F6B4BECCA470084032121E7923A668DA0BBC3AD1EF0C46513CDCF67080BC9B0";
char const * live_public_key_data_QN2 = "E4087493C2572DD52AB4AB67AE91C72DFF25CA2EFECECEC8205FF1764D75DDBB";
char const * live_public_key_data_QN3 = "2F51DF2104BAE5A0FEAAAA575F9AE458AC7353DFDF86F393A8D068BB5BCEA95D";
char const * live_public_key_data_QN4 = "EEE2111FBC1A25C3AFB02760B3FDD499D20CA507DEC145E2E7F9C07A1D9B9B30";
char const * live_public_key_data_QN5 = "7B6F0909957F2D5B25A01836B1527599DFD4453705397E7C89967DEE27E15286";
char const * test_genesis_data = R"%%%({
	"type": "state",
	"account": "qlc_3eyyzdhpgkmahq1fbusyxxxon5iqhgq1u869pojg7wjn4k8mf1omyi3ab6nq",
	"previous": "0000000000000000000000000000000000000000000000000000000000000000",
	"representative": "qlc_3eyyzdhpgkmahq1fbusyxxxon5iqhgq1u869pojg7wjn4k8mf1omyi3ab6nq",
	"balance": "340282366920938463463374607431768211455",
	"link": "B3DEFADF674A687DC0D4EF3EEF7B5A0E177BAE0D9887B562E2F234148D3682B3",
	"link_as_account": "qlc_3eyyzdhpgkmahq1fbusyxxxon5iqhgq1u869pojg7wjn4k8mf1omyi3ab6nq",
	"token": "3DA3D41A552B0135008A776EBBA7225E7773DBF867D86B9CD8FB650D6BAAD5DB",
	"Token_name": "Root_Token",
	"signature": "EF245C9BB426EB7574102EB11727AF760C4CB942D6864857BDAC7F2D28B344B1E2795E99B1509F49602AF54E7617EF4907AF47B2592870C62713947D3ADD4A0E",
	"work": "2b61e26528833017"
})%%%";

char const * beta_genesis_data = R"%%%({
	"type": "open",
	"source": "0311B25E0D1E1D7724BBA5BD523954F1DBCFC01CB8671D55ED2D32C7549FB252",
	"representative": "xrb_11rjpbh1t9ixgwkdqbfxcawobwgusz13sg595ocytdbkrxcbzekkcqkc3dn1",
	"account": "xrb_11rjpbh1t9ixgwkdqbfxcawobwgusz13sg595ocytdbkrxcbzekkcqkc3dn1",
	"work": "869e17b2bfa36639",
	"signature": "34DF447C7F185673128C3516A657DFEC7906F16C68FB5A8879432E2E4FB908C8ED0DD24BBECFAB3C7852898231544A421DC8CB636EF66C82E1245083EB08EA0F"
})%%%";
#if 0
char const * live_genesis_data = R"%%%({
	"type": "open",
	"source": "E89208DD038FBB269987689621D52292AE9C35941A7484756ECCED92A65093BA",
	"representative": "xrb_3t6k35gi95xu6tergt6p69ck76ogmitsa8mnijtpxm9fkcm736xtoncuohr3",
	"account": "xrb_3t6k35gi95xu6tergt6p69ck76ogmitsa8mnijtpxm9fkcm736xtoncuohr3",
	"work": "62f05417dd3fb691",
	"signature": "9F0C933C8ADE004D808EA1985FA746A7E95BA2A38F867640F53EC8F180BDFE9E2C1268DEAD7C2664F356E37ABA362BC58E46DBA03E523A7B5A19E4B6EB12BB02"
})%%%";
#endif

char const * live_genesis_data = R"%%%({
		"type": "state",
		"account": "qlc_16s9kn7qmjx3jjiw6td7wbth95ifjjirsqdkqady15jh8scww4urw6gg8zd5",
		"previous": "0000000000000000000000000000000000000000000000000000000000000000",
		"representative": "qlc_16s9kn7qmjx3jjiw6td7wbth95ifjjirsqdkqady15jh8scww4urw6gg8zd5",
		"balance": "340282366920938463463374607431768211455",
		"link": "1327950B79C7A18C61C26965E274F38E0D8C618CDD72BA17E00E2F3655CE0B78",
		"link_as_account": "qlc_16s9kn7qmjx3jjiw6td7wbth95ifjjirsqdkqady15jh8scww4urw6gg8zd5",
		"token": "3DA3D41A552B0135008A776EBBA7225E7773DBF867D86B9CD8FB650D6BAAD5DB",
		"signature": "766AC05375B5BBED2A7F870A541DE1FE36FDB853300A76B17F698EDF944ED8419B78C721EFB37C532F205D7E360A6A3BF0DF8ABFEC2C0C0C5791F947D9344803",
		"work": "f170a158fa6bead1"
})%%%";

//QLINK:增加smart contract block的初始信息
char const * smart_contrac_block_genesis_data = R"%%%({
    	"type": "smart_contract",
    	"internal-owned account": "qlc_16s9kn7qmjx3jjiw6td7wbth95ifjjirsqdkqady15jh8scww4urw6gg8zd5",
    	"external-owned account": "qlc_3sczdr37zykkha4im7jnbta6ian8rbnhbk9qmcbs5fqpkehowak8qacp864g",
    	"abi_hash": "FFF8FF5DF1B6ED4FC7F300848931416581AE742999A2399563842F579E018D6B",
    	"abi_length": "64",
    	"abi": "6060604052341561000F57600080FD5B336000806101000A81548173FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF021916908373FFFFFFFFFFFFFFFFFFFF",
    	"signature": "16B015C487C06829406D62CB3BD41736942C285A6328F9E855FEB5870724FA66131AFC15027BDBB97C1E1539A19C0C961362AD23BE55D1C986614FFC5E3AAA06",
    	"work": "ee99d963285bf3d5"
})%%%";

char const * live_genesis_data_QN1 = R"%%%({
		"type": "state",
		"account": "qlc_1uudbhpenjr1ii3k4ah9kax8f5f1qh3tt9qirjkj9mghew61qkfibw4haxtc",
		"previous": "0000000000000000000000000000000000000000000000000000000000000000",
		"representative": "qlc_16s9kn7qmjx3jjiw6td7wbth95ifjjirsqdkqady15jh8scww4urw6gg8zd5",
		"balance": "300000000000000000000000000000000000000",
		"link": "6F6B4BECCA470084032121E7923A668DA0BBC3AD1EF0C46513CDCF67080BC9B0",
		"link_as_account": "qlc_1uudbhpenjr1ii3k4ah9kax8f5f1qh3tt9qirjkj9mghew61qkfibw4haxtc",
		"token": "E66E8CAD20394B3D7FA984D07CC500829AD36B69005E98F45348D98C37BF8EDC",
		"signature": "3A8FF15030EB202FA10A2569C9596557E2A8DD2BA608B35301DB46574BFEE2D7D11D5DB23AC728B456A5227C54D3068AC0337F5E254CAB3DF3C16CA78FA9F407",
		"work": "a219f25924fe179c"
})%%%";

//QLINK:增加smart contract block QN1的初始信息
char const * smart_contrac_block_genesis_data_QN1 = R"%%%({
    "type": "smart_contract",
    "internal-owned account": "qlc_1uudbhpenjr1ii3k4ah9kax8f5f1qh3tt9qirjkj9mghew61qkfibw4haxtc",
    "external-owned account": "qlc_1btzbbfcia53gesj8ngy1h115o9ehekgoajn3hyndk4gwrd7kszqrebcockh",
    "abi_hash": "B07C285EC1C700B83FF76DB1642C8252132ABD4A515C35A9192059A139B59C76",
    "abi_length": "64",
    "abi": "815260200190815260200160002081905550611997806100C86000396000F3006060604052600436106100FC576000357C010000000000000000000000000000",
    "signature": "63D72B456F4C5158E9FC038E4863CACCC7CC73DAC1F591A7DE52D1E0E786EFFA8D7AE675EC2BA404F87E8C244FA0917092D98A8D81B8E733A7959BDD3816460A",
    "work": "e4dcdd81ac0c507d"
})%%%";

char const * live_genesis_data_QN2 = R"%%%({
	"type": "state",
    "account": "qlc_3s1agkbw6osftnodbcu9otawgdhz6q74xzpgsu641qzjgs8qdqfujim3z7ii",
    "previous": "0000000000000000000000000000000000000000000000000000000000000000",
    "representative": "qlc_16s9kn7qmjx3jjiw6td7wbth95ifjjirsqdkqady15jh8scww4urw6gg8zd5",
	"balance": "290000000000000000000000000000000000000",
    "link": "E4087493C2572DD52AB4AB67AE91C72DFF25CA2EFECECEC8205FF1764D75DDBB",
    "link_as_account": "qlc_3s1agkbw6osftnodbcu9otawgdhz6q74xzpgsu641qzjgs8qdqfujim3z7ii",
    "token": "3A938337C8B9F6BA8A6C94F3C53C02815E574E2BC2DCEC3EA2B60E67154FFECA",
    "signature": "B982719A99F2322FE7DE17AFAD9D20B02FC2CDB5BFA105E316C00BF9E1FA2119BB513511777F38B4914CADFBF0F2C721216ED456C96EF09C4FDB6542828CD207",
    "work": "db2763e347fddc52"
})%%%";

char const * smart_contrac_block_genesis_data_QN2 = R"%%%({
    "type": "smart_contract",
    "internal-owned account": "qlc_3s1agkbw6osftnodbcu9otawgdhz6q74xzpgsu641qzjgs8qdqfujim3z7ii",
    "external-owned account": "qlc_1r8mda3w54y6acd5dca4qdb435rx5oias1jnsnjffooce9cm8wpa78nxfedr",
    "abi_hash": "950B3F57F85C94308CD43D1F8AB5DD9040CEAC6A0E83C89DD1D840887B22E00A",
    "abi_length": "64",
    "abi": "815260200190815260200160002081905550611997806100C86000396000F3006060604052600436106100FC576000357C010000000000000000000123456789",
    "signature": "D357E76198AF1BF77BD7F7550F8B6D38B66F526F0EA865C7D160CEF0438DA0B3BCD4BE9C47F2D6CF95EF9C7BF2AA685CB061A59A7702887205D0D0B87F281F07",
    "work": "ecd595399bb3f286"
})%%%";

char const * live_genesis_data_QN3 = R"%%%({
    "type": "state",
    "account": "qlc_1dtjuwiibgq7n5zcockqdyfgap7egfbxzqw8ygbtjn5aqffwxccxensu53i8",
    "previous": "0000000000000000000000000000000000000000000000000000000000000000",
    "representative": "qlc_16s9kn7qmjx3jjiw6td7wbth95ifjjirsqdkqady15jh8scww4urw6gg8zd5",
	"balance": "280000000000000000000000000000000000000",
    "link": "2F51DF2104BAE5A0FEAAAA575F9AE458AC7353DFDF86F393A8D068BB5BCEA95D",
    "link_as_account": "qlc_1dtjuwiibgq7n5zcockqdyfgap7egfbxzqw8ygbtjn5aqffwxccxensu53i8",
    "token": "9A4171BAA9133AFB9F8B77848A36592465A680375C8591E1EC7ADBB980C189F1",
    "signature": "95F001918668FCA0DCA821333834B147582FAC9FD60AAD7350A8AD1DE400AD96153F3413339B7839E16D58637638911106F96150DA234411D1B8BECFA1DE6700",
    "work": "ae0605758374eaa6"
})%%%";

char const * smart_contrac_block_genesis_data_QN3 = R"%%%({
   "type": "smart_contract",
    "internal-owned account": "qlc_1dtjuwiibgq7n5zcockqdyfgap7egfbxzqw8ygbtjn5aqffwxccxensu53i8",
    "external-owned account": "qlc_19qejrfbe1pyiksgyyrcph91m5dn14yn5uquyx9mbdri89skcaj8k1nr1dyy",
    "abi_hash": "BAEE4F9FCE2466D22C11B34B5F56BF470B5C222BB5EC5093D9A8531E5F46C898",
    "abi_length": "64",
    "abi": "123456789190815260200160002081905550611997806100C86000396000F3006060604052600436106100FC576000357C010000000000000000000000000000",
    "signature": "C5D69FBE04C3DC0E444E05F81794F3A462307DA95D46B5032496578C363DC8749048E0CB072555EAFF4A3F95A7573BB79BFDE29CDBA9906884696DBB4E697005",
    "work": "7401df2835ab7449"
})%%%";

char const * live_genesis_data_QN4 = R"%%%({
	"type": "state",
    "account": "qlc_3uq446hur8j7rgqu1bu1phyxb8gk3kkihqp3aqjghyg1hagsq8sizm4n7596",
    "previous": "0000000000000000000000000000000000000000000000000000000000000000",
    "representative": "qlc_16s9kn7qmjx3jjiw6td7wbth95ifjjirsqdkqady15jh8scww4urw6gg8zd5",
	"balance": "270000000000000000000000000000000000000",
    "link": "EEE2111FBC1A25C3AFB02760B3FDD499D20CA507DEC145E2E7F9C07A1D9B9B30",
    "link_as_account": "qlc_3uq446hur8j7rgqu1bu1phyxb8gk3kkihqp3aqjghyg1hagsq8sizm4n7596",
    "token": "222DC0E09225D6F929C96F3F1A0BF584ED680B9C253A5E0AE2C5C951098DDA0D",
    "signature": "3756CE8C797FB08DB17A2C16CEDF102A4D2DBFFF061DF66EE373E45171FCCECB1810B85EABAF909F42987CA214A3FF72E5C36AF2D3AF6490B98255AF53C07C0D",
    "work": "d50d93d76dd958e6"

})%%%";

char const * smart_contrac_block_genesis_data_QN4 = R"%%%({
	"type": "smart_contract",
    "internal-owned account": "qlc_3uq446hur8j7rgqu1bu1phyxb8gk3kkihqp3aqjghyg1hagsq8sizm4n7596",
    "external-owned account": "qlc_3y97b8hk4fmhp3uihddikp9q84ge8fjt13uyjmjfga6ak9u6fmu65smrzimk",
    "abi_hash": "8DAA79BE72C3068143B536754D23C3F059291ADE91C6DDC867B4856BB74FABE1",
    "abi_length": "64",
    "abi": "123456789190815260200160002081905550611997806100C86000396000F3006060604052600436106100FC576000357C010000000000000000000123456789",
    "signature": "890A7685DDD9C4CB6DECE8EC0DFFF6A20AFECAF4E478465885A5621820DD9AF60E7503692152591287403813578BC46454305E7801E1873B5A6F9E5BD4355402",
    "work": "24621da31327a7af"

})%%%";

char const * live_genesis_data_QN5 = R"%%%({
	"type": "state",
    "account": "qlc_1yuh366sczsfdekt183pp7b9d8gztj4mg3bshsyam7mxxrmy4nn89638o3me",
    "previous": "0000000000000000000000000000000000000000000000000000000000000000",
    "representative": "qlc_16s9kn7qmjx3jjiw6td7wbth95ifjjirsqdkqady15jh8scww4urw6gg8zd5",
    "balance": "260000000000000000000000000000000000000",
    "link": "7B6F0909957F2D5B25A01836B1527599DFD4453705397E7C89967DEE27E15286",
    "link_as_account": "qlc_1yuh366sczsfdekt183pp7b9d8gztj4mg3bshsyam7mxxrmy4nn89638o3me",
    "token": "AD3313731172B299E50B338C2CB7589A79F8B518F35D61CAF60953A9B5653AF4",
    "signature": "742F85CE08D904A2D2A27A7A56CFD3464A3B0250868C2BE781887630284D3291808DC8B382A1541C839F51FD2834795E32BAB8D112BB0D8A2B908D062234CC09",
    "work": "bee2ed016a74a935"
})%%%";

char const * smart_contrac_block_genesis_data_QN5 = R"%%%({
	"type": "smart_contract",
    "internal-owned account": "qlc_1yuh366sczsfdekt183pp7b9d8gztj4mg3bshsyam7mxxrmy4nn89638o3me",
    "external-owned account": "qlc_3byq614969tc9hdmyg34suazq3ykshheb8jjakpdcg6qjiz89y95rdrkemqr",
    "abi_hash": "BCB607484CB6F3621C3092E11045F90F763A1B1C6A391233AD04707C73D8CF0B",
    "abi_length": "64",
    "abi": "123456789190815260200160002081905550611997806100C86000396000F3006060604052600436106100FC576000357C011234567890000000000000000000",
    "signature": "FC59CC0C54DFE0843D2193859754C3315FB32A876EC60C41B24430B784763C022ACA96DDA61C70D787466B8E17C2D6EFBE271CE39813B23FD5F6838652630A02",
    "work": "c8a0be5ac2299c89"
})%%%";

class ledger_constants
{
public:
	ledger_constants () :
	zero_key ("0"),
	test_genesis_key (test_private_key_data),
	rai_test_account (test_public_key_data),
	rai_beta_account (beta_public_key_data),
	rai_live_account (live_public_key_data),
	rai_test_genesis (test_genesis_data),
	rai_beta_genesis (beta_genesis_data),
	rai_live_genesis (live_genesis_data),
	chain_token_type ("3DA3D41A552B0135008A776EBBA7225E7773DBF867D86B9CD8FB650D6BAAD5DB"),
	chain_token_type_QN1 ("E66E8CAD20394B3D7FA984D07CC500829AD36B69005E98F45348D98C37BF8EDC"),
	chain_token_type_QN2 ("3A938337C8B9F6BA8A6C94F3C53C02815E574E2BC2DCEC3EA2B60E67154FFECA"),
	chain_token_type_QN3 ("9A4171BAA9133AFB9F8B77848A36592465A680375C8591E1EC7ADBB980C189F1"),
	chain_token_type_QN4 ("222DC0E09225D6F929C96F3F1A0BF584ED680B9C253A5E0AE2C5C951098DDA0D"),
	chain_token_type_QN5 ("AD3313731172B299E50B338C2CB7589A79F8B518F35D61CAF60953A9B5653AF4"),
	genesis_account (rai::rai_network == rai::rai_networks::rai_test_network ? rai_test_account : rai::rai_network == rai::rai_networks::rai_beta_network ? rai_beta_account : rai_live_account),
	rai_live_account_QN1 (live_public_key_data_QN1),
	rai_live_account_QN2 (live_public_key_data_QN2),
	rai_live_account_QN3 (live_public_key_data_QN3),
	rai_live_account_QN4 (live_public_key_data_QN4),
	rai_live_account_QN5 (live_public_key_data_QN5),
	genesis_block (rai::rai_network == rai::rai_networks::rai_test_network ? rai_test_genesis : rai::rai_network == rai::rai_networks::rai_beta_network ? rai_beta_genesis : rai_live_genesis),
	genesis_amount (std::numeric_limits<rai::uint128_t>::max ()),
	burn_account (0)
	{
		CryptoPP::AutoSeededRandomPool random_pool;
		// Randomly generating these mean no two nodes will ever have the same sentinel values which protects against some insecure algorithms
		random_pool.GenerateBlock (not_a_block.bytes.data (), not_a_block.bytes.size ());
		random_pool.GenerateBlock (not_an_account.bytes.data (), not_an_account.bytes.size ());
	}
	rai::keypair zero_key;
	rai::keypair test_genesis_key;
	rai::account rai_test_account;
	rai::account rai_beta_account;
	rai::account rai_live_account;
	rai::account rai_live_account_QN1;
	rai::account rai_live_account_QN2;
	rai::account rai_live_account_QN3;
	rai::account rai_live_account_QN4;
	rai::account rai_live_account_QN5;
	std::string rai_test_genesis;
	std::string rai_beta_genesis;
	std::string rai_live_genesis;
	rai::account genesis_account;
	std::string genesis_block;
	rai::uint128_t genesis_amount;
	rai::block_hash not_a_block;
	rai::account not_an_account;
	rai::account burn_account;
	rai::block_hash chain_token_type;
	rai::block_hash chain_token_type_QN1;
	rai::block_hash chain_token_type_QN2;
	rai::block_hash chain_token_type_QN3;
	rai::block_hash chain_token_type_QN4;
	rai::block_hash chain_token_type_QN5;
	std::unordered_map<rai::block_hash, std::list<std::string>> sc_infos = {
		{ chain_token_type, { "Root_Token", rai::xrb_ratio.convert_to<std::string> (), "24", "QLC" } },
		// FIXME: 生成合约代码
		{ chain_token_type_QN1, { "QN1", rai::uxrb_ratio.convert_to<std::string> (), "18", "QN1" } },
		{ chain_token_type_QN2, { "QN2", rai::xrb_ratio.convert_to<std::string> (), "8", "QN2" } },
		{ chain_token_type_QN3, { "QN3", rai::xrb_ratio.convert_to<std::string> (), "8", "QN3" } },
		{ chain_token_type_QN4, { "QN4", rai::Mxrb_ratio.convert_to<std::string> (), "30", "QN4" } },
		{ chain_token_type_QN5, { "QN5", rai::Gxrb_ratio.convert_to<std::string> (), "33", "QN5" } },
	};
	std::unordered_map<rai::account, std::list<std::string>> genesis_blocks = {
		{ rai_live_account, { live_genesis_data, smart_contrac_block_genesis_data } },
		{ rai_live_account_QN1, { live_genesis_data_QN1, smart_contrac_block_genesis_data_QN1 } },
		{ rai_live_account_QN2, { live_genesis_data_QN2, smart_contrac_block_genesis_data_QN2 } },
		{ rai_live_account_QN3, { live_genesis_data_QN3, smart_contrac_block_genesis_data_QN3 } },
		{ rai_live_account_QN4, { live_genesis_data_QN4, smart_contrac_block_genesis_data_QN4 } },
		{ rai_live_account_QN5, { live_genesis_data_QN5, smart_contrac_block_genesis_data_QN5 } },
	};
};
ledger_constants globals;
}

size_t constexpr rai::send_block::size;
size_t constexpr rai::receive_block::size;
size_t constexpr rai::open_block::size;
size_t constexpr rai::change_block::size;
size_t constexpr rai::state_block::size;

rai::keypair const & rai::zero_key (globals.zero_key);
rai::keypair const & rai::test_genesis_key (globals.test_genesis_key);
rai::account const & rai::rai_test_account (globals.rai_test_account);
rai::account const & rai::rai_beta_account (globals.rai_beta_account);
rai::account const & rai::rai_live_account (globals.rai_live_account);
std::string const & rai::rai_test_genesis (globals.rai_test_genesis);
std::string const & rai::rai_beta_genesis (globals.rai_beta_genesis);
std::string const & rai::rai_live_genesis (globals.rai_live_genesis);

rai::account const & rai::genesis_account (globals.genesis_account);
std::string const & rai::genesis_block (globals.genesis_block);
rai::uint128_t const & rai::genesis_amount (globals.genesis_amount);
rai::block_hash const & rai::not_a_block (globals.not_a_block);
rai::block_hash const & rai::not_an_account (globals.not_an_account);
rai::account const & rai::burn_account (globals.burn_account);
rai::block_hash const & rai::chain_token_type (globals.chain_token_type);
rai::block_hash const & rai::chain_token_type_QN1 (globals.chain_token_type_QN1);
rai::block_hash const & rai::chain_token_type_QN2 (globals.chain_token_type_QN2);
rai::block_hash const & rai::chain_token_type_QN3 (globals.chain_token_type_QN3);
rai::block_hash const & rai::chain_token_type_QN4 (globals.chain_token_type_QN4);
rai::block_hash const & rai::chain_token_type_QN5 (globals.chain_token_type_QN5);
std::unordered_map<rai::block_hash, std::list<std::string>> rai::map_sc_info (globals.sc_infos.begin (), globals.sc_infos.end ());
std::unordered_map<rai::account, std::list<std::string>> rai::map_genesis_blocks (globals.genesis_blocks);

rai::votes::votes (std::shared_ptr<rai::block> block_a) :
id (block_a->root ())
{
	rep_votes.insert (std::make_pair (rai::not_an_account, block_a));
}

rai::tally_result rai::votes::vote (std::shared_ptr<rai::vote> vote_a)
{
	rai::tally_result result;
	auto existing (rep_votes.find (vote_a->account));
	if (existing == rep_votes.end ())
	{
		// Vote on this block hasn't been seen from rep before
		result = rai::tally_result::vote;
		rep_votes.insert (std::make_pair (vote_a->account, vote_a->block));
	}
	else
	{
		if (!(*existing->second == *vote_a->block))
		{
			// Rep changed their vote
			result = rai::tally_result::changed;
			existing->second = vote_a->block;
		}
		else
		{
			// Rep vote remained the same
			result = rai::tally_result::confirm;
		}
	}
	return result;
}

bool rai::votes::uncontested ()
{
	bool result (true);
	if (!rep_votes.empty ())
	{
		auto block (rep_votes.begin ()->second);
		for (auto i (rep_votes.begin ()), n (rep_votes.end ()); result && i != n; ++i)
		{
			result = *i->second == *block;
		}
	}
	return result;
}

// Create a new random keypair
rai::keypair::keypair ()
{
	random_pool.GenerateBlock (prv.data.bytes.data (), prv.data.bytes.size ());
	ed25519_publickey (prv.data.bytes.data (), pub.bytes.data ());
}

// Create a keypair given a hex string of the private key
rai::keypair::keypair (std::string const & prv_a)
{
	auto error (prv.data.decode_hex (prv_a));
	assert (!error);
	ed25519_publickey (prv.data.bytes.data (), pub.bytes.data ());
}

// Serialize a block prefixed with an 8-bit typecode
void rai::serialize_block (rai::stream & stream_a, rai::block const & block_a)
{
	write (stream_a, block_a.type ());
	block_a.serialize (stream_a);
}

std::unique_ptr<rai::block> rai::deserialize_block (MDB_val const & val_a)
{
	rai::bufferstream stream (reinterpret_cast<uint8_t const *> (val_a.mv_data), val_a.mv_size);
	return deserialize_block (stream);
}

rai::account_info::account_info () :
head (0),
rep_block (0),
open_block (0),
balance (0),
modified (0),
block_count (0),
token_type (0),
account (0)
{
}

rai::account_info::account_info (MDB_val const & val_a)
{
	assert (val_a.mv_size == sizeof (*this));
	static_assert (sizeof (head) + sizeof (rep_block) + sizeof (open_block) + sizeof (balance) + sizeof (modified) + sizeof (block_count) + sizeof (token_type) + sizeof (account) == sizeof (*this), "Class not packed");
	std::copy (reinterpret_cast<uint8_t const *> (val_a.mv_data), reinterpret_cast<uint8_t const *> (val_a.mv_data) + sizeof (*this), reinterpret_cast<uint8_t *> (this));
}

rai::account_info::account_info (rai::block_hash const & head_a, rai::block_hash const & rep_block_a, rai::block_hash const & open_block_a, rai::amount const & balance_a, uint64_t modified_a, uint64_t block_count_a, rai::block_hash const & token_hash_a, rai::account const & account_a) :
head (head_a),
rep_block (rep_block_a),
open_block (open_block_a),
balance (balance_a),
modified (modified_a),
block_count (block_count_a),
token_type (token_hash_a),
account (account_a)
{
}

void rai::account_info::serialize (rai::stream & stream_a) const
{
	write (stream_a, head.bytes);
	write (stream_a, rep_block.bytes);
	write (stream_a, open_block.bytes);
	write (stream_a, balance.bytes);
	write (stream_a, modified);
	write (stream_a, block_count);
	write (stream_a, token_type.bytes);
	write (stream_a, account.bytes);
}

bool rai::account_info::deserialize (rai::stream & stream_a)
{
	auto error (read (stream_a, head.bytes));
	if (!error)
	{
		error = read (stream_a, rep_block.bytes);
		if (!error)
		{
			error = read (stream_a, open_block.bytes);
			if (!error)
			{
				error = read (stream_a, balance.bytes);
				if (!error)
				{
					error = read (stream_a, modified);
					if (!error)
					{
						error = read (stream_a, block_count);
						if (!error)
						{
							error = read (stream_a, token_type.bytes);
							if (!error)
							{
								error = read (stream_a, account.bytes);
							}
						}
					}
				}
			}
		}
	}
	return error;
}

bool rai::account_info::operator== (rai::account_info const & other_a) const
{
	return head == other_a.head && rep_block == other_a.rep_block && open_block == other_a.open_block && balance == other_a.balance && modified == other_a.modified && block_count == other_a.block_count && other_a.token_type == token_type && other_a.account == account;
}

bool rai::account_info::operator!= (rai::account_info const & other_a) const
{
	return !(*this == other_a);
}

rai::mdb_val rai::account_info::val () const
{
	return rai::mdb_val (sizeof (*this), const_cast<rai::account_info *> (this));
}

rai::block_counts::block_counts () :
send (0),
receive (0),
open (0),
change (0),
smart_contract (0),
state (0)
{
}

size_t rai::block_counts::sum ()
{
	return send + receive + open + change + state + smart_contract;
}

rai::pending_info::pending_info () :
source (0),
amount (0),
token_type (0)
{
}

rai::pending_info::pending_info (MDB_val const & val_a)
{
	assert (val_a.mv_size == sizeof (*this));
	static_assert (sizeof (source) + sizeof (amount) + sizeof (token_type) == sizeof (*this), "Packed class");
	std::copy (reinterpret_cast<uint8_t const *> (val_a.mv_data), reinterpret_cast<uint8_t const *> (val_a.mv_data) + sizeof (*this), reinterpret_cast<uint8_t *> (this));
}

rai::pending_info::pending_info (rai::account const & source_a, rai::amount const & amount_a, rai::block_hash const & token_type_a) :
source (source_a),
amount (amount_a),
token_type (token_type_a)
{
}

void rai::pending_info::serialize (rai::stream & stream_a) const
{
	rai::write (stream_a, source.bytes);
	rai::write (stream_a, amount.bytes);
	rai::write (stream_a, token_type.bytes);
}

bool rai::pending_info::deserialize (rai::stream & stream_a)
{
	auto result (rai::read (stream_a, source.bytes));
	if (!result)
	{
		result = rai::read (stream_a, amount.bytes);
		if (!result)
		{
			result = rai::read (stream_a, token_type.bytes);
		}
	}
	return result;
}

bool rai::pending_info::operator== (rai::pending_info const & other_a) const
{
	return source == other_a.source && amount == other_a.amount && token_type == other_a.token_type;
}

rai::mdb_val rai::pending_info::val () const
{
	return rai::mdb_val (sizeof (*this), const_cast<rai::pending_info *> (this));
}

rai::pending_key::pending_key (rai::account const & account_a, rai::block_hash const & hash_a) :
account (account_a),
hash (hash_a)
{
}

rai::pending_key::pending_key (MDB_val const & val_a)
{
	assert (val_a.mv_size == sizeof (*this));
	static_assert (sizeof (account) + sizeof (hash) == sizeof (*this), "Packed class");
	std::copy (reinterpret_cast<uint8_t const *> (val_a.mv_data), reinterpret_cast<uint8_t const *> (val_a.mv_data) + sizeof (*this), reinterpret_cast<uint8_t *> (this));
}

void rai::pending_key::serialize (rai::stream & stream_a) const
{
	rai::write (stream_a, account.bytes);
	rai::write (stream_a, hash.bytes);
}

bool rai::pending_key::deserialize (rai::stream & stream_a)
{
	auto error (rai::read (stream_a, account.bytes));
	if (!error)
	{
		error = rai::read (stream_a, hash.bytes);
	}
	return error;
}

bool rai::pending_key::operator== (rai::pending_key const & other_a) const
{
	return account == other_a.account && hash == other_a.hash;
}

rai::mdb_val rai::pending_key::val () const
{
	return rai::mdb_val (sizeof (*this), const_cast<rai::pending_key *> (this));
}

rai::asset_key::asset_key (rai::account const & account_a, boost::asio::mutable_buffer const & key_a) :
sc_account (account_a), key (key_a), key_length (key_a.size ())
{
}

rai::asset_key::asset_key (MDB_val const & val_a)
{
	assert (val_a.mv_size > sizeof (rai::account));
	auto data (val_a.mv_data);
	std::copy (reinterpret_cast<uint8_t const *> (data), reinterpret_cast<uint8_t const *> (data) + sizeof (account), sc_account.bytes.data ());
	key_length = val_a.mv_size - sizeof (rai::account);
	assert (key_length > 0);
	const auto p1 = static_cast<uint8_t *> (key.data ());
	std::copy (reinterpret_cast<uint8_t const *> (data) + sizeof (account), reinterpret_cast<uint8_t const *> (data) + val_a.mv_size, p1);
}

rai::mdb_val rai::asset_key::val () const
{
	return rai::mdb_val (sizeof (*this), const_cast<rai::asset_key *> (this));
}

size_t rai::asset_key::len () const
{
	return sizeof (rai::account) + key.size ();
}

void rai::asset_key::serialize (rai::stream & stream_a) const
{
	rai::write (stream_a, sc_account.bytes);
	rai::write (stream_a, key_length.bytes);
	stream_a.sputn (reinterpret_cast<uint8_t const *> (key.data ()), key.size ());
}

bool rai::asset_key::deserialize (rai::stream & stream_a)
{
	auto error (rai::read (stream_a, sc_account.bytes));
	if (!error)
	{
		error = rai::read (stream_a, key_length.bytes);
		if (!error)
		{
			const auto amount_read (stream_a.sgetn (reinterpret_cast<uint8_t *> (key.data ()), static_cast<std::streamsize> (key_length.number ())));
			error = amount_read == key_length.number ();
		}
	}
	return error;
}

bool rai::asset_key::operator== (rai::asset_key const & other_a) const
{
	return sc_account == other_a.sc_account && key_length == other_a.key_length && key.size () == other_a.key.size () && memcmp (key.data (), other_a.key.data (), key.size ());
}

rai::asset_value::asset_value (boost::asio::mutable_buffer const & value_a, track_state const & state_a) :
value (value_a), value_length (value_a.size ()), state (state_a)
{
}

rai::asset_value::asset_value (MDB_val const & val_a)
{
	assert (val_a.mv_size > 0);
	auto data (val_a.mv_data);
	bufferstream asset_stream (static_cast<const uint8_t *> (val_a.mv_data), val_a.mv_size);
	deserialize (asset_stream);
}

rai::mdb_val rai::asset_value::val () const
{
	return rai::mdb_val (sizeof (*this), const_cast<rai::asset_value *> (this));
}

size_t rai::asset_value::len () const
{
	return value.size ();
}

void rai::asset_value::serialize (rai::stream & stream_a) const
{
	rai::write (stream_a, value_length.bytes);
	stream_a.sputn (reinterpret_cast<uint8_t const *> (value.data ()), value.size ());
}

bool rai::asset_value::deserialize (rai::stream & stream_a)
{
	auto error (rai::read (stream_a, value_length.bytes));
	if (!error)
	{
		const auto amount_read (stream_a.sgetn (reinterpret_cast<uint8_t *> (value.data ()), static_cast<std::streamsize> (value_length.number ())));
		error = amount_read == value_length.number ();
	}
	return error;
}

bool rai::asset_value::operator== (rai::asset_value const & other) const
{
	return value.size () == other.value.size () && value_length == other.value_length && memcmp (value.data (), other.value.data (), value.size ());
}

rai::block_info::block_info () :
account (0),
balance (0)
{
}

rai::block_info::block_info (MDB_val const & val_a)
{
	assert (val_a.mv_size == sizeof (*this));
	static_assert (sizeof (account) + sizeof (balance) == sizeof (*this), "Packed class");
	std::copy (reinterpret_cast<uint8_t const *> (val_a.mv_data), reinterpret_cast<uint8_t const *> (val_a.mv_data) + sizeof (*this), reinterpret_cast<uint8_t *> (this));
}

rai::block_info::block_info (rai::account const & account_a, rai::amount const & balance_a) :
account (account_a),
balance (balance_a)
{
}

void rai::block_info::serialize (rai::stream & stream_a) const
{
	rai::write (stream_a, account.bytes);
	rai::write (stream_a, balance.bytes);
}

bool rai::block_info::deserialize (rai::stream & stream_a)
{
	auto error (rai::read (stream_a, account.bytes));
	if (!error)
	{
		error = rai::read (stream_a, balance.bytes);
	}
	return error;
}

bool rai::block_info::operator== (rai::block_info const & other_a) const
{
	return account == other_a.account && balance == other_a.balance;
}

rai::mdb_val rai::block_info::val () const
{
	return rai::mdb_val (sizeof (*this), const_cast<rai::block_info *> (this));
}

bool rai::vote::operator== (rai::vote const & other_a) const
{
	return sequence == other_a.sequence && *block == *other_a.block && account == other_a.account && signature == other_a.signature;
}

bool rai::vote::operator!= (rai::vote const & other_a) const
{
	return !(*this == other_a);
}

std::string rai::vote::to_json () const
{
	std::stringstream stream;
	boost::property_tree::ptree tree;
	tree.put ("account", account.to_account ());
	tree.put ("signature", signature.number ());
	tree.put ("sequence", std::to_string (sequence));
	tree.put ("block", block->to_json ());
	boost::property_tree::write_json (stream, tree);
	return stream.str ();
}

rai::amount_visitor::amount_visitor (MDB_txn * transaction_a, rai::block_store & store_a) :
transaction (transaction_a),
store (store_a),
current_amount (0),
current_balance (0),
amount (0)
{
}

void rai::amount_visitor::send_block (rai::send_block const & block_a)
{
	current_balance = block_a.hashables.previous;
	amount = block_a.hashables.balance.number ();
	current_amount = 0;
}

void rai::amount_visitor::receive_block (rai::receive_block const & block_a)
{
	current_amount = block_a.hashables.source;
}

void rai::amount_visitor::open_block (rai::open_block const & block_a)
{
	if (block_a.hashables.source != rai::genesis_account)
	{
		current_amount = block_a.hashables.source;
	}
	else
	{
		amount = rai::genesis_amount;
		current_amount = 0;
	}
}

void rai::amount_visitor::state_block (rai::state_block const & block_a)
{
	current_balance = block_a.hashables.previous;
	amount = block_a.hashables.balance.number ();
	current_amount = 0;
}

void rai::amount_visitor::change_block (rai::change_block const & block_a)
{
	amount = 0;
	current_amount = 0;
}

void rai::amount_visitor::compute (rai::block_hash const & block_hash)
{
	current_amount = block_hash;
	rai::genesis genesis (rai::rai_live_genesis);
	auto genesis_open_hash (genesis.hash ());
	while (!current_amount.is_zero () || !current_balance.is_zero ())
	{
		if (!current_amount.is_zero ())
		{
			auto block (store.block_get (transaction, current_amount));
			if (block != nullptr)
			{
				block->visit (*this);
			}
			else
			{
				if (block_hash == genesis_open_hash)
				{
					amount = std::numeric_limits<rai::uint128_t>::max ();
					current_amount = 0;
				}
				else
				{
					assert (false);
					amount = 0;
					current_amount = 0;
				}
			}
		}
		else
		{
			balance_visitor prev (transaction, store);
			prev.compute (current_balance);
			amount = amount < prev.balance ? prev.balance - amount : amount - prev.balance;
			current_balance = 0;
		}
	}
}

void rai::amount_visitor::smart_contract_block (rai::smart_contract_block const & block_a)
{
	amount = 0;
	current_amount = 0;
}

rai::balance_visitor::balance_visitor (MDB_txn * transaction_a, rai::block_store & store_a) :
transaction (transaction_a),
store (store_a),
current_balance (0),
current_amount (0),
balance (0)
{
}

void rai::balance_visitor::send_block (rai::send_block const & block_a)
{
	balance += block_a.hashables.balance.number ();
	current_balance = 0;
}

void rai::balance_visitor::receive_block (rai::receive_block const & block_a)
{
	rai::block_info block_info;
	if (!store.block_info_get (transaction, block_a.hash (), block_info))
	{
		balance += block_info.balance.number ();
		current_balance = 0;
	}
	else
	{
		current_amount = block_a.hashables.source;
		current_balance = block_a.hashables.previous;
	}
}

void rai::balance_visitor::open_block (rai::open_block const & block_a)
{
	current_amount = block_a.hashables.source;
	current_balance = 0;
}

void rai::balance_visitor::change_block (rai::change_block const & block_a)
{
	rai::block_info block_info;
	if (!store.block_info_get (transaction, block_a.hash (), block_info))
	{
		balance += block_info.balance.number ();
		current_balance = 0;
	}
	else
	{
		current_balance = block_a.hashables.previous;
	}
}

void rai::balance_visitor::state_block (rai::state_block const & block_a)
{
	balance = block_a.hashables.balance.number ();
	current_balance = 0;
}

void rai::balance_visitor::smart_contract_block (rai::smart_contract_block const &)
{
	current_amount = 0;
	current_amount = 0;
}

void rai::balance_visitor::compute (rai::block_hash const & block_hash)
{
	current_balance = block_hash;
	while (!current_balance.is_zero () || !current_amount.is_zero ())
	{
		if (!current_amount.is_zero ())
		{
			amount_visitor source (transaction, store);
			source.compute (current_amount);
			balance += source.amount;
			current_amount = 0;
		}
		else
		{
			auto block (store.block_get (transaction, current_balance));
			assert (block != nullptr);
			block->visit (*this);
		}
	}
}

rai::representative_visitor::representative_visitor (MDB_txn * transaction_a, rai::block_store & store_a) :
transaction (transaction_a),
store (store_a),
result (0)
{
}

void rai::representative_visitor::compute (rai::block_hash const & hash_a)
{
	current = hash_a;
	while (result.is_zero ())
	{
		auto block (store.block_get (transaction, current));
		assert (block != nullptr);
		block->visit (*this);
	}
}

void rai::representative_visitor::send_block (rai::send_block const & block_a)
{
	current = block_a.previous ();
}

void rai::representative_visitor::receive_block (rai::receive_block const & block_a)
{
	current = block_a.previous ();
}

void rai::representative_visitor::open_block (rai::open_block const & block_a)
{
	result = block_a.hash ();
}

void rai::representative_visitor::change_block (rai::change_block const & block_a)
{
	result = block_a.hash ();
}

void rai::representative_visitor::state_block (rai::state_block const & block_a)
{
	result = block_a.hash ();
}

rai::vote::vote (rai::vote const & other_a) :
sequence (other_a.sequence),
block (other_a.block),
account (other_a.account),
signature (other_a.signature)
{
}

rai::vote::vote (bool & error_a, rai::stream & stream_a)
{
	if (!error_a)
	{
		error_a = rai::read (stream_a, account.bytes);
		if (!error_a)
		{
			error_a = rai::read (stream_a, signature.bytes);
			if (!error_a)
			{
				error_a = rai::read (stream_a, sequence);
				if (!error_a)
				{
					block = rai::deserialize_block (stream_a);
					error_a = block == nullptr;
				}
			}
		}
	}
}

rai::vote::vote (bool & error_a, rai::stream & stream_a, rai::block_type type_a)
{
	if (!error_a)
	{
		error_a = rai::read (stream_a, account.bytes);
		if (!error_a)
		{
			error_a = rai::read (stream_a, signature.bytes);
			if (!error_a)
			{
				error_a = rai::read (stream_a, sequence);
				if (!error_a)
				{
					block = rai::deserialize_block (stream_a, type_a);
					error_a = block == nullptr;
				}
			}
		}
	}
}

rai::vote::vote (rai::account const & account_a, rai::raw_key const & prv_a, uint64_t sequence_a, std::shared_ptr<rai::block> block_a) :
sequence (sequence_a),
block (block_a),
account (account_a),
signature (rai::sign_message (prv_a, account_a, hash ()))
{
}

rai::vote::vote (MDB_val const & value_a)
{
	rai::bufferstream stream (reinterpret_cast<uint8_t const *> (value_a.mv_data), value_a.mv_size);
	auto error (rai::read (stream, account.bytes));
	assert (!error);
	error = rai::read (stream, signature.bytes);
	assert (!error);
	error = rai::read (stream, sequence);
	assert (!error);
	block = rai::deserialize_block (stream);
	assert (block != nullptr);
}

rai::uint256_union rai::vote::hash () const
{
	rai::uint256_union result;
	blake2b_state hash;
	blake2b_init (&hash, sizeof (result.bytes));
	blake2b_update (&hash, block->hash ().bytes.data (), sizeof (result.bytes));
	union
	{
		uint64_t qword;
		std::array<uint8_t, 8> bytes;
	};
	qword = sequence;
	blake2b_update (&hash, bytes.data (), sizeof (bytes));
	blake2b_final (&hash, result.bytes.data (), sizeof (result.bytes));
	return result;
}

void rai::vote::serialize (rai::stream & stream_a, rai::block_type)
{
	write (stream_a, account);
	write (stream_a, signature);
	write (stream_a, sequence);
	block->serialize (stream_a);
}

void rai::vote::serialize (rai::stream & stream_a)
{
	write (stream_a, account);
	write (stream_a, signature);
	write (stream_a, sequence);
	rai::serialize_block (stream_a, *block);
}
bool rai::vote::deserialize (rai::stream & stream_a)
{
	auto result (read (stream_a, account));
	if (!result)
	{
		result = read (stream_a, signature);
		if (!result)
		{
			result = read (stream_a, sequence);
			if (!result)
			{
				block = rai::deserialize_block (stream_a, block_type ());
				result = block == nullptr;
			}
		}
	}
	return result;
}
bool rai::vote::validate ()
{
	auto result (rai::validate_message (account, hash (), signature));
	return result;
}

rai::genesis::genesis (std::string genesis_block)
{
	boost::property_tree::ptree tree;
	std::stringstream istream (genesis_block);
	boost::property_tree::read_json (istream, tree);
	auto block (rai::deserialize_block_json (tree));
	//QLINK：更换创世区块为state block
	assert (dynamic_cast<rai::state_block *> (block.get ()) != nullptr);
	state.reset (static_cast<rai::state_block *> (block.release ()));
}

void rai::genesis::initialize (MDB_txn * transaction_a, rai::block_store & store_a) const
{
	auto hash_l (hash ());
	//	assert (store_a.latest_begin (transaction_a) == store_a.latest_end ());
	store_a.block_put (transaction_a, hash_l, *state);
	store_a.account_put (transaction_a, hash_l, { hash_l, hash_l, hash_l, state->hashables.balance, rai::seconds_since_epoch (), 1, state->hashables.token_hash, state->hashables.account });
	store_a.representation_put (transaction_a, rai::genesis_account, rai::genesis_amount);
	store_a.checksum_put (transaction_a, 0, 0, hash_l);
	store_a.frontier_put (transaction_a, hash_l, hash_l);
}

rai::block_hash rai::genesis::hash () const
{
	return state->hash ();
}
rai::genesis_sc_block::genesis_sc_block (std::string genesis_smart_contract_block)
{
	boost::property_tree::ptree tree;
	std::stringstream istream (genesis_smart_contract_block);
	boost::property_tree::read_json (istream, tree);
	auto block (rai::deserialize_block_json (tree));
	assert (dynamic_cast<rai::smart_contract_block *> (block.get ()) != nullptr);
	sc_block.reset (static_cast<rai::smart_contract_block *> (block.release ()));
}

void rai::genesis_sc_block::initialize (MDB_txn * transaction_a, rai::block_store & store_a) const
{
	auto hash_l (hash ());
	store_a.block_put (transaction_a, hash_l, *sc_block);
}

rai::block_hash rai::genesis_sc_block::hash () const
{
	return sc_block->hash ();
}
