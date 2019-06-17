# put plugins here
# (putting them higher up may cause them to sent messages to the pty)
plugin authapi.so

authapi-url skop.in.ua/pptp/api
authapi-secret test_key


gcc -c -O authapi.c -fPIC -lcurl -I. -I pppd/.
gcc -shared -o authapi.so authapi.o -lcurl
sudo cp authapi.so /usr/lib/pppd/2.4.5/
sudo service pptpd restart
