// Copyright (c) 2014-2016 The Dash developers
// Copyright (c) 2016-2022 The PIVX Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef PIVX_SPORKID_H
#define PIVX_SPORKID_H

/*
    Don't ever reuse these IDs for other sporks
    - This would result in old clients getting confused about which spork is for what
*/

enum SporkId : int32_t {
    SPORK_14_NEW_PROTOCOL_ENFORCEMENT           = 10013,
    SPORK_19_COLDSTAKING_MAINTENANCE            = 10019,
    SPORK_20_SAPLING_MAINTENANCE                = 10020,

    SPORK_INVALID                               = -1
};

// Default values
struct CSporkDef
{
    CSporkDef(): sporkId(SPORK_INVALID), defaultValue(0) {}
    CSporkDef(SporkId id, int64_t val, std::string n): sporkId(id), defaultValue(val), name(n) {}
    SporkId sporkId;
    int64_t defaultValue;
    std::string name;
};

#endif // PIVX_SPORKID_H
