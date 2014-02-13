#!/bin/bash
LOGFILE="$HOME/usage.log"
#du --summarize produces nasty output, of which we only want the first number. the cut command takes this out for us.
echo "[`date`] $HOME `du -k --summarize $HOME | cut -f 1`" >> $LOGFILE
