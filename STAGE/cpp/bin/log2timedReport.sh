echo date id depth voltage current measure rho stdDev nbVal quality
zgrep -A1 "measures saved for quadripole" $1 | egrep -v "measures saved for quadripole|\-\-" | sed 's/\[\|\]\|\t//g' | sed 's/null INFO (Probe.hpp:[^ ]* //g' | tr -s " " | sed 's/: /:/g' | sed 's/ | / /g' | sed 's/ [a-zA-Z]*:/ /g'


