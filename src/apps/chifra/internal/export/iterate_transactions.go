package exportPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExportOptions) readTransactions(
	theMap map[types.SimpleAppearance]*types.SimpleTransaction,
	filt *filter.AppearanceFilter,
	bar *logger.ProgressBar,
	readTraces bool,
) error {
	iterFunc := func(app types.SimpleAppearance, value *types.SimpleTransaction) error {
		if tx, err := opts.Conn.GetTransactionByAppearance(&app, readTraces); err != nil {
			return err
		} else {
			passes, _ := filt.ApplyTxFilters(tx)
			if passes {
				*value = *tx
			}
			if bar != nil {
				bar.Tick()
			}
			return nil
		}
	}

	// Set up and interate over the map calling iterFunc for each appearance
	iterCtx, iterCancel := context.WithCancel(context.Background())
	defer iterCancel()
	errChan := make(chan error)
	go utils.IterateOverMap(iterCtx, errChan, theMap, iterFunc)
	if stepErr := <-errChan; stepErr != nil {
		return stepErr
	} else if bar != nil {
		bar.Finish(true)
	}

	return nil
}
