
FILE=$1
CMD="$2 -t $3"

while true
do
    notifywait "$FILE"
    make -j4
    if [ "$?" == "0" ]; then
      $CMD
    fi
done
