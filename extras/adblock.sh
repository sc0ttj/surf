#!/bin/sh
# prx -- prx@ybad.name.
# Idea from https://sebsauvage.net/wiki/doku.php?id=dns-blocklist
# block evil domains. Works for IPv4 and IPv6
# Licence:  MIT
# Script to generate a /etc/hosts file to blacklist ads
# Add yours in $MYHOSTS in a file passed as argument
#    zerohosts file.txt

# DOWNLOADER (to standard output)
#D="wget -O-" # mosts linux
#D="curl -L" # with curl
D="ftp -V -o-"  # OpenBSD

# Number of days befor updating the list
DAYS=7

# host file
H=/etc/hosts

VERSION=1.1

# Run only if older than $DAYS
test -z "$(find $H -mtime +${DAYS} )" && exit

if [ -f "${1}" ]; then
	MYHOSTS=$(cat "${1}")
else
	MYHOSTS=""
fi

HOSTSSRC="
https://someonewhocares.org/hosts/hosts
https://www.malwaredomainlist.com/hostslist/hosts.txt
http://winhelp2002.mvps.org/hosts.txt
http://sysctl.org/cameleon/hosts
https://raw.githubusercontent.com/StevenBlack/hosts/master/hosts
https://adaway.org/hosts.txt
"

DOMAINSSRC="
https://pgl.yoyo.org/adservers/serverlist.php?hostformat=nohtml&showintro=1
https://mirror.cedia.org.ec/malwaredomains/justdomains
https://mirror.cedia.org.ec/malwaredomains/immortal_domains.txt
https://v.firebog.net/hosts/Easyprivacy.txt
https://v.firebog.net/hosts/Prigent-Ads.txt
https://raw.githubusercontent.com/notracking/hosts-blocklists/master/dnscrypt-proxy/dnscrypt-proxy.blacklist.txt
"


echo ""
echo "[zerohosts $VERSION]: running"

TMP=$(mktemp)

echo "[zerohosts $VERSION]: download lists"

for URL in ${DOMAINSSRC}; do
	echo "$URL"
	${D} "${URL}" | grep -v "#" >> "$TMP"
done

# hosts files
#     remove comments, localhosts and keep only domains
#     use only last field
for URL in ${HOSTSSRC}; do
	echo "$URL"
	${D} "${URL}" |\
		grep -v "#" |\
		egrep -v "localhost|local" |\
		grep "127.0.0.1" |\
		awk '{print $NF}' >> "$TMP"
done

# uniq lines
sort -u -o "$TMP" "$TMP"
# remove remaining comments
sed -i 's/#.*//g' $TMP
# remove empty lines + windows EOL (thx Cyril Rouiller)
sed -i -e '/^[[:space:]]*$/d;s/\r$//g' $TMP
#tr -s '[:blank:]' < $TMP

echo "[zerohosts $VERSION]: write /etc/hosts"
cat << EOF > $H
$(date '+# %Y-%m-%d %H:%M:%S')
127.0.0.1 localhost
::1 localhost
0.0.0.0 0.0.0.0
::      ::
${MYHOSTS}
EOF

while read -r line; do
    echo "0.0.0.0 ${line}" >> $H
    echo "::      ${line}" >> $H
done < "$TMP"

chmod 0644 $H

echo "[zerohosts $VERSION]: delete tmp file $TMP"
rm $TMP

echo "[zerohosts $VERSION]: Bye :)"
exit 0
