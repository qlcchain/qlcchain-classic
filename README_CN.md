<div align="center">
    <img src="logo.png" alt="Logo" width='auto' height='auto'/>
</div>

---
[![Build Status](https://travis-ci.org/nanocurrency/raiblocks.svg?branch=master)](https://travis-ci.org/nanocurrency/raiblocks)
[![Build status](https://ci.appveyor.com/api/projects/status/uwdcxi87h2y6jx94/branch/master?svg=true)](https://ci.appveyor.com/project/argakiig/raiblocks/branch/master)
## 什么是QLC Chain?

**[English](README.md)** **[中文](README_CN.md)**

---

QLC Chain 通过打造下一代分布式网络公链以提供网络即服务（NaaS）功能。

什么是网络即服务（NaaS）？它其实是云服务的一种。其他的云服务还包括基础设施即服务（IaaS），平台即服务（PaaS）和软件即服务（SaaS）。以上的云服务都作用于网络，防火墙等安全应用当中。

NaaS的覆盖范围很大。它包括虚拟私人网络（VPN），按需宽带，定制路由，多路传送协议，网络安全防火墙，网络入侵侦查与防御，广域网，内容寻址与过滤和杀毒服务。

QLC Chain采用了多维区块点阵结构并且利用虚拟机支持智能合约功能。在这个基础上，QLC Chain运用双重共识机制：委任权益证明（DPoS）和香农共识（Shannon Consensus）。其中，香农共识是由QLC Chain团队单独开发的下一代算力模型。通过双重共识机制，QLC Chain将会支持更快的每秒交易量（TPS），高可扩展性和为去中心化应用提供具有内在抗中心化特性的应用环境。QLC Chain的特性将会让每个人都能参与网络运营并从中受益。

---

## 核心特性

* 多维区块点阵 
* QLC Chain 智能合约 
* 双重共识
    > 更多QLC Chain技术信息，请QLC Chain黄皮书 [YellowPaper](https://github.com/qlcchain/YellowPaper). **我们已经通过Fork [NANO](https://github.com/nanocurrency/raiblocks/wiki/Build-Instructions) 验证多维区块点阵的核心功能, Golang语言版本的重写正在火热进行中.**

___

## 编译
### 依赖环境

- (macOS) XCode >= 7.3
- (Windows) Visual Studio 2015
- (Windows) NSIS package builder
- (*nix) Clang >= 3.5 or GCC >= 5
- CMake

#### CMake variables (cmake -DVARNAME=VARVALUE).
- BOOST_ROOT [boost] (`/usr/local/boost/` if bootstrapped)
- CMAKE_BUILD_TYPE Release (default)
- ACTIVE_NETWORK (default)
- Qt5_DIR [qt]lib/cmake/Qt5 (to build GUI wallet)
- RAIBLOCKS_GUI ON (to build GUI wallet)
- ENABLE_AVX2 ON, optional PERMUTE_WITH_GATHER ON, optional PERMUTE_WITH_SHUFFLES ON (for CPU with AXV2 support, choose fastest method - for your CPU with https://github.com/sneves/blake2-avx2/)
- CRYPTOPP_CUSTOM ON (more conservative building of Crypto++ for wider range of systems)
- BOOST_CUSTOM ON (use bundled FindBoost.cmake for boost 1.66 on Windows commit 8fd47b20dca0c9fc82c5c4d240432503d6fb11a4+)

### 编译 QLC Chain 节点
- git submodule update --init --recursive
- Generate with cmake then build with your compiler
- (*nix) to build node without GUI execute: `make qlc_node`
- (*nix) to build wallet with GUI execute: `make qlc_wallet`

___

## 更多信息

* [Yellow Paper](https://github.com/qlcchain/YellowPaper)
* [QLC Website](https://qlcchain.org)
* [Discord Chat](https://discord.gg/JnCnhjr)
* [Reddit](https://www.reddit.com/r/Qlink/)
* [Medium](https://medium.com/qlc-chain)
* [Twitter](https://twitter.com/QLCchain)
* [Telegram](https://t.me/qlinkmobile)
