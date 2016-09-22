APU2_IP=192.168.0.103 && ssh root@$APU2_IP remountrw && \
scp build/coreboot.rom root@$APU2_IP:/root && \
ssh root@$APU2_IP flashrom -w /root/coreboot.rom -p internal \
#&& ssh root@$APU2_IP reboot
