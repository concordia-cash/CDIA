// Copyright (c) 2021-2022 The PIVX Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php.

#include "tiertwo/init.h"

#include "budget/budgetdb.h"
#include "flatdb.h"
#include "guiinterface.h"
#include "guiinterfaceutil.h"
#include "scheduler.h"
#include "tiertwo/netfulfilledman.h"
#include "validation.h"
#include "wallet/wallet.h"

#include <boost/thread.hpp>

void InitTierTwoInterfaces()
{
}

void ResetTierTwoInterfaces()
{
}

void InitTierTwoPreChainLoad(bool fReindex)
{
}

void InitTierTwoPostCoinsCacheLoad(CScheduler* scheduler)
{
}

void InitTierTwoChainTip()
{
}

bool LoadTierTwo(int chain_active_height, bool load_cache_files)
{
    // ##################### //
    // ## Budget Manager ### //
    // ##################### //
    uiInterface.InitMessage(_("Loading budget cache..."));

    CBudgetDB budgetdb;
    const bool fDryRun = (chain_active_height <= 0);
    if (!fDryRun) g_budgetman.SetBestHeight(chain_active_height);
    CBudgetDB::ReadResult readResult2 = budgetdb.Read(g_budgetman, fDryRun);

    if (readResult2 == CBudgetDB::FileError)
        LogPrintf("Missing budget cache - budget.dat, will try to recreate\n");
    else if (readResult2 != CBudgetDB::Ok) {
        LogPrintf("Error reading budget.dat - cached data discarded\n");
    }

    // flag our cached items so we send them to our peers
    g_budgetman.ResetSync();
    g_budgetman.ReloadMapSeen();

    // ############################## //
    // ## Network Requests Manager ## //
    // ############################## //
    uiInterface.InitMessage(_("Loading network requests cache..."));
    CFlatDB<CNetFulfilledRequestManager> netRequestsDb(NET_REQUESTS_CACHE_FILENAME, NET_REQUESTS_CACHE_FILE_ID);
    if (load_cache_files) {
        if (!netRequestsDb.Load(g_netfulfilledman)) {
            LogPrintf("Failed to load network requests cache from %s\n", netRequestsDb.GetDbPath().string());
        }
    } else {
        CNetFulfilledRequestManager netfulfilledmanTmp(0);
        if (!netRequestsDb.Dump(netfulfilledmanTmp)) {
            LogPrintf("Failed to clear network requests cache at %s\n", netRequestsDb.GetDbPath().string());
        }
    }

    return true;
}

void RegisterTierTwoValidationInterface()
{
    RegisterValidationInterface(&g_budgetman);
}

void DumpTierTwo()
{
    DumpBudgets(g_budgetman);
    CFlatDB<CNetFulfilledRequestManager>(NET_REQUESTS_CACHE_FILENAME, NET_REQUESTS_CACHE_FILE_ID).Dump(g_netfulfilledman);
}

void SetBudgetFinMode(const std::string& mode)
{
    g_budgetman.strBudgetMode = mode;
    LogPrintf("Budget Mode %s\n", g_budgetman.strBudgetMode);
}

void StartTierTwoThreadsAndScheduleJobs(boost::thread_group& threadGroup, CScheduler& scheduler)
{
    scheduler.scheduleEvery(std::bind(&CNetFulfilledRequestManager::DoMaintenance, std::ref(g_netfulfilledman)), 60 * 1000);
}

void StopTierTwoThreads()
{
}

void DeleteTierTwo()
{
}

void InterruptTierTwo()
{
}
