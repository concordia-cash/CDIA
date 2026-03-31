// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2022 The PIVX Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "budget/budgetmanager.h"

#include "consensus/validation.h"
#include "netmessagemaker.h"
#include "tiertwo/tiertwo_sync_state.h"
#include "tiertwo/netfulfilledman.h"
#include "util/validation.h"
#include "validation.h"   // GetTransaction, cs_main

#ifdef ENABLE_WALLET
#include "wallet/wallet.h" // future: use interface instead.
#endif


#define BUDGET_ORPHAN_VOTES_CLEANUP_SECONDS (60 * 60) // One hour.
// Request type used in the net requests manager to block peers asking budget sync too often
static const std::string BUDGET_SYNC_REQUEST_RECV = "budget-sync-recv";

CBudgetManager g_budgetman;

// Used to check both proposals and finalized-budgets collateral txes
bool CheckCollateral(const uint256& nTxCollateralHash, const uint256& nExpectedHash, std::string& strError, int64_t& nTime, int nCurrentHeight, bool fBudgetFinalization);

void CBudgetManager::ReloadMapSeen()
{
}

void CBudgetManager::CheckOrphanVotes()
{
}

uint256 CBudgetManager::SubmitFinalBudget()
{
    return UINT256_ZERO;
}

void CBudgetManager::SetBudgetProposalsStr(CFinalizedBudget& finalizedBudget) const
{
}

std::string CBudgetManager::GetFinalizedBudgetStatus(const uint256& nHash) const
{
    return "";
}

bool CBudgetManager::AddFinalizedBudget(CFinalizedBudget& finalizedBudget, CNode* pfrom)
{
    return true;
}

void CBudgetManager::ForceAddFinalizedBudget(const uint256& nHash, const uint256& feeTxId, const CFinalizedBudget& finalizedBudget)
{
}

bool CBudgetManager::AddProposal(CBudgetProposal& budgetProposal)
{
    return true;
}

void CBudgetManager::CheckAndRemove()
{
}

void CBudgetManager::RemoveByFeeTxId(const uint256& feeTxId)
{
}

CBudgetManager::HighestFinBudget CBudgetManager::GetBudgetWithHighestVoteCount(int chainHeight) const
{
    int highestVoteCount = 0;
    const CFinalizedBudget* pHighestBudget = nullptr;
    
    return {pHighestBudget, highestVoteCount};
}

int CBudgetManager::GetHighestVoteCount(int chainHeight) const
{
    return 0;
}

bool CBudgetManager::GetPayeeAndAmount(int chainHeight, CScript& payeeRet, CAmount& nAmountRet) const
{
    return true;
}

bool CBudgetManager::GetExpectedPayeeAmount(int chainHeight, CAmount& nAmountRet) const
{
    return true;
}

bool CBudgetManager::FillBlockPayee(CMutableTransaction& txCoinbase, CMutableTransaction& txCoinstake, const int nHeight, bool fProofOfStake) const
{
    return true;
}

CFinalizedBudget* CBudgetManager::FindFinalizedBudget(const uint256& nHash)
{
    return nullptr;
}

const CBudgetProposal* CBudgetManager::FindProposalByName(const std::string& strProposalName) const
{
    return nullptr;
}

CBudgetProposal* CBudgetManager::FindProposal(const uint256& nHash)
{
    return nullptr;
}

bool CBudgetManager::GetProposal(const uint256& nHash, CBudgetProposal& bp) const
{
    return true;
}

bool CBudgetManager::GetFinalizedBudget(const uint256& nHash, CFinalizedBudget& fb) const
{
    return true;
}

bool CBudgetManager::IsBudgetPaymentBlock(int nBlockHeight, int& nCountThreshold) const
{
    return false;
}

bool CBudgetManager::IsBudgetPaymentBlock(int nBlockHeight) const
{
    int nCountThreshold;
    return IsBudgetPaymentBlock(nBlockHeight, nCountThreshold);
}

TrxValidationStatus CBudgetManager::IsTransactionValid(const CTransaction& txNew, const uint256& nBlockHash, int nBlockHeight) const
{
    return TrxValidationStatus::InValid;
}

std::vector<CBudgetProposal*> CBudgetManager::GetAllProposalsOrdered()
{
    std::vector<CBudgetProposal*> vBudgetProposalRet;
    return vBudgetProposalRet;
}

std::vector<CBudgetProposal> CBudgetManager::GetBudget()
{
    std::vector<CBudgetProposal> vBudgetProposalsRet;
    return vBudgetProposalsRet;
}

std::vector<CFinalizedBudget*> CBudgetManager::GetFinalizedBudgets()
{
    std::vector<CFinalizedBudget*> vFinalizedBudgetsRet;
    return vFinalizedBudgetsRet;
}

std::string CBudgetManager::GetRequiredPaymentsString(int nBlockHeight)
{
    return "";
}

CAmount CBudgetManager::GetTotalBudget(int nHeight)
{
    return 0;
}

void CBudgetManager::AddSeenProposalVote(const CBudgetVote& vote)
{
}

void CBudgetManager::AddSeenFinalizedBudgetVote(const CFinalizedBudgetVote& vote)
{
}

void CBudgetManager::RemoveStaleVotesOnProposal(CBudgetProposal* prop)
{
}

void CBudgetManager::RemoveStaleVotesOnFinalBudget(CFinalizedBudget* fbud)
{
}

CDataStream CBudgetManager::GetProposalVoteSerialized(const uint256& voteHash) const
{
    LOCK(cs_votes);
    CDataStream ss(SER_NETWORK, PROTOCOL_VERSION);
    ss.reserve(1000);
    ss << mapSeenProposalVotes.at(voteHash);
    return ss;
}

CDataStream CBudgetManager::GetProposalSerialized(const uint256& propHash) const
{
    LOCK(cs_proposals);
    return mapProposals.at(propHash).GetBroadcast();
}

CDataStream CBudgetManager::GetFinalizedBudgetVoteSerialized(const uint256& voteHash) const
{
    LOCK(cs_finalizedvotes);
    CDataStream ss(SER_NETWORK, PROTOCOL_VERSION);
    ss.reserve(1000);
    ss << mapSeenFinalizedBudgetVotes.at(voteHash);
    return ss;
}

CDataStream CBudgetManager::GetFinalizedBudgetSerialized(const uint256& budgetHash) const
{
    LOCK(cs_budgets);
    return mapFinalizedBudgets.at(budgetHash).GetBroadcast();
}

bool CBudgetManager::AddAndRelayProposalVote(const CBudgetVote& vote, std::string& strError)
{
    if (UpdateProposal(vote, nullptr, strError)) {
        AddSeenProposalVote(vote);
        vote.Relay();
        return true;
    }
    return false;
}

void CBudgetManager::UpdatedBlockTip(const CBlockIndex *pindexNew, const CBlockIndex *pindexFork, bool fInitialDownload)
{
}

int CBudgetManager::ProcessBudgetVoteSync(const uint256& nProp, CNode* pfrom)
{
    return 0;
}

int CBudgetManager::ProcessProposal(CBudgetProposal& proposal)
{
    return 0;
}

bool CBudgetManager::ProcessProposalVote(CBudgetVote& vote, CNode* pfrom, CValidationState& state)
{
    return true;
}

int CBudgetManager::ProcessFinalizedBudget(CFinalizedBudget& finalbudget, CNode* pfrom)
{
    return 0;
}

bool CBudgetManager::ProcessFinalizedBudgetVote(CFinalizedBudgetVote& vote, CNode* pfrom, CValidationState& state)
{
    return true;
}

void CBudgetManager::SetSynced(bool synced)
{
}

template<typename T>
static bool relayItemIfFound(const uint256& itemHash, CNode* pfrom, RecursiveMutex& cs, std::map<uint256, T>& map, const char* type)
{
    return true;
}

template<typename T>
static void relayInventoryItems(CNode* pfrom, RecursiveMutex& cs, std::map<uint256, T>& map, bool fPartial, GetDataMsg invType, const int mn_sync_budget_type)
{
}

void CBudgetManager::SyncSingleItem(CNode* pfrom, const uint256& nProp)
{
}


void CBudgetManager::Sync(CNode* pfrom, bool fPartial)
{
}

template<typename T>
static void TryAppendOrphanVoteMap(const T& vote,
                                   const uint256& parentHash,
                                   std::map<uint256, std::pair<std::vector<T>, int64_t>>& mapOrphan,
                                   std::map<uint256, T>& mapSeen)
{
}

bool CBudgetManager::UpdateProposal(const CBudgetVote& vote, CNode* pfrom, std::string& strError)
{
    return true;
}

bool CBudgetManager::UpdateFinalizedBudget(const CFinalizedBudgetVote& vote, CNode* pfrom, std::string& strError)
{
    return false;
}

std::string CBudgetManager::ToString() const
{
    return "";
}

/*
 * Check Collateral
 */
bool CheckCollateralConfs(const uint256& nTxCollateralHash, int nCurrentHeight, int nProposalHeight, std::string& strError)
{
    return false;
}

bool CheckCollateral(const uint256& nTxCollateralHash, const uint256& nExpectedHash, std::string& strError, int64_t& nTime, int nCurrentHeight, bool fBudgetFinalization)
{
    return false;
}
