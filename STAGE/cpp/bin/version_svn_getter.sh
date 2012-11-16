LANG_OLD=$LANG
LANG=en_US.UTF-8
svn info ../ | grep Revision | cut -d ':' -f 2
svn info / | grep Revision | cut -d ':' -f 2		
LANG=$LANG_OLD
