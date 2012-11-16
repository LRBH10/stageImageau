#!/bin/bash

cd /

# svn stuffs
find . -type d -iname '.svn' -exec rm -rf {} \;

# network stuffs
rm -f /etc/wvdial /etc/ppp/peers/dialondemand
/usr/sbin/deluser tunnel 2>>/dev/null
rm -rf /home/tunnel

# source stuffs
cd /home/remgeau/observatory/
for i in `ls -1 ./ | grep -v bin | grep -v dist`
do
    rm -rf ./$i
done

find . -iname '.depend*' -type f -exec rm -f {} \;
find . -iname '*make*' -type f -exec rm -f {} \;
find . -iname '*~' -type f -exec rm -f {} \;
find . -iname '*#*' -type f -exec rm -f {} \;

cd dist/conf
exclude_dir=moh1
for i in `ls -1 ./ | grep -v $exclude_dir`
do
    rm -rf $i
done
ln -sf /home/remgeau/observatory/dist/conf/$exclude_dir /home/remgeau/observatory/dist/conf/default

cd ..
rm -f cppunit* ftp* 
rm -f *log*

# session stuff
for i in .bash_history .viminfo .elinks .subversion .vim minicom.log
do
    rm -rf /root/$i
    rm -rf /home/remgeau/$i
done

# root script
cd /root/script/
rm -f *tunnel* checkIp* 
