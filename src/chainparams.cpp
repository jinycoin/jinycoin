// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2019 JINY developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <assert.h>

#include <chainparamsseeds.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 0 << OP_0 << 504365040 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "JINYCOIN";
    const CScript genesisOutputScript = CScript() << ParseHex("04f0cd09bd41b4d34636b96ebd90c198fff8078bf8772a4ae1510ab6312b82781e70c385757263aab8250e7432e21f77d0d1210e6ab47f1cfccf52769ef71ded90") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 20 * 365 * 1440; // 20 common years
        consensus.nMinimumSubsidy = 20.00000000 * COIN;

        consensus.nMasternodeMinimumConfirmations = 15;
	consensus.nMasternodePaymentsStartBlock = 50;
        consensus.nMasternodePaymentsIncreaseBlock = 50;
        consensus.nMasternodePaymentsIncreasePeriod = 20 * 365 * 1440; // 20 common years
        consensus.nMasternodeCollateralMinimum = 1000000; // starting MN collateral
        consensus.nMasternodeCollateralMaximum = 1000000; // MN collateral at infinity
        consensus.nMasternodeCollateralOnChain = 10000000; // MN collateral on chain

        consensus.nInstantSendKeepLock = 24;

        consensus.nBudgetPaymentsStartBlock = 365 * 1440; // 1 common year
        consensus.nBudgetPaymentsCycleBlocks = 10958; // weekly
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nBudgetProposalEstablishingTime = 86400; // 1 day

        consensus.nSuperblockStartBlock = 365 * 1440; // 1 common year
        consensus.nSuperblockCycle = 10958; // weekly

        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;

        // JINY TODO: BIP16Exception is valid for BTC blockchain only
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = 0; // genesis
        consensus.BIP34Hash = uint256S("0x1fc41f203d6b2305c5f527b6db69e84fa6a1db343ead5c5a5153d0a10afa9dff"); // genesis hash
        consensus.BIP65Height = 0; // genesis
        consensus.BIP66Height = 0; // genesis
        consensus.powLimit = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 86400; // one day
        consensus.nPowTargetSpacing = 150; // two and half minutes
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // JINY TODO:
        // Dash
        // Deployment of DIP0001
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 4032;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 3226; // 80% of 4032
        //
        //

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000200011"); // block 1

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x3b15ed0578c29346a7598f20609dd447f68df21c5026491b87d39cefd854be28"); // block 1

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xF1;
        pchMessageStart[1] = 0x49;
        pchMessageStart[2] = 0x94;
        pchMessageStart[3] = 0x1F;
        nDefaultPort = 19411;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1555818181, 10285862, 0x1e0ffff0, 536870912, consensus.nMinimumSubsidy);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x1fc41f203d6b2305c5f527b6db69e84fa6a1db343ead5c5a5153d0a10afa9dff"));
        assert(genesis.hashMerkleRoot == uint256S("0x00415229df8ebb07cdf53714c6b1220d2466b6d548fe78afc5f6e36ee651d87d"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        vSeeds.emplace_back("seeds.jinycoin.com"); // JINY founder seeds
        //vSeeds.emplace_back("seeds.on.another.domain"); // dns seeds template

        // JINY prefix 'j'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,105);
        // JINY prefix 'J'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,43);
        // JINY prefix 'X'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,207);
        // BIP32 prefixes
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "jiny";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
        strSporkPubKey = "042e0fbe10e95a976902731fe0744a20f555df2f0a1d8db346073a449f086c67cb6f6798b0b1b8fa7b24b68ea96801b4d6197655570d4e4fb3ce3e8d6a45e1f652";

        founderAddress = "";

        checkpointData = {
            {
                {     0, uint256S("0x1fc41f203d6b2305c5f527b6db69e84fa6a1db343ead5c5a5153d0a10afa9dff")},
                {     1, uint256S("0x3b15ed0578c29346a7598f20609dd447f68df21c5026491b87d39cefd854be28")},
                { 49000, uint256S("0x159cfbbb4af957ce2bc3a56699f876a7f340fc2c75f2ba6f5a4a445439e2d122")},
            }
        };
        // JINY BEGIN
        consensus.nlastValidPowHashHeight = 49000;
        // JINY

        chainTxData = ChainTxData{
            // Data as of block 6b95228469ee82436931bfbf765d44add581e0f483f2466ed0a536f2b16beace (height 1).
            1555718401, // * UNIX timestamp of last known number of transactions
            1,          // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0           // * estimated number of transactions per second after that timestamp
        };

        // JINY TODO: we need to resolve fee calculation bug and disable fallback
        ///* disable fallback fee on mainnet */
        //m_fallback_fee_enabled = false;
        m_fallback_fee_enabled = true;
        //
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 2 * 365 * 1440; // 2 common years
        consensus.nMinimumSubsidy = 0.00100000 * COIN;

        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nMasternodePaymentsStartBlock = 10; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 10;
        consensus.nMasternodePaymentsIncreasePeriod = 25;
        consensus.nMasternodeCollateralMinimum = 10; // starting MN collateral
        consensus.nMasternodeCollateralMaximum = 1000; // MN collateral at infinity
        consensus.nMasternodeCollateralOnChain = 10000000; // MN collateral on chain

        consensus.nInstantSendKeepLock = 6;

        consensus.nBudgetPaymentsStartBlock = 4100;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;

        consensus.nSuperblockStartBlock = 4200;
        consensus.nSuperblockCycle = 24;

        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;

        // JINY TODO: BIP16Exception is valid for BTC blockchain only
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = 0; // genesis
        consensus.BIP34Hash = uint256S("0x7bdb34e67def799569f215681c8e2788ce5c215f9bb615504ce7abcb32e50416"); // genesis hash
        consensus.BIP65Height = 0; // genesis
        consensus.BIP66Height = 0; // genesis
        consensus.powLimit = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 1 * 24 * 60 * 60; // one day
        consensus.nPowTargetSpacing = 1 * 60; // one minute
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // JINY TODO:
        // Dash
        // Deployment of DIP0001
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 100;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 50; // 50% of 100
        //
        //

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000"); // block 0

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0xdca443db9bcde23a61ee1d53741a61809e0d95bb1655672acd6c5e96ee7fcab7"); // block 0

        pchMessageStart[0] = 0x89;
        pchMessageStart[1] = 0x64;
        pchMessageStart[2] = 0x46;
        pchMessageStart[3] = 0x98;
        nDefaultPort = 29411;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1555818181, 21341209, 0x1e0ffff0, 536870912, consensus.nMinimumSubsidy);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xdca443db9bcde23a61ee1d53741a61809e0d95bb1655672acd6c5e96ee7fcab7"));
        assert(genesis.hashMerkleRoot == uint256S("0x7e4573c4fa6150c839ae703ed30302e2a3e6f7a1be18de13181dfb1e265c9694"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("testnet-seeds.jinycoin.com");

        // JINY prefix 't'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,128);
        // JINY prefix 'T'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,65);
        // JINY prefix 'Y'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,214);
        // BIP32 prefixes
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "testnet1jiny";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes
        strSporkPubKey = "04f0cd09bd41b4d34636b96ebd90c198fff8078bf8772a4ae1510ab6312b82781e70c385757263aab8250e7432e21f77d0d1210e6ab47f1cfccf52769ef71ded90";

        founderAddress = "";

        checkpointData = {
            {
                {0, uint256S("0xdca443db9bcde23a61ee1d53741a61809e0d95bb1655672acd6c5e96ee7fcab7")},
            }
        };
        // JINY BEGIN
        consensus.nlastValidPowHashHeight = 0;
        // JINY

        chainTxData = ChainTxData{
            // Data as of block 00000706ff7091032d187cf1c2515a5d7891b6797f414526ef8a277491117d08 (height 0)
            1527900028,
            5,
            0
        };

        /* enable fallback fee on testnet */
        m_fallback_fee_enabled = true;
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMinimumSubsidy = 10000.00000000 * COIN;

        consensus.nMasternodeMinimumConfirmations = 15;
	consensus.nMasternodePaymentsStartBlock = 240;
        consensus.nMasternodePaymentsIncreaseBlock = 350;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nMasternodeCollateralMinimum = 1; // starting MN collateral
        consensus.nMasternodeCollateralMaximum = 100; // MN collateral at infinity
        consensus.nMasternodeCollateralOnChain = 10000000; // MN collateral on chain

        consensus.nInstantSendKeepLock = 6;

        consensus.nBudgetPaymentsStartBlock = 1000;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;

        consensus.nSuperblockStartBlock = 1500;
        consensus.nSuperblockCycle = 10;

        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 100;

        // JINY TODO: BIP16Exception is valid for BTC blockchain only
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = 0; // BIP34 has activated on regtest (blocks v1 are rejected in tests)
        consensus.BIP34Hash = uint256S("0x4de29f1ca3467bd0f02604317ff373b06db06146469c652f78d15d2515ea75bd"); //genesis hash
        consensus.BIP65Height = 0; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 0; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 1 * 60; // one minute
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // JINY TODO:
        // Dash
        // Deployment of DIP0001
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 0;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        //
        //

        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0x99;
        pchMessageStart[1] = 0x44;
        pchMessageStart[2] = 0x66;
        pchMessageStart[3] = 0x88;
        nDefaultPort = 39411;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1555818181, 11134245, 0x1e0ffff0, 536870912, consensus.nMinimumSubsidy);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x4de29f1ca3467bd0f02604317ff373b06db06146469c652f78d15d2515ea75bd"));
        assert(genesis.hashMerkleRoot == uint256S("0x534c55b8ab6fd704e1867a80fa4865a4f83d58a50c5f130930babf113cb857a7"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        founderAddress = "";

        checkpointData = {
            {
                {0, uint256S("0x4de29f1ca3467bd0f02604317ff373b06db06146469c652f78d15d2515ea75bd")},
            }
        };
        // JINY BEGIN
        consensus.nlastValidPowHashHeight = 0;
        // JINY

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        // Bitcoin defaults
        // JINY prefix 'c'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,88);
        // JINY prefix 'C'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,28);
        // JINY prefix 'Z'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,221);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "regtest1jiny";

        /* enable fallback fee on regtest */
        m_fallback_fee_enabled = true;
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
