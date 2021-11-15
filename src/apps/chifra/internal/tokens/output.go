package tokensPkg

/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/

import (
	"net/http"

	"github.com/spf13/cobra"
)

var Options TokensOptions

func RunTokens(cmd *cobra.Command, args []string) error {
	Options.Addrs2 = args
	opts := Options

	err := opts.ValidateTokens()
	if err != nil {
		return err
	}

	return opts.Globals.PassItOn("getTokens", opts.ToDashStr())
}

func ServeTokens(w http.ResponseWriter, r *http.Request) {
	opts := FromRequest(w, r)

	err := opts.ValidateTokens()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return
	}

	opts.Globals.PassItOn("getTokens", opts.ToDashStr())
}
