#!/bin/csh
gen.py `/bin/ls [012345]* | fgrep -v .html`
