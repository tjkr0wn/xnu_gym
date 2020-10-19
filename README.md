# xnu_gym
`
xnu_gym is a project meant to purposely inject bugs into an iOS kernel that are trigerrable from Userspace.
This is perfect for any beginner to iOS kernel exploitation so that they can practice their skills from exploitation to launching a full jailbreak on a device.
`

# How it works
Currently, KTRW is needed for this project, although I hope to port it to a standalone pongoOS module.

Once KTRW is running and kernel read/write is possible, xnu_gym will insert a trivial bug into a
Userspace-accesable method in IOKIT. This will allow any researcher to practice their exploitation
skills from the sandbox.
