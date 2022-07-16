#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <fstream>
using namespace std;

void usage() {
    printf("Usage: sudo -i, sudo -u (user) -Z (context) -c (command) -s /path/to/su -a -mm\n");
    printf("-i - Login to shell\n");
    printf("  Write /data/data/com.termux/files/home/.shell for setup default shell\n");
    printf("-u (user) - switch to user\n");
    printf("-Z (context) - setup SELinux context\n");
    printf("-c (command) - run command\n");
    printf("-a - Force use Android nevironment instead of Termux\n");
    printf("-mm - Force run in global mount namespace\n");
    printf("-h - print this help menu\n");
    printf("-s /path/to/su - path to su instead of /sbin/su\n");
};

int main(int argc, char *argv[]) {
    vector<string> args;
    if (argc == 1) {
        usage();
    } else {
        for (unsigned int k=1; k < argc; k++) {
            args.push_back(argv[k]);
        }
        vector<string>::iterator it;
        it = find(args.begin(), args.end(), "-h");
        if (it != args.end()) {
            usage();
        } else {
            bool login = false;
            int userpos = -1;
            int conpos = -1;
            int compos = -1;
            bool gms = false;
            bool android = false;
            int supos = -1;
            string command;
            it = find(args.begin(), args.end(), "-i");
            if (it != args.end()) {
                login = true;
            } else {
                login = false;
            }
            it = find(args.begin(), args.end(), "-u");
            if (it != args.end()) {
                userpos = it - args.begin();
            } else {
                userpos = -1;
            }
            it = find(args.begin(), args.end(), "-Z");
            if (it != args.end()) {
                conpos = it - args.begin();
            } else {
                conpos = -1;
            }
            it = find(args.begin(), args.end(), "-c");
            if (it != args.end()) {
                compos = it - args.begin();
            } else {
                compos = -1;
            }
            it = find(args.begin(), args.end(), "-a");
            if (it != args.end()) {
                android = true;
            } else {
                android = false;
            }
            it = find(args.begin(), args.end(), "-mm");
            if (it != args.end()) {
                gms = true;
            } else {
                gms = false;
            }
            it = find(args.begin(), args.end(), "-s");
            if (it != args.end()) {
                supos = it - args.begin();
            } else {
                supos = -1;
            }
            if (android == false) {
                command = "PATH=\"$PATH\" LD_PRELOAD=/data/data/com.termux/files/usr/lib/libtermux-exec.so ";
            }
            if (supos > -1) {
                command += args[supos + 1];
            } else {
                command += "/sbin/su";
            }
            if (userpos > -1) {
                command += " ";
                command += args[userpos + 1];
            }
            if (gms) {
                command += " -mm";
            }
            if (conpos > -1) {
                command += " -c /system/bin/runcon ";
                command += args[conpos + 1];
                command += " ";
            } else {
                command += " -c ";
            }
            if (compos > -1) {
                command += args[compos + 1];
            } else if (login) {
                if (android == false) {
                    const bool ifext = std::__fs::filesystem::exists("/data/data/com.termux/files/home/.shell");
                    if (ifext) {
                        string shstr;
                        ifstream sudoif;
                        sudoif.open("/data/data/com.termux/files/home/.shell", ios::in);
                        sudoif >> shstr;
                        sudoif.close();
                        command += shstr;
                    } else {
                        command += "/data/data/com.termux/files/usr/bin/bash";
                    }
                } else {
                    unsetenv("LD_PRELOAD");
                    command += "/system/bin/sh";
                }
            }
            system(command.c_str());
        }
    }
    return 0;
}

