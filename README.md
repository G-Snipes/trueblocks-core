##### Contact Info

[Email](mailto:jrush@quickblocks.io) 
[Website](http://quickblocks.io) 
[Slack](https://join.slack.com/t/quickblocks/shared_invite/enQtMzEzMzYzODI2MDY4LWRjZWUzMzg3N2RhYjMzN2FiYWMzYzlmYTA1MzJhNDM3NTUyMmZlOWU1ZWY5ZjdjMjcyZTMzMjk2NDI0ZTVhZTg)

#### Installing QuickBlocks

First things first. We start with the [installation instructions](src/other/install/INSTALL.md).

#### What is QuickBlocks?

QuickBlocks is a collection of software [libraries](src/libs), [applications](src/apps), [tools](src/tools), and [examples](src/samples) that allow you to retrieve 
Ethereum blockchain data (a) more quickly, (b) with higher information content, (c) in an fully decentralized way, (d) in a fully automated way, and (e) in a highly 
maintenance free way. The many potential use cases for QuickBlocks are described 
[here](https://github.com/Great-Hill-Corporation/quickBlocks/blob/master/docs/README_use_cases.md).

#### More Quickly

QuickBlocks interacts with the Ethereum blockchain via the RPC interface to accumulate `blocks`, `transactions`, `receipts`, and event `logs`. Prior to storing the 
data, we optimize it for later retrieval. The primary goal of QuickBlocks is to return the data as quickly as possible while maintaining a fully decentralized 
stance. These optimizations include:

1. attaching `receipts` and `logs` directly to `transactions`,
2. identifying in-error transactions using the node's trace functionality,
3. pre-processing incoming internal transactions,
4. storing the data optimized for quick, later retrieval.

Depending on the use case, QuickBlocks is able to achieve more than 150 times speedup relative to the node's RPC interface.

#### Higher Information Content

An Ethereum node knows nothing about Solidity source code. It only deals with byte code and therefore can only return hexadecimal bytes. In order to make the data useful, each user must translate the data back into the language of the smart contract. For example, this data:

    {
      "hash": "0xb9f174f61ef78661df519dba92f7a724593d1376e18edc8f2cc477684f089d13",
      "blockNumber": 2243947,
      "transactionIndex": 9,
      "timestamp": 1473657968,
      "from": "0x80f10e72de8a35d57efe477c1d1bd147be50307d",
      "to": "0xbb9bc244d798123fde783fcc1c72d3bb8c189413",
      "value": 0,
      "input": "0xc9d27afe00000000000000000000000000000000000000000000000000000000000001290000000000000000000000000000000000000000000000000000000000000001",
    }

is simply vote on a DAO contract:

    {
      "from": "0x80f10e72de8a35d57efe477c1d1bd147be50307d",
      "vote": {
        "proposal": 297,
        "supports": 1
      } 
    }

By default, QuickBlocks returns the raw hexadecimal data identical to the RPC, but with a simple switch, it can return what we call `articulated data` or data in the language of your smart contract.

QuickBlocks is able to articulate the data by using the smart contract's ABI file. If available, an ABI file contains all the information necessary to translate the 
RPC data back into the language of the smart contract. This functionality is implemented in the open source [grabABI](src/apps/grabABI) 
application. Instead of dealing with **hashes** and **merkel roots** and **sha3**, you deal with **votes** and **transfers** and **withdrawals**.

#### Fully Decentralized

Unlike other block chain scrapers such as http://etherscan.io, QuickBlocks is, by default, fully decentralized. QuickBlocks interacts with only your local Ethereum 
node. (Although, it can, if you wish, interact with any node such as Infura). This aspect of our system distinguishes QuickBlocks from other options. If you 
understand and believe in the benefits of decentralization, you will understand why this is important. If you don't, you'll undervalue this aspect of our work.

#### Fully Automatic (Yet Customizable)

The source code for ***all of the functionality*** mentioned above is generated automatically (i.e. programmatically) using the [grabABI](src/apps/grabABI) and 
[makeClass](src/apps/makeClass) apps. This makes standing up a [monitor](src/monitors/README.md) for any smart contract (or system of smart contracts) trivial. With 
a one line command to [chifra](src/apps/chifra/README.md), a fully functional C++ static library is created. At the same time, because the generated code is fairly 
simple C++ code, it is fully customizable.

#### Low Maintenance

Every smart contract, once deployed, is immutable. That means that the automatically generated C++ code is also immutable (if you want it to). You could literally 
automatically generate a data delivery layer for your smart contract that requires no further interaction. It can simply run forever, standing off-chain, but beside 
your smart contract, and deliver all your contact's data at high speed.

#### Structure of the Project

1. [src](src) - source code for the QuickBlocks libraries, core applications, tools, and sample monitors.
2. bin - location of primary executables generated by QuickBlocks
3. docs - documentation
