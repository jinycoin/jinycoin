// Copyright (c) 2015 The Bitcoin Core developers
// Copyright (c) 2019 JINY developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef JINY_DSNOTIFICATIONINTERFACE_H
#define JINY_DSNOTIFICATIONINTERFACE_H

#include <validationinterface.h>

class CDSNotificationInterface : public CValidationInterface
{
public:
    CDSNotificationInterface(CConnman& connmanIn): connman(connmanIn) {}
    virtual ~CDSNotificationInterface() = default;

    // a small helper to initialize current block height in sub-modules on startup
    void InitializeCurrentBlockTip();

protected:
    // CValidationInterface
    void AcceptedBlockHeader(const CBlockIndex *pindexNew) override;
    void NotifyHeaderTip(const CBlockIndex *pindexNew, bool fInitialDownload) override;
    void UpdatedBlockTip(const CBlockIndex *pindexNew, const CBlockIndex *pindexFork, bool fInitialDownload) override;
    void SyncTransaction(const CTransaction &tx, const CBlock *pblock) override;

private:
    CConnman& connman;
};

#endif // JINY_DSNOTIFICATIONINTERFACE_H
