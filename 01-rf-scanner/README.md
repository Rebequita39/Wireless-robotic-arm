# 01-rf-scanner

## Purpose
Scans the 2.4 GHz band and reports per-channel occupancy so the link can start on the quietest NRF24 channel.

## Wiring
TODO: link to the scanner's own pin table once imported.

## Build / Flash
TODO: depends on whether the scanner is imported (copy) or linked (git submodule).

## Test / Demo
Run the scanner next to a busy WiFi router and confirm the occupied channels show up as busier than the rest.

## Known Issues
None yet.

## Integration notes
Stage 5 needs the occupancy logic callable at boot (pick best channel, return a number), not only as a standalone display tool.
