if [ -z "$1" ]                           # Is parameter 1 zero length?
    then
    echo "Usage:\n\t$0 executable"
    exit -1
fi

LOGNAME=val_`basename $1`

valgrind \
    --gen-suppressions=all \
    --suppressions=`dirname $0`/valgrind_suppression.list \
    --track-fds=yes \
    --num-callers=20 \
    --track-origins=yes \
    --leak-check=full \
    --leak-resolution=high $* 2>${LOGNAME}_err.log
# >$LOGNAME.log

