time_max = 60                   # maksymalny  czas jaki może pracować jedna maszyna w tygodniu
ppk = [9 7 6 5]                 # price per kilo produktów 1-4
cpk = [4 1 1 1]                 # cost per kilo za produkty 
ipk = ppk - cpk                 # income per kilo za profukty 
cph = [2, 2, 3]                 # cost per hour dla maszyn 1-3
maks_demand = [400 100 150 500] # maksymalny tygodniowy popyt na produkty 1, 2, 3, 4

# minutes per kilogram dla produktów i maszyn
mpk = [
#   M1 M2 M3
    5 10 6; # P1
    3 6 4;  # P2
    4 5 3;  # P3
    4 2 1   # P4
]