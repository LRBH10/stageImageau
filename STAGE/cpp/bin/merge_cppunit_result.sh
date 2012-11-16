if [ -z $1 ] 
    then
    echo "Usage: $0 output_file_name"
    exit 0
fi

echo "<TestRunners>" > $1
for i in `ls cppunit_result_*.xml`
do
  echo "<Runner name='$i' />" >> $1
  cat $i >> $1
done
echo "</TestRunners>" >> $1

cat $1 | sed 's/^..xml .*$//g' >$1.tmp

echo "<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>" > $1
cat $1.tmp >> $1

rm $1.tmp
