// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"context"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) HandleIndex(blockNums []uint64) error {
	if len(opts.Belongs) > 0 {
		return opts.HandleIndexBelongs(blockNums)
	}

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		showIndex := func(walker *index.CacheWalker, path string, first bool) (bool, error) {
			if path != cache.ToBloomPath(path) {
				return false, fmt.Errorf("should not happen in showIndex")
			}

			path = cache.ToIndexPath(path)
			if !file.FileExists(path) {
				// Bloom files exist, but index files don't. It's okay.
				return true, nil
			}

			header, err := index.ReadChunkHeader(path, true)
			if err != nil {
				return false, err
			}

			rng, err := base.RangeFromFilenameE(path)
			if err != nil {
				return false, err
			}

			s := simpleChunkIndex{
				Range:           rng,
				Magic:           header.Magic,
				Hash:            base.HexToHash(header.Hash.Hex()),
				AddressCount:    header.AddressCount,
				AppearanceCount: header.AppearanceCount,
				Size:            file.FileSize(path),
			}

			modelChan <- &s
			return true, nil
		}

		walker := index.NewCacheWalker(
			opts.Globals.Chain,
			opts.Globals.TestMode,
			100, /* maxTests */
			showIndex,
		)
		if err := walker.WalkBloomFilters(blockNums); err != nil {
			errorChan <- err
			cancel()
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

// TODO: BOGUS2 - MUST DOCUMENT
type simpleChunkIndex struct {
	Range           base.FileRange `json:"range"`
	Magic           uint32         `json:"magic"`
	Hash            base.Hash      `json:"hash"`
	AddressCount    uint32         `json:"nAddresses"`
	AppearanceCount uint32         `json:"nAppearances"`
	Size            int64          `json:"fileSize"`
}

func (s *simpleChunkIndex) Raw() *types.RawModeler {
	return nil
}

func (s *simpleChunkIndex) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	return types.Model{
		Data: map[string]any{
			"range":        s.Range,
			"magic":        fmt.Sprintf("0x%x", s.Magic),
			"hash":         s.Hash,
			"nAddresses":   s.AddressCount,
			"nAppearances": s.AppearanceCount,
			"fileSize":     s.Size,
		},
		Order: []string{
			"range",
			"magic",
			"hash",
			"nAddresses",
			"nAppearances",
			"fileSize",
		},
	}
}
