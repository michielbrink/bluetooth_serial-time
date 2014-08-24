msp430g2553 firmware:
-------
compile with:	msp430-gcc -mmcu=msp430g2553 -o main.elf main.c

upload with:	sudo mspdebug rf2500 "prog main.elf" "run"

client
-------

klok.py send the time to my clock 

weather
--------
weather api information (not implented yet) 

http://weather.yahooapis.com/forecastrss?w=727145&u=c

http://developer.yahoo.com/weather/
