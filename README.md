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

> You can make `alias` with your `bash_aliases` to shorten typing for `-t ...`.

# Support

## Perforce

* Normally without`-du`
* With `-du[n]`

# License
MIT, Wasin Thonkaew
