iptables -t mangle -A PREROUTING -p tcp -d 192.168.1.125/32 -j MARK --set-mark 99
ip rule add fwmark 99 lookup 100
ip route add local 0.0.0.0/0 dev lo table 100
echo 1 >/proc/sys/net/ipv4/ip_forward
