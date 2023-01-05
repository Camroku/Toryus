# Toryus
<p align="center">
<img src="assets/toryus.png" width="256px"/><br/>
A hobby operating system project in C<br/><br/>
<a href="https://discord.gg/7qnUz5KUdD"><img src="https://img.shields.io/badge/Discord-%237289DA.svg?style=for-the-badge&logo=discord&logoColor=white" alt="Discord"/></a>
<a href="https://github.com/Camroku/Toryus"><img src="https://img.shields.io/badge/git-%23F05033.svg?style=for-the-badge&logo=git&logoColor=white" alt="Git"/></a>
<a href="https://wdcd.camroku.tech/toryus"><img src="https://img.shields.io/badge/wdcd-%233f2457.svg?style=for-the-badge" alt="wdcd"/></a>
</p>

## Compiling
You need `grub`, `i686-elf-tools` and `nasm` installed.
```sh
cd src
make build
```

## Running
You need qemu installed.
```sh
make run
```

## History
I (Arnolxu) used to have an OS project named Ember. Then, I cancelled the project (because it was a dead project). After a few months, Yasin came to my [Discord server](https://discord.gg/5X3eHPnBq7) and asked about the sources we used. Then, I learned that he was my friend's friend, so I joined their project Arus and contributed to them. After about a month, I was still the only contributor and the project was dead again and I didn't want it to end up like Ember, so I left the project (just the Arus project, I didn't leave Artado completely) and made a fork called Toryus. I archived everything in the first commit of Arus so I could have a proof against them in case they decide to tell me that they made Arus themselves. [The Archive](https://web.archive.org/web/20220714235301/https://github.com/Artado-Project/arus/tree/e96e59cfe1a331721b8fadab14111668db5ba541). You can see the entire source code matches the Ember's source code at the Discord server of Ember (Except for some of kernel.cpp), the README.md literally is the README.md of Ember, and the logo's file name starts with ember (and it's the Ember logo).

So, Toryus, is in fact, a fork of Ember, not a fork of Arus, and since Ember is also written by me (Arnolxu), it's technically not based on anything.

## License
This software is licensed under a modified version of 4-clause BSD license, described in [LICENSE](LICENSE). No other licenses may apply.
