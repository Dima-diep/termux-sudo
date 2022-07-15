# termux-sudo

## What is it?
 It's modified version of `sudo` for Termux. This `sudo` can not only change user, with saving Termux environment, it can change SELinux context by usage of `/system/bin/runcon`. By default, it normally works only with Magisk, `/sbin/su`.

## Options
```
Usage: sudo -i, sudo -u (user) -Z (context) -c (command) -s /path/to/su -a -mm
-i - Login to shell
  Create symlink $PREFIX/bin/shell for setup default shell
-u (user) - switch to user
-Z (context) - setup SELinux context
-c (command) - run command
-a - Force use Android nevironment instead of Termux
-mm - Force run in global mount namespace
-h - print this help menu
-s /path/to/su - path to su instead of /sbin/su
```
