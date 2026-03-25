// Copyright (c) 2021 The PIVX Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php.

#ifndef PIVX_TIERTWO_TIERTWO_SYNC_STATE_H
#define PIVX_TIERTWO_TIERTWO_SYNC_STATE_H

#include <atomic>
#include <map>

#define MASTERNODE_SYNC_INITIAL 0
#define MASTERNODE_SYNC_SPORKS 1
#define MASTERNODE_SYNC_FAILED 998
#define MASTERNODE_SYNC_FINISHED 999

// Sync threshold
#define MASTERNODE_SYNC_THRESHOLD 2

// Chain sync update window.
// Be careful with this value. The smaller the value is, the more the tiertwo sync locks 'g_best_block_mutex'.
#define CHAIN_SYNC_UPDATE_TIME 30

class uint256;

class TierTwoSyncState {
public:
    bool IsBlockchainSynced() const { return fBlockchainSynced; };
    bool IsSynced() const { return m_current_sync_phase == MASTERNODE_SYNC_FINISHED; }
    bool IsSporkListSynced() const { return m_current_sync_phase > MASTERNODE_SYNC_SPORKS; }

    // Reset seen data
    void ResetData();

    // Only called from masternodesync and unit tests.
    void SetBlockchainSync(bool f, int64_t cur_time) {
        fBlockchainSynced = f;
        last_blockchain_sync_update_time = cur_time;
    };
    void SetCurrentSyncPhase(int sync_phase) { m_current_sync_phase = sync_phase; };
    int GetSyncPhase() const { return m_current_sync_phase; }

    // True if the last chain sync update was more than CHAIN_SYNC_UPDATE_TIME seconds ago
    bool CanUpdateChainSync(int64_t cur_time) const { return cur_time > last_blockchain_sync_update_time + CHAIN_SYNC_UPDATE_TIME; }

private:
    std::atomic<bool> fBlockchainSynced{false};
    std::atomic<int64_t> last_blockchain_sync_update_time{0};
    std::atomic<int> m_current_sync_phase{0};
};

extern TierTwoSyncState g_tiertwo_sync_state;

#endif // PIVX_TIERTWO_TIERTWO_SYNC_STATE_H
