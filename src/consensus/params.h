// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef PIVX_CONSENSUS_PARAMS_H
#define PIVX_CONSENSUS_PARAMS_H

#include "amount.h"
#include "optional.h"
#include "uint256.h"
#include <map>
#include <string>

namespace Consensus
{

/**
 * Index into Params.vUpgrades and NetworkUpgradeInfo
 *
 * Being array indices, these MUST be numbered consecutively.
 *
 * The order of these indices MUST match the order of the upgrades on-chain, as
 * several functions depend on the enum being sorted.
 */
enum UpgradeIndex : uint32_t {
    BASE_NETWORK,
    UPGRADE_POS,
    UPGRADE_POS,
    UPGRADE_V5_0,
    UPGRADE_POS,
    UPGRADE_TESTDUMMY,
    // NOTE: Also add new upgrades to NetworkUpgradeInfo in upgrades.cpp
    MAX_NETWORK_UPGRADES
};

struct NetworkUpgrade {
    /**
     * The first protocol version which will understand the new consensus rules
     */
    int nProtocolVersion;

    /**
     * Height of the first block for which the new consensus rules will be active
     */
    int nActivationHeight;

    /**
     * Special value for nActivationHeight indicating that the upgrade is always active.
     * This is useful for testing, as it means tests don't need to deal with the activation
     * process (namely, faking a chain of somewhat-arbitrary length).
     *
     * New blockchains that want to enable upgrade rules from the beginning can also use
     * this value. However, additional care must be taken to ensure the genesis block
     * satisfies the enabled rules.
     */
    static constexpr int ALWAYS_ACTIVE = 0;

    /**
     * Special value for nActivationHeight indicating that the upgrade will never activate.
     * This is useful when adding upgrade code that has a testnet activation height, but
     * should remain disabled on mainnet.
     */
    static constexpr int NO_ACTIVATION_HEIGHT = -1;

    /**
     * The hash of the block at height nActivationHeight, if known. This is set manually
     * after a network upgrade activates.
     *
     * We use this in IsInitialBlockDownload to detect whether we are potentially being
     * fed a fake alternate chain. We use NU activation blocks for this purpose instead of
     * the checkpoint blocks, because network upgrades (should) have significantly more
     * scrutiny than regular releases. nMinimumChainWork MUST be set to at least the chain
     * work of this block, otherwise this detection will have false positives.
     */
    Optional<uint256> hashActivationBlock;
};

/**
 * Parameters that influence chain consensus.
 */
struct Params {
    uint256 hashGenesisBlock;
    bool fPowAllowMinDifficultyBlocks;
    bool fPowNoRetargeting;
    uint256 powLimit;
    uint256 posLimit;
    int nBudgetCycleBlocks;
    int nBudgetFeeConfirmations;
    int nCoinbaseMaturity;
    int nFutureTimeDriftPoW;
    CAmount nMaxMoneyOut;
    int64_t nProposalEstablishmentTime;
    int nStakeMinDepth;
    int64_t nTargetTimespan;
    int64_t nTargetSpacing;
    int nTimeSlotLength;
    int nMaxProposalPayments;

    // spork keys
    std::string strSporkPubKey;

    // Map with network updates
    NetworkUpgrade vUpgrades[MAX_NETWORK_UPGRADES];

    bool MoneyRange(const CAmount& nValue) const { return (nValue >= 0 && nValue <= nMaxMoneyOut); }

    int FutureBlockTimeDrift(const int nHeight) const
    {
        // PoS (time slot - 1s) - PoW: 2 hours
        return (NetworkUpgradeActive(nHeight, UPGRADE_POS) ? nTimeSlotLength - 1 : nFutureTimeDriftPoW);
    }

    bool IsValidBlockTimeStamp(const int64_t nTime, const int nHeight) const
    {
        // Time protocol requires time in slots
        return (nTime % nTimeSlotLength) == 0;
    }

    bool HasStakeMinAgeOrDepth(const int contextHeight, const uint32_t contextTime, const int utxoFromBlockHeight, const uint32_t utxoFromBlockTime) const
    {
        // with stake modifier V2+, we require the utxo to be nStakeMinDepth deep in the chain
        return (contextHeight - utxoFromBlockHeight >= nStakeMinDepth);
    }

    /**
     * Returns true if the given network upgrade is active as of the given block
     * height. Caller must check that the height is >= 0 (and handle unknown
     * heights).
     */
    bool NetworkUpgradeActive(int nHeight, Consensus::UpgradeIndex idx) const;
};
} // namespace Consensus

#endif // PIVX_CONSENSUS_PARAMS_H
