## chifra explore

`chifra explore` opens Etherscan (and other explorers -- including our own) to the block, transaction hash, or address you specify. It's a handy (configurable) way to open an explorer from the command line, nothing more.

### usage

`Usage:`    chifra explore [-l|-g|-h] &lt;term&gt; [term...]  
`Purpose:`  Open an explorer for one or more addresses, blocks, or transactions.

`Where:`

{{<td>}}
|          | Option               | Description                                                         |
| -------- | -------------------- | ------------------------------------------------------------------- |
|          | terms                | one or more addresses, names, block, or transaction<br/>identifiers |
| &#8208;l | &#8208;&#8208;local  | open the local TrueBlocks explorer                                  |
| &#8208;g | &#8208;&#8208;google | search google excluding popular blockchain explorers                |
| &#8208;h | &#8208;&#8208;help   | display this help screen                                            |
{{</td>}}

**Source code**: [`apps/fireStorm`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/fireStorm)
