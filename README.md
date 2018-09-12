<div align="center">
    <img src="logo.png" alt="Logo" width='auto' height='auto'/>
</div>

---
[![Build Status](https://travis-ci.com/qlcchain/qlcchain-classic.svg?branch=master)](https://travis-ci.com/qlcchain/qlcchain-classic)
## What is QLC Chain?

**[English](README.md)** **[中文](README_CN.md)**

---

QLC Chain is a next generation public blockchain designed for the NaaS. It deploys a multidimensional Block Lattice architecture and uses virtual machines (VM) to manage and support integrated Smart Contract functionality. Additionally, QLC Chain utilizes dual consensus: Delegated Proof of Stake (DPoS), and Shannon Consensus, which is a novel consensus developed by the QLC Chain team. Through the use of this dual consensus protocol, QLC Chain is able to deliver a high number of transactions per second (TPS), massive scalability and an inherently decentralized environment for NaaS related decentralized applications (dApp). The framework of QLC Chain will enable everyone to operate network services and benefit from it.

Network-as-a-Service (NaaS) is sometimes listed as a separate cloud provider along with Infrastructure- as-a-Service (IaaS), Platform-as-a-Service (PaaS), and Software-as-a-Service (SaaS).
This factors out networking, firewalls, related security, etc.

NaaS can include flexible and extended Virtual Private Network (VPN), bandwidth on demand, custom routing, multicast protocols, security firewall, intrusion detection and prevention, Wide Area Network (WAN), content addressing and filtering, and antivirus.

---

## Key Features

* Multidimensional Block Lattice Structure 
* QLC Chain Smart Contract 
* Dual Consensus Protocol 
    > For more information, see [YellowPaper](https://github.com/qlcchain/YellowPaper). **Currently we forked [NANO](https://github.com/nanocurrency/raiblocks/wiki/Build-Instructions) to verify Multidimensional Block Lattice Structure, our own public chain which is developed by Golang is under heavy development.**

## Build Instructions
### Required build tools

- (macOS) XCode >= 7.3
- (Windows) Visual Studio 2015
- (Windows) NSIS package builder
- (*nix) Clang >= 3.5 or GCC >= 5
- CMake

#### CMake variables (cmake -DVARNAME=VARVALUE).
- BOOST_ROOT [boost] (`/usr/local/boost/` if bootstrapped)
- CMAKE_BUILD_TYPE Release (default)
- ACTIVE_NETWORK rai_live_network (default)
- Qt5_DIR [qt]lib/cmake/Qt5 (to build GUI wallet)
- RAIBLOCKS_GUI ON (to build GUI wallet)
- ENABLE_AVX2 ON, optional PERMUTE_WITH_GATHER ON, optional PERMUTE_WITH_SHUFFLES ON (for CPU with AXV2 support, choose fastest method - for your CPU with https://github.com/sneves/blake2-avx2/)
- CRYPTOPP_CUSTOM ON (more conservative building of Crypto++ for wider range of systems)
- BOOST_CUSTOM ON (use bundled FindBoost.cmake for boost 1.66 on Windows commit 8fd47b20dca0c9fc82c5c4d240432503d6fb11a4+)

### Build RaiBlocks
- git submodule update --init --recursive
- Generate with cmake then build with your compiler
- (*nix) to build node without GUI execute: `make qlc_node`
- (*nix) to build wallet with GUI execute: `make qlc_wallet`

## Links & Resources
* [Yellow Paper](https://github.com/qlcchain/YellowPaper)
* [QLC Website](https://qlcchain.org)
* [Discord Chat](https://discord.gg/JnCnhjr)
* [Reddit](https://www.reddit.com/r/Qlink/)
* [Medium](https://medium.com/qlc-chain)
* [Twitter](https://twitter.com/QLCchain)
* [Telegram](https://t.me/qlinkmobile)
