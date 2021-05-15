# Wireshark
Install wireshark and grant permissions

## Config

```
$ sudo apt-get install wireshark
$ sudo dpkg-reconfigure wireshark-common
$ sudo usermod -a -G wireshark $USER
$ gnome-session-quit --logout --no-prompt
```

Lua plugin
1. In wireshark go to Help –> About Wireshark –> Folders
2. Put ./wireshark/dissector.lua file in directory in 'Personal Lua Plugins'
3. Restart Wireshark or Ctrl+Shift+L

## Using
1. Run wireshark
2. Capture from any or default internet interface
3. Filter by 'tin'
4. Now you have "Tin analyzer" tree with our nodeId, nodeRole and nodeMessage


# Tests
Requires boost library 

1. Installing boost
```bash
sudo apt install libboost-all-dev
```

2. Building tests
In project main directory
```bash
cd cmake-build-debug;
cmake -D BUILD_TESTS=ON ..;
make;
./test/tin_test
```