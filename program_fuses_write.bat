#je nutne opravit u PWM3B avrdude.conf:
#puvodne tam je neco jako:
#     memory "efuse"
#         size            = 1;
#         write           = "1 0 1 0  1 1 0 0  1 0 1 0  0 1 0 0",
#                           "x x x x  x x x x  0 0 0 0  0 0 0 i";
#to je nutno zmenit na:
#     memory "efuse"
#         size            = 1;
#         write           = "1 0 1 0  1 1 0 0  1 0 1 0  0 1 0 0",
#                           "x x x x  x x x x  i i i i  i i i i";
#a pak pouzit -C"C:\Program Files\WinAVR\bin\avrdude.conf" !!!!!!!!!!
#jinak nepujde naprogramovat "efuse"
#tedy:

avrdude -p PWM3B -E noreset -c pony-stk200 -U lfuse:w:0xE5:m -U hfuse:w:0xDA:m -U efuse:w:0xF8:m -C"C:\Program Files\WinAVR\bin\avrdude.conf"

