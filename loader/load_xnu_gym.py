import sys
import usb.core
import os

load_mod = True

try:
    data = sys.argv[1]

except IndexError:
    print("python3 load_xnu_gym.py {location of module build}\nPass 'n' instead if module is already loaded on deviceor 'd' for loading the module from the build dir....")
    sys.exit(1337)

if data == "n":
    load_mod = False
elif data == "d":
    script_run_path = os.path.dirname(os.path.abspath(__file__))
    data = os.path.abspath(os.path.join(script_run_path, os.pardir)) + "/xnu_gym/build/xnu_gym"

dev = usb.core.find(idVendor=0x05ac, idProduct=0x4141)
if dev is None:
    raise ValueError('Device not found')
dev.set_configuration()

if load_mod:
    data = open(data, "rb").read()
    dev.ctrl_transfer(0x21, 2, 0, 0, 0)
    dev.ctrl_transfer(0x21, 1, 0, 0, 0)
    dev.write(2,data,100000)
    if len(data) % 512 == 0:
    	dev.write(2,"")
    dev.ctrl_transfer(0x21, 3, 0, 0, "modload\n")

print("\txnu_gym commands:")
print("\t\tt | Patches task_for_pid(0) for ANY process (warning: unsafe).")
print("\t\ts | Reintroduces the sock_puppet bug.")
print("\t\tr | Reintroduces the Trident bug(s).")
print("\t\tm | Reintroduces the mach_portal bug(s).")

print("Select a patch to perform, type q to quit the console and boot your device")

while True:
    i = input(">")
    if i == "t":
        dev.ctrl_transfer(0x21, 3, 0, 0, "xnu_gym -t\n")
    elif i == "s":
	       dev.ctrl_transfer(0x21, 3, 0, 0, "xnu_gym -s\n")
    elif i == "r":
        dev.ctrl_transfer(0x21, 3, 0, 0, "xnu_gym -r\n")
    elif i == "m":
        dev.ctrl_transfer(0x21, 3, 0, 0, "xnu_gym -m\n")
    elif i == "q":
        break
    else:
        print("Unrecognized command: " + i)

try:
    print("Booting...")
    dev.ctrl_transfer(0x21, 3, 0, 0, "bootx\n")

except:
    pass
