# diffc

diff colorifying tool e.g. coloring diff output from perforce, etc

Some tools didn't have color output for diff operation properly, although mainly
one of those tools is `p4`. Git properly output diff color just fine.

# Build

* `make`
* `sudo make install`

# Usage

* `p4 diff | diffc -t perforce`
* `p4 diff -du | diffc -t perforce`
* `p4 describe -S 1234567 | diffc -t perforce`

> Substitute `diffc` with `diffc.exe` when execute it on Windows.

# Shorten the keys strokes

* Linux, you can use `alias` with your `bash_aliases` to shorten typing for `-t ...`. Windows
* Windows, you can use `doskey` with similar effect.

# Support

## Perforce

* `p4 diff` with or without `-du`
* `p4 describe`

# License
MIT, Wasin Thonkaew
