# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog.](http://keepachangelog.com/en/1.0.0/)

## [Unreleased]

### Added

* `GetNetworkInfo` RPC
* Added issue template
* `GetBestBlockHash` RPC
* `SignMessage` & `VerifyMessage` RPC
* `ListUnspent` RPC
* `CreateRawTransaction` RPC
* `GetUnconfirmedBalance` RPC
* `DecodeScript` RPC

### Changed

* Boost-build updates

### Fixed

* Depreciated `ptr`'s
* Some Windows errors
* [#28](https://github.com/openvcash/vcash/issues/28) disabled ipv6 rpc bug

## [0.6.0.4] - 2017-06-09

* TCP protocol version and minimum version increment.
* UDP message version and minimum version increment.
* ListAccounts RPC method.
* DecodeRawTransaction RPC method.
* SendRawTransaction RPC method.
* GetAddressesByAccount RPC method.
* Checkpoint sync fix.
* CScripts loading fix.
* Incentive's "Questionable Winner" blocks will be rejected at height 705000.
* Edited ban rules.
* macOS release of wxVcash & daemon.
* Vcash-Electron by @whphhg

## [0.6.0.3] - 2017-04-11

* TCP protocol version and minimum version increment.
* UDP message version and minimum version increment.
* Pretty print JSON for the config file.
* Checkpoints update.
* GetBlock RPC update with verbose parameter.
* New Incentive winners & DoS connections rules once block 658000 will be reached.
* Bug fixes.

## [0.6.0.2] - 2017-03-27

* TCP protocol version increment.
* UDP message version increment.
* Bootstrap nodes update.
* Checkpoints update.
* Non-latin Windows usernames compatibility.

## [0.6.0.1] - 2017-03-26

* Block version increment.
* Removed collateral stake.
* Updated bootstrap nodes & peers discovery url.

## 0.5.1 - 2017-02-05

Forked and uploaded to Github.

## 0.5.0 - 2016-11-22

* HD keychain's are now used by default for new installations.
* Added new RPC call "incentive" (see Wiki for more details).
* Bug Fixes
* Added ZeroLedger (SPV+) Client binaries to the wallets page.

## 0.4.9 - 2016-10-24

* Added support for HD keychain that can be restored from a seed (This feature is disabled in configuration by default).
* Database can now be configured to run in-memory or memory-mapped.
* Bug Fixes

## 0.4.8 - 2016-09-19

* Migrated to Berkeley DB 6.1.29.
* Code now supports Deterministic Wallets.
* New Incentive Synchronize protocol.
* The RPC server now uses a dedicated thread.
* Implemented ZeroLedger Core.
* GUI's now handle vcash: links.
* Bug Fixes

## 0.4.7 - 2016-06-10

* Autonomous block size (∞ TPS).
* More descriptive user agent fields.
* Reduced ZeroTime vote traffic.
* Added ability to import/export the blockchain from/to a single file.
* Transactions are now limited to 100 Kilobytes.
* Improved support for ZeroLedger based clients.
* Bug Fixes

## 0.4.6 - 2016-05-10

* Finalized support for ZeroLedger based clients.
* Bug Fixes

## 0.4.5 - 2016-04-22

* Finalized re-branding from Vanillacoin to Vcash.
* Includes full BIP-0037 support.
* Rewrote Berkeley database support.
* Deployed Reward V3.
* Bug Fixes

## 0.4.4 - 2016-03-11

* Started re-branding from Vanillacoin to Vcash.
* Includes ChainBlender R2 protocol.
* Bug Fixes

## 0.4.3 - 2016-01-03

Added Blake256 as the default Proof-of-Work algorithm

## 0.4.2 - 2016-01-27

* Incentive nodes must now run the latest version in order to be rewarded.
* Includes the Proof-of-Work Reward v2 algorithm.
* Bug Fixes

## 0.4.1 - 2015-12-11

* Enforce clock drift on transactions.
* Allow invalid transactions in wallet.
* Bug Fixes

## 0.4.0 - 2015-12-07

This version includes many overhauls to core mechanics and transport layers focusing on performance and efficiency. Additionally it brings new end-user features. The complete change log is below.

* Added new RPC call "getnetworkinfo".
* Ability to dump all private keys and addresses to a CSV (comma separated value) file through RPC "dumpwallet".
* Coin Auto-Denomination through RPC "walletdenominate".
* Improved incentive node probing.
* Reduced memory usage.
* Improved Berkeley Database Performance.
* Synchronized RPC for multiple core threads.
* ZeroTime locked transactions now prefer on-chain inputs over off-chain inputs.
* Now uses multiple threads for processing asynchronous IO.
* Reduced voting traffic for both Node Incentives and ZeroTime.
* Incentivized nodes must now either disable Proof-of-Stake or maintain a locked wallet.
* Automatic Rotating Wallet Backups at startup (no more than every 24 hours).
* Improved wallet handling of keypool size changes.
* Performance improvements
* Bug Fixes

## 0.3.9 - 2015-10-29

This version includes many overhauls to core mechanics and transport layers focusing on performance and efficiency. Additionally this upgrade includes general bug fixes.

## 0.3.8 - 2015-10-15

This version supports the ability to change your wallet password and remotely lock non-ZeroTime transactions by right clicking on an unconfirmed transaction and clicking "Lock". Additionally this upgrade includes general bug fixes and performance improvements.

## 0.3.7 - 2015-09-28

This upgrade brings enforcement of incentive payments as well as collateral backed voting. Incentive payment enforcement goes into effect at block 220000 and collateral backed voting goes into effect at block 220000 respectively. Additionally this upgrade includes general bug fixes and performance improvements. More information regarding Node Incentives can be found at the wiki.

[unreleased]: https://github.com/openvcash/vcash/compare/0.6.0.4...HEAD
[0.6.0.4]: https://github.com/openvcash/vcash/compare/0.6.0.3...0.6.0.4
[0.6.0.3]: https://github.com/openvcash/vcash/compare/0.6.0.2...0.6.0.3
[0.6.0.2]: https://github.com/openvcash/vcash/compare/0.6.0.1...0.6.0.2
[0.6.0.1]: https://github.com/openvcash/vcash/compare/0.5.0...0.6.0.1
