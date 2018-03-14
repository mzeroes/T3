#!/bin/bash

# Make sure we're in the project root.
cd $(dirname "$0")/..

mkdir -p scripts/backup


DATE=`date +%d%m%y%H%M%S`
echo `tar cvfj ./scripts/backup/$DATE.tar.bz2 * --exclude scripts/backup | sha1sum`
