import sys
import usb.core

try:
    data = open(sys.argv[1], "rb").read()

except IndexError:
    print("python3 load_xnu_gym.py {location of module build}")
    sys.exit(1337)

dev = usb.core.find(idVendor=0x05ac, idProduct=0x4141)
if dev is None:
    raise ValueError('Device not found')
dev.set_configuration()

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
    elif i = "s":
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
