#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "conversions.h"

namespace qblocks {

    //--------------------------------------------------------------------------------
    extern string_q getVersionStr(bool incProg = true, bool incGit = true);
    extern uint32_t getVersionNum(uint16_t maj, uint16_t min, uint16_t build);
    extern uint32_t getVersionNum(void);

    extern string_q GIT_COMMIT_BRANCH;
    extern string_q GIT_COMMIT_HASH;
    extern timestamp_t GIT_COMMIT_TS;

}  // namespace qblocks
