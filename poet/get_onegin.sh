wget "http://lib.ru/LITRA/PUSHKIN/p4.txt"
cat p4.txt | iconv -f koi8-r -t UTF-8 > p4_utf.txt
sed -n '15, 6522 p' p4_utf.txt > onegin.txt
rm p4.txt
rm p4_utf.txt
