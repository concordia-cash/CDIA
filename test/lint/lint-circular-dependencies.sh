#!/usr/bin/env bash
#
# Copyright (c) 2018-2021 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
#
# Check for circular dependencies

export LC_ALL=C

EXPECTED_CIRCULAR_DEPENDENCIES=(
    "budget/budgetmanager -> validation -> budget/budgetmanager"
    "chain -> legacy/stakemodifier -> chain"
    "chainparamsbase -> util/system -> chainparamsbase"
    "consensus/params -> consensus/upgrades -> consensus/params"
    "kernel -> validation -> kernel"
    "policy/fees -> txmempool -> policy/fees"
    "policy/policy -> validation -> policy/policy"
    "sapling/saplingscriptpubkeyman -> wallet/wallet -> sapling/saplingscriptpubkeyman"
    "spork -> sporkdb -> spork"
    "spork -> validation -> spork"
    "txmempool -> validation -> txmempool"
    "validation -> validationinterface -> validation"
    "validation -> zpiv/zpivmodule -> validation"
    "wallet/fees -> wallet/wallet -> wallet/fees"
    "wallet/scriptpubkeyman -> wallet/wallet -> wallet/scriptpubkeyman"
    "wallet/wallet -> wallet/walletdb -> wallet/wallet"
    "chain -> legacy/stakemodifier -> stakeinput -> chain"
    "chain -> legacy/stakemodifier -> validation -> chain"
    "chain -> legacy/stakemodifier -> validation -> validationinterface -> chain"
    "chain -> legacy/stakemodifier -> stakeinput -> txdb -> chain"
    "chain -> legacy/stakemodifier -> validation -> checkpoints -> chain"
    "chain -> legacy/stakemodifier -> validation -> undo -> chain"
    "chain -> legacy/stakemodifier -> validation -> pow -> chain"
)

EXIT_CODE=0

CIRCULAR_DEPENDENCIES=()

IFS=$'\n'
for CIRC in $(cd src && ../contrib/devtools/circular-dependencies.py {*,*/*,*/*/*}.{h,cpp} | sed -e 's/^Circular dependency: //'); do
    CIRCULAR_DEPENDENCIES+=( "$CIRC" )
    IS_EXPECTED_CIRC=0
    for EXPECTED_CIRC in "${EXPECTED_CIRCULAR_DEPENDENCIES[@]}"; do
        if [[ "${CIRC}" == "${EXPECTED_CIRC}" ]]; then
            IS_EXPECTED_CIRC=1
            break
        fi
    done
    if [[ ${IS_EXPECTED_CIRC} == 0 ]]; then
        echo "A new circular dependency in the form of \"${CIRC}\" appears to have been introduced."
        echo
        EXIT_CODE=1
    fi
done

for EXPECTED_CIRC in "${EXPECTED_CIRCULAR_DEPENDENCIES[@]}"; do
    IS_PRESENT_EXPECTED_CIRC=0
    for CIRC in "${CIRCULAR_DEPENDENCIES[@]}"; do
        if [[ "${CIRC}" == "${EXPECTED_CIRC}" ]]; then
            IS_PRESENT_EXPECTED_CIRC=1
            break
        fi
    done
    if [[ ${IS_PRESENT_EXPECTED_CIRC} == 0 ]]; then
        echo "Good job! The circular dependency \"${EXPECTED_CIRC}\" is no longer present."
        echo "Please remove it from EXPECTED_CIRCULAR_DEPENDENCIES in $0"
        echo "to make sure this circular dependency is not accidentally reintroduced."
        echo
        EXIT_CODE=1
    fi
done

exit ${EXIT_CODE}
